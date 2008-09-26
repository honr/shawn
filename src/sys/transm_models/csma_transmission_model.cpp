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
		CsmaTransmissionModel(
				double short_inter_frame_spacing,
				double long_inter_frame_spacing,
				int max_short_inter_frame_spacing_size,
				int bandwidth, 
				bool slotted_backoff,
				double backoff, 
				int max_sending_attempts,
				int backoff_factor_base,
				int min_backoff_exponent,
				int max_backoff_exponent) :
	/*received_(0), 
		dropped_(0), 
		packet_failure_(0), 
		average_delay_(0), 
		jitter_(0), 
		deliver_num_(0),*/
		short_inter_frame_spacing_(short_inter_frame_spacing),
		long_inter_frame_spacing_(long_inter_frame_spacing),
		max_short_inter_frame_spacing_size_(max_short_inter_frame_spacing_size),
		slotted_backoff_(slotted_backoff),
		backOff_(backoff),
		bandwidth_(bandwidth),
		nodes_(NULL),
		max_sending_attempts_(max_sending_attempts),
		backoff_factor_base_(backoff_factor_base),
		min_backoff_exponent_(min_backoff_exponent),
		max_backoff_exponent_(max_backoff_exponent)
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
		TransmissionModel::reset();
		if( nodes_ != NULL )
		{
			delete nodes_;
			nodes_ = NULL;
		}
		//This gives us an Array of Messages for all nodes which can be received in O(1)
		//nodes_ = new DynamicNodeArray<CsmaState>(world_w());
		
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
		csma_msg* new_msg = new csma_msg(&mi, (double) mi.msg_->size() / bandwidth_);
		#ifdef CSMA_DEBUG
			std::cout <<"new msg="<<new_msg<<" at node "<<mi.src_->id()<< ((mi.msg_->is_ack())?(" ack"):(" msg")) << std::endl;
		#endif
		
		if (mi.msg_->is_ack())
		{
			//acks are treated differently from other message:
			// - they are scheduled for immediate delivery (ignoring IFS
			// - They are not queued, i.e. they can "overtake" regular messages in beeing sent
			
			(*nodes_)[*(mi.src_)].outgoing_messages_.push_front(new_msg);
			#ifdef CSMA_DEBUG
				EventScheduler::EventHandle eh = world_w().scheduler_w().new_event(*this, world().current_time(),new NodeInfo(mi.src_));

				std::cout <<eh<<" sched in send message at "<< eh->time()<<" for ack"<<std::endl;
				std::cout.flush();
			#else
				world_w().scheduler_w().new_event(*this, world().current_time(),new NodeInfo(mi.src_));
			#endif
			
		} else {
			(*nodes_)[*(mi.src_)].outgoing_messages_.push_back(new_msg);
			if (!(*nodes_)[*(mi.src_)].busy_)
			{
				(*nodes_)[*(mi.src_)].busy_= true;

			#ifdef CSMA_DEBUG
				EventScheduler::EventHandle eh = world_w().scheduler_w().new_event(*this, world().current_time()+0.00001,new NodeInfo(mi.src_));
				std::cout <<eh<<" sched in send message at "<< eh->time()<<" for next msg"<<std::endl;
				std::cout.flush();
			} else std::cout <<"no schedule, node busy"<<std::endl;
			std::cout.flush();
			#else
				world_w().scheduler_w().new_event(*this, world().current_time()+0.00001,new NodeInfo(mi.src_));
			}
			#endif
		}
	}


	// ----------------------------------------------------------------------
	void 
		CsmaTransmissionModel::
		timeout(EventScheduler & event_scheduler, EventScheduler::EventHandle event_handle, double time, EventScheduler::EventTagHandle & event_tag_handle) throw()
	{
		
		NodeInfo* ni = dynamic_cast<NodeInfo* >(event_tag_handle.get());
		if (ni!=NULL) // this means that a new message must be scheduled
		{
#ifdef CSMA_DEBUG
			std::cout <<event_handle<<" fired (handle next message) at "<< event_handle->time()<< " at node "<<ni->n_->id()<< std::endl;
#endif
			if (!((*nodes_)[*(ni->n_)]).outgoing_messages_.empty()) // get next outgoing message
			{
				csma_msg *new_msg = ((*nodes_)[*(ni->n_)]).outgoing_messages_.front();
				((*nodes_)[*(ni->n_)]).outgoing_messages_.pop_front();
				handle_next_message(new_msg);
			} else { //no more messages to be sent
				((*nodes_)[*(ni->n_)]).busy_=false;
			}
			
		} else {
		
			csma_msg* msg = dynamic_cast<csma_msg* >(event_tag_handle.get());
			if(msg != NULL){
				if(!msg->sending_) //message not on air yet. We should now check wether the medium is free, and eventually send
				{
#ifdef CSMA_DEBUG
					std::cout <<event_handle<<" fired (mac access) at "<< event_handle->time()<<" msg="<<msg<<std::endl;
#endif
					while( (!msg->pmi_->msg_->is_ack())&& ((*nodes_)[*(msg->pmi_->src_)].busy_until_ > msg->deliver_time_) && (msg->sending_attempts_< max_sending_attempts_))
					{
						
						double wait_periods = shawn::uniform_random_0i_1i()* (int)(pow(backoff_factor_base_,std::min(min_backoff_exponent_+msg->sending_attempts_, max_backoff_exponent_))-1);
						if (slotted_backoff_) wait_periods = round(wait_periods);
						msg->deliver_time_= world().current_time() + (backOff_ *  wait_periods);

						//New Event to now + backoff
			            ++msg->sending_attempts_;
					}
					if (msg->deliver_time_ <= world().current_time())
						start_send(msg); // If the message has not been send yet.
					else 
					{
						if (msg->sending_attempts_< max_sending_attempts_) // reschedule for next media acess
						{
#ifdef CSMA_DEBUG
							EventScheduler::EventHandle eh = world_w().scheduler_w().new_event(*this, msg->deliver_time_ ,msg);
							std::cout <<eh<<" sched for new mac access at "<< eh->time()<<" msg="<<msg<<std::endl;
#else
							world_w().scheduler_w().new_event(*this, msg->deliver_time_ ,msg);
#endif
						}
						else // media access finally failed
						{
							const Message* m = msg->pmi_->msg_.get();
							if (m->has_sender_proc())
			                	(m->sender_proc_w()).process_sent_indication( ConstMessageHandle(msg->pmi_->msg_), shawn::Processor::SHAWN_TX_STATE_CHANNEL_ACCESS_FAILURE, msg->sending_attempts_ );
							

#ifdef CSMA_DEBUG
							EventScheduler::EventHandle eh = world_w().scheduler_w().new_event(*this, world().current_time()+0.00001,new NodeInfo(msg->pmi_->src_));
							std::cout <<eh<<" sched in media access at "<< eh->time()<<" for next msg"<<std::endl;
							std::cout.flush();
#else
							world_w().scheduler_w().new_event(*this, world().current_time()+0.00001,new NodeInfo(msg->pmi_->src_));
#endif

						}
					}
					
						
				}
				else    //Otherwise the transmission has terminated
				{
				    
//					const Message* m = msg->pmi_->msg_.get();
	                
	                //deliver_num_++;
#ifdef CSMA_DEBUG
					std::cout <<event_handle<<" fired (end_send) at "<< event_handle->time()<<" msg="<<msg<<std::endl;
#endif
	                end_send(msg);
//	                if (m->has_sender_proc())
//	                	(m->sender_proc_w()).process_sent_indication( ConstMessageHandle(msg->pmi_->msg_), shawn::Processor::SHAWN_TX_STATE_SUCCESS, msg->sending_attempts_ );
	                
				
				}

			}
		}
#ifdef CSMA_DEBUG
		std::cout.flush();
#endif
	}

	// ----------------------------------------------------------------------
	void CsmaTransmissionModel::
		deliver_messages() throw(){}
	
	

	// ----------------------------------------------------------------------
	void CsmaTransmissionModel::
		start_send(csma_msg* msg) throw()
		{
#ifdef CSMA_DEBUG
		std::cout <<"csma: "<< msg->pmi_->src_->id() << " start sending at " << world().current_time() << " deliver_time_="<< msg->deliver_time_ << std::endl;
#endif				
		(*nodes_)[*(msg->pmi_->src_)].busy_until_ = msg->deliver_time_ + msg->duration_;
		//(*nodes_)[*(msg->pmi_->src_)].clean_rx_busy_until_ = msg->deliver_time_ + msg->duration_;
			(*nodes_)[*(msg->pmi_->src_)].current_message_ = msg;

			if(!msg->pmi_->msg_->is_ack())  //ack is not followed by its own IFS, but "defers" the IFS of messages it belongs to
			{
				double ifs_time = (msg->pmi_->msg_->size() <=max_short_inter_frame_spacing_size_) ? short_inter_frame_spacing_ : long_inter_frame_spacing_;
				(*nodes_)[*(msg->pmi_->src_)].ifs_end_=std::max(msg->deliver_time_ + msg->duration_+ifs_time, (*nodes_)[*(msg->pmi_->src_)].ifs_end_);
#ifdef CSMA_DEBUG
				std::cout <<"csma: msg clean_busy="<<msg->deliver_time_ + msg->duration_<<" IFS_time " << ifs_time <<" IFS_end " << (*nodes_)[*(msg->pmi_->src_)].ifs_end_ << std::endl;
#endif
			} else {
				(*nodes_)[*(msg->pmi_->src_)].ifs_end_+=msg->duration_;
#ifdef CSMA_DEBUG
				std::cout <<"csma: ack IFS_end " << (*nodes_)[*(msg->pmi_->src_)].ifs_end_ << std::endl;
#endif
			}	
			msg->sending_=true;
			
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
#ifdef CSMA_DEBUG
			std::cout <<"deliver time: "<< msg->deliver_time_ << " , duration " << msg->duration_ << "sched at "<< msg->deliver_time_ + msg->duration_<< " msg="<<msg<<std::endl;
#endif


#ifdef CSMA_DEBUG
			EventScheduler::EventHandle eh = world_w().scheduler_w().new_event(*this, msg->deliver_time_ + msg->duration_,msg);
			std::cout <<eh<<" sched in start send at "<< eh->time()<<" msg="<<msg<<std::endl;
			std::cout.flush();
#else
			world_w().scheduler_w().new_event(*this, msg->deliver_time_ + msg->duration_,msg);
#endif
		}
	

	
	
	

	// ----------------------------------------------------------------------
	void CsmaTransmissionModel::
		end_send(csma_msg* msg) throw()
		{
#ifdef CSMA_DEBUG
			std::cout <<"csma: "<< msg->pmi_->src_->id() << " end  sending at " << world().current_time()  <<" deliver time: "<< msg->deliver_time_ << " , duration " << msg->duration_ << " endtime="<< msg->deliver_time_ + msg->duration_<< " msg="<<msg<< std::endl;
			assert(world().current_time()+0.00001 >= msg->deliver_time_ + msg->duration_);
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
			const Message* m = msg->pmi_->msg_.get();
           if (m->has_sender_proc())
            	(m->sender_proc_w()).process_sent_indication( ConstMessageHandle(msg->pmi_->msg_), shawn::Processor::SHAWN_TX_STATE_SUCCESS, msg->sending_attempts_ );
			
			if (!msg->pmi_->msg_->is_ack()) // acks are scheduled separately, so do not schedule next message if this one was an ack
			{
#ifdef CSMA_DEBUG
				EventScheduler::EventHandle eh = world_w().scheduler_w().new_event(*this, world().current_time()+0.00001,new NodeInfo(msg->pmi_->src_));
				std::cout <<eh<<" sched in end send at "<< eh->time()<<" for next msg"<<std::endl;
			} else {
				std::cout <<"no sched in end send, was ack"<<std::endl;
				std::cout.flush();
#else
				world_w().scheduler_w().new_event(*this, world().current_time()+0.00001,new NodeInfo(msg->pmi_->src_));
#endif
			}

		}
	void CsmaTransmissionModel::
		start_receive(Node* target, csma_msg* msg) throw()
		{
#ifdef CSMA_DEBUG
			std::cout <<"csma: start receive " << target->id() << " at " << world().current_time() << " delivertime_=" << msg->deliver_time_<< std::endl;
#endif
			// If target is not busy --> no collision (so far)
			if ( (*nodes_)[*target].busy_until_ <= msg->deliver_time_ ) 
			{
				(*nodes_)[*target].current_message_ = msg;
				//(*nodes_)[*(target)].clean_rx_busy_until_ = msg->deliver_time_ + msg->duration_;
				if(!msg->pmi_->msg_->is_ack()) // calc end of inter frame spacing
				{
					
					double ifs_time = (msg->pmi_->msg_->size() <=max_short_inter_frame_spacing_size_) ? short_inter_frame_spacing_ : long_inter_frame_spacing_;
					(*nodes_)[*(target)].ifs_end_=std::max(msg->deliver_time_ + msg->duration_+ifs_time, (*nodes_)[*(target)].ifs_end_);
#ifdef CSMA_DEBUG
					std::cout <<"csma: msg clean_busy="<<msg->deliver_time_ + msg->duration_<<" IFS_time " << ifs_time <<" IFS_end " << (*nodes_)[*(target)].ifs_end_ << std::endl;
#endif
				} else {  //ack is not followed by its own IFS, but "defers" the IFS of messages it belongs to
					(*nodes_)[*(target)].ifs_end_=std::max((*nodes_)[*(target)].ifs_end_+msg->duration_, msg->deliver_time_ + msg->duration_);
#ifdef CSMA_DEBUG
					std::cout <<"csma: ack IFS_end " << (*nodes_)[*(target)].ifs_end_ << std::endl;
#endif
				}	
			}
			else //--> collision
			{
				(*nodes_)[*target].current_message_ = NULL;
			}
			// busy until latest packet is delivered
			if ((*nodes_)[*target].busy_until_ < msg->deliver_time_ + msg->duration_)
				(*nodes_)[*target].busy_until_ = msg->deliver_time_ + msg->duration_;
			
			
			
		}


	// ----------------------------------------------------------------------
	void CsmaTransmissionModel::
		end_receive(Node* target, csma_msg* msg) throw()
		{
			//std::cout <<"csma: end receive " << target->id() << " at " << world().current_time() << std::endl;
			if ((*nodes_)[*target].current_message_ == msg)
			{
				(*nodes_)[*target].current_message_ = NULL;
#ifdef CSMA_DEBUG
				std::cout <<"csma:     "<< target->id()<< " receives msg from " <<  msg->pmi_->src_->id() << " at " << world().current_time() << " IFS_end " << (*nodes_)[*(target)].ifs_end_ << std::endl;
#endif				
				target->receive(ConstMessageHandle(msg->pmi_->msg_));
			}
			else
			{
#ifdef CSMA_DEBUG
				std::cout <<"csma: ---- COLLISION at "<< target->id()<< " of msg from " <<  msg->pmi_->src_->id() << " at " << world().current_time() << std::endl;
#endif				
				target->receive_dropped(ConstMessageHandle(msg->pmi_->msg_));
			}
		}
	

	// ----------------------------------------------------------------------
	void CsmaTransmissionModel::
	handle_next_message(csma_msg *new_msg) throw()
	{
		if(new_msg->pmi_->msg_->is_ack()) //send ack without csma
		{
			/*
#ifdef CSMA_DEBUG
			if(((*nodes_)[*(new_msg->pmi_->src_)].clean_rx_busy_until_) > world().current_time())
			{
				std::cout<<"curtime="<< world().current_time()<< " clean_busy_until="<< (*nodes_)[*(new_msg->pmi_->src_)].clean_rx_busy_until_  << " at " <<new_msg->pmi_->src_->id()<<std::endl;
			}
#endif
						
			assert(((*nodes_)[*(new_msg->pmi_->src_)].clean_rx_busy_until_) <= world().current_time());*/
#ifdef CSMA_DEBUG
			if ( ((*nodes_)[*(new_msg->pmi_->src_)].ifs_end_) <= world().current_time())
			{
				std::cout<<"curtime="<< world().current_time()<< " ifs_end="<< (*nodes_)[*(new_msg->pmi_->src_)].ifs_end_ << " at " <<new_msg->pmi_->src_->id()<<std::endl;
			}
#endif
			assert(((*nodes_)[*(new_msg->pmi_->src_)].ifs_end_) > world().current_time());
			assert(((*nodes_)[*(new_msg->pmi_->src_)].ifs_end_) > world().current_time()+new_msg->duration_);
			new_msg->pmi_->time_ = world().current_time();
			new_msg->deliver_time_ = new_msg->pmi_->time_;
			start_send(new_msg);
			//EventScheduler::EventHandle eh = world_w().scheduler_w().new_event(*this, new_msg->pmi_->time_,new_msg);
			//std::cout <<eh<<" sched in handle next(1)at "<< eh->time()<<" msg="<<new_msg<<std::endl;
			
		} else {
			/*
			#ifdef CSMA_DEBUG		
				if (((*nodes_)[*(new_msg->pmi_->src_)].clean_rx_busy_until_) > world().current_time())
					std::cout << "hande next:: delayed csma "  <<  std::endl;
				if((*nodes_)[*(new_msg->pmi_->src_)].ifs_end_<(*nodes_)[*(new_msg->pmi_->src_)].clean_rx_busy_until_)
				{
					std::cout<<"curtime="<< world().current_time()<< " ifs_end="<< (*nodes_)[*(new_msg->pmi_->src_)].ifs_end_ <<" clean_busy_until="<< (*nodes_)[*(new_msg->pmi_->src_)].clean_rx_busy_until_ << " at " <<new_msg->pmi_->src_->id()<<std::endl;
				}
				
			#endif
				assert((*nodes_)[*(new_msg->pmi_->src_)].ifs_end_>=(*nodes_)[*(new_msg->pmi_->src_)].clean_rx_busy_until_);*/
				new_msg->pmi_->time_ = std::max((*nodes_)[*(new_msg->pmi_->src_)].ifs_end_, world().current_time());

			    //New Event to now + backoff
			double wait_periods = shawn::uniform_random_0i_1i()* (int)(pow(backoff_factor_base_,min_backoff_exponent_)-1);
			if (slotted_backoff_) wait_periods = round(wait_periods);
			new_msg->pmi_->time_+= backOff_ *  wait_periods;
            ++new_msg->sending_attempts_;
			
			new_msg->deliver_time_ = new_msg->pmi_->time_;
			// Create a new Event. Important is the MessageTag (new_msg). It defines which message will be send
			assert(!new_msg->sending_);
#ifdef CSMA_DEBUG
			EventScheduler::EventHandle eh = world_w().scheduler_w().new_event(*this, new_msg->pmi_->time_,new_msg);
			std::cout <<eh<<" sched in handle next (2) at "<< eh->time()<<" msg="<<new_msg<<std::endl;
#else
			world_w().scheduler_w().new_event(*this, new_msg->pmi_->time_,new_msg);
#endif
			
		}
#ifdef CSMA_DEBUG
		std::cout.flush();
#endif
	}
		

	}
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/transm_models/csma_transmission_model.cpp,v $
* Version $Revision: 38 $
* Date    $Date: 2007-06-08 14:30:12 +0200 (Fr, 08 Jun 2007) $
*-----------------------------------------------------------------------
* $Log: csma_transmission_model.cpp,v $
*-----------------------------------------------------------------------*/
