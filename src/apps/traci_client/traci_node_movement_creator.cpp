#include "traci_node_movement_creator.h"
//#include "traci_processor.h"
#include "sys/node_movements/playback/movement_controller.h"
#include "sys/node_movements/playback/movement_info.h"
#include "sys/node_movements/linear_movement.h"
#include "sys/node_movements/jump_movement.h"
#include "sys/processors/processor_keeper.h"
#include "sys/node_movement.h"
#include "sys/node.h"
#include "sys/tag.h"
#include "sys/taggings/basic_tags.h"
#include "sys/world.h"
#include "sys/misc/tokenize.h"
#include <string>
#include <sstream>
#include <limits>
#include <cmath>
#ifdef ENABLE_TRACICLIENT


using namespace std;
using namespace tcpip;
using namespace shawn;

namespace traci{

	// Add domains here. 
	// Make sure that: 
	// -the counts of elements in both arrays are the same and
	// -the order of the domain_ids corresponds to the order of corresponding domain_names
	const int traci::TraCINodeMovementCreator::DOMAIN_IDS[] = {0x01,0x02};
	const std::string traci::TraCINodeMovementCreator::DOMAIN_NAMES[] = {"vehicle","traffic_light"};

	TraCINodeMovementCreator::
		TraCINodeMovementCreator(SimulationController& sc) : 
	sc_(sc),
	socket_(NULL),
	remotehost_("localhost"),
	remoteport_(8888),
	time_interval_(1.0),
    feed_time_(0.0),
	target_time_(0.0),
	mi_(NULL)
	{
		cout << "TraCINodeMovementCreator ctor" << endl;
	}
	//---------------------------------------------------------------------
   TraCINodeMovementCreator::
	   ~TraCINodeMovementCreator(void)
   {
	   if (socket_) close();
	   reset();
	   cout << "TraCINodeMovementCreator dtor" << endl;
   }
	//---------------------------------------------------------------------
   string 
	   TraCINodeMovementCreator::
	   name( void ) 
	   const 
	   throw()
   {
		return "TraCI";
   }
	//---------------------------------------------------------------------
   string 
	   TraCINodeMovementCreator::
	   description( void ) 
	   const 
	   throw()
   {
		return "TraCINodeMovementCreator.";
   }
	//---------------------------------------------------------------------
	void 
		TraCINodeMovementCreator::
		node_added( Node& ) 
		throw()
	{
		// Do nothing here
	}
	//---------------------------------------------------------------------
	void 
		TraCINodeMovementCreator::
		node_removed( Node& node ) 
		throw()
	{
		remove_node_from_map(&node);
	}
	//---------------------------------------------------------------------
	void 
		TraCINodeMovementCreator::
		id_changed( int, int ) 
		throw()
	{
		ABORT_NOT_IMPLEMENTED;
	}
	//---------------------------------------------------------------------
	bool 
		TraCINodeMovementCreator::
		invalidate( void ) 
		throw()
	{
		// Do nothing here
		return true;
	}
	//---------------------------------------------------------------------
   void 
	   TraCINodeMovementCreator::
	   run(SimulationController&) 
	   throw( runtime_error )
   {
	   require_world(sc_);
	   sc_.world_w().add_node_change_listener(*this);
	   // Fetch parameters
	   remoteport_ = sc_.environment().optional_int_param("remote_port",remoteport_);
	   remotehost_ = sc_.environment().optional_string_param("remote_host",remotehost_);
       feed_time_ =  sc_.environment().optional_double_param("feed_time",feed_time_);
	   //target_time_ = sc_.environment().optional_double_param("target_time",target_time_);
	   //time_interval_ = sc_.environment().optional_double_param("time_interval",time_interval_);

	   fetch_processor_factories();
	   // Connect it!
	   if (!connect()) abort();

      if (feed_time_ - time_interval_ > 0.0)
      {
         // Let TraCI server run feed_time_ seconds of simulation time
		   Storage s;
		   // Lenght of command
		   s.writeByte(11);
		   // CommandID
		   s.writeChar(CMD_SIMSTEP);
		   // Target time
		   s.writeDouble(feed_time_ - time_interval_);
		   // Result type (No positions needed)
		   s.writeChar(POSITION_NONE);

		   // Send command
		   try{
			   socket_->sendExact(s);
		   }catch(SocketException e){
			   cerr << "Error in method TraCINodeMovementCreator::next_movement() while sending: " 
						 << e.what() << endl;
			   abort();
		   }

		   try{
			   socket_->receiveExact(s);
		   }catch(SocketException e){
			   cerr << "Error in method TraCINodeMovementCreator::next_movement while receiving: "
						 << e.what() << endl;
			   abort();
		   }
		   
		   // First result command serves as status information
		   string description;
		   extract_command_status(s, CMD_SIMSTEP, description);

		   bool aatl = sc_.environment_w().optional_bool_param("traffic_light_nodes",false);
		   if (aatl) add_traffic_lights();

         // No node position informations requested
      }
   }
	//---------------------------------------------------------------------
   // next_movement = command_simulation_step -> scheduled "magically"
   MovementInfo* 
	   TraCINodeMovementCreator::
	   next_movement()
       throw( runtime_error )
   {
	   if (!socket_){
		  cerr << "Error in method TraCINodeMovementCreator::next_movement, socket_ == NULL!" << endl;
	      abort();
	   }

      
	   // If in-storage is empty ask for new data from movement simulator (e.g. SUMO)
	   if (!in_.valid_pos()){
         // If the right simulation time for sending the next question is not yet arrived ...
         if (sc_.world().scheduler().current_time() < target_time_)
         {
            //... wait until then
            mi_ = new MovementInfo();
            mi_->set_urgency(MovementInfo::Delayed);
            mi_->set_dispatch_time(target_time_);
            mi_->set_node(NULL);
            mi_->set_node_movement(NULL);
            return mi_;
         }
         // If target_time is reached, ask for new movements
	      target_time_ = sc_.world().scheduler().current_time() + time_interval_;
		   check_for_unused_vehicle_nodes();
		   current_vehicle_nodes_.clear();
		   // Build command
		   Storage out;
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
		   }catch(SocketException e){
			   cerr << "Error in method TraCINodeMovementCreator::next_movement() while sending: " 
					<< e.what() << endl;
			   abort();
		   }
	   
		   try{
			   socket_->receiveExact(in_);
		   }catch(SocketException e){
			   cerr << "Error in method TraCINodeMovementCreator::next_movement() while receiving: "
					<< e.what() << endl;
			   abort();
		   }
		   
		   // First result command serves as status information
		   string description;
		   extract_command_status(in_, CMD_SIMSTEP, description);

	   }
		
	   // Now follow the single node movements
	   int node_id = -1;
	   double dest_time = 0.0;
	   double x = 0.0;
	   double y = 0.0;
	   double now = sc_.world().scheduler().current_time();
	   double velocity = 0.0;
	   Node* node = NULL;

	   unsigned int command_start = 0;
	   unsigned int command_length = 0;

	   //while (in_.valid_pos()){
	   command_start = in_.position();
	   command_length = in_.readByte();

	   if (unsigned int command_id = in_.readChar() != CMD_MOVENODE){
			// Move node command expected
		   cerr << "Error in method TraCINodeMovementCreator::next_movement, move node command \
						expected. Got command_id " << command_id << endl;
		   abort();
	   }

	   node_id = in_.readInt();
	   dest_time = in_.readDouble();

	   // PositionType
	   if (in_.readByte() != POSITION_2D){
		   cerr << "Error in method TraCINodeMovementCreator::next_movement, \
						I can handle only 2D-coordinates!" << endl;
		   abort();
	   }
	   x = in_.readFloat();
	   y = in_.readFloat();

	   bool is_new_node = false;
	   node = find_node_by_traci_id_w(TraCIID(0x01,node_id));
	   // If node doesn't exist create new one
	   if (!node){
		   node = new_node(TraCIID(0x01,node_id));
		   is_new_node = true;
	   }
	   current_vehicle_nodes_.insert(TraCIID(0x01,node_id));
	   JumpMovement* jm = NULL;
	   LinearMovement* lm = NULL;
	   mi_ = new MovementInfo();
	   if ( is_new_node ){
		    // "Fresh" node
			jm = new JumpMovement();
			jm->set_dimension(JumpMovement::X,x);
			jm->set_dimension(JumpMovement::Y,y);
			jm->set_dimension(JumpMovement::Z,0.0);
	   }else{
		    // Old node
		    // Set node exactly to old destination
         const LinearMovement* old_lm = dynamic_cast<const LinearMovement*>(&node->movement());
		 if (old_lm) node->set_real_position(old_lm->destination());
			lm = new LinearMovement();
			if (dest_time > now){
				velocity = sqrt( (node->real_position().x() - x)*(node->real_position().x() - x) +
								 (node->real_position().y() - y)*(node->real_position().y() - y) )/
								 (dest_time - now);
				lm->set_parameters(velocity,Vec(x,y,0.0),sc_.world_w());
			}
		}
		mi_->set_urgency(shawn::MovementInfo::Immediately);
		mi_->set_dispatch_time(now);
		mi_->set_node(node);
		assert(bool(jm) ^ bool(lm));
		if (jm) mi_->set_node_movement(jm);
		if (lm) mi_->set_node_movement(lm);


	   // Right length???
       if (command_start + command_length != in_.position()){
		   // Last read command has wrong length
		   cerr << "Error in method TraCINodeMovementCreator::next_movement, command at position " 
				<< command_start << " was read with wrong length." << endl;
		   abort();
       }

	   return mi_;
   }
   //---------------------------------------------------------------------
   void
	   TraCINodeMovementCreator::
	   add_traffic_lights()
   {
	   int count = 0;
	   Node* node = NULL;
	   if (command_scenario_integer(false,0x02,-1,0x01,count)){
		   for (int i = 0; i < count; ++i){
				node = new_node(TraCIID(0x02,i));
				float x = 0.0, y = 0.0, z = 0.0;
				if(command_scenario_pos3d(false,0x02,i,0x02,x,y,z)){
					node->set_real_position(Vec(x,y,z));
				}else{
					cerr << "Positioning of traffic light: " << i << " failed!" << endl;
					abort();
				}
		   }
	   }else{
			cerr << "Couldn't aquire count of traffic lights!" << endl;
			abort();
	   }
   }
   //---------------------------------------------------------------------
   void 
	   TraCINodeMovementCreator::
	   extract_command_status(Storage& s, unsigned char command_id, string& description)
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
   //--------------------------------------------------------------------- 
   void 
	   TraCINodeMovementCreator::
	   reset()
   {
	   for (map<int,set<ProcessorFactoryHandle>* >::iterator it = 
		   domain_processor_factories_.begin(); it != domain_processor_factories_.end();
		   ++it){
			   it->second->clear();
			   delete it->second;
	   }
	   node_change_listeners_.clear();
	   current_vehicle_nodes_.clear();
	   node_to_traci_ids_.clear();
	   traci_ids_to_node_.clear();
   }
	//---------------------------------------------------------------------
   bool 
	   TraCINodeMovementCreator::
	   connect()
   {
		if (socket_) close();

		socket_ = new Socket(remotehost_, remoteport_);
		socket_->set_blocking(true);
		try{
			socket_->connect();
		}
		catch(SocketException e){
			cerr << "Error in method TraCINodeMovementCreator::connect(): "
						 << e.what() << endl;
			delete socket_;
			socket_ = NULL;
			return false;
		}
		return true;
   }
	//---------------------------------------------------------------------
   void
	   TraCINodeMovementCreator::
	   close()
   {
	   if(!socket_){
		   cerr << "Error in method TraCINodeMovementCreator::close(): socket_ == NULL" << endl;
		   abort();
	   }
	   Storage out;
	   out.writeByte(2);
	   out.writeChar(CMD_CLOSE);
	   try{
		socket_->sendExact(out);
	   }
	   catch(SocketException e){
			cerr << "Error in method TraCINodeMovementCreator::close(): "
						 << e.what() << endl;
			delete socket_;
			socket_ = NULL;
			abort();
	   }
	   
		delete socket_;
		socket_ = NULL;
   }
	//---------------------------------------------------------------------
   void
	   TraCINodeMovementCreator::
	   fetch_processor_factories()
   {
	   for(int i = 0; i < sizeof(DOMAIN_IDS)/sizeof(int); ++i){
		   ProcessorFactoryHandle pfh;
		   set<ProcessorFactoryHandle>* processor_factories = new set<ProcessorFactoryHandle>();
		   StrTok tok(sc_.environment().optional_string_param(DOMAIN_NAMES[i] + "_processors",""), ", ");
		   for(StrTok::iterator it = tok.begin(); it!=tok.end(); ++it){
				pfh = sc_.processor_keeper_w().find_w( *it );
				assert( pfh != NULL );
				processor_factories->insert(pfh);
			}
		   domain_processor_factories_.
			   insert(pair<int,set<ProcessorFactoryHandle>*>(DOMAIN_IDS[i],processor_factories));
	   }
   }
	//---------------------------------------------------------------------
   void 
	   TraCINodeMovementCreator::
	   add_node_change_listener( TraCINodeChangeListener& listener ) 
	   throw()
   {
	   node_change_listeners_.push_back(&listener);
   }
	//---------------------------------------------------------------------
   Node* 
	   TraCINodeMovementCreator::
	   new_node(const TraCIID& traci_id, bool attach_processors_by_domain)
   {
	   // If node exists return NULL
	   if (find_node_by_traci_id_w(traci_id)) return NULL;

	   Node* node = new Node();
	   add_node(node,traci_id);

	   if (attach_processors_by_domain){
		   map<int, set<ProcessorFactoryHandle>* >::const_iterator it1 = 
			   domain_processor_factories_.find(traci_id.domain());

		   if (it1 != domain_processor_factories_.end()){
			   set<ProcessorFactoryHandle>* processor_factories = it1->second;
			   for (set<ProcessorFactoryHandle>::const_iterator it2 = processor_factories->begin();
				   it2 != processor_factories->end(); ++it2){
					ProcessorHandle ph = it2->get()->create();
					node->add_processor(ph);
			   }
		   }
	   }
	   return node;
   }
	//---------------------------------------------------------------------
   void 
	   TraCINodeMovementCreator::
	   check_for_unused_vehicle_nodes()
   {
	   for (World::node_iterator ni = sc_.world_w().begin_nodes_w(); 
		    ni != sc_.world_w().end_nodes_w(); ++ni){

			TraCIID ti;
			try{
				ti = find_traci_id_by_node(*ni);
			}catch(logic_error e){
				// Not found
				continue;
			}
			if (ti.domain() == 0x01 && current_vehicle_nodes_.find(ti) == current_vehicle_nodes_.end()){
				sc_.world_w().remove_node(*ni);
			}
	   }
   }
   //---Movement-Simulator-Commands---
   //---------------------------------------------------------------------
   void 
	   TraCINodeMovementCreator::
	   command_set_maximum_speed(const Node& node, double max_speed)
   {
	   if (!socket_){
		   cerr << "Error in method TraCINodeMovementCreator::command_set_maximum_speed, \
						socket_ == NULL" << endl;
		   abort();
	   }

	   // Build command
	   Storage out;
	   // Length of command
	   out.writeUnsignedByte(10);
	   // CommandID
	   out.writeChar(CMD_SETMAXSPEED);
	   // Node ID
	   TraCIID ti;
	   try{
			ti = find_traci_id_by_node(node);
	   }catch(logic_error e){
		   cerr << "Error in method TraCINodeMovementCreator::command_set_maximum_speed: " << e.what() << endl;
		   abort();
	   }
	   check_domain(0x01,ti,true,"Error in method TraCINodeMovementCreator::command_set_maximum_speed: \
					Command not applicable for elements in domain " + number_to_string(ti.domain()) + "!");
	   out.writeInt(ti.id());
	   // Max speed
	   out.writeFloat(static_cast<float>(max_speed));

	   // Send command

	   try{
		   socket_->sendExact(out);
	   }catch(SocketException e){
		   cerr << "Error in method TraCINodeMovementCreator::command_set_maximum_speed \
						while sending: " << e.what() << endl;
		   abort();
	   }
	
	   Storage in;
	   try{
		   socket_->receiveExact(in);
	   }catch(SocketException e){
		   cerr << "Error in method TraCINodeMovementCreator::command_set_maximum_speed \
						while receiving: " << e.what() << endl;
		   abort();
	   }

	   // First result command serves as status informatin
	   string description;
	   extract_command_status(in, CMD_SETMAXSPEED, description);
   }
	//---------------------------------------------------------------------
   void 
	   TraCINodeMovementCreator::
	   command_stop(const Node& node, double x, double y, double radius, double wait_time)
   {
	   if (!socket_){
		   cerr << "Error in method TraCINodeMovementCreator::command_stop, \
						socket_ == NULL" << endl;
		   abort();
	   }

	   // Build command
	   Storage out;
	   // Length of command
	   out.writeUnsignedByte(27);
	   // Command ID
	   out.writeChar(CMD_STOP);
	   // Node ID
	   TraCIID ti;
	   try{
			//out.writeInt(find_traci_id_by_node(node).id());
		   ti = find_traci_id_by_node(node);
	   }catch(logic_error e){
		   cerr << "Error in method TraCINodeMovementCreator::command_stop: " << e.what() << endl;
		   abort();
	   }
	   check_domain(0x01,ti,true,"Error in method TraCINodeMovementCreator::command_stop: \
					Command not applicable for elements in domain " + number_to_string(ti.domain()) + "!");
	   out.writeInt(ti.id());
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
	   }catch(SocketException e){
		   cerr << "Error in method TraCINodeMovementCreator::command_stop while sending: "
					 << e.what() << endl;
		   abort();
	   }
		
	   Storage in;
	   try{
		   socket_->receiveExact(in);
	   }catch(SocketException e){
		   cerr << "error in method TraCINodeMovementCreator::command_stop while recieving: "
					 << e.what() << endl;
		   abort();
	   }

		// First result command serves as status information
	   string description;
	   extract_command_status(in, CMD_STOP, description);	
   }
	//---------------------------------------------------------------------
   void 
	   TraCINodeMovementCreator::
	   command_change_route(const Node& node, string road_id, double travel_time)
   {
	   if (!socket_){
		   cerr << "Error in method TraCINodeMovementCreator::command_change_route, \
						socket == NULL" << endl;
		   abort();
	   }

	   // Build command
	   Storage out;
	   // Length of command
	   out.writeUnsignedByte(18+road_id.length());
	   // Command ID
	   out.writeChar(CMD_CHANGEROUTE);
	   // Node ID
	   TraCIID ti;
	   try{
			ti = find_traci_id_by_node(node);
	   }catch(logic_error e){
		   cerr << "Error in method TraCINodeMovementCreator::command_change_route: " << e.what() << endl;
		   abort();
	   }
	   check_domain(0x01,ti,true,"Error in method TraCINodeMovementCreator::command_change_route: \
					Command not applicable for elements in domain " + number_to_string(ti.domain()) + "!");
	   out.writeInt(ti.id());
	   // Road ID
	   out.writeString(road_id);
	   // Travel time
	   out.writeDouble(travel_time);

	   // Send command
	   try{
		   socket_->sendExact(out);
	   }catch(SocketException e){
		   cerr << "Error in method TraCINodeMovementCreator::coammand_change_route while sending: " 
			   << e.what() << endl;
		   abort();
	   }
	   Storage in;
	   try{
		   socket_->receiveExact(in);
	   }catch(SocketException e){
		   cerr << "Error in method TraCINodeMovementCreator::command_change_route while receiving: " 
					 << e.what() << endl;
		   abort();
	   }

	   string description;
	   extract_command_status(in, CMD_CHANGEROUTE, description);
   }
	//---------------------------------------------------------------------
   void 
	   TraCINodeMovementCreator::
	   command_change_lane(const Node& node, char lane, float time)
   {
		if (!socket_){
		   cerr << "Error in method TraCINodeMovementCreator::command_change_lane, \
						socket == NULL" << endl;
		   abort();
	   }

		
	  // Build command
	  Storage out;
	  // Length of command
	  out.writeUnsignedByte(9);
	  // Command ID
	  out.writeChar(CMD_CHANGELANE);
	  // Node ID
	  TraCIID ti;
	  try{
		  ti = find_traci_id_by_node(node);
	  }catch(logic_error e){
		   cerr << "Error in method TraCINodeMovementCreator::command_change_lane: " << e.what() << endl;
		   abort();
	  }
	  check_domain(0x01,ti,true,"Error in method TraCINodeMovementCreator::command_change_lane: \
				   Command not applicable for elements in domain " + number_to_string(ti.domain()) + "!");
	  out.writeInt(ti.id());
	  // Lane 
	  out.writeByte(lane);
	  // Time
	  out.writeFloat(time);

	  // Send command
	   try{
		   socket_->sendExact(out);
	   }catch(SocketException e){
		   cerr << "Error in method TraCINodeMovementCreator::coammand_change_lane while sending: " 
			   << e.what() << endl;
		   abort();
	   }
	   Storage in;
	   try{
		   socket_->receiveExact(in);
	   }catch(SocketException e){
		   cerr << "Error in method TraCINodeMovementCreator::command_change_lane while receiving: " 
					 << e.what() << endl;
		   abort();
	   }

	   string description;
	   extract_command_status(in, CMD_CHANGELANE, description);

   }
	//---------------------------------------------------------------------
	bool 
		TraCINodeMovementCreator::
		command_roadmap_position(const Node& node, double targetTime, string& roadId_, float& relPosition_, string& laneID_)
	{
		// Preconditions
		if (socket_ == NULL)
		{
			cerr << "Error in method MobilityInterfaceClient::command_roadmap_position, socket_ == NULL" << endl;
			abort();
		}
     
		target_time_ = targetTime;
	 
		//cout << "Target time to retrieve: " << target_time_ << endl;
		// build command
		Storage out;
		// Length of command
		out.writeByte(11);
		// CommandID
		out.writeChar(CMD_SIMSTEP);
		// TargetTime
		out.writeDouble(targetTime);
		// ResultType (Position representation)
		out.writeChar(POSITION_ROADMAP);
	 
		// Send command
		try{
			socket_->sendExact(out);
		}
		catch (SocketException e){
			cerr << "Error in method MobilityInterfaceClient::command_roadmap_position while sending: " << e.what() << endl;
			abort();
		}
	
		Storage in;
		try{
			socket_->receiveExact(in);
		}
		catch (SocketException e){
			cerr << "Error in method MobilityInterfaceClient::command_roadmap_position while receiving: " << e.what() << endl;
			abort();
		}

		// First result command serves as status information
		string description;
		extract_command_status(in, CMD_SIMSTEP, description);
   
		int id = 0;
		double destTime = 0.0;
		string roadId = "";
		float relPosition = 0.0;
		//int laneID = 0;
   
		unsigned int commandStart = 0;
		unsigned int commandLength = 0;

		while (in.valid_pos()){
			// Save start and length of next command
			commandStart = in.position();
			commandLength = in.readByte();

         
			if ( unsigned char commandId = in.readChar() != CMD_MOVENODE){
				// move node command expected
				cerr << "Error in method TraCINodeMovementCreator::command_roadmap_position, move node command expected. \
						Got commandId " << commandId << endl;
				abort();
			}

			// Read node address, end time and destination position (road)
			id = in.readInt();
			destTime = in.readDouble();
     
			// PositionType
			if (in.readByte() == POSITION_ROADMAP ){
				roadId = in.readString(); 
				relPosition = in.readFloat();
				//laneID = in.readByte();
            
				// Update the node's road map position
				TraCIID ti;
				try{
					ti = find_traci_id_by_node(node);
				}catch(logic_error e){
					cerr << "Error in method TraCINodeMovementCreator::command_roadmap_position: " << e.what() << endl;
					abort();
				}
				//check_domain(0x01,ti,true,"Error in method TraCINodeMovementCreator::command_roadmap_position: \
				//			 Command not applicable for elements in domain " + number_to_string(ti.domain()) + "!");
				if(ti.id() == id){
					//string laneIdString = "";
					roadId_ = roadId;
					relPosition_ = relPosition;
					// Consider only the first lane (this should be changed)
					laneID_ = roadId_ + "_0";
					//laneID_.append("_"); 
					//laneID_ = roadId + "_" + laneID;
					//cout << "TraCIClient.commandRoadmapPosition() ---> " << destTime << ", " << id << ", " << roadId << ", " << relPosition << endl;
				}
       
			}   
		}
     return true;
	}
	//---------------------------------------------------------------------
	bool 
		TraCINodeMovementCreator::
		command_scenario_integer(bool write, int domain, int domainId, int variable, int& value)
	{
		// Preconditions
		if (socket_ == NULL){
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioInteger, socket_ == NULL" << endl;
			abort();
		}
 
		// build command
		Storage out;
		// Length of command  
		if (write) {
			out.writeUnsignedByte(1+1+1+1+4+1+1+4);
		} else {
			out.writeUnsignedByte(1+1+1+1+4+1+1);
		}
		// CommandID
		out.writeChar(CMD_SCENARIO);
		// write Flage
		out.writeUnsignedByte(write?1:0);
		// Domain
		out.writeUnsignedByte(domain);
		// DomainId
		out.writeInt(domainId);
		// Variable
		out.writeUnsignedByte(variable);
		// DataType is fixed to integer
		out.writeUnsignedByte(TYPE_INTEGER);
		// value (datatype int)
		if (write) out.writeInt(value);
 
		// Send command
		try{
			socket_->sendExact(out);
		}
		catch (SocketException e){
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioInteger while sending: " << e.what() << endl;
			abort();
		}
 
		Storage in;
		try{
			socket_->receiveExact(in);
		}
		catch (SocketException e){
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioInteger while receiving: " << e.what() << endl;
			abort();
		}
 
		// First result command serves as status information
		string description;
		extract_command_status(in, CMD_SCENARIO, description);
 
		// Next received command ahs the same structure as the send command and contains the always the value-field
		if (!in.valid_pos()){
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioInteger, expected additional answer command" << endl;
			abort();
		}
 
		unsigned int commandStart = in.position();
		unsigned int commandLength = in.readUnsignedByte();
 
		if (unsigned char commandId = in.readChar() != CMD_SCENARIO){
			// scenario command expected
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioInteger while parsing answer command, \
					scenario command expected. Got commandId " << showbase << hex << commandId << endl;
			abort();
		}
 
		if (bool writeFlag = static_cast<bool>(in.readUnsignedByte()) != write){
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioInteger while parsing answer command, writeFlag=" 
				<< write << " expected. Got writeFlag=" << writeFlag << endl;
			abort();
		}
 
		if (int rcvdDomain = in.readUnsignedByte() != domain){
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioInteger while parsing answer command, domain=" 
				<< domain << " expected. Got domain=" << rcvdDomain << endl;
			abort();
		}
 
		if (int rcvdDomainId = in.readInt() != domainId){
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioInteger while parsing answer command, domainId=" 
				<< domainId << " expected. Got domainId=" << rcvdDomainId << endl;
			abort();
		}
 
		if (int rcvdVariable = in.readUnsignedByte() != variable){
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioInteger while parsing answer command, variable=" 
				<< variable << " expected. Got variable=" << rcvdVariable << endl;
			abort();
		}

		if (int rcvdDataType = in.readUnsignedByte() != TYPE_INTEGER){
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioInteger while parsing answer command, datatype integer \
					expected. Got datatype=" << rcvdDataType << endl;
			abort();
		}
 
		value = in.readInt();
 
		// Right length???
		if (commandStart + commandLength != in.position()){
			// Last read command has wrong length
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioInteger, command at position " 
				 << commandStart << " was read with wrong length - expected " << commandLength << " but was " << 
				 in.position()-commandStart << endl;
			abort();
		}

	return true;
	}
	//---------------------------------------------------------------------
	bool 
		TraCINodeMovementCreator::
		command_scenario_string(bool write, int domain, int domainId, int variable, string& value)
	{
		// Preconditions
		if (socket_ == NULL)
		{
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioString, socket_ == NULL" << endl;
			abort();
		}
 
		// build command
		Storage out;
		// Length of command  
		if (write) {
			out.writeUnsignedByte(1+1+1+1+4+1+1+ 4 + value.length());
		} else {
			out.writeUnsignedByte(1+1+1+1+4+1+1);
		}
		// CommandID
		out.writeChar(CMD_SCENARIO);
		// write Flage
		out.writeUnsignedByte(write?1:0);
		// Domain
		out.writeUnsignedByte(domain);
		// DomainId
		out.writeInt(domainId);
		// Variable
		out.writeUnsignedByte(variable);
		// DataType is fixed to integer
		out.writeUnsignedByte(TYPE_STRING);
		// value (datatype int)
		if (write) out.writeString(value);
 
		// Send command
		try
		{
			socket_->sendExact(out);
		}
		catch (SocketException e)
		{
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioString while sending: " << e.what() << endl;
			abort();
		}
 
		Storage in;
		try
		{
			socket_->receiveExact(in);
		}
		catch (SocketException e)
		{
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioString while receiving: " << e.what() << endl;
			abort();
		}
 
		// First result command serves as status information
		string description;
		extract_command_status(in, CMD_SCENARIO, description);
 
		// Next received command ahs the same structure as the send command and contains the always the value-field
		if (!in.valid_pos())
		{
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioString, expected additional answer command" << endl;
			abort();
		}
 
		unsigned int commandStart = in.position();
		unsigned int commandLength = in.readUnsignedByte();
		if (commandLength == 0) commandLength = in.readInt();
 
		if (unsigned char commandId = in.readChar() != CMD_SCENARIO)
		{
			// scenario command expected
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioString while parsing answer command, \
					scenario command expected. Got commandId " << showbase << hex << commandId << endl;
			abort();
		}
 
		if (bool writeFlag = static_cast<bool>(in.readUnsignedByte()) != write)
		{
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioString while parsing answer command, writeFlag=" << write 
				 << " expected. Got writeFlag=" << writeFlag << endl;
			abort();
		}
 
		if (int rcvdDomain = in.readUnsignedByte() != domain)
		{
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioString while parsing answer command, domain=" << domain 
				 << " expected. Got domain=" << rcvdDomain << endl;
			abort();
		}
 
		if (int rcvdDomainId = in.readInt() != domainId)
		{
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioString while parsing answer command, domainId=" << domainId 
				 << " expected. Got domainId=" << rcvdDomainId << endl;
			abort();
		}
 
		if (int rcvdVariable = in.readUnsignedByte() != variable)
		{
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioString while parsing answer command, variable=" << variable 
				 << " expected. Got variable=" << rcvdVariable << endl;
			abort();
		}
 
		if (int rcvdDataType = in.readUnsignedByte() != TYPE_STRING)
		{
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioString while parsing answer command, datatype integer expected.\
					Got datatype=" << rcvdDataType << endl;
			abort();
		}
 
		value = in.readString();
 
		// Right length???
		if (commandStart + commandLength != in.position())
		{
			// Last read command has wrong length
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioString, command at position " 
				 << commandStart << " was read with wrong length - expected " << commandLength << " but was " 
				 << in.position()-commandStart << endl;
			abort();
		}
 
		return true;
	}
	//---------------------------------------------------------------------
	bool 
		TraCINodeMovementCreator::
		command_scenario_pos3d(bool write, int domain, int domainId, int variable, float& x, float& y, float& z)
	{
		// Preconditions
		if (socket_ == NULL){
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioPos3d, socket_ == NULL" << endl;
			abort();
		}
 
		// build command
		Storage out;
		// Length of command  
		if (write) {
			out.writeUnsignedByte(1+1+1+1+4+1+1+4+4+4);
		} else {
			out.writeUnsignedByte(1+1+1+1+4+1+1);
		}
		// CommandID
		out.writeChar(CMD_SCENARIO);
		// write Flage
		out.writeUnsignedByte(write?1:0);
		// Domain
		out.writeUnsignedByte(domain);
		// DomainId
		out.writeInt(domainId);
		// Variable
		out.writeUnsignedByte(variable);
		// DataType is fixed to integer
		out.writeUnsignedByte(POSITION_3D);
		// value (datatype int)
		if (write){
			out.writeFloat(x);
			out.writeFloat(y);
			out.writeFloat(z);
		}
 
		// Send command
		try{
			socket_->sendExact(out);
		}
		catch (SocketException e){
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioPos3d while sending: " << e.what() << endl;
			abort();
		}
 
		Storage in;
		try{
			socket_->receiveExact(in);
		}
		catch (SocketException e){
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioPos3d while receiving: " << e.what() << endl;
			abort();
		}
 
		// First result command serves as status information
		string description;
		extract_command_status(in, CMD_SCENARIO, description);
 
		// Next received command has the same structure as the send command and contains the always the value-field
		if (!in.valid_pos()){
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioPos3d, expected additional answer command" << endl;
			abort();
		}
 
		unsigned int commandStart = in.position();
		unsigned int commandLength = in.readUnsignedByte();
 
		if (unsigned char commandId = in.readChar() != CMD_SCENARIO){
			// scenario command expected
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioPos3d while parsing answer command, scenario command\
					expected. Got commandId " << showbase << hex << commandId << endl;
			abort();
		}
 
		if (bool writeFlag = static_cast<bool>(in.readUnsignedByte()) != write){
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioPos3d while parsing answer command, writeFlag=" 
				 << write << " expected. Got writeFlag=" << writeFlag << endl;
			abort();
		}
 
		if (int rcvdDomain = in.readUnsignedByte() != domain){
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioPos3d while parsing answer command, domain=" << domain 
				 << " expected. Got domain=" << rcvdDomain << endl;
			abort();
		}
 
		if (int rcvdDomainId = in.readInt() != domainId){
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioPos3d while parsing answer command, domainId=" << domainId 
				 << " expected. Got domainId=" << rcvdDomainId << endl;
			abort();
		}
 
		if (int rcvdVariable = in.readUnsignedByte() != variable){
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioPos3d while parsing answer command, variable=" << variable 
				 << " expected. Got variable=" << rcvdVariable << endl;
			abort();
		}
 
		if (int rcvdDataType = in.readUnsignedByte() != POSITION_3D){
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioPos3d while parsing answer command, datatype integer \
					expected. Got datatype=" << rcvdDataType << endl;
			abort();
		}
 
		x = in.readFloat();
		y = in.readFloat();
		z = in.readFloat();
 
		// Right length???
		if (commandStart + commandLength != in.position()){
			// Last read command has wrong length
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioPos3d, command at position " 
				 << commandStart << " was read with wrong length - expected " << commandLength 
				 << " but was " << in.position()-commandStart << endl;
			abort();
		}
 
		return true;
	}
	//---------------------------------------------------------------------
	bool 
		TraCINodeMovementCreator::
		command_scenario_bounding_box(bool write, int domain, int domainId, int variable, float& x1, float& y1, float& x2, float& y2)
	{
		// Preconditions
		if (socket_ == NULL)
		{
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioBoundingBox, socket_ == NULL" << endl;
			abort();
		}
 
		// build command
		Storage out;
		// Length of command  
		if (write) {
			out.writeUnsignedByte(1+1+1+1+4+1+1+4+4+4+4);
		} else {
			out.writeUnsignedByte(1+1+1+1+4+1+1);
		}
		// CommandID
		out.writeChar(CMD_SCENARIO);
		// write Flage
		out.writeUnsignedByte(write?1:0);
		// Domain
		out.writeUnsignedByte(domain);
		// DomainId
		out.writeInt(domainId);
		// Variable
		out.writeUnsignedByte(variable);
		// DataType is fixed to integer
		out.writeUnsignedByte(TYPE_BOUNDINGBOX);
		// value (datatype int)
		if (write)
		{
			out.writeFloat(x1);
			out.writeFloat(y1);
			out.writeFloat(x2);
			out.writeFloat(y2);
		}
 
		// Send command
		try
		{
			socket_->sendExact(out);
		}
		catch (SocketException e)
		{
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioBoundingBox while sending: " 
				 << e.what() << endl;
			abort();
		}
 
		Storage in;
		try
		{
			socket_->receiveExact(in);
		}
		catch (SocketException e)
		{
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioBoundingBox while receiving: " 
				 << e.what() << endl;
			abort();
		}
 
		// First result command serves as status information
		string description;
		extract_command_status(in, CMD_SCENARIO, description);
 
		// Next received command has the same structure as the send command and contains the always the value-field
		if (!in.valid_pos())
		{
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioBoundingBox, \
					expected additional answer command" << endl;
			abort();
		}
 
		unsigned int commandStart = in.position();
		unsigned int commandLength = in.readUnsignedByte();
 
		if (unsigned char commandId = in.readChar() != CMD_SCENARIO)
		{
			// scenario command expected
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioBoundingBox while parsing answer command, \
					scenario command expected. Got commandId " << showbase << hex << commandId << endl;
			abort();
		}
 
		if (bool writeFlag = static_cast<bool>(in.readUnsignedByte()) != write)
		{
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioBoundingBox while parsing answer command, \
					writeFlag=" << write << " expected. Got writeFlag=" << writeFlag << endl;
			abort();
		}
 
		if (int rcvdDomain = in.readUnsignedByte() != domain)
		{
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioBoundingBox while parsing answer command, \
					domain=" << domain << " expected. Got domain=" << rcvdDomain << endl;
			abort();
		}
 
		if (int rcvdDomainId = in.readInt() != domainId)
		{
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioBoundingBox while parsing answer command, \
					domainId=" << domainId << " expected. Got domainId=" << rcvdDomainId << endl;
			abort();
		}
 
		if (int rcvdVariable = in.readUnsignedByte() != variable)
		{
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioBoundingBox while parsing answer command, \
					variable=" << variable << " expected. Got variable=" << rcvdVariable << endl;
			abort();
		}
 
		if (int rcvdDataType = in.readUnsignedByte() != TYPE_BOUNDINGBOX)
		{
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioBoundingBox while parsing answer command, \
					datatype integer expected. Got datatyoe=" << rcvdDataType << endl;
			abort();
		}
 
		x1 = in.readFloat();
		y1 = in.readFloat();
		x2 = in.readFloat();
		y2 = in.readFloat();
 
		// Right length???
		if (commandStart + commandLength != in.position())
		{
			// Last read command has wrong length
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioBoundingBox, command at position " 
			<< commandStart << " was read with wrong length - expected " << commandLength << " but was " 
			<< in.position()-commandStart << endl;
			abort();
		}
 
     return true;
	}
	//---------------------------------------------------------------------
	float 
		TraCINodeMovementCreator::
		command_scenario_distance_to_roadmap_position(bool write, int domain, int domainId, int variable, 
													  string roadId, float roadPos, int lane)
	{
		// Preconditions
		if (socket_ == NULL){
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioDistanceToRoadmapPosition, socket_ == NULL" << endl;
			abort();
		}
 
		// build command
		Storage out;
		// Length of command  
		//if (write) {
			out.writeUnsignedByte(1+1+1+1+4+1+1+ 4 + roadId.length() + 4 + 1);
		//} else {
			//out.writeUnsignedByte(1+1+1+1+4+1+1);
		//}
		// CommandID
		out.writeChar(CMD_SCENARIO);
		// write Flage
		out.writeUnsignedByte(write?1:0);
		// Domain
		out.writeUnsignedByte(domain);
		// DomainId
		out.writeInt(domainId);
		// Variable
		out.writeUnsignedByte(variable);
		// DataType is fixed to integer
		out.writeUnsignedByte(POSITION_ROADMAP);
		// value
		out.writeString(roadId);
		out.writeFloat(roadPos);
		out.writeUnsignedByte(lane);
		// Send command
		try{
			socket_->sendExact(out);
		}
		catch (SocketException e){
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioDistanceToRoadmapPosition while sending: " 
				 << e.what() << endl;
			abort();
		}
 
		Storage in;
		try{
			socket_->receiveExact(in);
		}
		catch (SocketException e){
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioDistanceToRoadmapPosition while receiving: " 
				 << e.what() << endl;
			abort();
		}
 
		// First result command serves as status information
		string description;
		extract_command_status(in, CMD_SCENARIO, description);
 
		// Next received command has the same structure as the send command and contains the always the value-field
		if (!in.valid_pos()){
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioDistanceToRoadmapPosition, \
					expected additional answer command" << endl;
			abort();
		}
 
		unsigned int commandStart = in.position();
		unsigned int commandLength = in.readUnsignedByte();
 
		if (unsigned char commandId = in.readChar() != CMD_SCENARIO){
			// scenario command expected
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioDistanceToRoadmapPosition while parsing answer command, \
					scenario command expected. Got commandId " << showbase << hex << commandId << endl;
			abort();
		}
 
		if (bool writeFlag = static_cast<bool>(in.readUnsignedByte()) != write){
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioDistanceToRoadmapPosition while parsing answer command, \
					writeFlag=" << write << " expected. Got writeFlag=" << writeFlag << endl;
			abort();
		}
 
		if (int rcvdDomain = in.readUnsignedByte() != domain){
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioDistanceToRoadmapPosition while parsing answer command, \
					domain=" << domain << " expected. Got domain=" << rcvdDomain << endl;
			abort();
		}
 
		if (int rcvdDomainId = in.readInt() != domainId){
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioDistanceToRoadmapPosition while parsing answer command, \
					domainId=" << domainId << " expected. Got domainId=" << rcvdDomainId << endl;
			abort();
		}
 
		if (int rcvdVariable = in.readUnsignedByte() != variable){
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioDistanceToRoadmapPosition while parsing answer command, \
					variable=" << variable << " expected. Got variable=" << rcvdVariable << endl;
			abort();
		}
 
		if (int rcvdDataType = in.readUnsignedByte() != TYPE_FLOAT){
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioDistanceToRoadmapPosition while parsing answer command, \
					datatype float expected. Got datatype=" << rcvdDataType << endl;
			abort();
		}
 
		float distance = in.readFloat();
 
		// Right length???
		if (commandStart + commandLength != in.position()){
			// Last read command has wrong length
			cerr << "Error in method TraCINodeMovementCreator::commandScenarioBoundingBox, command at position " 
				 << commandStart << " was read with wrong length - expected " << commandLength << " but was " 
				 << in.position()-commandStart << endl;
			abort();
		}
 
		return distance;
	}
	//---------------------------------------------------------------------
	bool 
		TraCINodeMovementCreator::
		command_get_TL_status(int tlId, double from, double until, TLSwitchInfoVec& switchInfos)
	{
		// Preconditions
		if (socket_ == NULL){
			cerr << "Error in method TraCINodeMovementCreator::commandgetTLStatus, socket_ == NULL" << endl;
			abort();
		}
     
		// build command
		Storage out;
		// Length of command
		out.writeByte(1+1+4+8+8);
		// CommandID
		out.writeChar(CMD_GETTLSTATUS);
		// traffic light id
		out.writeInt(tlId);
		// timefrom
		out.writeDouble(from + feed_time_);
		// timeto
		out.writeDouble(until + feed_time_);
 
		// Send command
		try{
			socket_->sendExact(out);
		}
		catch (SocketException e){
			cerr << "Error in method TraCINodeMovementCreator::commandgetTLStatus while sending: " 
				 << e.what() << endl;
			abort();
		}
 
		Storage in;
		try{
			socket_->receiveExact(in);
		}
		catch (SocketException e){
		cerr << "Error in method TraCINodeMovementCreator::commandgetTLStatus while receiving: " 
			 << e.what() << endl;
			abort();
		}
		// First result command serves as status information
		string description;
		extract_command_status(in, CMD_GETTLSTATUS, description);
 
		unsigned int commandStart = 0;
		unsigned int commandLength = 0;
 
		while (in.valid_pos()){
			// Save start and length of next command
			commandStart = in.position();
			commandLength = in.readByte();
			TLSwitchInfo tlswitch;
 
			if (unsigned char commandId = in.readChar() != CMD_TLSWITCH){
				// move node command expected
				cerr << "Error in method TraCINodeMovementCreator::commandgetTLStatus, TLSWITCH command expected. Got commandId " 
					 << showbase << hex << commandId << endl;
				abort();
			}
 
			// Read switchTime, fromEdge, toEdge, newStatus (TLPHASE_GREEN or TLPHASE_RED) and yellowTime
			tlswitch.switchTime = in.readDouble() - feed_time_;
			tlswitch.fromEdge = in.readString();
			tlswitch.fromEdgePos = in.readFloat();
			tlswitch.toEdge = in.readString();
			tlswitch.newStatus = in.readUnsignedByte();
			if (tlswitch.newStatus != TLPHASE_RED && tlswitch.newStatus != TLPHASE_GREEN){
				cerr << "Error in method TraCINodeMovementCreator::commandgetTLStatus while parsing answer command, new phase <" 
					 << tlswitch.newStatus << "> unknown." << endl;
				abort();
			}
			tlswitch.yellowTime = in.readDouble();
 
			// Correct length???
			if (commandStart + commandLength != in.position()){
				// Last read command has wrong length
				cerr << "Error in method TraCINodeMovementCreator::commandgetTLStatus, command at position " 
					 << commandStart << " was read with wrong length - expected " << commandLength << " but was " 
					 << in.position()-commandStart << endl;
				abort();
			}
			switchInfos.push_back(tlswitch);
		}
		return true;
	}
	//---------------------------------------------------------------------
	const Node* 
		TraCINodeMovementCreator::
		find_node_by_traci_id(const TraCIID& traci_id)
	{
		return find_node_by_traci_id_w(traci_id);
	}
	//---------------------------------------------------------------------
	Node* 
		TraCINodeMovementCreator::
		find_node_by_traci_id_w(const TraCIID& traci_id)
	{
		TraCIIDToNodeMap::iterator it = traci_ids_to_node_.find(traci_id);
		return it != traci_ids_to_node_.end() ? it->second : NULL;
	}
	//---------------------------------------------------------------------
	const TraCINodeMovementCreator::TraCIID& 
		TraCINodeMovementCreator::
		find_traci_id_by_node(const Node& node)
		throw(logic_error)
	{
		NodeToTraCIIDMap::const_iterator it = node_to_traci_ids_.find(const_cast<Node*>(&node));
		if (it == node_to_traci_ids_.end()) throw logic_error("node not found");
		return it->second;
	}
	//---------------------------------------------------------------------
	void 
	   TraCINodeMovementCreator::
	   add_node(Node* node, const TraCIID& traci_id)
	{	
		assert(node);
		sc_.world_w().add_node( *node );
		assert(node_to_traci_ids_.find(node) == node_to_traci_ids_.end());
		assert(traci_ids_to_node_.find(traci_id) == traci_ids_to_node_.end());

		node_to_traci_ids_.insert(make_pair<Node*,TraCIID>(node,traci_id));
		traci_ids_to_node_.insert(make_pair<TraCIID,Node*>(traci_id,node));
		assert(traci_ids_to_node_.size() == node_to_traci_ids_.size());

		// Inform the listeners
		for (TraCINodeChangeListenerList::iterator it = node_change_listeners_.begin();
			 it != node_change_listeners_.end(); ++it){
				 (*it)->node_added(*node,traci_id);
		}
	}
	//---------------------------------------------------------------------
	void 
		TraCINodeMovementCreator::
		remove_node_from_map(Node* node)
	{
		assert(node);

		TraCIID ti; 
		try{
			ti = find_traci_id_by_node(*node);
		}catch(logic_error){
			assert(traci_ids_to_node_.size() == node_to_traci_ids_.size());
			// Not found, nothing to remove!
			return;
		}

		// Inform the listeners
		for (TraCINodeChangeListenerList::iterator it = node_change_listeners_.begin();
			 it != node_change_listeners_.end(); ++it){
				 (*it)->node_removed(*node,ti);
		}

		TraCIIDToNodeMap::iterator ttn = traci_ids_to_node_.find(ti);
		NodeToTraCIIDMap::iterator ntt = node_to_traci_ids_.find(node);
		if ( ttn != traci_ids_to_node_.end() ){
			//assert(ttn != traci_ids_to_node_.end());
			traci_ids_to_node_.erase(ttn);
		}
		if ( ntt != node_to_traci_ids_.end() ){
			//assert(ntt != node_to_traci_ids_.end());
			node_to_traci_ids_.erase(ntt);
		}
		assert(traci_ids_to_node_.size() == node_to_traci_ids_.size());
	}
	//---------------------------------------------------------------------
	bool 
		TraCINodeMovementCreator::
		check_domain(int domain, const TraCIID& traci_id, bool abrt, string err_msg)
	{
		if (domain == traci_id.domain()) return true;
		if (abrt){
			cerr << err_msg << endl;
			abort();
		}
		return false;
	}
	//---------------------------------------------------------------------
	std::string 
		TraCINodeMovementCreator::
		number_to_string(double n)
	{
		stringstream oss;
		oss << n;
		return oss.str();
	}
	//---------------------------------------------------------------------
	traci::TraCINodeMovementCreator::NodeToTraCIIDMap::const_iterator 
		TraCINodeMovementCreator::
		begin_traci_nodes()
		const 
		throw()
	{
		return node_to_traci_ids_.begin();
	}
	//---------------------------------------------------------------------
	traci::TraCINodeMovementCreator::NodeToTraCIIDMap::const_iterator 
		TraCINodeMovementCreator::
		end_traci_nodes()
		const 
		throw()
	{
		return node_to_traci_ids_.end();
	}	
	//---------------------------------------------------------------------
	//---TraCIID---
	//---------------------------------------------------------------------
	TraCINodeMovementCreator::
		TraCIID::
		TraCIID() :
		domain_(-1),
		id_(-1)
	{}
	//---------------------------------------------------------------------
	TraCINodeMovementCreator::
		TraCIID::
		TraCIID(int d, int i) : 
		domain_(d), 
		id_(i)
	{}
	//---------------------------------------------------------------------
	TraCINodeMovementCreator::
		TraCIID::
		TraCIID(const TraCIID& o) : 
		domain_(o.domain()),
		id_(o.id())
	{}
	//---------------------------------------------------------------------
	TraCINodeMovementCreator::
		TraCIID::
		~TraCIID()
	{}
	//---------------------------------------------------------------------
	int 
		TraCINodeMovementCreator::
		TraCIID::
		domain()
		const
	{
		return domain_;
	}
	//---------------------------------------------------------------------
	int
		TraCINodeMovementCreator::
		TraCIID::
		id()
		const
	{
		return id_;
	}
	//---------------------------------------------------------------------
	traci::TraCINodeMovementCreator::TraCIID& 
		TraCINodeMovementCreator::
		TraCIID::
		operator=(const traci::TraCINodeMovementCreator::TraCIID& rv)
	{
		if(this == & rv) return *this;
		id_ = rv.id();
		domain_ = rv.domain();
		return *this;
	}
	//---------------------------------------------------------------------
	int 
		operator<(const traci::TraCINodeMovementCreator::TraCIID& left, 
				  const traci::TraCINodeMovementCreator::TraCIID& right)
	{
		if(left.domain_ < right.domain_) return 1;
		if(left.domain_ > right.domain_) return 0;
		if(left.id_ < right.id_) return 1;
		if(left.id_ > right.id_) return 0;
		return 0;
	}
	//---------------------------------------------------------------------
	int 
		operator>(const traci::TraCINodeMovementCreator::TraCIID& left, 
				  const traci::TraCINodeMovementCreator::TraCIID& right)
	{
		return (right < left);
	}
	//---------------------------------------------------------------------
	int 
		operator==(const traci::TraCINodeMovementCreator::TraCIID& left, 
				   const traci::TraCINodeMovementCreator::TraCIID& right)
	{
		if(left.domain_ == right.domain_ && left.id_ == right.id_) return 1;
		return 0;
	}

}
#endif
