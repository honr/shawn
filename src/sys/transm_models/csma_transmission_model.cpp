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

namespace shawn
	{
	CsmaTransmissionModel::
		CsmaTransmissionModel(int bandwidth, double backoff, double sending_jitter, double sending_jitter_lb) :
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
		sending_jitter_lb_(sending_jitter_lb)
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
		new_msg->deliver_time = mi.time_;
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
			if(!msg->sending)
				deliver(msg); // If the message has not been send yet.
			else    //Otherwise the transmission has terminated
				(*neighbors_)[*(msg->pmi->src_)].erase(msg);

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
				// TODO: Is this the correct position to indicate that the message has been sent?
				const Message* m = msg->pmi->msg_.get();
				if (m->has_sender_proc())
		        	(m->sender_proc_w()).process_sent_indication( ConstMessageHandle(msg->pmi->msg_) );
				
				Node* source = msg->pmi->src_;
				deliver_num_++;
				/*std::cout <<"csma: "<< "Source Node: " <<  source->id() 
					<< ". Msg. send time: " <<  msg->deliver_time
					<< ". Msg. deliver time: " <<  msg->pmi->time_
					<< std::endl;*/
				for(unsigned int i=0; i< msg->destinations_.size(); i++){
					//All receiving nodes will check, if they already receive a message 
					receive(msg->destinations_[i]->dest_node_,msg);
					}
				//Define a new Event for the end of the transmision. (Specified by msg->isSending())
				event_handle_= world_w().scheduler_w().new_event(*this, msg->deliver_time + msg->duration_,msg);
			}
		}

	void CsmaTransmissionModel::
		listening(csma_msg* msg) throw(){
			double nextFreeTime=world().current_time();
			bool sending = false;
			//All neighbors are checked here
			EdgeModel::adjacency_iterator it, endit;
			for( it = world_w().begin_adjacent_nodes_w( *msg->pmi->src_ ),
				endit = world_w().end_adjacent_nodes_w( *msg->pmi->src_ );
				it != endit;
			++it )
				{
				if(msg->pmi->src_->id() ==it->id()) continue;
				MessageList* m = &((*neighbors_)[*it]);
				MessageList::iterator iter = m->begin();
				//Each neighbors messages are tested
				for(; iter!=m->end();++iter){
					csma_msg* temp = *iter;
					if(msg->deliver_time > temp->deliver_time && msg->deliver_time <= temp->deliver_time + temp->duration_)
						{
						/*std::cout <<"csma: "<< "Node " << temp->pmi->src_->id() << " is interfering" << std::endl;*/
						double time_fin = temp->deliver_time + temp->duration_;
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
				//std::cout<<"csma: "<<"Other node sending. Message from node: "<<msg->pmi->src_->id() <<" delayed"<< std::endl;
				msg->deliver_time= nextFreeTime + msg->backoff;
				//New Event. To the next time the medium is free + a backoff
				event_handle_ = world_w().scheduler_w().new_event(*this,msg->deliver_time,msg);
				}
		}

	void CsmaTransmissionModel::
		receive(Node* target, csma_msg* msg) throw(){
			bool inUse=false;
			EdgeModel::adjacency_iterator it, endit;
			// The target checks if one of it's neighbors (others than the msgs. one) is already sending
			for( it = world_w().begin_adjacent_nodes_w( *target),
				endit = world_w().end_adjacent_nodes_w( *target );
				it != endit;
			++it )
				{
				if(msg->pmi->src_->id() != it->id()){
					MessageList* m = &((*neighbors_)[*it]);
					MessageList::iterator iter = m->begin();
					//Here all messages of one node is checked
					for(; iter!=m->end();++iter){
						// First condition ckecks if a node not adjacent to the sender is delivering a msg. to target at the moment 
						// as well as if sender and receiver are delivering at the same moment.
						// This procedure correlates with reality. Both nodes are sending and cannot "hear" the other one
						if((msg->deliver_time >= (*iter)->deliver_time && msg->deliver_time <= (*iter)->deliver_time + (*iter)->duration_) )
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
				double cur_delay=(msg->deliver_time-msg->pmi->time_);
				average_delay_ +=cur_delay; 
				if( deliver_num_>1 )
					jitter_ += (cur_delay>last_delay_)? cur_delay-last_delay_ : last_delay_-cur_delay;
				last_delay_ = cur_delay;
				target->receive(ConstMessageHandle(msg->pmi->msg_));
				}

		}

	// ----------------------------------------------------------------------
	void CsmaTransmissionModel::
		find_destinations( csma_msg* pmsg )
		{
		EdgeModel::adjacency_iterator it, endit;
		for( it = world_w().begin_adjacent_nodes_w( *pmsg->pmi->src_ ),
			endit = world_w().end_adjacent_nodes_w( *pmsg->pmi->src_ );
			it != endit;
		++it )
			{
			if(pmsg->pmi->src_->id()!= it->id())
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
