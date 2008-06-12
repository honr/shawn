/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_SYS_TRANS_MODELS_CSMA_H
#define __SHAWN_SYS_TRANS_MODELS_CSMA_H

#include "sys/transmission_model.h"
#include "sys/event_scheduler.h"
#include <sys/misc/dynamic_node_array.h>
#include "sys/transm_models/csma_transmission_model_message.h"
#include <set>

namespace shawn
{
	/**
	 *@brief A CsmaTransmissionmodel senses the channel before it delivers a message.
	 * This class implements the CSMA/CA transmission model.
	 * A processor waits for the medium to be free before sending. 
	 * If a neighbor is already sending, the new message will be delayed until the end of
	 * the transmission plus a backoff time
	 * The parameters for this model are:
	 * bandwith: the medium's bandwith in bits/sec
	 * backoff: fixed (not variable) backoff in sec.
	 * sending_jitter: defines the upperbound of the jitter that is use to simulate the
	 *                  "asynchronous behaviour" of the nodes. 
	 * sending_jitter_lb: defines the lower_bound of the sending jitter.  
	 * This module can be called by:
	 * transm_model=csma bandwidth=9600 backoff=0.05 sending_jitter=0.02 sending_jitter_lb= 0.001
	 */
	class CsmaTransmissionModel : public TransmissionModel,EventScheduler::EventHandler
	{
	public:
		///@name construction, destruction and support for life cycle
		///@{
		CsmaTransmissionModel(int bandwidth, double backoff, double sending_jitter, double sending_jitter_lb,
		    int max_sending_attempts, int backoff_factor_base);
		~CsmaTransmissionModel();
		/**
		 *@brief Initialize the csma transmission model
		 *
		 * Gets the list of neighbors
		 */
		virtual void init() throw();
		/**
		 *@brief Reset the csma transmission model
		 *
		 *Clear the statistic variables and set the parameters to its default value.
		 */
		virtual void reset() throw();
		///@}

		///@name Transmission Model Implementation
		///@{
		/**
		 *@brief Mobility is depending on mobility support from the edge model
		 *
		 *The edge model is used to determine the 1-hop neighbours 
         *which will receive the message
		 */

		virtual bool supports_mobility( void ) const throw(std::logic_error);
		
		/**
		 *@brief Stores each message in a vector for delivery at the next simulation round start
		 *
		 *A new structure of csma_msg will be build and inserted into aired_messages_ 
		 *@see aired_messages_
		 */
        virtual void send_message( MessageInfo& mi ) throw();

		  ///@name basic method inherited from EventHandler 
		  ///@{
		  /**
			* Timeout Event. Receives a csma_msg as a EventScheduler::EventTagHandle.
			* If message has not been sent yet, deliver() will be called.
			* Otherwise message has been transfered and will be deleted from sending processor's MessageList
		  */
		  virtual void timeout(EventScheduler & event_scheduler, EventScheduler::EventHandle event_handle, double time, EventScheduler::EventTagHandle & event_tag_handle) throw();
		  ///@}

		 /**
		 *@brief Delivers all messages which are in the vector
		 * Method inherited from shawn::TransmissionModel. Is not implemented in this module since the messages are 
		 * added to the shawn::EventScheduler and do not have to be delivered at the beginning of a round.
		 */ 
        virtual void deliver_messages() throw();
		///@}

		///@name Delivers a message.
		/**
		* This method implements the CSMA/CA algorithm.
		* The processor listens if a neighbor is sending. 
		* If no neighbor is sending, it will deliver its message, otherwise
		* it will delay the message to the end of the transmission plus a random backoff.
		*@param msg Pointer to the message to be send
		*/
		void deliver(csma_msg* msg) throw();
		///}
		private:
		///@name Method which tests if medium is free
		///@{
		/**
		* This method tests if any neighbor is sending a message.
		* If medium is free, message is set to sending otherwise not.
		*@param msg Pointer to the message to be send
		*/
		void listening(csma_msg* msg) throw();
		///}
		///@name Method in which a given node will receive a message
		///@{
		/**
		* This method delivers a message to a given node.
		* It has to check that the target is not receiving a message from a node which is not adjacent to the sender.
		* In this case the message will be dropped.
		*@param target Pointer to the target node
		*@param msg Pointer to the message to be send
		*/
		void receive(Node* target, csma_msg* msg) throw();
		///@}

		/**
		 *@brief Find the neighbors of the node who send a message
		 *
		 *Find all the destinations of a new-coming message, which are the neighbours of its source node
		 *@param pmsg the message whose source node's neighbors to be determined
		 */
		void find_destinations( csma_msg* pmsg );
		

		///Number of received messages
		int received_;

		///Number of messages dropped
		int dropped_;

		// The number of transmission failure
		int packet_failure_;

		///Number of packets failed to reach the destination
		double average_delay_;

		///The average variation of delay
		double jitter_;

		///Number of messages delivered so far
		int deliver_num_;

		///The delay of last message
		double last_delay_;

		///Backoff that will be waited after a message has been delayed
		double backOff_;
		///Bandwidth, defines the throughput
		int bandwidth_;
		/// EventHandle
		EventScheduler::EventHandle event_handle_;
		/// Jitter for starting the transmission
		double sending_jitter_;

		double sending_jitter_lb_;
		// Maximum number of attempts to send a message
		int max_sending_attempts_;
		//Factor specifying the base of the factor for increasing backoff 
		int backoff_factor_base_;

		///The messages that have been sent by the nodes and are waiting for delivery
		typedef std::set<csma_msg*> MessageList;
		/// List of nodes in transmission range.
		DynamicNodeArray<MessageList>*  neighbors_;
	};
}
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/transm_models/csma_transmission_model.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fr, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: csma_transmission_model.h,v $
 *-----------------------------------------------------------------------*/
