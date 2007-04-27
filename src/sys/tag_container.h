/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_TAG_CONTAINER_H
#define __SHAWN_SYS_TAG_CONTAINER_H


#include <map>
#include <string>
#include "sys/util/refcnt_pointer.h"
#include "sys/util/refcnt_pointable.h"
#include "sys/util/defutils.h"

namespace shawn
{
   class Tag;

   DECLARE_HANDLES(TagContainer);

   class TagContainer
      : public RefcntPointable
   {
   public:
      ///@name Public Types
      ///@{
      ///
      typedef std::map<std::string, RefcntPointer<Tag> > TagMap;
      ///
      typedef TagMap::const_iterator tag_iterator;
      ///@}


      ///@name Ctor, Dtor
      ///@{
      ///
      TagContainer();
      ///
      virtual ~TagContainer();
      ///@}


      ///@name Tag Modification
      ///@{
      /// overwrites already existing tag with same name
      virtual void add_tag( const RefcntPointer<Tag>& ) throw();
      /// tag must be contained in this container!
      virtual void remove_tag( const RefcntPointer<const Tag>& ) throw();
      /// passing non-existing tag name is okay
      virtual void remove_tag_by_name( const std::string& ) throw();
      ///
      virtual void clear_tags( void ) throw();
      ///@}

      ///@name Tag Access
      ///@{      
      /// NULL if nonex
      virtual RefcntPointer<const Tag> find_tag( const std::string& ) const throw();
      /// NULL if nonex
      virtual RefcntPointer<Tag> find_tag_w( const std::string& ) throw();
      ///
      virtual tag_iterator begin_tags( void ) const throw();
      ///
      virtual tag_iterator end_tags( void ) const throw();
      ///
      virtual unsigned int count_tags( void ) const throw();
      ///@}

   private:
      TagMap tags_;
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/tag_container.h,v $
 * Version $Revision: 1.4 $
 * Date    $Date: 2005/08/05 10:00:37 $
 *-----------------------------------------------------------------------
 * $Log: tag_container.h,v $
 *-----------------------------------------------------------------------*/
