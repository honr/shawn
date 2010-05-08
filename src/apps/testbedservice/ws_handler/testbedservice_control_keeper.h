/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_TESTBEDSERVICE_CONTROL_KEEPER_H_
#define __SHAWN_APPS_TESTBEDSERVICE_CONTROL_KEEPER_H_

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/ws_handler/testbedservice_control.h"
#include "sys/util/handle_keeper.h"

namespace testbedservice
{
    ///  Keeper that holds Testbedservice Control Instances.
    /**
     */
    class TestbedserviceControlKeeper
        : public shawn::HandleKeeper<TestbedserviceControl>
    {
    public:
        ///@name construction / destruction
        ///@{
        ///
        TestbedserviceControlKeeper();
        ///
        virtual ~TestbedserviceControlKeeper();
        ///@}
    };


}

#endif
#endif
