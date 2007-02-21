/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_EDGE_MODELS_LIST_EDGE_MODEL_H
#define __SHAWN_SYS_EDGE_MODELS_LIST_EDGE_MODEL_H

#include "sys/edge_models/manual_edge_model.h"
#include <map>
#include <set>

namespace shawn
{

   class ListEdgeModel
      : public ManualEdgeModel
   {
   public:
      template<typename NodeType,
               typename NodeHoodIt>
      class ListIteratorHelper
         : public AbstractIteratorHelper<NodeType>
      {
      public:
         typedef AbstractIteratorHelper<NodeType> base_type;
         ListIteratorHelper( const ListEdgeModel&,
							 CommunicationPattern<NodeType>& p,
							 NodeType& n,
                             NodeHoodIt,
                             NodeHoodIt );
         virtual ~ListIteratorHelper();
         virtual void init( void ) throw();
         virtual void next( void ) throw();
         virtual NodeType* current( void ) const throw();
         virtual AbstractIteratorHelper<NodeType>* clone( void ) const throw();
      private:
         const ListEdgeModel&       edge_model_;
         NodeHoodIt				hood_it_;
         NodeHoodIt				hood_end_it_;
		 NodeType&				    node_;
      };

   public:
      


      ///
      ListEdgeModel();
      ///
      virtual ~ListEdgeModel();

      ///
      virtual int
		  nof_adjacent_nodes( const Node&, CommunicationDirection d = CD_BIDI  ) 
         const throw();
      ///
      virtual const_adjacency_iterator
		  begin_adjacent_nodes( const Node&, CommunicationDirection d = CD_BIDI  )
         const throw();
      ///
      virtual const_adjacency_iterator
      end_adjacent_nodes( const Node& )
         const throw();
      ///
      virtual adjacency_iterator
		  begin_adjacent_nodes_w( Node&, CommunicationDirection d = CD_BIDI )
         throw();
      ///
      virtual adjacency_iterator
      end_adjacent_nodes_w( Node& )
         throw();
      /** do not use directly -- use add_edge() of the communication
       *  model, which should be a ManualEdgeCommunicationModel
       */
      virtual void add_edge( Node&, Node& ) throw();
      
      ///
      virtual Box observer_initial_zone(Node&, const Vec& pos, const Vec& velo ) throw();
      ///
      Box observer_update_zone(Node&, const Vec& newpos, const Vec& velo ) throw();
      ///
      bool supports_mobility( void ) const throw();

      virtual void node_added( Node& ) throw();
      virtual void node_removed( Node& ) throw();

   protected:
      virtual void add_node_neighbors( Node&, const Vec& pos, const Vec& velo ) throw();
   private:
	   std::map<int, std::set<Node*> > neighbors_;
		
//dynamic node array
   };


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/edge_models/list_edge_model.h,v $
 * Version $Revision: 1.11 $
 * Date    $Date: 2007/01/02 15:37:20 $
 *-----------------------------------------------------------------------
 * $Log: list_edge_model.h,v $
 *-----------------------------------------------------------------------*/
