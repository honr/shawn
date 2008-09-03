#include "traci_node_movement_creator.h"
#include "traci_processor.h"
#include "sys/node_movements/playback/movement_controller.h"
#include "sys/node_movements/playback/movement_info.h"
#include "sys/node_movements/linear_movement.h"
#include "sys/node_movements/jump_movement.h"
#include "sys/processors/processor_keeper.h"
#include "sys/node_movement.h"
#include "sys/node.h"
#include "sys/world.h"
#include "sys/misc/tokenize.h"
#include <string>
#include <limits>
#include <cmath>
#ifdef ENABLE_TRACICLIENT


using namespace std;

namespace traci{

	TraCINodeMovementCreator::
		TraCINodeMovementCreator(shawn::SimulationController& sc) : 
	sc_(sc),
	socket_(NULL),
	remotehost_("localhost"),
	remoteport_(8888),
	time_interval_(1.0),
	target_time_(0.0),
	mi_(NULL)
	{
		std::cout << "TraCINodeMovementCreator ctor" << std::endl;
	}

   TraCINodeMovementCreator::
	   ~TraCINodeMovementCreator(void)
   {
	   if (socket_) close();
	   std::cout << "TraCINodeMovementCreator dtor" << std::endl;
   }

   std::string 
	   TraCINodeMovementCreator::
	   name( void ) 
	   const 
	   throw()
   {
		return "TraCI";
   }

   std::string 
	   TraCINodeMovementCreator::
	   description( void ) 
	   const 
	   throw()
   {
		return "TraCINodeMovementCreator.";
   }

   void 
	   TraCINodeMovementCreator::
	   run(shawn::SimulationController&) 
	   throw( std::runtime_error )
   {
	   // Fetch parameters
	   remoteport_ = sc_.environment().optional_int_param("remote_port",remoteport_);
	   remotehost_ = sc_.environment().optional_string_param("remote_host",remotehost_);
	   //target_time_ = sc_.environment().optional_double_param("target_time",target_time_);
	   //time_interval_ = sc_.environment().optional_double_param("time_interval",time_interval_);

	   fetch_processor_factories();
	   // Connect it!
	   if (! connect() ){
		   std::cerr << "Connection failed!" << std::endl;
		   abort();
	   }
   }

   shawn::MovementInfo * 
	   TraCINodeMovementCreator::
	   next_movement()
       throw( std::runtime_error )
   {
	   if (!socket_){
		   std::cerr << "Error in method TraCINodeMovementCreator::next_movement, socket_ == NULL!" << std::endl;
	      abort();
	   }

      
	   // If in-storage is empty ask for new data from movement simulator (e.g. SUMO)
	   if (!in_.valid_pos()){
         // If the right simulation time for sending the next question is not yet arrived ...
         if (sc_.world().scheduler().current_time() < target_time_)
         {
            //... wait until then
            mi_ = new shawn::MovementInfo();
            mi_->set_urgency(shawn::MovementInfo::Delayed);
            mi_->set_dispatch_time(target_time_);
            mi_->set_node(NULL);
            mi_->set_node_movement(NULL);
            return mi_;
         }
         // If target_time is reached, ask for new movements
	      target_time_ = sc_.world().scheduler().current_time() + time_interval_;
		   check_for_unused_nodes();
		   current_nodes_.clear();
		   // Build command
		   tcpip::Storage out;
		   // Lenght of command
		   out.writeByte(11);
		   // CommandID
		   out.writeChar(CMD_SIMSTEP);
		   // Target time
		   out.writeDouble(target_time_);
		   // Result type (Position representation)
		   out.writeChar(POSITION_2D);

		   // Send command
		   try{
			   socket_->sendExact(out);
		   }catch(tcpip::SocketException e){
			   std::cerr << "Error in method TraCINodeMovementCreator::next_movement() while sending: " 
						 << e.what() << std::endl;
			   abort();
		   }
	   

		   try{
			   socket_->receiveExact(in_);
		   }catch(tcpip::SocketException e){
			   std::cerr << "Error in method TraCINodeMovementCreator::next_movement while receiving: "
						 << e.what() << std::endl;
		   }
		   
		   // First result command serves as status information
		   std::string description;
		   extract_command_status(in_, CMD_SIMSTEP, description);

	   }
		
	   // Now follow the single node movements
	   int node_id = 0;
	   double dest_time = 0.0;
	   double x = 0.0;
	   double y = 0.0;
	   double now = sc_.world().scheduler().current_time();
	   double velocity = 0.0;
	   shawn::Node * node = NULL;

	   unsigned int command_start = 0;
	   unsigned int command_length = 0;

	   //while (in_.valid_pos()){
	   command_start = in_.position();
	   command_length = in_.readByte();

	   if (unsigned int command_id = in_.readChar() != CMD_MOVENODE){
			// Move node command expected
		   std::cerr << "Error in method TraCINodeMovementCreator::next_movement, move node command \
						expected. Got command_id " << command_id << std::endl;
		   abort();
	   }

	   node_id = in_.readInt();
	   dest_time = in_.readDouble();

	   // PositionType
	   if (in_.readByte() != POSITION_2D){
		   std::cerr << "Error in method TraCINodeMovementCreator::next_movement, \
						I can handle only 2D-coordinates!" << std::endl;
		   abort();
	   }
	   x = in_.readFloat();
	   y = in_.readFloat();

	   // Increase Nodes
	   if (sc_.world().node_count() <= node_id){
		   increase_nodes(node_id - sc_.world().node_count());
	   }

	   node = sc_.world_w().find_node_by_id_w(node_id);
	   assert( node );
	   current_nodes_.insert(node_id);
	   
	   shawn::JumpMovement * jm = NULL;
	   shawn::LinearMovement * lm = NULL;
	   mi_ = new shawn::MovementInfo();
	   traci::TraCIProcessor * tp = node->get_processor_of_type_w<traci::TraCIProcessor>();
	   assert(tp);
	   if ( tp->disabled() && 
		    node->real_position().x() == 0 && node->real_position().y() == 0 ){
		    // "Fresh" node (is disabled and they appear at (0.0,0.0))
			jm = new shawn::JumpMovement();
			jm->set_dimension(shawn::JumpMovement::X,x);
			jm->set_dimension(shawn::JumpMovement::Y,y);
			jm->set_dimension(shawn::JumpMovement::Z,0.0);
			// Switch the new node on
			tp->enable_node();
	   }else{
		    // Old node
		    // Set node exactly to old destination
         const shawn::LinearMovement* old_lm = dynamic_cast<const shawn::LinearMovement*>(&node->movement());
			if (old_lm) node->set_real_position(old_lm->destination());
			lm = new shawn::LinearMovement();
			if (dest_time > now){
				velocity = sqrt( (node->real_position().x() - x)*(node->real_position().x() - x) +
								 (node->real_position().y() - y)*(node->real_position().y() - y) )/
								 (dest_time - now);
				lm->set_parameters(velocity,shawn::Vec(x,y,0.0),sc_.world_w());
			}
	   }
	   mi_->set_urgency(shawn::MovementInfo::Immediately);
	   mi_->set_dispatch_time(now);
	   mi_->set_node(node);
	   assert(jm || lm);
	   if (jm) mi_->set_node_movement(jm);
	   if (lm) mi_->set_node_movement(lm);
	   //}

	   // Right length???
       if (command_start + command_length != in_.position()){
		   // Last read command has wrong length
		   cerr << "Error in method TraCINodeMovementCreator::next_movement, command at position " 
				<< command_start << " was read with wrong length." << endl;
		   abort();
       }

	   return mi_;
   }

	void 
		TraCINodeMovementCreator::
		extract_command_status(tcpip::Storage& s, unsigned char command_id, string& description)
	{
		 unsigned int command_start = s.position();
		 unsigned int command_length = s.readByte();

		 // CommandID needs to fit
		 if (unsigned char rcvd_command_id = s.readChar() != command_id)
		 {
			 cerr << sc_.world().scheduler().current_time() 
				  << ": Error in method TraCINodeMovementCreator::extract_command_status, Server \
					 answered to command: " << rcvd_command_id << ". Expected command: " << command_id << endl;
			 abort();
		 }

		 // Get result and description
		 unsigned char result = s.readChar();
		 description = s.readString();

		 if (result != RTYPE_OK)
		 {
			cerr << sc_.world().scheduler().current_time() 
				 << ": Error in method TraCINodeMovementCreator::extract_command_status, Server returned error " 
				 << "[" << result << "] "
				 << description << endl;
			abort();
		 }

		 // Right length?
		 if (command_start + command_length != s.position())
		 {
			// Last read command has wrong length
			 cerr << sc_.world().scheduler().current_time() 
			     << ": Error in method TraCINodeMovementCreator::extract_command_status, command at position " 
				 << command_start << " was read with wrong length." << endl;
			 abort();
		 }
 
	}

   
   void 
	   TraCINodeMovementCreator::
	   reset()
   {
      
   }

   bool 
	   TraCINodeMovementCreator::
	   connect()
   {
		if (socket_) close();

		socket_ = new tcpip::Socket(remotehost_, remoteport_);
		socket_->set_blocking(true);
		socket_->connect();
		// If not successful, delete socket
		if (! socket_->has_client_connection() ){
			delete socket_;
			socket_ = NULL;
		}
		return (socket_ != NULL);
   }

   void
	   TraCINodeMovementCreator::
	   close()
   {
	   if (!socket_){
			delete socket_;
			socket_ = NULL;
	   }
   }

   void
	   TraCINodeMovementCreator::
	   fetch_processor_factories()
   {
	   shawn::ProcessorFactoryHandle pfh;
	   bool already_equipped = false;
	   shawn::StrTok tok(sc_.environment().optional_string_param("processors",""), ", ");
	   for(shawn::StrTok::iterator it = tok.begin(); it!=tok.end(); ++it){
			pfh = sc_.processor_keeper_w().find_w( *it );
			assert( pfh != NULL );
			processor_factories_.push_back(pfh);
			if (*it == "TraCI") already_equipped = true;
        }
	  // Add the TraCI processor
	  if (!already_equipped){
		  pfh = sc_.processor_keeper_w().find_w("TraCI");
		  assert( pfh != NULL );
		  processor_factories_.push_back( pfh );
	  }
   }

   void 
	   TraCINodeMovementCreator::
	   increase_nodes(int to)
   {
	   for (int i = 0; i <= to; ++i){
		   shawn::Node * node = new_node();
		   traci::TraCIProcessor * tp = node->get_processor_of_type_w<traci::TraCIProcessor>();
		   assert(tp);
		   tp->disable_node();
		   sc_.world_w().add_node( *node );
	   }
   }

   shawn::Node * 
	   TraCINodeMovementCreator::
	   new_node()
   {
	   shawn::Node * node = new shawn::Node();

	   for (std::list<shawn::ProcessorFactoryHandle>::const_iterator it = processor_factories_.begin();
		   it != processor_factories_.end(); ++it){
			   shawn::ProcessorHandle ph = it->get()->create();
			   node->add_processor(ph);
	   }
	   return node;
   }

   void 
	   TraCINodeMovementCreator::
	   check_for_unused_nodes()
   {
	   for (shawn::World::node_iterator ni = sc_.world_w().begin_nodes_w(); 
		    ni != sc_.world_w().end_nodes_w(); ++ni){
		   if (current_nodes_.find(ni->id()) == current_nodes_.end()){
			   traci::TraCIProcessor * tp = (ni->get_processor_of_type_w<traci::TraCIProcessor>());
			   assert(tp);
			   tp->disable_node();
		   }
	   }
   }

   void 
	   TraCINodeMovementCreator::
	   command_set_maximum_speed(shawn::Node& node, double max_speed)
   {
	   if (!socket_){
		   std::cerr << "Error in method TraCINodeMovementCreator::command_set_maximum_speed, \
						socket_ == NULL" << std::endl;
	   }

	   // Build command
	   tcpip::Storage out;
	   // Length of command
	   out.writeUnsignedByte(10);
	   // CommandID
	   out.writeChar(CMD_SETMAXSPEED);
	   // Node ID
	   out.writeInt(node.id());
	   // Max speed
	   out.writeFloat(static_cast<float>(max_speed));

	   // Send command

	   try{
		   socket_->sendExact(out);
	   }catch(tcpip::SocketException e){
		   std::cerr << "Error in method TraCINodeMovementCreator::command_set_maximum_speed \
						while sending: " << e.what() << std::endl;
		   abort();
	   }
	
	   tcpip::Storage in;
	   try{
		   socket_->receiveExact(in);
	   }catch(tcpip::SocketException e){
		   std::cerr << "Error in method TraCINodeMovementCreator::command_set_maximum_speed \
						while receiving: " << e.what() << std::endl;
		   abort();
	   }

	   // First result command serves as status informatin
	   std::string description;
	   extract_command_status(in, CMD_SETMAXSPEED, description);
   }

   void 
	   TraCINodeMovementCreator::
	   command_stop(shawn::Node& node, double x, double y, double radius, double wait_time)
   {
	   if (!socket_){
		   std::cerr << "Error in method TraCINodeMovementCreator::command_stop, \
						socket_ == NULL" << std::endl;
	   }

	   // Build command
	   tcpip::Storage out;
	   // Length of command
	   out.writeUnsignedByte(27);
	   // Command ID
	   out.writeChar(CMD_STOP);
	   // Target time
	   out.writeInt(node.id());
	   // Position type
	   out.writeUnsignedByte(POSITION_2D);
	   // Position
	   out.writeFloat(static_cast<float>(node.real_position().x()));
	   out.writeFloat(static_cast<float>(node.real_position().y()));
	   // Radius
	   out.writeFloat(static_cast<float>(radius));
	   // Wait time
	   out.writeDouble(wait_time);

	   // Send command
	   try{
		   socket_->sendExact(out);
	   }catch(tcpip::SocketException e){
		   std::cerr << "Error in method TraCINodeMovementCreator::command_stop while sending: "
					 << e.what() << std::endl;
		   abort();
	   }
		
	   tcpip::Storage in;
	   try{
		   socket_->receiveExact(in);
	   }catch(tcpip::SocketException e){
		   std::cerr << "error in method TraCINodeMovementCreator::command_stop while recieving: "
					 << e.what() << std::endl;
		   abort();
	   }

		// First result command serves as status information
	   std::string description;
	   extract_command_status(in, CMD_STOP, description);	
   }

   void 
	   TraCINodeMovementCreator::
	   command_change_route(shawn::Node& node, std::string road_id, double travel_time)
   {
	   if (!socket_){
		   std::cerr << "Error in method TraCINodeMovementCreator::command_change_route, \
						socket == NULL" << std::endl;
		   abort();
	   }

	   // Build command
	   tcpip::Storage out;
	   // Length of command
	   out.writeUnsignedByte(18+road_id.length());
	   // Command ID
	   out.writeChar(CMD_CHANGEROUTE);
	   // Node ID
	   out.writeInt(node.id());
	   // Road ID
	   out.writeString(road_id);
	   // Travel time
	   out.writeDouble(travel_time);

	   // Send command
	   try{
		   socket_->sendExact(out);
	   }catch(tcpip::SocketException e){
		   std::cerr << "Error in method TraCINodeMovementCreator::coammand_change_route while sending: " 
			   << e.what() << std::endl;
		   abort();
	   }
	   tcpip::Storage in;
	   try{
		   socket_->receiveExact(in);
	   }catch(tcpip::SocketException e){
		   std::cerr << "Error in method TraCINodeMovementCreator::command_change_route while receiving: " 
					 << e.what() << std::endl;
		   abort();
	   }

	   std::string description;
	   extract_command_status(in, CMD_CHANGEROUTE, description);
   }

   void 
	   TraCINodeMovementCreator::
	   command_change_lane(shawn::Node& node, char lane, float time)
   {
		if (!socket_){
		   std::cerr << "Error in method TraCINodeMovementCreator::command_change_lane,\
						socket == NULL" << std::endl;
		   abort();
	   }

	  // Build command
	  tcpip::Storage out;
	  // Length of command
	  out.writeUnsignedByte(9);
	  // Command ID
	  out.writeChar(CMD_CHANGELANE);
	  // Node ID
	  out.writeInt(node.id());
	  // Lane 
	  out.writeByte(lane);
	  // Time
	  out.writeFloat(time);

	  // Send command
	   try{
		   socket_->sendExact(out);
	   }catch(tcpip::SocketException e){
		   std::cerr << "Error in method TraCINodeMovementCreator::coammand_change_lane while sending: " 
			   << e.what() << std::endl;
		   abort();
	   }
	   tcpip::Storage in;
	   try{
		   socket_->receiveExact(in);
	   }catch(tcpip::SocketException e){
		   std::cerr << "Error in method TraCINodeMovementCreator::command_change_lane while receiving: " 
					 << e.what() << std::endl;
		   abort();
	   }

	   std::string description;
	   extract_command_status(in, CMD_CHANGELANE, description);

   }

}
#endif
