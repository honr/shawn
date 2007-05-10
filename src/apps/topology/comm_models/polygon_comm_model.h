/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_TOPOLOGY_COMM_MODELS_TOPOLOGY_COMM_MODEL_H
#define __SHAWN_APPS_TOPOLOGY_COMM_MODELS_TOPOLOGY_COMM_MODEL_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY
#include "shawn_config.h"
#ifdef HAVE_CGAL

#include "apps/topology/topology/topology_2d.h"
#include <CGAL/Polygon_2.h>
#include "sys/cgal/types.h"
#include "sys/util/keeper_managed.h"
#include "sys/util/refcnt_pointer.h"
#include "sys/util/defutils.h"
#include "sys/comm_models/communication_model_factory.h"
#include "sys/communication_model.h"

#include <vector>
#include <string>
#include <vector>

namespace topology
{

   class PolygonTopologyCommunicationModel
	   : public shawn::CommunicationModel
   {
   public:

      ///@name construction / destruction
      ///@{
      ///
		PolygonTopologyCommunicationModel();
		///
		virtual ~PolygonTopologyCommunicationModel();
		///
		virtual void init( void ) throw();
      ///@}

      ///@name CommunicationModel interface
      ///@{
		///
		virtual bool can_communicate_bidi( const shawn::Node&, const shawn::Node& ) const throw();
		///
		virtual bool can_communicate_uni( const shawn::Node&, const shawn::Node& ) const throw();
		/// returns whether communication_upper_bound() returns a useful value
		virtual bool exists_communication_upper_bound( void ) const throw();

		/** if exists_communication_upper_bound(), nodes whose euclidean
		*  distance exceeds communication_upper_bound() can never communicate
		*  in any direction
		*/
		virtual double communication_upper_bound( void ) const throw();
		///
		virtual bool is_status_available_on_construction( void ) const throw();

		virtual void set_size_hint(double size_hint)  throw();
      ///@}

   };
   

}

#endif
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/comm_models/disk_graph_model.h,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: disk_graph_model.h,v $
 *-----------------------------------------------------------------------*/
