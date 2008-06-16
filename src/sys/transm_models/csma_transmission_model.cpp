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


//#define CSMA_DEBUG

namespace shawn
	{
	CsmaTransmissionModel::
		CsmaTransmissionModel(int bandwidth, double backoff, double sending_jitter, double sending_jitter_lb,int max_sending_attempts,int backoff_factor_base) :
	/*received_(0), 
		dropped_(0), 
		packet_failure_(0), 
		average_delay_(0), 
		jitter_(0), 
		deliver_num_(0),*/
		backOff_(backoff),
		bandwidth_(bandwidth),
		nodes_(NULL),
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
		if( nodes_ != NULL )
		{
			delete nodes_;
			nodes_ = NULL;
		}
		//This gives us an Array of Messages for all nodes which can be received in O(1)
		nodes_ = new DynamicNodeArray<CsmaState>(world_w());
		std::cout << "csma: Initialised." << std::endl;
	}

	// ----------------------------------------------------------------------
	void CsmaTransmissionModel::reset()
		throw()
	{
		//average_delay_ = deliver_num_>0? average_delay_/deliver_num_ : 0;
		//jitter_ = deliver_num_ > 1 ? jitter_ / (deliver_num_ -1) : 0;
		/*std::cout << "CSMA: "<<  
			received_ <<" msgs. to be sent. "<< dropped_ << " msgs. dropped." 
			<< packet_failure_ << " packets fail to reach the destination." << " And average delay is "
			<< average_delay_  <<". Jitter is "<<jitter_<< std::endl;*/

		TransmissionModel::reset();
		if( nodes_ != NULL )
		{
			delete nodes_;
			nodes_ = NULL;
		}
		//This gives us an Array of Messages for all nodes which can be received in O(1)
		nodes_ = new DynamicNodeArray<CsmaState>(world_w());
		
		/*
		received_ = 0;
		dropped_ = 0;
		packet_failure_ = 0;
		average_delay_ = 0;
		deliver_num_ = 0;
		jitter_ = 0;*/
		
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
		csma_msg* new_msg = new csma_msg(&mi, (double) mi.msg_->size() / bandwidth_, backOff_);
		//std::cout <<"csma: " << mi.src_->id() << " send_message at "<< world().current_time() << " size " << mi.msg_->size() << " bandwidth " << bandwidth_ << std::endl;
//std::cout <<"duration " << new_msg->duration_ << std::endl;
		if (((*nodes_)[*(mi.src_)].outgoing_messages_).empty())
		{
			handle_next_message(new_msg);
		}
		(*nodes_)[*(mi.src_)].outgoing_messages_.push_back(new_msg);

	}

	void CsmaTransmissionModel::timeout(EventScheduler & event_scheduler, EventScheduler::EventHandle event_handle, double time, EventScheduler::EventTagHandle & event_tag_handle) throw()
	{
		
		csma_msg* msg = dynamic_cast<csma_msg* >(event_tag_handle.get());
		if(msg != NULL){
			if(!msg->sending_)
			{
				
				while( ((*nodes_)[*(msg->pmi_->src_)].busy_until_ > msg->deliver_time_) && (msg->sending_attempts_< max_sending_attempts_))
				{
					msg->deliver_time_= world().current_time() + shawn::uniform_random_0i_1i()*msg->backoff_ * (int)(pow(backoff_factor_base_,msg->sending_attempts_)) ;
				    //New Event to now + backoff
		            ++msg->sending_attempts_;
				}
				if (msg->deliver_time_ <= world().current_time())
					start_send(msg); // If the message has not been send yet.
				else 
				{
					if (msg->sending_attempts_< max_sending_attempts_)
						world_w().scheduler_w().new_event(*this, msg->deliver_time_ ,msg);
					else
					{
						const Message* m = msg->pmi_->msg_.get();
						if (m->has_sender_proc())
		                	(m->sender_proc_w()).process_sent_indication( ConstMessageHandle(msg->pmi_->msg_), shawn::Processor::SHAWN_TX_STATE_CHANNEL_ACCESS_FAILURE, msg->sending_attempts_ );
						(*nodes_)[*(msg->pmi_->src_)].outgoing_messages_.pop_front();
						if (!((*nodes_)[*(msg->pmi_->src_)].outgoing_messages_).empty())
						{
							csma_msg *new_msg = (*nodes_)[*(msg->pmi_->src_)].outgoing_messages_.front();
							handle_next_message(new_msg);
						}
					}
				}
				
					
			}
			else    //Otherwise the transmission has terminated
			  {
				    
					const Message* m = msg->pmi_->msg_.get();
	                
	                //deliver_num_++;
	                end_send(msg);
	                if (m->has_sender_proc())
	                	(m->sender_proc_w()).process_sent_indication( ConstMessageHandle(msg->pmi_->msg_), shawn::Processor::SHAWN_TX_STATE_SUCCESS, msg->sending_attempts_ );
	                
	                /*
	                for(unsigned int i=0; i< msg->destinations_.size(); i++)
	                {
	                    //All receiving nodes will check, if they already receive a message 
	                    receive(msg->destinations_[i]->dest_node_,msg);
                    }
					(*neighbors_)[*(msg->pmi_->src_)].erase(msg);
					*/
				
			  }

			}
		//else
			/*std::cout<< "msg = NULL"<<std::endl*/;

	}

	// ----------------------------------------------------------------------
	void CsmaTransmissionModel::
		deliver_messages() throw(){}

	void CsmaTransmissionModel::
		start_send(csma_msg* msg) throw()
		{
#ifdef CSMA_DEBUG
		std::cout <<"csma: "<< msg->pmi_->src_->id() << " start sending at " << world().current_time() << std::endl;
#endif				
			
		//TODO: Start sending at the exactly at the same time?
			(*nodes_)[*(msg->pmi_->src_)].busy_until_ = msg->deliver_time_ + msg->duration_;
			(*nodes_)[*(msg->pmi_->src_)].current_message_ = msg;
			msg->setSending();
			
			EdgeModel::adjacency_iterator it, endit;
			for( it = world_w().begin_adjacent_nodes_w( *msg->pmi_->src_ , EdgeModel::CD_OUT),
				endit = world_w().end_adjacent_nodes_w( *msg->pmi_->src_ );
				it != endit; ++it )
			{
				if (*it != *(msg->pmi_->src_))
				{
					(*nodes_)[*(msg->pmi_->src_)].destinations_.insert(&(*it));
					start_receive(&(*it), msg);
				}
			}
			//std::cout <<"deliver time: "<< msg->deliver_time_ << " , duration " << msg->duration_ << std::endl;

			world_w().scheduler_w().new_event(*this, msg->deliver_time_ + msg->duration_,msg);
				
			/*
			//Checks if medium is free
			listening(msg);
			//When medium is free msg->isSending() otherwise not
			if(msg->isSending())
			{
			  //Define a new Event for the end of the transmision. (Specified by msg->isSending())
			  event_handle_= world_w().scheduler_w().new_event(*this, msg->deliver_time_ + msg->duration_,msg);
			}*/
		}
	
	void CsmaTransmissionModel::
		end_send(csma_msg* msg) throw()
		{
#ifdef CSMA_DEBUG
			std::cout <<"csma: "<< msg->pmi_->src_->id() << " end  sending at " << world().current_time() << std::endl;
#endif				
			
			if ((*nodes_)[*(msg->pmi_->src_)].current_message_ == msg)
			{
				(*nodes_)[*(msg->pmi_->src_)].current_message_ = NULL;
				
				std::set<Node*>::iterator it, endit;
				for( it = (*nodes_)[*(msg->pmi_->src_)].destinations_.begin(),
					endit = (*nodes_)[*(msg->pmi_->src_)].destinations_.end();
					it != endit; ++it )
				{
					end_receive(*it, msg);
				}
				
				(*nodes_)[*(msg->pmi_->src_)].destinations_.clear();
			}
			
			(*nodes_)[*(msg->pmi_->src_)].outgoing_messages_.pop_front();
			if (!((*nodes_)[*(msg->pmi_->src_)].outgoing_messages_).empty())
			{
				csma_msg *new_msg = (*nodes_)[*(msg->pmi_->src_)].outgoing_messages_.front();
				handle_next_message(new_msg);
				
			}

		}
/*
	void CsmaTransmissionModel::
		listening(csma_msg* msg) throw(){
			double nextFreeTime=world().current_time();
#ifdef SHAWN_CSMA_DEBUG
			std::cout <<"csma: "<< msg->pmi_->src_->id() << " listening at " << nextFreeTime << std::endl;
#endif
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
				//All neighbor's messages are tested
				for(; iter!=m->end();++iter){
					csma_msg* temp = *iter;
					if(msg->deliver_time_ > temp->deliver_time_ && msg->deliver_time_ <= temp->deliver_time_ + temp->duration_)
					{
#ifdef SHAWN_CSMA_DEBUG
						std::cout <<"csma: "<< "Node " << temp->pmi_->src_->id() << " is interfering with me: " << msg->pmi_->src_->id() << std::endl;
#endif
						double time_fin = temp->deliver_time_ + temp->duration_;
						sending = true;
						nextFreeTime = (nextFreeTime>= time_fin)? (nextFreeTime):(time_fin);
					}
// Case when two messages are send at the same time is treated at the receiving node.
					}
				}
			if(!sending)
			{
#ifdef SHAWN_CSMA_DEBUG
				std::cout <<"csma: "<< msg->pmi_->src_->id() << " start sending at " << world().current_time() << std::endl;
#endif
				//If no neighbor is sending a message this message will be send
				msg->setSending();
			}
			else{
#ifdef SHAWN_CSMA_DEBUG
			    std::cout <<"csma: "<< msg->pmi_->src_->id() << " media access fail at " << world().current_time() << std::endl;
#endif			  
				if(msg->sending_attempts_< max_sending_attempts_)
			      { 

					  msg->deliver_time_= world().current_time() + msg->backoff_ * (int)(pow(backoff_factor_base_,msg->sending_attempts_)) ;
				      //New Event to now + backoff
		              ++msg->sending_attempts_;
		              event_handle_ = world_w().scheduler_w().new_event(*this,msg->deliver_time_,msg);
				
			      }
			      else
			      {
				      //std::cout<< "CSMA: Node "<<msg->pmi_->src_->id() << ": Maximum number of sending attempts have been reached. Message will be dropped."<< std::endl;
	                  const Message* m = msg->pmi_->msg_.get(); 
	                  if (m->has_sender_proc())
	                  {
	                	  (m->sender_proc_w()).process_sent_indication( ConstMessageHandle(msg->pmi_->msg_), shawn::Processor::SHAWN_TX_STATE_CHANNEL_ACCESS_FAILURE, msg->sending_attempts_ );
	                  }
	                  //  Maximum number of sending atempts have been reached 
				      // therefore message will be dropped.
				      (*neighbors_)[*(msg->pmi_->src_)].erase(msg);
			        
			      }
			}
			      
		}
	*/
	void CsmaTransmissionModel::
		start_receive(Node* target, csma_msg* msg) throw()
		{
			//std::cout <<"csma: start receive " << target->id() << " at " << world().current_time() << std::endl;
			// If target is not busy
			if ( (*nodes_)[*target].busy_until_ <= msg->deliver_time_ ) 
			{
				(*nodes_)[*target].current_message_ = msg;
				(*nodes_)[*(msg->pmi_->src_)].clean_rx_busy_until_ = msg->deliver_time_ + msg->duration_;
			}
			else
			{
				(*nodes_)[*target].current_message_ = NULL;
			}
			// busy until latest packet is delivered
			if ((*nodes_)[*target].busy_until_ < msg->deliver_time_ + msg->duration_)
				(*nodes_)[*target].busy_until_ = msg->deliver_time_ + msg->duration_;
			
			
			
		}

	void CsmaTransmissionModel::
		end_receive(Node* target, csma_msg* msg) throw()
		{
			//std::cout <<"csma: end receive " << target->id() << " at " << world().current_time() << std::endl;
			if ((*nodes_)[*target].current_message_ == msg)
			{
				(*nodes_)[*target].current_message_ = NULL;
#ifdef CSMA_DEBUG
				std::cout <<"csma:     "<< target->id()<< " receives msg from " <<  msg->pmi_->src_->id() << " at " << world().current_time() << std::endl;
#endif				
				target->receive(ConstMessageHandle(msg->pmi_->msg_));
			}
#ifdef CSMA_DEBUG
			else
				std::cout <<"csma: ---- COLLISION at "<< target->id()<< " of msg from " <<  msg->pmi_->src_->id() << " at " << world().current_time() << std::endl;
#endif				
			
		}
	
	void CsmaTransmissionModel::
	handle_next_message(csma_msg *new_msg) throw()
	{
#ifdef CSMA_DEBUG		
		if (((*nodes_)[*(new_msg->pmi_->src_)].clean_rx_busy_until_) > world().current_time())
			std::cout << "hande next:: delayed csma "  <<  std::endl;
#endif
		new_msg->pmi_->time_ = std::max((*nodes_)[*(new_msg->pmi_->src_)].clean_rx_busy_until_, world().current_time());
		// If sending jitter is set, deliver time will be changed. ( Needed to avoid same deliver times due to 
		// processors sending at the beginning of a round.)
		if ( sending_jitter_ > 0.0 )
			new_msg->pmi_->time_+= ( new_msg->random( sending_jitter_lb_, sending_jitter_ ));
		new_msg->deliver_time_ = new_msg->pmi_->time_;
		// Create a new Event. Important is the MessageTag (new_msg). It defines which message will be send
		world_w().scheduler_w().new_event(*this, new_msg->pmi_->time_,new_msg);
	}
		
/*
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
*/
	}
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/transm_models/csma_transmission_model.cpp,v $
* Version $Revision: 38 $
* Date    $Date: 2007-06-08 14:30:12 +0200 (Fr, 08 Jun 2007) $
*-----------------------------------------------------------------------
* $Log: csma_transmission_model.cpp,v $
*-----------------------------------------------------------------------*/
