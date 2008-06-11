/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_PROCESSOR_H
#define __SHAWN_SYS_PROCESSOR_H

#include "shawn_config.h"
#include "sys/util/refcnt_pointable.h"
#include "sys/util/refcnt_pointer.h"
#include "sys/message.h"
#include "sys/util/defutils.h"
#include "sys/tag_syncable.h"
#include "sys/logging/logger.h"



namespace shawn
{
   class Node;

   DECLARE_HANDLES(Processor);


   /// Message-processing protocol resp. algorithm for nodes
   /** Superclass for anything that runs inside a node and
    *  processes messages. A node can contain multiple processors, eg, one for
    *  each protocol that is simulated simultaneously.
    *
    *  Most important methods are
    *  - boot(), which is called once after startup
    *  - work(), which is called periodically
    *  - process_message(), which is called for every message a node receives
    *  - send(), which sends a message
    */
   class Processor
      : public RefcntPointable,
        public TagSyncable
#ifdef ENABLE_LOGGING
        , public Logger
#endif
   {
   public:

      enum ProcessorState
         {
            Active,   ///< usual state
            Sleeping, ///< message reception stopped, but work() continues
            Inactive  ///< nonrevertable death
         };
      
      ///@name construction / destruction
      ///@{
		///
		Processor();
		///
		virtual ~Processor();
      ///@}


      ///@name node membership
      ///@{

		/** called by Node when processor gets added */
		virtual void set_owner( Node& ) throw();

		/** \return const Node that contains this Processor */
		virtual const Node& owner( void ) const throw();

		/** \return writable Node that contains this Processor */
		virtual Node& owner_w( void ) throw();

      ///@}

      ///@name processor interface
      ///@{

		/** This method is called \em once for every Processor that is
		*  contained in one special node, which is chosen arbitrarily. It
		*  can be identified by Node::is_special_node() 
		*
		*  It is called before boot() occurs. A Processor whose special_boot()
		*  is called will still get a boot() afterwards.
		*/
		virtual void special_boot( void ) throw();

		/** This method is called once for every Processor before the
		*  simulation starts (and again, if the simulation is reset)
		*/
		virtual void boot( void ) throw();

		/** This method is called if a Node receives a Message that is
		*  to be processed. Overload this method in derived
		*  Processors to produce specific protocols/algorithms.
		*
		*  \return \c true iff the Processor recognizes the passed
		*  message to be of "his own type". This makes the Node stop
		*  processing the message by passing it to all his Processors.
		*/
		virtual bool process_message( const ConstMessageHandle& ) throw();

		/** This method is called by the transmission model 
		* if a Message has been sent out and if a processor is set as sender_proc_ 
		* of the message. Overload this method in derived
		*  Processors to produce specific protocols/algorithms.
		*
		*  \return \c true iff the Processor recognizes the passed
		*  message to be of "his own type". This makes the Node stop
		*  processing the message by passing it to all his Processors.
		*/
		virtual bool process_sent_indication( const ConstMessageHandle& ) throw();

		/** Called once in each simulation round. Used for periodic tasks
		*  like collecting sensor values and generating messages if someting
		*  interesting has happened.
		*/
		virtual void work( void ) throw();

      ///@}


      ///@name misc access
      ///@{

		/** \return the containing Node's id number
		*  \sa Node::id()  */
		virtual int id( void ) const throw();

		/** \return the state of the Processor
		*  \sa set_state() */
		virtual ProcessorState state( void ) const throw();

		/** \return 
		*  \sa set_auto_terminate() */
		bool auto_terminate( void ) const throw();

		/** \return the current iteration round */
		virtual int simulation_round( void ) const throw();

		/** to be implemented */
		virtual void sync_to_tags( TagContainer& ) throw();

		/** to be implemented */
		virtual void sync_from_tags( const TagContainer& ) throw( std::runtime_error );

      ///@}

   protected:

      ///@name Message sending
      ///@{

		/** Enqueues a message for sending. Because the message is
		*  passed as a handle (see refcnt_pointer), a construct like
		*  \code
		*  send( new MyCoolMessage(1,2,3,4) );
		*  \endcode
		*  is fine. Whether a message is broadcast or
		*  unicast is decided by Message::is_unicast(). 
		*/
		virtual void send( const MessageHandle& ) throw();
	///@}

	///@name State management
	///@{

		/** Sets the state of the processor, where
		*  - Processor::Active   denotes the usual state
		*  - Processor::Sleeping stops message reception, but work() continues
		*  - Processor::Inactive is final
		*/
		virtual void set_state( const ProcessorState& ) throw();

		/** Sets the state of the processor, where
		*/
		void set_auto_terminate( bool enable ) throw();

      ///@}

   private:
      Node*          owner_;
      ProcessorState state_;
	  bool			 auto_terminate_;
   };


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/processor.h,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: processor.h,v $
 *-----------------------------------------------------------------------*/
