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

#include <stdlib.h>
#include <time.h>
#include <algorithm>

namespace shawn
{
	CsmaTransmissionModel::
		CsmaTransmissionModel() :
			received_(0), 
			dropped_(0), 
			packet_failure_(0), 
			average_delay_(0), 
			jitter_(0), 
			deliver_num_(0),
			with_random_backoff_(false)
	{}

	// ----------------------------------------------------------------------
	CsmaTransmissionModel::
		~CsmaTransmissionModel()
	{}

	// ----------------------------------------------------------------------
	void CsmaTransmissionModel::init()
		throw()
	{
		TransmissionModel::init();

		const SimulationEnvironment& se = world().simulation_controller().environment();
		with_random_backoff_ = se.optional_bool_param( "with_random_backoff", false );
		if(with_random_backoff_)
		{
			backoff_window_size_ = se.required_int_param("backoff_window_size");
			backoff_period_ = se.required_double_param("backoff_period");
		}

//		srand( unsigned(time(NULL)) );

        std::cout << "CSMA: Initialised. ";
		if( with_random_backoff_ )
			std::cout << "The size of backoff window is " <<  backoff_window_size_
			<< ", the duration of window slot is " << backoff_period_;
		else
			std::cout << "Without random backoff.";
		std::cout << std::endl;
	}

	// ----------------------------------------------------------------------
	void CsmaTransmissionModel::reset()
		throw()
	{
		average_delay_ = deliver_num_>0? average_delay_/deliver_num_ : 0;
		jitter_ = deliver_num_>1? jitter_/(deliver_num_-1) :0;

		std::cout << "CSMA: "<<  
            received_ <<" msgs. to be sent. "<< dropped_ << " msgs. dropped." 
			<< packet_failure_ << " packets fail to reach the destination." << " And average delay is "
            << average_delay_ << ", jitter is "<< jitter_ << std::endl;

		TransmissionModel::reset();
		received_ = 0;
		dropped_ = 0;
		packet_failure_ = 0;
		average_delay_ = 0;
		jitter_ = 0;
		deliver_num_ = 0;
		with_random_backoff_ = false;
	}

	// ----------------------------------------------------------------------
    bool
        CsmaTransmissionModel::
        supports_mobility( void )
        const throw(std::logic_error)
    {
        return world().edge_model().supports_mobility();
    }

	// ----------------------------------------------------------------------
	void CsmaTransmissionModel::send_message( MessageInfo &mi )
		throw()
	{
		received_++;
		csma_msg* new_msg = new csma_msg(&mi);
		find_destinations( new_msg );
		aired_messages_.push_back( new_msg );
	}

	// ----------------------------------------------------------------------
	void CsmaTransmissionModel::deliver_messages()
		throw()
	{
		if( aired_messages_.empty() )
			return;
		sort( aired_messages_.begin(),aired_messages_.end(), msg_pre() );
		int cur_round = (int)aired_messages_.front()->deliver_time;
		
		std::vector<csma_msg*> cur_round_msg;
		while( !aired_messages_.empty() && cur_round<this->world().simulation_round() ){
			
			arrange_one_round( cur_round, cur_round_msg );
			for(std::vector<csma_msg*>::iterator cur_it = cur_round_msg.begin();
				cur_it!=cur_round_msg.end(); cur_it++ )
			{
				deliver_one_message( *cur_it );
				deliver_num_++;
				double cur_delay = (*cur_it)->deliver_time-(*cur_it)->pmi->time_;
				average_delay_ += cur_delay;

				if( deliver_num_>1 )
					jitter_ += (cur_delay>last_delay_)? cur_delay-last_delay_ : last_delay_-cur_delay;
				
				last_delay_ = cur_delay;
				delete (*cur_it);
			}
			cur_round++;
		}

//		average_delay_ = deliver_num_>0? average_delay_/deliver_num_ : 0;
//		jitter_ = deliver_num_>1? jitter_/(deliver_num_-1) :0;
	}

	// ----------------------------------------------------------------------
	void
		CsmaTransmissionModel::
		arrange_one_round( int round, std::vector<csma_msg*>& cur_round_msg )
		throw()
	{
		sort( aired_messages_.begin(),aired_messages_.end(), msg_pre() );
		cur_round_msg.clear();

		std::cout << "arrange round " << round << std::endl;

		std::vector<csma_msg*>::iterator it=find_if(aired_messages_.begin(), aired_messages_.end(), same_round(round));

		for( bool first_msg=true; it!=aired_messages_.end() && (int)(*it)->deliver_time==round; )
		{
			if( first_msg )
			{
				std::cout << "The first msg of this round from node " << (*it)->pmi->src_->id() << std::endl;
				cur_round_msg.push_back( *it );
				it = aired_messages_.erase( it );
				first_msg = false;
			}
			else
			{
				std::cout << "arrange this msg from " << (*it)->pmi->src_->id() << std::endl;
				std::vector<csma_msg*>::iterator start_it;

				for( start_it=cur_round_msg.begin(); start_it!=cur_round_msg.end(); start_it++)
				{
					if( need_delay( (*it), (*start_it) ) )
					{
						if( (*it)->deliver_time==(*start_it)->deliver_time )
						{
							// Collision occurs	
							std::cout << "collision: node " << (*it)->pmi->src_->id() << " and " 
								<< (*start_it)->pmi->src_->id() << std::endl;
							remove_collided_destinations( (*it), (*start_it) );

							// To see if there are other collisions
							continue;
							//cur_round_msg.insert( start_it+1, *it );
							//it = aired_messages_.erase(it);
						}
						else
						{
							std::cout << "msg. from node " << (*it)->pmi->src_->id() << " Delay to next round" << std::endl;
							// Delay to next round
							int random_ = rand();
							std::cout << "old backoff: " << (*it)->backoff << "random: " << random_ 
								<< " backoff window: " << backoff_window_size_ << std::endl;
							(*it)->backoff = !with_random_backoff_? 0 :
								( (*it)->backoff<0? backoff_period_*(random_%backoff_window_size_) : 
								std::max<double>(0,(*it)->deliver_time-(*start_it)->deliver_time) );
						
							(*it)->deliver_time = (int)(*it)->deliver_time+1.0+(*it)->backoff; 
							
							std::cout << "\tnew backoff=" << (*it)->backoff << std::endl
								<< "\tdeliver time=" << (*it)->deliver_time <<std::endl;

							for(std::vector<csma_msg*>::iterator ii=aired_messages_.begin();
								ii!=aired_messages_.end(); ii++)
							{
								std::cout << "msg from node " << (*ii)->pmi->src_->id() << std::endl;
							}

							it++;	
							
						}
						break;
					}
				}
				if( start_it==cur_round_msg.end() )
				{
					cur_round_msg.push_back( *it );
					it = aired_messages_.erase( it );
				}
			}
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
			pmsg->push_new_destination( &(*it) );
		}

		//print destination info and othe info of a msg.
		std::cout << "\nSource Node: " << pmsg->pmi->src_->id() << " send time: " << pmsg->pmi->time_ <<std::endl;
		std::cout << "The neighbours: "  << std::endl;
		shawn::Node* pNode;
		for( unsigned int i=0; i<pmsg->destinations_.size(); i++)
		{
			pNode = pmsg->destinations_[i]->dest_node_;
			std::cout << "[neighbour" << i << "]: " << pNode->id() << "; ";
		}
		std::cout << std::endl;
		//
	}

	// ----------------------------------------------------------------------
	bool
		CsmaTransmissionModel::
		remove_collided_destinations( csma_msg* msg1, csma_msg* msg2 )
		throw()
	{
		bool removed_ = false;

		for( std::vector<csma_msg::msg_destination*>::iterator it1=msg1->destinations_.begin(); 
			it1 != msg1->destinations_.end(); it1++)
		{
			// Find the common destination node and set the valid_ tag of collided destination to be false
			for( std::vector<csma_msg::msg_destination*>::iterator it2=msg2->destinations_.begin();
				it2 != msg2->destinations_.end(); it2++ )
			{
				if( (*it1)->dest_node_==(*it2)->dest_node_ && ((*it1)->valid_||(*it2)->valid_))
				{
					packet_failure_ = (*it1)->valid_? packet_failure_+1 : packet_failure_;
					packet_failure_ = (*it2)->valid_? packet_failure_+1 : packet_failure_;
					(*it1)->valid_ = (*it2)->valid_ = false;
					removed_ = true;
					break;
				}
			}
		}
		return removed_;
	}

	// ----------------------------------------------------------------------
	bool
		CsmaTransmissionModel::
		deliver_one_message( csma_msg* msg )
		throw()
	{
		if( msg->pmi->msg_->is_unicast() )
        {
            ABORT_INCONSISTENT_CONFIGURATION("Unicast is not supported by the csma transmission model. Implement it -> NOW.");
        }

		// print msg. info.
		std::cout << "Source Node: " <<  msg->pmi->src_->id() 
			<< ". Msg. send time: " <<  msg->pmi->time_ 
			<< ". Msg. deliver time: " <<  msg->deliver_time
			<< std::endl;
		std::cout << "The valid neighbours:\n";

		bool no_destination = true;
		for( std::vector<csma_msg::msg_destination*>::iterator it=msg->destinations_.begin();
			it != msg->destinations_.end();
            ++it )
		{
			if( (*it)->valid_ )
			{
				(*it)->dest_node_->receive( ConstMessageHandle(msg->pmi->msg_) );
				std::cout << (*it)->dest_node_->id() << std::endl;
				no_destination = false;
			}
		}
		dropped_ = no_destination? dropped_+1 : dropped_;
		return !no_destination;
	}

	// ----------------------------------------------------------------------
	bool 
		CsmaTransmissionModel::
		need_delay( csma_msg *new_msg, csma_msg* msg )
		throw()
	{
		std::cout << "see if msgs from node " << new_msg->pmi->src_->id() << " and " << msg->pmi->src_->id() 
			<< " will interfere." << std::endl;
		 
		if( (int)new_msg->deliver_time != (int)msg->deliver_time )
		{
			std::cout << "\tno interfere" << std::endl;
			return false;
		}

		// The new message need to be delayed if the source nodes of new_msg and msg can communicate with each other,
		// that means the source node of new_msg(N) should have received a RTS message from the source node of msg(O).
		if( world_w().can_communicate_bidi( *new_msg->pmi->src_, *msg->pmi->src_ ) )
		{
			std::cout << "\t interfere" << std::endl;
			return true;
		}

		// new_msg should also be delayed if N and O have common neighbor, that means N should have received a CTS
		// message from that neighbor.
		for( EdgeModel::adjacency_iterator it=world_w().begin_adjacent_nodes_w( *new_msg->pmi->src_ ), 
			endit=world_w().end_adjacent_nodes_w( *new_msg->pmi->src_ );
			it != endit;
			++it)
		{
			for( EdgeModel::adjacency_iterator it1=world_w().begin_adjacent_nodes_w( *msg->pmi->src_ ), 
			endit1=world_w().end_adjacent_nodes_w( *msg->pmi->src_ );
			it1 != endit1;
			++it1)
			{
				// compare the address of Node structure
				if( &(*it) == &(*it1))
				{
					std::cout << "\tinterfere" << std::endl;
					return true;
				}
			}
		}
		std::cout << "\tno interfere" << std::endl;
		return false;
	}
}
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/transm_models/csma_transmission_model.cpp,v $
 * Version $Revision: 1.5 $
 * Date    $Date: 2005/08/05 10:00:43 $
 *-----------------------------------------------------------------------
 * $Log: csma_transmission_model.cpp,v $
 *-----------------------------------------------------------------------*/
