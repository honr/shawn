/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2007 by AutoNomos (www.auto-nomos.de)                **
 ** This part of Shawn is free software; you can redistribute it and/or**
 ** modify it under the terms of the BSD License. Refer to the		   **
 ** shawn-licence.txt file in the root of the Shawn source tree for	   **
 ** further details.												   **
 **                                                                    **
 ** \author Axel Wegener <wegener@itm.uni-luebeck.de>				   **
 ** \author Torsten Teubler <teubler@itm.uni-luebeck.de>			   **
 **                                                                    **
 ************************************************************************/

#include "autocast_processor.h"
#ifdef ENABLE_AUTOCAST

#include <iostream>
#include <fstream>
#include <math.h>
#include <assert.h>
#include <sstream>
#include <fstream>
#include <string>
#include "sys/node.h"
#include "sys/vec.h"
#include "sys/simulation/simulation_task_keeper.h"
#include "sys/transmission_model.h"
#include "apps/autocast/autocast/autocast_processor.h"
#include "apps/autocast/autocast/autocast_message.h"
#include "apps/autocast/autocast_task/autocast_task.h"

using namespace std;
using namespace shawn;

namespace autocast
{

   int autocast::AutoCastProcessor::uid_counter_ = 0;

   AutoCastProcessor::
	   AutoCastProcessor() : neighborhood_(0.0),
							 update_time_(1),
							 packets_sent_total_(0),
							 bytes_sent_total_(0),
							 dataUnits_sent_total_(0),
							 dataUnits_bytes_sent_total_(0),
							 received_DataUnits_total_(0),
							 max_update_packet_size_(AUTOCAST_MAX_PACKET_LENGTH),
							 max_update_data_units_(AUTOCAST_DEFAULT_MAX_DATAUNITS_PER_PACKET),
							 max_startup_time_(AUTOCAST_DEFAULT_STARTUP_TIME),
							 min_update_time_(AUTOCAST_DEFAULT_MIN_UPDATE_TIME),
							 max_update_time_(AUTOCAST_DEFAULT_MAX_UPDATE_TIME),
							 dataUnit_max_live_time_(AUTOCAST_DEFAULT_DATAUNIT_LIVETIME),
							 update_timer_(NULL),
							 answer_timer_(NULL),
							 request_timer_(NULL),
							 /*NEW*/flood_timer_(NULL)/*,
							 /*FOR DEBUG PURPOSES*/
							 /*neighbors_count_(0),
							 real_neighbors_count_(0),
							 velocity_count_(0),
							 update_time_count_(0)*/

   {}
   // ----------------------------------------------------------------------
   AutoCastProcessor::
   ~AutoCastProcessor()
   {
     clean_up();
   }
   // ----------------------------------------------------------------------
   void
   AutoCastProcessor::
   boot( void )
      throw()
   {
	   //std::cerr << "#: " << owner().id() << " boot called" << std::endl;
		neighborhood_.set_owner(&owner());
		fetch_parameters();
		
		double lb2 = owner().world().simulation_controller().environment().optional_double_param("_lb2__boot",0.9);
		double ub1 = owner().world().simulation_controller().environment().optional_double_param("_ub1__boot",1.1);
		update_timer_ = owner_w().world_w().scheduler_w().new_event(*this,fabs(max_startup_time_) * uniform_random(lb2,ub1),NULL);
		max_iterations_ = owner().world().simulation_controller().environment().required_int_param("max_iterations");
   }
   // ----------------------------------------------------------------------
   bool
   AutoCastProcessor::
   process_message( const ConstMessageHandle& mh ) 
      throw()
   {
	   const autocast::AutoCastMessage * acm = dynamic_cast<const autocast::AutoCastMessage*>(mh.get());
	   if (!acm){
		   std::cerr << "Cast to AutoCastMessage failed!" << std::endl;
		   return false;
	   }
	   if (acm->last_hop_addr() == owner().id()){
		   /// Prevent processor from receiving his sent messages
		   return false;
	   }
	   if(received_messages_ids_total_.find(acm->uid()) != received_messages_ids_total_.end()){
		   /// For debug purposes only
		   std::cerr << "Message with id " << acm->uid() << " already received!" << std::endl;
		   return false;
	   }
	   /// Now receive the packet!
	   received_messages_ids_total_.insert(acm->uid());
	   double now = owner().world().scheduler().current_time();
	   /// Update the local DataUnits
	   local_update();

	   neighborhood_.append_to_neighborhood( acm->last_hop_addr(),acm->next_update_interval() );

	   if (acm->packet_type() != AUTOCAST_TYPE_UPDATE){
		   std::cerr << "_" << acm->source().id() << "_received UNKNOWN packet @ " << owner().world().simulation_controller().world().scheduler().current_time() << std::endl;
		   std::cerr << "Type is " << acm->packet_type() << " and not " << AUTOCAST_TYPE_UPDATE << std::endl;
		   abort();
	   }

	   bool answer_decision = false;
	   int neighborhood_size = neighborhood_.neighborhood_size();

	   int barker = owner().world().simulation_controller().environment().optional_int_param("_barker__process_message",5);

	   /// Two of the neighborhood should flood the information or answer with unknown data
	   double lb1 = owner().world().simulation_controller().environment().optional_double_param("_lb1__process_message",0.0);
	   double ub1 = owner().world().simulation_controller().environment().optional_double_param("_ub1__process_message",1.0);
	   double factor = owner().world().simulation_controller().environment().optional_double_param("_factor__process_message",1.0);
	   answer_decision = (neighborhood_size <= barker || uniform_random(lb1,ub1) <= (factor * barker / neighborhood_size));

	   /// Get the known ID's from the sender
	   /// received_ids saves the ID's received in this "receiving round"
	   std::set<unsigned int> received_ids(acm->known_DataUnits());

	   /// Fetching the complete DataUnits and give it to the applications or
	   /// hold them in the transport layer
	   bool need_to_flood = false;
	   for (std::set<ConstDataUnitHandle>::const_iterator du_it = acm->complete_DataUnits().begin();
		    du_it != acm->complete_DataUnits().end(); du_it++){
			// Add also the ID's of the complete DataUnits
		    received_ids.insert((*du_it)->id());
			received_DataUnit_ids_total_.insert((*du_it)->id());
			received_DataUnits_total_++;

			/// Give to applications
			bool application_responsible = false;
			for (Node::ProcessorList::const_iterator app_it = owner().processors().begin(); app_it != owner().processors().end(); app_it++){
					const autocast::AutoCastApplication * ac_app = dynamic_cast<const autocast::AutoCastApplication*>((*app_it).get());
					if (ac_app){
						application_responsible = ac_app->receive_DataUnit(*du_it);
					}
			}
			if ( !application_responsible && (*du_it)->distribution_area() &&
				( now < (*du_it)->time() + (*du_it)->max_life_time() ) ){
				/// Set the flag if the DataUnit is new
				bool is_new_DataUnit = false;
				/// Interested in if is a new DataUnit and enable logging
				autocast::AutoCastProcessor::LocalDataUnit * ldu = handle_DataUnit(*du_it,&is_new_DataUnit,true);
				if (ldu){
					/// New DataUnit
					unknown_DataUnit_ids_.erase(ldu->dataUnit()->id());
				}
				if (ldu && answer_decision && is_new_DataUnit){
					ldu->unknown_count(ldu->unknown_count() + 1);
					need_to_flood = true;
				}else if(ldu && !is_new_DataUnit){
					/// else old object or no answer_decision
					if(ldu->unknown_count() > 0) ldu->unknown_count(ldu->unknown_count() - 1);
				}/// else out of region
			}/// else no one feels responsible
	   }

	   bool need_to_answer = false;
	   if(answer_decision){
		   for (std::map<int,autocast::AutoCastProcessor::LocalDataUnit*>::iterator it = complete_DataUnits_.begin();
			   it != complete_DataUnits_.end(); it++){
			   if ( received_ids.find( it->second->dataUnit()->id() ) == received_ids.end() ){
				    it->second->unknown_count(it->second->unknown_count() + 1);
				    need_to_answer = true;
			   }
		   }
	   }
	   /// For request
	   for(std::set<unsigned int>::iterator it = received_ids.begin(); it != received_ids.end(); ++it){
		   if(complete_DataUnits_.find(*it) == complete_DataUnits_.end()){
			   unknown_DataUnit_ids_.insert(*it);
		   }
	   }

	   // If answer_timer_ is not NULL, it's pending
	   if (answer_timer_ || need_to_answer){
			/// Answer fast
		    double lb2 = owner().world().simulation_controller().environment().optional_double_param("_lb2__process_message",0.0275);
			double ub2 = owner().world().simulation_controller().environment().optional_double_param("_ub2__process_message",0.0325);
			double at = now + uniform_random(lb2,ub2);

			if ( answer_timer_ ){
				owner_w().world_w().scheduler_w().delete_event(answer_timer_);
				answer_timer_ = NULL;
			}
			answer_timer_ = owner_w().world_w().scheduler_w().new_event(*this,at,NULL);

	   }else if(flood_timer_ || need_to_flood){
		    double lb3 = owner().world().simulation_controller().environment().optional_double_param("_lb3__process_message",0.0575);
			double ub3 = owner().world().simulation_controller().environment().optional_double_param("_ub3__process_message",0.0625);
			double at = now + uniform_random(lb3,ub3);

			if ( flood_timer_ ){
				owner_w().world_w().scheduler_w().delete_event(flood_timer_);
				flood_timer_ = NULL;
			}
			flood_timer_ = owner_w().world_w().scheduler_w().new_event(*this,at,NULL);
	   }


	   if ( request_timer_ ){
			 owner_w().world_w().scheduler_w().delete_event(request_timer_);
			 request_timer_ = NULL;
	   }

       if( unknown_DataUnit_ids_.size() ){
		   double lb4 = owner().world().simulation_controller().environment().optional_double_param("_lb4__process_message",0.0875);
		   double ub4 = owner().world().simulation_controller().environment().optional_double_param("_ub4__process_message",0.0925);
		   double at = now + uniform_random(lb4,ub4);
		   request_timer_ = owner_w().world_w().scheduler_w().new_event(*this,at,NULL);
       }
	   return true;
   }
   // ----------------------------------------------------------------------
   void
   AutoCastProcessor::
   work( void )
      throw()
   {
	   if (simulation_round() == max_iterations_ - 1){
		    KeeperManagedHandle kmh = owner_w().world_w().simulation_controller_w().simulation_task_keeper_w().find_managed_w("AutoCastTask");
			assert(kmh.is_not_null());
			autocast::AutoCastTask* act = dynamic_cast<autocast::AutoCastTask*>( kmh.get() );
			if (act){
				act->process_sent_statistic(&owner(),packets_sent_total_,bytes_sent_total_,dataUnits_sent_total_,dataUnits_bytes_sent_total_,received_messages_ids_total_.size(),received_DataUnits_total_,received_DataUnit_ids_total_.size()/*,1.0*neighbors_count_/(simulation_round()+1),1.0*real_neighbors_count_/(simulation_round()+1),velocity_count_/(simulation_round()+1),update_time_count_/(simulation_round()+1)*/);
			}

			// And at least
			clean_up();
	   }
   }
   // ----------------------------------------------------------------------
   bool 
	   AutoCastProcessor::
	   send_to(const ConstDataUnitHandle& duh, shawn::Processor * creator)
	   throw()
   { 
	   LocalDataUnit * ldu = NULL;
	   /// Not interested in if is a new DataUnit and disable logging
	   ldu = handle_DataUnit(duh,NULL,false);
	   if (ldu != NULL){
		   ldu->creator(creator);
		   if (creator) ldu->refresh_before_send(true);
		   int unknown_count = owner().world().simulation_controller().environment().optional_int_param("_unknown_count__send",10);
		   ldu->unknown_count(unknown_count);

		   update();
		   return true;
	   }
	   return false;
   }
   // ----------------------------------------------------------------------
   void
   AutoCastProcessor::
   timeout( shawn::EventScheduler& es, shawn::EventScheduler::EventHandle eh, 
   double time, shawn::EventScheduler::EventTagHandle& eth) 
		throw()
   {	
	   if(eh == update_timer_){
		   // Set the pointer NULL
		   update_timer_ = NULL;
	   }
	   if(eh == answer_timer_){
		   // Set the pointer NULL
		   answer_timer_ = NULL;
	   }
	   if(eh == flood_timer_){
		    // Set the pointer NULL
			flood_timer_ = NULL;
	   }
	   if(eh == request_timer_){
		   // Set the pointer NULL
		   request_timer_ = NULL;
	   }
	   update();
   }
   // ----------------------------------------------------------------------
   void 
	   AutoCastProcessor::
	   update() 
	   throw()
   {
	   double now = owner().world().simulation_controller().world().scheduler().current_time();
	   double lb = owner().world().simulation_controller().environment().optional_double_param("_lb__update",0.95);
	   double ub = owner().world().simulation_controller().environment().optional_double_param("_ub__update",1.05);
	   double time_interval = get_update_time() * uniform_random(lb,ub);

	   local_update();
	   unknown_DataUnit_ids_.clear();
	   send_update_packet(time_interval);

	   for (DataUnitsMap::iterator it = complete_DataUnits_.begin(); it != complete_DataUnits_.end(); it++){
		   it->second->unknown_count(0);
	   }

	   if( answer_timer_ ){
		   owner_w().world_w().scheduler_w().delete_event(answer_timer_);
		   answer_timer_ = NULL;
	   }

	   if( flood_timer_ ){
		   owner_w().world_w().scheduler_w().delete_event(flood_timer_);
		   flood_timer_ = NULL;
	   }

	   if ( update_timer_ ) 
	   {
		   owner_w().world_w().scheduler_w().delete_event(update_timer_);
		   update_timer_ = NULL;
	   }

	   if ( request_timer_ ){
			owner_w().world_w().scheduler_w().delete_event(request_timer_);
			request_timer_ = NULL;
	   }
	   update_timer_ = owner_w().world_w().scheduler_w().new_event(*this,now + time_interval,NULL);
   }
   // ----------------------------------------------------------------------
   double
		AutoCastProcessor::
		uniform_random(double lb, double ub, bool lbi, bool ubi)
   {
	    shawn::UniformRandomVariable urv;
		urv.set_lower_bound(lb);
		urv.set_upper_bound(ub);
		urv.set_lower_bound_inclusive(lbi);
		urv.set_upper_bound_inclusive(ubi);
		urv.init();
		return urv;
   }

   // ----------------------------------------------------------------------
	void 
		AutoCastProcessor::
		local_update()
	throw()
    {
		double now = owner().world().scheduler().current_time();

		/// Throw away old DataUnits
		for (DataUnitsMap::iterator it = complete_DataUnits_.begin(); it != complete_DataUnits_.end(); it++){
			if ( (it->second->dataUnit()->max_life_time() >= 0) && 
				(now >= it->second->dataUnit()->time() + it->second->dataUnit()->max_life_time()) ){
					delete it->second;
					complete_DataUnits_.erase(it);
			}
		}
	}
	// ----------------------------------------------------------------------
	/// Retruns NULL if DataUnit is out of region
	autocast::AutoCastProcessor::LocalDataUnit* 
		AutoCastProcessor::
		handle_DataUnit(const ConstDataUnitHandle& du, bool* is_new, bool logging) 
		throw()
	{
		double now = owner().world().scheduler().current_time();
		autocast::AutoCastProcessor::LocalDataUnit * ldu = NULL;
		// First we assume an old DataUnit
		if(is_new) *is_new = false;
		// DataUnit out of region returning NULL
		if ( !(du->distribution_area() && du->distribution_area()->is_inside(owner().real_position().x(), owner().real_position().y())) ){
			return NULL;
		}
		DataUnitsMap::const_iterator old_du = complete_DataUnits_.find(du->id());
		if ( old_du == complete_DataUnits_.end() ){
			// Completely new DataUnit
			ldu = new LocalDataUnit(du);
			// The DataUnit is flaged as new
			if(is_new) *is_new = true;
			ldu->last_received_time(now);
			complete_DataUnits_.insert(std::pair<int,LocalDataUnit*>(ldu->dataUnit()->id(),ldu));
			if (logging){
				// Do the logging of completely new DataUnit
				KeeperManagedHandle kmh = owner_w().world_w().simulation_controller_w().simulation_task_keeper_w().find_managed_w("AutoCastTask");
				assert(kmh.is_not_null());
				AutoCastTask* act = dynamic_cast<AutoCastTask*>( kmh.get() );
				assert(act != NULL);
				act->process_latencies(du,&owner(),owner().world().simulation_controller());
			}
		}else{
			/// Old DataUnit
			/// With newer timestamp?
			if ( old_du->second->dataUnit()->time() < du->time() ){
				if (logging){
					//Do the logging of old DataUnit with newer timestamp
					KeeperManagedHandle kmh = owner_w().world_w().simulation_controller_w().simulation_task_keeper_w().find_managed_w("AutoCastTask");
					assert(kmh.is_not_null());
						autocast::AutoCastTask* act = dynamic_cast<autocast::AutoCastTask*>( kmh.get() );
					assert(act != NULL);
						act->process_latencies(du,&owner(),owner().world().simulation_controller());
				}

				/// If the timestap is newer replace the DataUnit in the LocalDataUnit
				old_du->second->dataUnit(du);
				ldu = old_du->second;
				/// The DataUnit is flaged as new
				if(is_new) *is_new = true;
			}
			/// DataUnit is really old
			/// Set the last_received_time of the LocalDataUnit
			old_du->second->last_received_time(now);
			ldu = old_du->second;
		}
		return ldu;
	}
	// ----------------------------------------------------------------------
	void 
		AutoCastProcessor::
		send_update_packet(const double update_time) 
		throw()
	{
		double now = owner().world().scheduler().current_time();
		uid_counter_++;
		AutoCastMessage * acm = new AutoCastMessage(uid_counter_,AUTOCAST_TYPE_UPDATE,update_time,owner().id(),owner().real_position().x(),owner().real_position().y());
		autocast::AutoCastProcessor::LocalDataUnit * ldu = most_urgent_DataUnit();
		/// Append complete DataUnits
		int dataUnits_bytes = 0;
		int id_space = 4 * complete_DataUnits_.size();
		while( ldu && ( acm->complete_DataUnits().size() <= max_update_data_units_ ) && 
			 ( ( acm->size() + id_space + ldu->dataUnit()->size() ) <= max_update_packet_size_ ) ){
			ldu->refresh();
			ldu->last_send_time(now);
			ldu->unknown_count(0);
			/// Detaching DataUnit
			DataUnit * du = new DataUnit( *(ldu->dataUnit()) );
			/// Incrementing DataUnits hop count
			du->hop_count( ldu->dataUnit()->hop_count() + 1 );
			dataUnits_bytes += du->size();
			// Assertion
			if (now > du->time() + du->max_life_time()){
				std::cerr << "object " << ldu->dataUnit()->id() << " is too old. Stop!" << std::endl;
				abort();
			}

			acm->insert_complete_DataUnit(du);
			id_space -= 4;
			ldu = most_urgent_DataUnit();
		}

		/// Append list with also known objects
		for (DataUnitsMap::iterator it = complete_DataUnits_.begin(); it != complete_DataUnits_.end(); it++){
  		    if (it->second->last_send_time() < now) acm->insert_id(it->second->dataUnit()->id());
		}

		/// Assertion
		if (acm->size() > max_update_packet_size_){
			assert(max_update_packet_size_ == AUTOCAST_MAX_PACKET_LENGTH);
			std::cerr << "Time: " << now << "  Node: " << owner().id() 
            << " wants to send " << acm->size()
            << " byte, but only " << max_update_packet_size_ << " are allowed!";
			exit(1);
		}

		/// Statistics
		packets_sent_total_++;
		bytes_sent_total_ += acm->size();
		dataUnits_sent_total_ += acm->complete_DataUnits().size();
		dataUnits_bytes_sent_total_ += dataUnits_bytes;

		owner_w().send(acm);
	}
	// ----------------------------------------------------------------------
	double 
		AutoCastProcessor::
		get_update_time() 
		throw()
	{
		if(min_update_time_ == max_update_time_) return max_update_time_;

		int neighborhood_size = neighborhood_.neighborhood_size();

		double range = owner().world().simulation_controller().environment().required_double_param("range");
		double standard_neighbors = owner().world().simulation_controller().environment().optional_double_param("_standard_neighbors__get_update_time",4.0);
		double alpha = owner().world().simulation_controller().environment().optional_double_param("_alpha__get_update_time",1.0);
		double delta_r = owner().world().simulation_controller().environment().optional_double_param("_delta_r__get_update_time",0.1);
		double add_to_velocity = owner().world().simulation_controller().environment().optional_double_param("_add_to_velocity__get_update_time",0.01);

		update_time_ = (1 - alpha) * update_time_ + alpha * (neighborhood_size + 1)*(neighborhood_size + 1)/((double)standard_neighbors + 1) * (range * delta_r)/(owner().movement().velocity().euclidean_norm() + add_to_velocity); 
		if (update_time_ < min_update_time_) update_time_ = min_update_time_;
		if (update_time_ > max_update_time_) update_time_ = max_update_time_;
		return update_time_;
	}
	// ----------------------------------------------------------------------
	void
		AutoCastProcessor::
		fetch_parameters()
	{
		max_startup_time_ = owner().world().simulation_controller().environment().optional_double_param("max_startup_time",AUTOCAST_DEFAULT_STARTUP_TIME);
		min_update_time_ = owner().world().simulation_controller().environment().optional_double_param("min_update_time",AUTOCAST_DEFAULT_MIN_UPDATE_TIME);
		max_update_time_ = owner().world().simulation_controller().environment().optional_double_param("max_update_time",AUTOCAST_DEFAULT_MAX_UPDATE_TIME);
		dataUnit_max_live_time_ = owner().world().simulation_controller().environment().optional_double_param("dataUnit_max_live_time",AUTOCAST_DEFAULT_DATAUNIT_LIVETIME);
		max_update_packet_size_ = owner().world().simulation_controller().environment().optional_int_param("max_update_packet_size",AUTOCAST_MAX_PACKET_LENGTH);
		max_update_data_units_ = owner().world().simulation_controller().environment().optional_double_param("max_update_data_units",AUTOCAST_DEFAULT_MAX_DATAUNITS_PER_PACKET);
	}
	// ----------------------------------------------------------------------
	autocast::AutoCastProcessor::LocalDataUnit * 
		AutoCastProcessor::
		most_urgent_DataUnit() 
		throw()
	{
		double now = owner().world().scheduler().current_time();

		std::vector<LocalDataUnit*> tmp_objects;

		for (DataUnitsMap::const_iterator it = complete_DataUnits_.begin(); it != complete_DataUnits_.end(); ++it){
			if (it->second->last_send_time() < now && it->second->unknown_count() > 0){
				for (int i = 0; i < it->second->unknown_count(); i++){
					tmp_objects.push_back(it->second);
				}
			}
		}
		if (tmp_objects.size() == 0) return NULL;
		int idx = static_cast<int>(uniform_random(0.0,tmp_objects.size(),true,false));
		return tmp_objects.at(idx);
	}
	// ----------------------------------------------------------------------
	bool 
		AutoCastProcessor::
		booted() 
		const 
		throw()
	{
		return booted_;
	}
	// ----------------------------------------------------------------------
	void
		AutoCastProcessor::
		clean_up()
		throw()
	{
		/// Cleaning up!
		for(DataUnitsMap::iterator it = complete_DataUnits_.begin(); it != complete_DataUnits_.end(); it++){
			if (it->second) delete it->second;
			complete_DataUnits_.erase(it);
		}
		complete_DataUnits_.clear();
		received_messages_ids_total_.clear();
		received_DataUnit_ids_total_.clear();
		//if ( owner().is_special_node() ) owner_w().world_w().scheduler_w().clear();
	}
	// ----------------------------------------------------------------------
	void
		AutoCastProcessor::
		set_state(const Processor::ProcessorState& state) 
		throw()
	{
		if (state == Inactive || state == Sleeping){
			if ( flood_timer_ ){
				owner_w().world_w().scheduler_w().delete_event(flood_timer_);
				flood_timer_ = NULL;
			}
			if ( update_timer_ ){
			   owner_w().world_w().scheduler_w().delete_event(update_timer_);
			   update_timer_ = NULL;
			}
			if ( answer_timer_ ){
				owner_w().world_w().scheduler_w().delete_event(answer_timer_);
				answer_timer_ = NULL;
			}
			if ( request_timer_ ){
				owner_w().world_w().scheduler_w().delete_event(request_timer_);
				request_timer_ = NULL;
			}
		}
		Processor::set_state(state);
	}

}
#endif

/*-----------------------------------------------------------------------
 * Source  $HeadURL$
 * Version $LastChangedRevision$
 * Date    $LastChangedDate$
 *-----------------------------------------------------------------------*/
