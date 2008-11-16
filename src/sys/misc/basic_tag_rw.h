/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_SYS_MISC_BASIC_TAG_RW_H
#define __SHAWN_SYS_MISC_BASIC_TAG_RW_H

#include "sys/node.h"
#include "sys/taggings/basic_tags.h"
#include <string>

namespace shawn
{

   template <typename T> struct tag_traits;
   // ----------------------------------------------------------------------
   template<>
   struct tag_traits<int>
   {
      typedef IntegerTag BaseClass;
   };
   // ----------------------------------------------------------------------
   template<>
   struct tag_traits<double>
   {
      typedef DoubleTag BaseClass;
   };
   // ----------------------------------------------------------------------
   template<>
   struct tag_traits<std::string>
   {
      typedef StringTag BaseClass;
   };
   // ----------------------------------------------------------------------
   template<>
   struct tag_traits<bool>
   {
      typedef BoolTag BaseClass;
   };
   // ----------------------------------------------------------------------
   template <typename T>
   void write_tag( Node& node, const std::string& tag_name, T value )
      throw()
   {
      typedef typename tag_traits<T>::BaseClass BaseClass;
      
      TagHandle tag = node.find_tag_w( tag_name );
      if ( tag.is_not_null() )
      {
         BaseClass* tagtype = dynamic_cast<BaseClass*>( tag.get() );
         if ( tagtype != NULL )
         {
            tagtype->set_value( value );
         }
      }
      else
      {
         BaseClass *tagtype = new BaseClass( tag_name, value );
         tagtype->set_persistency( true );
         node.add_tag( tagtype );
      }
   };
   // ----------------------------------------------------------------------
   template <typename T>
   bool read_tag( const Node& node, const std::string& tag_name, T& value )
      throw()
   {
      typedef typename tag_traits<T>::BaseClass BaseClass;
      
      ConstTagHandle tag = node.find_tag( tag_name );
      if ( tag.is_not_null() )
      {
         const BaseClass* tagtype = dynamic_cast<const BaseClass*>( tag.get() );
         value = tagtype->value();
         return true;
      }
   
      return false;
   };
      
}

#endif
