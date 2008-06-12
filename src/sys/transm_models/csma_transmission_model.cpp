/************************************************************************
** This file is part of the network simulator Shawn.                  **
** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
** Shawn is free software; you can redistribute it and/or modify it   **
** under the terms of the BSD License. Refer to the shawn-licence.txt **
** file in the root of the Shawn source tree for further details.     **
************************************************************************/

#include "sys/transm_models/csma_transmission_model.h"
#include "sys/edge_model.h"
#include "sys/world.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"
#include <cmath>

namespace shawn
	{
	CsmaTransmissionModel::
		CsmaTransmissionModel(int bandwidth, double backoff, double sending_jitter, double sending_jitter_lb,int max_sending_attempts,int backoff_factor_base) :
	received_(0), 
		dropped_(0), 
		packet_failure_(0), 
		average_delay_(0), 
		jitter_(0), 
		deliver_num_(0),
		backOff_(backoff),
		bandwidth_(bandwidth),
		neighbors_(NULL),
		sending_jitter_(sending_jitter),
		sending_jitter_lb_(sending_jitter_lb),
		max_sending_attempts_(max_sending_attempts),
		backoff_factor_base_(backoff_factor_base)
		{
		}

	// ----------------------------------------------------------------------
	CsmaTransmissionModel::
		~CsmaTransmissionModel()
	{
	}

	// ----------------------------------------------------------------------
	void CsmaTransmissionModel::init()
		throw()
	{
		TransmissionModel::init();
		if( neighbors_ != NULL ){
			delete neighbors_;
			neighbors_ = NULL;
		}
		//This gives us an Array of Messages for all nodes which can be received in O(1)
		neighbors_ = new DynamicNodeArray<MessageList>(world_w());
		std::cout << "csma: Initialised." << std::endl;
	}

	// ----------------------------------------------------------------------
	void CsmaTransmissionModel::reset()
		throw()
	{
		average_delay_ = deliver_num_>0? average_delay_/deliver_num_ : 0;
		jitter_ = deliver_num_ > 1 ? jitter_ / (deliver_num_ -1) : 0;
		/*std::cout << "CSMA: "<<  
			received_ <<" msgs. to be sent. "<< dropped_ << " msgs. dropped." 
			<< packet_failure_ << " packets fail to reach the destination." << " And average delay is "
			<< average_delay_  <<". Jitter is "<<jitter_<< std::endl;*/

		TransmissionModel::reset();
		received_ = 0;
		dropped_ = 0;
		packet_failure_ = 0;
		average_delay_ = 0;
		deliver_num_ = 0;
		jitter_ = 0;
	}
	// ----------------------------------------------------------------------


	// ----------------------------------------------------------------------
	bool
		CsmaTransmissionModel::
		supports_mobility( void )
		const throw(std::logic_error)
		{
		return world().edge_model().supports_mobility();
		}

	// ----------------------------------------------------------------------
	void 
		CsmaTransmissionModel::
		send_message( MessageInfo &mi )
	throw()
	{
		csma_msg* new_msg = new csma_msg(&mi, mi.msg_->size() / bandwidth_, backOff_);
		// If sending jitter is set, deliver time will be changed. ( Needed to avoid same deliver times due to 
		// processors sending at the beginning of a round.)
		mi.time_=( sending_jitter_ > 0.0 ) ? ( new_msg->random( sending_jitter_lb_, sending_jitter_ ) + mi.time_) : ( mi.time_ );
		new_msg->deliver_time_ = mi.time_;
		find_destinations( new_msg );
		//Adds a message to the DynamicNodeArray
		MessageList* m = &((*neighbors_)[*(mi.src_)]);
		m->insert(new_msg);
		// Create a new Event. Important is the MessageTag (new_msg). It defines which message will be send
		event_handle_ = world_w().scheduler_w().new_event(*this, mi.time_,new_msg);
	}

	void CsmaTransmissionModel::timeout(EventScheduler & event_scheduler, EventScheduler::EventHandle event_handle, double time, EventScheduler::EventTagHandle & event_tag_handle) throw()
	{
		csma_msg* msg = dynamic_cast<csma_msg* >(event_tag_handle.get());
		if(msg != NULL){
			if(!msg->sending_)
				deliver(msg); // If the message has not been send yet.
			else    //Otherwise the transmission has terminated
			  {
			    // TODO: Is this the correct position to indicate that the message has been sent?
                                const Message* m = msg->pmi_->msg_.get();
                                if (m->has_sender_proc())
                                (m->sender_proc_w()).process_sent_indication( ConstMessageHandle(msg->pmi_->msg_) );
                                
                                deliver_num_++;
                                for(unsigned int i=0; i< msg->destinations_.size(); i++){
                                        //All receiving nodes will check, if they already receive a message 
                                        receive(msg->destinations_[i]->dest_node_,msg);
                                        }
                                
				(*neighbors_)[*(msg->pmi_->src_)].erase(msg);
				
			  }

			}
		else
			/*std::cout<< "msg = NULL"<<std::endl*/;

	}

	// ----------------------------------------------------------------------
	void CsmaTransmissionModel::
		deliver_messages() throw(){}

	void CsmaTransmissionModel::
		deliver(csma_msg* msg) throw()
		{
			//Checks if medium is free
			listening(msg);
			//When medium is free msg->isSending() otherwise not
			if(msg->isSending())
			{
			  //Define a new Event for the end of the transmision. (Specified by msg->isSending())
			  event_handle_= world_w().scheduler_w().new_event(*this, msg->deliver_time_ + msg->duration_,msg);
			}
		}

	void CsmaTransmissionModel::
		listening(csma_msg* msg) throw(){
			double nextFreeTime=world().current_time();
			bool sending = false;
			//All neighbors are checked here
			EdgeModel::adjacency_iterator it, endit;
			for( it = world_w().begin_adjacent_nodes_w( *msg->pmi_->src_ , EdgeModel::CD_IN),
				endit = world_w().end_adjacent_nodes_w( *msg->pmi_->src_ );
				it != endit;
			++it )
				{
				if(msg->pmi_->src_->id() ==it->id()) continue;
				MessageList* m = &((*neighbors_)[*it]);
				MessageList::iterator iter = m->begin();
				//Each neighbors messages are tested
				for(; iter!=m->end();++iter){
					csma_msg* temp = *iter;
					if(msg->deliver_time_ > temp->deliver_time_ && msg->deliver_time_ <= temp->deliver_time_ + temp->duration_)
						{
						/*std::cout <<"csma: "<< "Node " << temp->pmi->src_->id() << " is interfering" << std::endl;*/
						double time_fin = temp->deliver_time_ + temp->duration_;
						sending = true;
						nextFreeTime = (nextFreeTime>= time_fin)? (nextFreeTime):(time_fin);
						}
// Case when to messages are send at the same time is dealt with at the receiving node.
					}
				}
			if(!sending){
				//If no neighbor is sending a message this message will be send
				msg->setSending();
				}
			else{
			      if(msg->sending_attempts_< max_sending_attempts_){ 

			        msg->deliver_time_= nextFreeTime + msg->backoff_ * (int)(pow(backoff_factor_base_,msg->sending_attempts_)) ;
				//New Event. To the next time the medium is free + a backoff
	                        ++msg->sending_attempts_;
				event_handle_ = world_w().scheduler_w().new_event(*this,msg->deliver_time_,msg);
				
			      }
			      else{
                                const Message* m = msg->pmi_->msg_.get();
                                if (m->has_sender_proc())
                                (m->sender_proc_w()).process_sent_indication( ConstMessageHandle(msg->pmi_->msg_) );
			        //  Maximum number of sending atempts have been reached 
			        // therefore message will be dropped.
			        
			        (*neighbors_)[*(msg->pmi_->src_)].erase(msg);
			        std::cout<< "CSMA: Node "<<msg->pmi_->src_<< ": Maximum number of sending attempts have been reached. Message will be dropped."<< std::endl;
			      }
			}
			      
		}

	void CsmaTransmissionModel::
		receive(Node* target, csma_msg* msg) throw(){
			bool inUse=false;
			EdgeModel::adjacency_iterator it, endit;
			// The target checks if one of it's neighbors (others than the msgs. one) is already sending
			for( it = world_w().begin_adjacent_nodes_w( *target,EdgeModel::CD_IN),
				endit = world_w().end_adjacent_nodes_w( *target);
				it != endit;
			++it )
				{
				if(msg->pmi_->src_->id() != it->id()){
					MessageList* m = &((*neighbors_)[*it]);
					MessageList::iterator iter = m->begin();
					//Here all messages of one node is checked
					for(; iter!=m->end();++iter){
						// First condition ckecks if a node not adjacent to the sender is delivering a msg. to target at the moment 
						// as well as if sender and receiver are delivering at the same moment.
						// This procedure correlates with reality. Both nodes are sending and cannot "hear" the other one
						if((msg->deliver_time_ >= (*iter)->deliver_time_ && msg->deliver_time_ <= (*iter)->deliver_time_ + (*iter)->duration_) )
							{
							inUse = true;
							break;
							}					
						}
					if(inUse) break;
					}
				}

			if(inUse){
				/*std::cout<<"csma: "<<"Message from Node:"<< msg->pmi->src_->id() << "dropped. Due to conflict at node:"
					<< target->id()<< std::endl;*/
				++dropped_;

				}
			else{
				/*std::cout<<"csma: "<<"Node:"<< target->id() << "received msg. from node:"
					<< msg->pmi->src_->id()<< std::endl;*/
				++received_;
				double cur_delay=(msg->deliver_time_-msg->pmi_->time_);
				average_delay_ +=cur_delay; 
				if( deliver_num_>1 )
					jitter_ += (cur_delay>last_delay_)? cur_delay-last_delay_ : last_delay_-cur_delay;
				last_delay_ = cur_delay;
				target->receive(ConstMessageHandle(msg->pmi_->msg_));
				}

		}

	// ----------------------------------------------------------------------
	void CsmaTransmissionModel::
		find_destinations( csma_msg* pmsg )
		{
		EdgeModel::adjacency_iterator it, endit;
		for( it = world_w().begin_adjacent_nodes_w( *pmsg->pmi_->src_ , EdgeModel::CD_OUT),
			endit = world_w().end_adjacent_nodes_w( *pmsg->pmi_->src_ );
			it != endit;
		++it )
			{
			if(pmsg->pmi_->src_->id()!= it->id())
				pmsg->push_new_destination( &(*it) );
			}
		}

	}
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/transm_models/csma_transmission_model.cpp,v $
* Version $Revision: 38 $
* Date    $Date: 2007-06-08 14:30:12 +0200 (Fr, 08 Jun 2007) $
*-----------------------------------------------------------------------
* $Log: csma_transmission_model.cpp,v $
*-----------------------------------------------------------------------*/
