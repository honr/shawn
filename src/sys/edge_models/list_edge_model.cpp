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

using namespace std;

namespace shawn
{
	// ----------------------------------------------------------------------
	/// Compares two ListEdgeModel::NodeInfo instances using the Node* node_ member
	bool operator<(const struct ListEdgeModel::NodeInfo& a, const struct ListEdgeModel::NodeInfo& b) 
	{ 
		return a.node_ < b.node_; 
	}

	// ----------------------------------------------------------------------
	template<typename NodeType, typename NodeHoodIt>
	shawn::ListEdgeModel::ListIteratorHelper<NodeType, NodeHoodIt>::
	ListIteratorHelper( const ListEdgeModel& em, EdgeModel::CommunicationDirection dir, NodeType& n, NodeHoodIt it, NodeHoodIt endit )
		: AbstractIteratorHelper<NodeType>(dir),
		  edge_model_( em ),
  		  hood_it_( it ),
		  hood_end_it_( endit ),
		  node_( n )
	{}

	// ----------------------------------------------------------------------
	template<typename NodeType, typename NodeHoodIt>
	shawn::ListEdgeModel::ListIteratorHelper<NodeType, NodeHoodIt>::
		~ListIteratorHelper()
	{}

	// ----------------------------------------------------------------------
	template<typename NodeType, typename NodeHoodIt>
	void
		shawn::ListEdgeModel::ListIteratorHelper<NodeType, NodeHoodIt>::
		init( void )
		throw()
	{
		//Skip forward to the first neighboring node that is in reach with the given communication direction
		while( hood_it_ != hood_end_it_ ) 
		{
			if( (*hood_it_).comm_dir_[direction_] )
				break;
			++hood_it_; 
		}
	}

	// ----------------------------------------------------------------------
	template<typename NodeType, typename NodeHoodIt>
	void
		shawn::ListEdgeModel::ListIteratorHelper<NodeType, NodeHoodIt>::
		next( void )
		throw()
	{
		if( hood_it_ != hood_end_it_ ) 
		{
			//Skip the current element and call init which skips to the next element
			++hood_it_;
			init();
		}
	}

	// ----------------------------------------------------------------------
	template<typename NodeType, typename NodeHoodIt>
	NodeType*
		shawn::ListEdgeModel::ListIteratorHelper<NodeType, NodeHoodIt>::
		current( void )
		const throw()
	{
		if( hood_it_ == hood_end_it_ )
			return NULL;
		else
			return (*hood_it_).node_;
	}

	// ----------------------------------------------------------------------
	template<typename NodeType,	typename NodeHoodIt>
	shawn::EdgeModel::AbstractIteratorHelper<NodeType>*
		shawn::ListEdgeModel::ListIteratorHelper<NodeType,NodeHoodIt>::
		clone( void )
		const throw()
	{
		std::cerr << "shawn::ListEdgeModel::ListIteratorHelper<NodeType, NodeHoodIt>::clone!" << std::endl << std::flush;
		return new shawn::ListEdgeModel::ListIteratorHelper<NodeType,NodeHoodIt>
			( edge_model_, direction_, node_, hood_it_, hood_end_it_ );
	}

	// ----------------------------------------------------------------------
	template class shawn::ListEdgeModel::ListIteratorHelper<Node, std::set< ListEdgeModel::NodeInfo>::iterator>;

	// ----------------------------------------------------------------------
	template class shawn::ListEdgeModel::ListIteratorHelper<const Node, std::set< ListEdgeModel::NodeInfo>::const_iterator>;




	// ----------------------------------------------------------------------
	ListEdgeModel::
		~ListEdgeModel()
	{}

	// ----------------------------------------------------------------------
	void
		ListEdgeModel::
		set_world( World& w ) 
		throw()
	{
		EdgeModel::set_world(w);
		neighbors_ = new DynamicNodeArray< std::set< ListEdgeModel::NodeInfo > >(w);
	}

	// ----------------------------------------------------------------------
	Box
		ListEdgeModel::
		observer_initial_zone(Node& v, const Vec& pos, const Vec& velo ) 
		throw()
	{
		//TODO: Could possible also work with SimpleMovement by returning single-point boxes and repairing neighborhoods
		assert( dynamic_cast<const NoMovement*>(&v.movement()) != NULL );
		return Box::INFINITE_3D_SPACE;
	}

	// ----------------------------------------------------------------------  
	void
		ListEdgeModel::
		node_added(Node& n)
		throw()
	{
		//Must be done here, because the edge model's node_added is always called before the dynamic node array
		neighbors_->node_added(n);
		//Add all neighbors of the node to our internal list
		add_node_neighbors(n);
	}


	// ----------------------------------------------------------------------
	void
		ListEdgeModel::
		add_node_neighbors(Node& v)
		throw()
	{
		//For all nodes in the world. Please note that both directions must be checked here because a node
		//may have been added later to the world than another node and hence the neighborship must be checked
		//again
		for( World::node_iterator it = world_w().begin_nodes_w(), endit = world_w().end_nodes_w(); it != endit; ++it )
		{
			//Check if u->v can communicate unidirectional
			if( communication_model().can_communicate_uni(v, *it) )
				add_edge(v, *it);

			//Check if v->u can communicate unidirectional
			if( communication_model().can_communicate_uni(*it, v) )
				add_edge(*it, v);
		}
	}

	// ----------------------------------------------------------------------
	void
		ListEdgeModel::
		add_edge( Node& u, Node& v )
		throw()
	{
		//u -> v: Set the OUTgoing link and update the CD_ANY and CD_BIDI state
		ListEdgeModel::NodeInfo& uv = node_info(u, v);
		uv.comm_dir_[CD_OUT] = true;
		uv.update();

		//v -> u: Set the INcoming link and update the CD_ANY and CD_BIDI state
		ListEdgeModel::NodeInfo& vu = node_info(v, u);
		vu.comm_dir_[CD_IN] = true;
		vu.update();
	}

	// ----------------------------------------------------------------------
	int
		ListEdgeModel::
		nof_adjacent_nodes( const Node& v, CommunicationDirection d  ) 
		const throw()
	{
		if (d == CD_ANY) 
		{
			//For the any communication direction, this is simply the size of the neighborhood
			assert( neighbors_ != NULL );
			return (*neighbors_)[v].size();
		}
		else
		{
			//For any other communication direction, actually count the neighbors
			int count = 0;
			for (EdgeModel::const_adjacency_iterator it = begin_adjacent_nodes(v, d), endit = end_adjacent_nodes(v); it!=endit;++it) 
				count++;
			return count;
		}
	}

	// ----------------------------------------------------------------------
	EdgeModel::const_adjacency_iterator
		ListEdgeModel::
		begin_adjacent_nodes(const Node& v, CommunicationDirection d)
		const throw()
	{
		assert( neighbors_ != NULL );
		std::set< ListEdgeModel::NodeInfo >& s = (*neighbors_)[v];

		return EdgeModel::const_adjacency_iterator( 
			new ListEdgeModel::ListIteratorHelper<const Node, std::set< ListEdgeModel::NodeInfo>::const_iterator>
					( *this, d, v,  s.begin(), s.end() ) );
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
		assert( neighbors_ != NULL );
		std::set< ListEdgeModel::NodeInfo >& s = (*neighbors_)[v];

		return EdgeModel::adjacency_iterator
			( new ListEdgeModel::ListIteratorHelper<Node, std::set< ListEdgeModel::NodeInfo>::iterator >
				( *this, d, v, s.begin(), s.end() ) );
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
	ListEdgeModel::NodeInfo&
		ListEdgeModel::
		node_info(Node& u, Node& v)
	{
		assert( neighbors_ != NULL );
		std::set< ListEdgeModel::NodeInfo >& l = (*neighbors_)[u];
		std::set< ListEdgeModel::NodeInfo >::iterator pos = l.find(&v);

		if( pos == l.end() ) 
		{
			ListEdgeModel::NodeInfo	ni;
			ni.node_ = &v;
			l.insert(ni);
			return *l.find(&v);
		}

		return *pos;
	}

	// ----------------------------------------------------------------------
	Box
		ListEdgeModel::
		observer_update_zone(Node& v, const Vec& newpos, const Vec& velo )
		throw()
	{
		assert( velo.euclidean_norm() < EPSILON);
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
		node_removed(Node& node)
		throw()
	{
		assert( neighbors_ != NULL );

		//Remove the node from the neighbor lists of all other nodes
		for(World::node_iterator it = world_w().begin_nodes_w(), end = world_w().end_nodes_w(); it!=end; ++it)
			(*neighbors_)[*it].erase( &node );

		//Delete the entry of the node itself
		(*neighbors_).node_removed( node );
	}

}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/edge_models/list_edge_model.cpp,v $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: list_edge_model.cpp,v $
*-----------------------------------------------------------------------*/
