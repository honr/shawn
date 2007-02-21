/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/comm_models/stochastic_comm_model.h"
#include "sys/node.h"
#include "sys/world.h"
#include "sys/node_distance_estimate.h"
#include "sys/distance_estimates/distance_estimate_keeper.h"
#include "sys/misc/random/uniform_random_variable.h"
#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

namespace shawn
{

   StochasticCommunicationModel::
   StochasticCommunicationModel( int mode, double smooth_factor )
      : 
        range_       ( 1.0 ),
		mode_			( mode ),
		smooth_factor_	( smooth_factor ),
		ready_			(false),
		connectivity_	(vector< vector<bool> >(5))
		//		connectivity_	(NULL )
   {}
   // ----------------------------------------------------------------------
   StochasticCommunicationModel::
   ~StochasticCommunicationModel()
   {
	   //if (connectivity_ != NULL) delete connectivity_;
   }
   // ----------------------------------------------------------------------
   void
   StochasticCommunicationModel::
   init( void )
      throw()
   {
      CommunicationModel::init();
	  cout<< "world with nodes: " << world().node_count() << endl;
//      initialized_ = true;
   }
   // ----------------------------------------------------------------------
   void
   StochasticCommunicationModel::
   set_transmission_range( double tr )
      throw()
   {
//      assert( !initialized_ );
      range_ = tr;
	  ready_ = false;
//      has_range_ = true;
      cout << "StochasticCommunicationModel: Transmission range set to ["<< range_ <<"]" << endl;
	  //calculate_connectivity();
   }
   // ----------------------------------------------------------------------
   double
   StochasticCommunicationModel::
   transmission_range( void )
      const throw()
   {
      return range_;
   }
   // ----------------------------------------------------------------------
   bool
   StochasticCommunicationModel::
   can_communicate_bidi( const Node& u,
                         const Node& v )
      const throw()
   {
      return ( can_communicate_uni(u, v) && can_communicate_uni(v, u) );
   }
   // ----------------------------------------------------------------------
   bool
   StochasticCommunicationModel::
   can_communicate_uni( const Node& u,
                        const Node& v )
      const throw()
   {
	   if (!ready_ || u.id() >= int(connectivity_.size())  || v.id() >= int(connectivity_.size()) ) {
		    StochasticCommunicationModel* s = (StochasticCommunicationModel*)(this);
		    s->calculate_connectivity();
	   }
      return connectivity_[u.id()][ v.id()];
   }
   // ----------------------------------------------------------------------
   bool
   StochasticCommunicationModel::
   exists_communication_upper_bound( void )
      const throw()
   {
      return true;
   }
   // ----------------------------------------------------------------------
   double
   StochasticCommunicationModel::
   communication_upper_bound( void )
      const throw()
   {
      return range_+range_*smooth_factor_;
   }
   // ----------------------------------------------------------------------
   bool
   StochasticCommunicationModel::
   is_status_available_on_construction( void )
      const throw()
   {
      return true;
   }
   // ----------------------------------------------------------------------
/*   void 
   StochasticCommunicationModel::
   set_size_hint(double size_hint)
   throw()
   {
        if(has_range_) 
            return;

        has_range_ = true;
        range_ = size_hint;
        cout << "StochasticCommunicationModel: Using size hint ["<< range_ <<"] as comm range" << endl;
   }
*/


   void 
   StochasticCommunicationModel::
   calculate_connectivity()

   {
		UniformRandomVariable urv;
		urv.set_lower_bound( -1.0);
		urv.set_upper_bound( 1.0 );
		urv.set_lower_bound_inclusive( true );
		urv.set_upper_bound_inclusive( true );
		urv.init();

		int n = world().node_count();
		//int n1, n2;
		double dist;
		Vec v1;
//		cout << "StochasticCommunicationModel::calculate_connectivity(): creating vectors for " << n << " nodes" << endl << flush;
		//connectivity_.clear();
		//connectivity_ = new vector<vector<bool> >(n);
		connectivity_.resize(n);
		for (int i = 0; i< n; i++) connectivity_[i] = vector<bool>(n);
//		n1 = n2 = 0;
		for( shawn::World::const_node_iterator it = world().begin_nodes(), endit = world().end_nodes();it != endit; ++it) 
		{
			for( shawn::World::const_node_iterator it2 = world().begin_nodes(), endit2 = world().end_nodes();it2 != endit2; ++it2) 
			{    
				//cout << "n1=" << it->id() << " n2=" << it2->id() << endl << flush;
				v1 = it->real_position()-it2->real_position();
				dist = v1.euclidean_norm();
//				connectivity_[it->id()][it2->id()] = ( dist *  dist <= range_ * range_ * ( 1 + smooth_factor_ * urv ));
				connectivity_[it->id()][it2->id()] = ( dist  <=  range_ * ( 1 + smooth_factor_ * urv ));

				//				n2++;
			}
//			n1++;
		}
		ready_ = true;

   }


}

