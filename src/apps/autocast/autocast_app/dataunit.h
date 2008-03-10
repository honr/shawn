/************************************************************************
 ** This file is part of the network simulator ns-2.                   **
 ** Copyright (C) 2006 by AutoNomos (www.auto-nomos.de)                **
 ** This part of ns-2 is free software; you can redistribute it and/or **
 ** modify it under the terms of the GNU General Public License,       **
 ** version 2.                                                         **
 **                                                                    **
 ** \author Axel Wegener <wegener@itm.uni-luebeck.de>                  **
 **                                                                    **
 ************************************************************************/
#ifndef ns_autocastdataunit_h
#define ns_autocastdataunit_h

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_AUTOCAST

#include "sys/util/refcnt_pointer.h"
#include "sys/util/refcnt_pointable.h"
#include "sys/util/defutils.h"
#include "distribution_area.h"
#include "autocast_app_processor.h"
#include "sys/message.h"
#include "sys/processor.h"
#include <string>
#include <map>
#include <vector>


namespace autocast {

	class AutoCastAppProcessor;
	class AutoCastProcessor;

	//using namespace std;

	/** \brief This class describes the content of an Hovering Data Cloud. 
	 **        It should be derived by different applications that use the 
	 **        HDCtransport layer to add the applications data.            
	 **/
	DECLARE_HANDLES(DataUnit);
	class DataUnit 
		: public shawn::RefcntPointable
	{
		friend class AutoCastAppProcessor;
		//friend class AutoCastProcessor;

	public:
		DataUnit();
		/// Copy constructor
		DataUnit(const DataUnit& o);
		
		/// Constructor
		DataUnit(int id,
				 int from_addr,
				 double x,
				 double y,
				 double time,
				 double max_life_time,
				 int priority,
				 int responsible_app,
				 const autocast::DistributionArea* area);

		/// Destructor
		virtual ~DataUnit();

		/// Returns an id composed of creation time and fromAddr_
		int id() const throw();

		/// Returns the creation time
		double time() const throw();

		/// Returns x-value of creation 
		double x() const throw();

		/// Returns y-value of creation
		double y() const throw();

		/// Returns the size of this object for use from the message size()
		virtual int size() const throw();

		/// Every application processor has an id to which a DataUnit is associated
		int responsible_app() const throw();

		/// Returns the distribution area of the DataUnit
		const autocast::DistributionArea* distribution_area() const throw();

		/// Returns the maximum livetime interval of the DataUnit. If this time expires
		/// the DataUnit runs out of validity.
		double max_life_time() const throw();

		/// The hop count of the DataUnit
		int hop_count() const throw();

		// For incrementing the hop count while sending
		void hop_count(const int) throw();

		/// A priority among the DataUnits - range[0,255]
		int priority() const throw();

		/// The address of the node the DataUnit was created
		int from_addr() const throw();

	protected:
	
		virtual void size(const int s) throw();
	
	private:
		/// The size of the DataUnit in bytes
		int size_;

		/// Describes the area in that the HDC should be distributed
		const autocast::DistributionArea* area_;

		/// Maximum lifetime (relative to time_).
		/// Set to -1 (default), if the data unit should not be dispatched by the AutoCast layer.
		double max_life_time_;

		/// Maximum HopCount
		/// Set to -1 (default), if the data unit should not be dispatched by the AutoCast layer
		/// Otherwise hopCount is reduced by one every hop
		int hop_count_;

		/// Priority between 0 (rather unimportant) up to 255 (high important)
		int priority_;
	    
		/// the sender's address
		int from_addr_;

		/// Time and place of creation
		double time_;
		double x_;
		double y_;

		/// The responsible application id
		int responsible_app_;
		
		/// The unique id of hte DataUnit
		int id_;

		friend int operator<(const DataUnit& lv, const DataUnit& rv);
	};
	/// Less operation by id
	int operator<(const autocast::DataUnit& lv, const autocast::DataUnit& rv);
}
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source$
 * Version $Revision: 270 $
 * Date    $Date: 2007-01-22 17:34:49 +0100 (Mo, 22 Jan 2007) $
 *-----------------------------------------------------------------------
 * $Log$
 * Revision 1.11  2006/08/24 13:38:57  wegener
 * - spyglass support
 * - HDCtransport erweitert
 *
 * Revision 1.10  2006/08/03 12:43:56  wegener
 * *** empty log message ***
 *
 * Revision 1.9  2006/07/11 12:01:08  wegener
 * annotation added
 *
 * Revision 1.8  2006/07/10 12:49:42  wegener
 * First version of HDCtransport
 *
 * Revision 1.7  2006/06/16 14:39:04  wegener
 * compile errors cleard
 *
 * Revision 1.6  2006/06/16 11:44:55  wegener
 * *** empty log message ***
 *
 * Revision 1.5  2006/06/15 14:18:14  wegener
 * doxygen
 *
 * Revision 1.4  2006/06/15 13:18:58  wegener
 * added copyright and CVS-log
 *
 *-----------------------------------------------------------------------*/
