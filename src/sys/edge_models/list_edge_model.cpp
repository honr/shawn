/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/edge_models/list_edge_model.h"
#include "sys/world.h"
#include "sys/communication_model.h"
#include "sys/misc/box.h"
#include "sys/node_movements/no_movement.h"
#include <iostream>


namespace shawn
{

	template<typename NodeType, typename NodeHoodIt>
		shawn::ListEdgeModel::ListIteratorHelper<NodeType,NodeHoodIt>::
		ListIteratorHelper( const ListEdgeModel& em,
		CommunicationPattern<NodeType>& p,
		NodeType& n,
		NodeHoodIt it,
		NodeHoodIt endit )
		: 
		AbstractIteratorHelper<NodeType>(p),
		edge_model_   ( em ),
		hood_it_      ( it ),
		hood_end_it_  ( endit ),
		node_		  ( n )
	{}
	// ----------------------------------------------------------------------
	template<typename NodeType,
		typename NodeHoodIt>
		shawn::ListEdgeModel::ListIteratorHelper<NodeType,NodeHoodIt>::
		~ListIteratorHelper()
	{}
	// ----------------------------------------------------------------------
	template<typename NodeType,
		typename NodeHoodIt>
		void
		shawn::ListEdgeModel::ListIteratorHelper<NodeType,NodeHoodIt>::
		init( void )
		throw()
	{
		while( hood_it_ != hood_end_it_ ) {
			if (base_type::pattern_(node_, *(*hood_it_))) break;
			++hood_it_; 
		}
}
	// ----------------------------------------------------------------------
	template<typename NodeType,
		typename NodeHoodIt>
		void
		shawn::ListEdgeModel::ListIteratorHelper<NodeType,NodeHoodIt>::
		next( void )
		throw()
	{

		if( hood_it_ != hood_end_it_ ) {
			++hood_it_;
			while( hood_it_ != hood_end_it_ ) {
				if (base_type::pattern_(node_, *(*hood_it_))) break;
				++hood_it_; 
			}
		}
	}
	// ----------------------------------------------------------------------
	template<typename NodeType,
		typename NodeHoodIt>
		NodeType*
		shawn::ListEdgeModel::ListIteratorHelper<NodeType,NodeHoodIt>::
		current( void )
		const throw()
	{
		if( hood_it_ == hood_end_it_ ) {
			return NULL;
		} else {
			return *hood_it_;

		}
	}
	// ----------------------------------------------------------------------
	template<typename NodeType,
		typename NodeHoodIt>
		shawn::EdgeModel::AbstractIteratorHelper<NodeType>*
		shawn::ListEdgeModel::ListIteratorHelper<NodeType,NodeHoodIt>::
		clone( void )
		const throw()
	{
		std::cout << "clone" << std::endl << std::flush;

		return new
			shawn::ListEdgeModel::ListIteratorHelper
			<NodeType,NodeHoodIt>( edge_model_,
                                *(base_type::pattern_.clone()),
			node_,
			hood_it_,
			hood_end_it_ );
	}
	// ----------------------------------------------------------------------
	template class shawn::ListEdgeModel::ListIteratorHelper<Node,std::set<Node*>::iterator>;
	template class shawn::ListEdgeModel::ListIteratorHelper<const Node,std::set<Node*>::const_iterator>;




	ListEdgeModel::
		ListEdgeModel()
	{}
	// ----------------------------------------------------------------------
	ListEdgeModel::
		~ListEdgeModel()
	{
		//delete(neighbors_);
	}
	// ----------------------------------------------------------------------
	Box
		ListEdgeModel::
		observer_initial_zone(Node& v, const Vec& pos, const Vec& velo ) 
		throw()
	{
		// could possible also work with SimpleMovement by
		// return single-point boxes and repairing neighborhoods
		// but that's future stuff
		assert( dynamic_cast<const NoMovement*>(&v.movement()) != NULL );

		//std::cout << "observer_init..." << std::endl;

		//v.set_minimum_storage(1);

		if( communication_model().is_status_available_on_construction() )
			add_node_neighbors(v,pos,velo);

		return Box::INFINITE_3D_SPACE;
	}
	// ----------------------------------------------------------------------
	void
		ListEdgeModel::
		add_node_neighbors( Node& v, const Vec& pos, const Vec& velo )
		throw()
	{
		const CommunicationModel& cm = communication_model();

		for( World::node_iterator
			it    = world_w().begin_nodes_w(),
			endit = world_w().end_nodes_w();
		it != endit; ++it )
			if( cm.can_communicate_uni(v,*it) || cm.can_communicate_uni(*it, v) ) {
				add_edge(v,*it);
			}
	}
	// ----------------------------------------------------------------------
	int
		ListEdgeModel::
		nof_adjacent_nodes( const Node& v, CommunicationDirection d  ) 
		const throw()
	{
		//Node* n = (Node*)(&v);
		int count;

		if (d == CD_ANY) {
			std::set<Node*> s;
			std::map<int, std::set<Node*> >::const_iterator pos;
			pos = neighbors_.find(v.id());
			count = s.size();
		} else 
		{
			count=0;
			for (EdgeModel::const_adjacency_iterator it = begin_adjacent_nodes(v, d), endit = end_adjacent_nodes(v); it!=endit;++it) 
				count++;
		}
		return count;
		assert(false);
		return -1;
	}

	// ----------------------------------------------------------------------
	EdgeModel::const_adjacency_iterator
		ListEdgeModel::
		begin_adjacent_nodes( const Node& v, CommunicationDirection d )
		const throw()
	{
		// Node* n = (Node*)(&v);
		//assert(false);
		//return NULL;
		//std::cout << "list.const_begin_adj_nodes" << std::endl;
		const std::set<Node*>* s;
		std::map<int, std::set<Node*> >::const_iterator pos;
		pos = neighbors_.find(v.id());

		assert(pos!=neighbors_.end());
		s = &(pos->second);

		/*
		std::set<Node*> ss;
		int i=0;
		for( shawn::World::const_node_iterator it2 = world().begin_nodes(); ((it2 != world().end_nodes())&& (++i<10)); ++it2) {
			shawn::Node* n = (Node*)(&(*it2));
			std::cout << "Inserting: " << n << " " << n->id() << std::endl << std::flush;
			//		i++;
			ss.insert(n);
		}
		*/

//		std::cout << "SIze: " << s->size() << std::endl << std::flush;

		return EdgeModel::const_adjacency_iterator
			( new ListEdgeModel::ListIteratorHelper
			<const Node,std::set<Node*>::const_iterator>
			( *this, *(get_communication_pattern(d)), v,  s->begin(), s->end() ) );
	}
	// ----------------------------------------------------------------------
	EdgeModel::const_adjacency_iterator
		ListEdgeModel::
		end_adjacent_nodes( const Node& )
		const throw()
	{
		return EdgeModel::const_adjacency_iterator( NULL );
	}
	// ----------------------------------------------------------------------
	EdgeModel::adjacency_iterator
		ListEdgeModel::
		begin_adjacent_nodes_w( Node& v, CommunicationDirection d )
		throw()
	{
		return EdgeModel::adjacency_iterator
			( new ListEdgeModel::ListIteratorHelper
			<Node,std::set<Node*>::iterator>
			( *this, *(get_communication_pattern_w(d)), v, neighbors_[v.id()].begin(), neighbors_[v.id()].end() ) );
	}
	// ----------------------------------------------------------------------
	EdgeModel::adjacency_iterator
		ListEdgeModel::
		end_adjacent_nodes_w( Node& v )
		throw()
	{
		return EdgeModel::adjacency_iterator( NULL );
	}
	// ----------------------------------------------------------------------
	void
		ListEdgeModel::
		add_edge( Node& u,
		Node& v )
		throw()
	{
		//std::cout << "adding edge from " << u.label() << " to " << v.label() << std::endl;
		neighbors_[u.id()].insert(&v);
		neighbors_[v.id()].insert(&u);
		//u.add_neighbour(v);
		//v.add_neighbour(u);
	}
	// ----------------------------------------------------------------------
	Box
		ListEdgeModel::
		observer_update_zone(Node& v, 
		const Vec& newpos,
		const Vec& velo )
		throw()
	{
		assert( velo.euclidean_norm()<EPSILON);
		return Box::INFINITE_3D_SPACE;;
	}
	// ----------------------------------------------------------------------
	bool
		ListEdgeModel::
		supports_mobility( void )
		const throw()
	{
		return false;
	}
   // ----------------------------------------------------------------------  
   void
   ListEdgeModel::
   node_added( Node& )
      throw()
   {
      // do nothing, the important 'add-method' here is add_edge()
   }
   // ----------------------------------------------------------------------
   void
   ListEdgeModel::
   node_removed( Node& node )
      throw()
   {
      // delete the node in the neighborhood of all other nodes
      for ( std::map<int, std::set<Node*> >::iterator
               it = neighbors_.begin();
               it != neighbors_.end();
               ++it )
      {
         (*it).second.erase( &node );
      }

      // delete the entry of the node itself
      neighbors_.erase( node.id() );
   }


}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/edge_models/list_edge_model.cpp,v $
* Version $Revision: 1.18 $
* Date    $Date: 2007/01/03 13:00:55 $
*-----------------------------------------------------------------------
* $Log: list_edge_model.cpp,v $
 *-----------------------------------------------------------------------*/
