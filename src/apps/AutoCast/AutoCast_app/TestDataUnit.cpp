#include "TestDataUnit.h"
#ifdef ENABLE_AUTOCAST_APP

namespace autocast{
	
	TestDataUnit::TestDataUnit() : DataUnit()
	{}

	TestDataUnit::TestDataUnit(int id,
				               int from_addr,
				               double x,
				               double y,
				               double time,
				               double max_life_time,
				               int priority,
				               int responsible_app,
				               const autocast::DistributionArea* area,
							   int size) : DataUnit(id,from_addr,x,y,time,max_life_time,priority,responsible_app,area) 
	{
		this->size(size);
	}

	TestDataUnit::TestDataUnit(const TestDataUnit& o) : DataUnit(o.id(),o.from_addr(),o.x(),o.y(),o.time(),o.max_life_time(),o.priority(),o.responsible_app(),o.distribution_area())
	{}

	TestDataUnit::~TestDataUnit()
	{}
}

#endif
