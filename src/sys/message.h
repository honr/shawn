/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_MESSAGE_H
#define __SHAWN_SYS_MESSAGE_H

#include "sys/util/refcnt_pointer.h"
#include "sys/util/refcnt_pointable.h"
#include "sys/util/defutils.h"


namespace shawn
{
   class Node;

   DECLARE_HANDLES(Message);

   /// Superclass of all messages
   /** Abstract superclass. Derive specific message types to
    *  get specialized message classes.
    *
    *  It is not always necessary to overload anything or to add
    *  methods in derived classes -- type detection via
    *  dynamic_cast<>() may be sufficient.
    */
   class Message
      : public RefcntPointable
   {
   public:
      ///@name construction / destruction
      ///@{
      ///
      Message();
      ///
      virtual ~Message();
      ///@}


      ///@name Type of message
      ///@{
      /** \return whether this message is to be sent to a single
       *  node, unicast_target(), only. If receiver is no neighbor of the
       *  sender, the message will be silently lost.
       *
       *  This implementation always returns \c false, ie, all messages
       *  are broadcast by default.
       *  \sa unicast_target()
       */
      virtual bool is_unicast( void )
         const throw();
      /** Target of a unicast message.
       *  \warning Use of ID numbers might be replaced by (Node*) soon
       */
      virtual int unicast_target( void )
         const throw();
      /** \return whether the message is received not only by neighbors,
       *  but also by the sender itself. Only useful if
       *  is_unicast() is \c false.
       *
       *  This implementation always returns \c false, ie, all messages
       *  are broadcast by default.
       */
      virtual bool is_loopback_message( void )
         const throw();
      ///@}


      ///@name Message properties
      ///@{
      /** ID of the sender. Set via set_source() automatically from within
       *  Processor::send() resp. Node::send().
       */
      const Node& source( void )
         const throw();
      /** ID of the sender. Set via set_source() automatically from within
       *  Processor::send() resp. Node::send().
       */
      Node& source_w( void )
         const throw();
      /** \return \c true if source has already been set, \c false if source is
       *  still NULL..
       */
      bool has_source( void )
         const throw();
      /** Simulation round when message was sent.
       *  Set via set_timestamp() automatically from within
       *  Processor::send() resp. Node::send().
       */
      int timestamp_simulation_round( void )
         const throw();
      /** Time when message was sent.
       *  Set via set_timestamp() automatically from within
       *  Processor::send() resp. Node::send().
       */
      double timestamp_time( void )
         const throw();
      /** Called by Processor::send() resp. Node::send().
       *  \internal
       */
      void set_source( Node& ) throw();
      /** Called by Processor::send() resp. Node::send().
       *  \internal
       */
      void set_timestamp( int, double ) throw();
      ///
      virtual int size( void ) const throw();
      ///@}
	  virtual void setSize( int )throw();

   private:

      mutable Node* source_;
      int stamp_round_;
      double stamp_time_;
		int size_;
   };


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/message.h,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: message.h,v $
 *-----------------------------------------------------------------------*/
