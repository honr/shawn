/** \file
 *  Definitions of constants for TraCI
 *
 *  \author Axel Wegener <wegener@itm.uni-luebeck.de>
 *  \author Björn Hendriks
 */

/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef TRACICONSTANTS_H
#define TRACICONSTANTS_H


namespace traci
{

struct TraciConstants
{
    enum Command
    {
        /// command: simulation step
        CmdSimstep = 0x01,

        /// command: set maximum speed
        CmdSetmaxspeed = 0x11,

        /// command: stop node
        CmdStop = 0x12,

        /// command: set lane
        CmdChangelane = 0x13,

        /// command: slow down
        CmdSlowdown = 0x14,

        /// command: change route
        CmdChangeroute = 0x30,

        /// command: change target
        CmdChangetarget = 0x31,

        /// command: simulation parameter
        CmdSimparameter = 0x70,

        /// command: position conversion
        CmdPositionconversion = 0x71,

        /// command: distance request
        CmdDistancerequest = 0x72,

        /// command: scenario
        CmdScenario = 0x73,

        /// command: move node
        CmdMovenode = 0x80,

        /// command: close sumo
        CmdClose = 0x7f,

        /// command:
        CmdUpdatecalibrator = 0x50,

        /// command: get all traffic light ids
        CmdGetalltlids = 0x40,

        /// command: get traffic light status
        CmdGettlstatus = 0x41,

        /// command: report traffic light id
        CmdTlidlist = 0x90,

        /// command: report traffic light status switch
        CmdTlswitch = 0x91,

        /// command: get lane variable
        CmdGetLaneVariable = 0xa3,
        /// command: get vehicle variable
        CmdGetVehicleVariable = 0xa4,
        /// response command: get lane variable
        CmdGetLaneVariableResponse = 0xb3,
        /// command: change vehicle state
        CmdChangeVehicleState = 0xc4
    };


    enum DataType
    {
        /// omit position
        PositionNone    = 0x00,
        /// 2d Cartesian coordinates
        Position2d      = 0x01,
        /// 2.5d Cartesian coordinates
        Position25d    = 0x02,
        /// 3d Cartesian coordinates
        Position3d      = 0x03,
        /// position on road map
        PositionRoadmap = 0x04,
        /// boundary box
        TypeBoundingbox = 0x05,
        /// polygon
        TypePolygon = 0x06,
        /// unsigned byte
        TypeUbyte = 0x07,
        /// signed byte
        TypeByte = 0x08,
        /// 32 bit integer
        TypeInteger = 0x09,
        /// color
        TypeColor = 0x11,
        /// float
        TypeFloat = 0x0a,
        /// double
        TypeDouble = 0x0b,
        /// 8 bit ascii string
        TypeString = 0x0c,
        /// list of traffic light phases
        TypeTlphaselist = 0x0d,
        /// list of strings
        TypeStringlist = 0x0e,
        /// compound object
        TypeCompound = 0x0f
    };


    /// Domain IDs for scenario command
    enum DomainIdScenario
    {
        /// road map domain
        DomRoadmap = 0x00,
        /// vehicle domain
        DomVehicle = 0x01,
        /// traffic lights domain
        DomTrafficlights = 0x02,
        /// points of interest domain
        DomPoi = 0x03,
        /// polygon domain
        DomPolygon = 0x04
    };


    /// Scenario Command Variable IDs
    enum ScenarioVariableId
    {
        /// count of domain objects
        DomvarCount = 0x01,
        /// position of a domain object
        DomvarPosition = 0x02,
        /// boundaries of simulation net
        DomvarBoundingbox = 0x03,
        /// speed of a node
        DomvarSpeed = 0x04,
        /// actual phase of a traffic light
        DomvarCurtlphase = 0x05,
        /// next phase of a traffic light
        DomvarNexttlphase = 0x06,
        /// type of a domain object (POI, polygon)
        DomvarType = 0x07,
        /// layer a domain object is located at (POI, polygon)
        DomvarLayer = 0x08,
        /// shape of a polygon
        DomvarPolyshape = 0x09,
        /// max count of vehicles
        DomvarMaxcount = 0x0a,
        /// count of TraCI vehicles
        DomvarEquippedcount = 0x0b,
        /// max count of TraCI vehicles
        DomvarEquippedcountmax = 0x0c,
        /// ID string of a domain object
        DomvarName = 0x0d,
        /// route, a car plans to drive
        DomvarRoute = 0x0e,
        /// distance from a certain object to a position
        DomvarAirdistance = 0x10,
        /// distance from a certain object to a position
        DomvarDrivingdistance = 0x11
    };

    /** General Value Retrieval Variable IDs
     *
     *  These variable IDs are common for several value retrieval commands.
     */
    enum GenValueRetrievalVariable
    {
        /// variable id list for value retrieval commands
        VarIdList = 0x00,
        /// variable CO2 emissions
        VarCo2Emissions = 0x60,
        /// variable CO emissions
        VarCoEmissions = 0x61,
        /// variable HC emissions
        VarHcEmissions = 0x62,
        /// variable PMx emissions
        VarPmxEmissions = 0x63,
        /// variable NOx emissions
        VarNoxEmissions = 0x64,
        /// variable fuel consumption
        VarFuelConsumption = 0x65,
        /// variable noise emission
        VarNoiseEmission = 0x66,
        /// variable last step vehicle number
        VarLastStepVehicleNumber = 0x10,
        /// variable last step mean speed
        VarLastStepMeanSpeed = 0x11,
        /// variable last step vehicle IDs
        VarLastStepVehicleIds = 0x12,
        /// variable last step vehicle occupancy
        VarLastStepOccupancy = 0x13,
        /// variable last step mean vehicle length
        VarLastStepMeanVehicleLength = 0x15
    };

    /// Lane Value Retrieval Variable IDs
    enum LaneValueRetrievalVariable
    {
        /// lane variable link number
        VarLaneLinkNumber = 0x30,
        /// lane variable edge id
        VarLaneEdge = 0x31,
        /// lane variable links
        VarLaneLinks = 0x33,
        /// lane variable allowed vehicle classes
        VarLaneAllowedVehicleClasses = 0x34,
        /// lane variable disallowed vehicle classes
        VarLaneDisallowedVehicleClasses = 0x35,
        /// lane variable length
        VarLaneLength = 0x44,
        /// lane variable vmax
        VarLaneVmax = 0x41,
        /// lane variable shape
        VarLaneShape = 0x4e
    };

    /// Vehicle Value Retrieval Variable IDs
    enum VehicleValueRetrievalVariable
    {
        /// vehicle variable speed
        VarVehicleSpeed = 0x40,
        /// speed without TraCI influence
        VarVehicleSpeedWithoutTraci = 0x41,
        /// vehicle variable position
        VarVehiclePosition = 0x42,
        /// vehicle variable angle
        VarVehicleAngle = 0x43,
        /// vehicle variable road ID
        VarVehicleRoadId = 0x50,
        /// vehicle variable lane id
        VarVehicleLaneId = 0x51,
        /// vehicle variable lane index
        VarVehicleLaneIndex = 0x52,
        /// vehicle variable type id
        VarVehicleTypeId = 0x4f,
        /// vehicle variable route ID
        VarVehicleRouteId = 0x53,
        /// vehicle variable edges
        VarVehicleEdges = 0x54,
        /// vehicle variable color
        VarVehicleColor = 0x45,
        /// vehicle variable lane position
        VarVehicleLanePosition = 0x56
    };

    /// Change Vehicle State Variable IDs
    enum ChangeVehicleStateVariable
    {
        /// set vehicle state max speed
        VarVehicleSetMaxSpeed = 0x11,
        /// set vehicle state stop
        VarVehicleSetStop = 0x12,
        /// set vehicle state change lane
        VarVehicleSetLane = 0x13,
        /// set vehicle state slow down
        VarVehicleSetSlowDown = 0x14,
        /// set vehicle state change target
        VarVehicleSetTarget = 0x31,
        /// set vehicle state change route by id
        VarVehicleSetRouteId = 0x54,
        /// set vehicle state change route
        VarVehicleSetRoute = 0x57,
        /// set vehicle state change edge travel time information
        VarVehicleSetEdgeTravelTime = 0x58,
        /// set vehicle state change edge effort information
        VarVehicleSetEdgeEffort = 0x59,
        /// set vehicle state reroute by travel time
        VarVehicleSetRerouteTravelTime = 0x90,
        /// set vehicle state reroute by effort
        VarVehicleSetRerouteEffort = 0x91
    };


    /// Traffic Light Phases
    enum TrafficLightPhase
    {
        /// red phase
        TlphaseRed = 0x01,
        /// yellow phase
        TlphaseYellow = 0x02,
        /// green phase
        TlphaseGreen = 0x03,
        /// TL is blinking
        TlphaseBlinking = 0x04,
        /// TL is off and not blinking
        TlphaseNosignal = 0x05
    };

};


}   // namespace traci

#endif  // #ifndef TRACICONSTANTS_H



