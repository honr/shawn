/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef SHAWN_APPS_TRACI_CLIENT_TRACI_NODE_MOVEMENT_CREATOR_H
#define SHAWN_APPS_TRACI_CLIENT_TRACI_NODE_MOVEMENT_CREATOR_H

#include "_apps_enable_cmake.h"

#include <string>
#include <list>
#include <map>

#include "sys/node_movements/playback/node_movement_creator.h"
#include "sys/node_movements/playback/node_movement_task.h"
#include "sys/misc/node_change_listener.h"
#include "sys/processors/processor_factory.h"

#include "apps/tcpip/socket.h"
#include "apps/tcpip/storage.h"

#include "mobility_interface_constants.h"
#include "tracitype.h"
#include "dynamic_length_datatypes.h"
#include "complex_datatypes.h"

#ifdef ENABLE_TRACICLIENT

namespace traci
{


class TraCIClient :
            public shawn::NodeMovementCreator,
            public shawn::NodeChangeListener
{
    friend class shawn::SimulationTaskNodeMovement;

public:
    ~TraCIClient();

    /// Get single instance.
    static TraCIClient* instance();

    /// Executes traffic simulation until TraCIClient::target_time_.
    shawn::MovementInfo* next_movement( void ) throw( std::runtime_error );

    void reset();

    /// Is called whenever a node gets added to the world
    virtual void node_added( shawn::Node& ) throw();
    /// Is called whenever a node gets removed from the world
    virtual void node_removed( shawn::Node& ) throw();
    /// Is called whenever the id of a node has changed.
    virtual void id_changed( int, int ) throw();
    /// Is called when the world gets deleted.
    virtual bool invalidate( void ) throw();

    struct TraCIID;
    typedef std::map<shawn::Node*, const TraCIClient::TraCIID> NodeToTraCIIDMap;
    typedef std::map<const TraCIClient::TraCIID, shawn::Node*> TraCIIDToNodeMap;

    TraCIClient::NodeToTraCIIDMap::const_iterator begin_traci_nodes() const throw();
    TraCIClient::NodeToTraCIIDMap::const_iterator end_traci_nodes() const throw();

    // Interface for listeners
    class TraCINodeChangeListener;
    struct TLSwitchInfo;
    typedef std::vector<TLSwitchInfo> TLSwitchInfoVec;

    /** \name Commands
     *
     *  These member functions send special TraCI-commands and analyze the answer.
     */
    //@{

    /// Send set maximum speed command (0x11).
    void command_set_maximum_speed( const shawn::Node& node, double max_speed );

    /// Send stop node command (0x12) in 2DPosition format.
    void command_stop( const shawn::Node& node, double x, double y, double radius,
                       double wait_time );

    /// Send change route command (0x30).
    void command_change_route( const shawn::Node& node, std::string road_id,
                               double travel_time );

    /// Send change lane command (0x30).
    void command_change_lane( const shawn::Node& node, char lane, float time );

    /// Send simulation step command (0x80) and extracts %position data of \p node from answer.
    bool command_roadmap_position( const shawn::Node& node, double targetTime,
                                   std::string& roadId_, float& relPosition_,
                                   std::string& laneID_ );

    /// Send scenario command (0x73) with \c ValueDataType integer (0x09).
    bool command_scenario_integer( bool write, int domain, int domainId,
                                   int variable, int& value );

    /// Send scenario command (0x73) with \c ValueDataType string (0x0C).
    bool command_scenario_string( bool write, int domain, int domainId,
                                  int variable, std::string& value );

    /// Send scenario command (0x73) with \c ValueDataType 3DPosition (0x03).
    bool command_scenario_pos3d( bool write, int domain, int domainId,
                                 int variable, float& x, float& y, float& z );

    /// Send scenario command (0x73) with \c ValueDataType Boundary Box (0x05).
    bool command_scenario_bounding_box( bool write, int domain, int domainId,
                                        int variable, float& x1, float& y1,
                                        float& x2, float& y2 );

    /// Send scenario command (0x73) with \c ValueDataType Road Map Position (0x04).
    float command_scenario_distance_to_roadmap_position( bool write, int domain,
            int domainId, int variable, std::string roadId, float roadPos, int lane );

    /// Send get traffic light status command (0x41).
    bool command_get_TL_status( int tlId, double from, double until,
                                TLSwitchInfoVec& switchInfos );

    /// Send value retrieval command
    void command_get_value( unsigned char command, const VariableId &variableId,
                            traci::TraciType & value, unsigned char response_command = 0 );

    /// Send state change command
    template < class ValueType >
    void command_set_value( unsigned char command, const VariableId & variableId,
                            const ValueType & value );

    /// Send scenario command to get a value of the same type as sent
    template < class ScenarioValueType >
    void command_scenario_get( traci::ScenarioRequest::Domain domain, int domainId,
                               unsigned char variable, ScenarioValueType &value );
    /// Send scenario command to get a value of a different type as sent
    template < class ScenarioRequestValueType, class ScenarioResponseValueType >
    void command_scenario_get( traci::ScenarioRequest::Domain domain, int domainId,
                               unsigned char variable,
                               const ScenarioRequestValueType & requestValue,
                               ScenarioResponseValueType & responseValue );
    /// Send scenario command to set a value
    template < class ScenarioValueType >
    void command_scenario_set( traci::ScenarioRequest::Domain domain, int domainId,
                               unsigned char variable, const ScenarioValueType &value );
    //@}

    /// Send arbitrary command_content and return its answer
    void send_command( unsigned char command, tcpip::Storage &command_content,
                       tcpip::Storage &answer ) throw( tcpip::SocketException );
    /// Send arbitrary TraciType command and return its answer
    void send_command( unsigned char commandId, const traci::TraciType &content,
                       tcpip::Storage &answer ) throw( tcpip::SocketException );
    /// Send arbitrary TraciType command without expected answer
    void send_command( unsigned char commandId, const traci::TraciType &content )
                       throw( tcpip::SocketException );

    /// Read length from storage
    unsigned int read_length( tcpip::Storage &storage ) const;

    /// Check if actual position in \p storage is as expected.
    void check_position( const tcpip::Storage &storage, unsigned int expected_position ) const;

public:

    const shawn::Node* find_node_by_traci_id( const TraCIID& traci_id );
    shawn::Node* find_node_by_traci_id_w( const TraCIID& traci_id );
    const TraCIID& find_traci_id_by_node( const shawn::Node& node ) throw( std::logic_error );
    shawn::Node* new_node( const TraCIID& traci_id, bool attach_processors_by_domain = true );
    void add_traffic_lights();
    void add_node_change_listener( TraCIClient::TraCINodeChangeListener& listener ) throw();

private:
    /// code common to command_scenario_get and command_scenario_set
    unsigned int command_scenario_getset_common( const TraciType & command, tcpip::Storage & in );

    template < class ScenarioValueType >
    void command_scenario_special_common( bool write, int domain, int domainId, int variable, ScenarioValueType & value );

    void checkCommandId( unsigned char expectedId, unsigned char actualId, const std::string &functionName = "" ) const;

    // Reads a Status command that is expected to start at s' actual position.
    // If the status is unequal to RTYPE_OK or the given commandId differs from
    // that in the Storage, Shawn will stop.
    void extract_command_status( tcpip::Storage& s, unsigned char commandId,
                                 std::string& description );

    void run( shawn::SimulationController& sc ) throw();

    // Connect and disconnect to the mobility server
    // The connect method is called by startSimStepHandler
    // The close method is called by the destructor
    bool connect();

    void close();

    void fetch_processor_factories();

    bool check_domain( int domain, const TraCIID& traci_id, bool abrt = false,
                       std::string err_msg = "" );

    std::string number_to_string( double n );

    void check_for_unused_vehicle_nodes();

    void add_node( shawn::Node* node, const TraCIID& traci_id );

    void remove_node_from_map( shawn::Node* node );

    // Constructors
    TraCIClient();
    TraCIClient( const TraCIClient& );

    // Internal variables
    shawn::SimulationController* sc_;
    tcpip::Socket* socket_;
    tcpip::Storage in_;
    std::string remotehost_;
    int remoteport_;
    double time_interval_;
    double target_time_;
    double feed_time_;
    shawn::MovementInfo *mi_;
    // Every domain has it's associated processor factories map<domain,processor factories>
    std::map<int, std::set<shawn::ProcessorFactoryHandle>* > domain_processor_factories_;
    // Current nodes of the step
    std::set<TraCIID> current_vehicle_nodes_;
    // Two-way "node id" mapping from TraCI to shawn
    NodeToTraCIIDMap node_to_traci_ids_;
    TraCIIDToNodeMap traci_ids_to_node_;

    typedef std::list<TraCIClient::TraCINodeChangeListener*> TraCINodeChangeListenerList;
    TraCINodeChangeListenerList node_change_listeners_;

    static const int DOMAIN_IDS[];
    static const std::string DOMAIN_NAMES[];
};


// This is a function template so we have to define it here in the header.
/// Send state change command
template < class ValueType >
inline void TraCIClient::command_set_value( unsigned char command, const VariableId & variableId,
                                            const ValueType & value )
{
    // we need to pass references to TraciType so we have to convert variableId to its TraciType
    const typename ChangeState< ValueType >::ChangeStateKeyValuePair keyValue( VariableIdType( variableId ), value );
    const typename ChangeState< ValueType >::ChangeStateTraciType keyValueType( keyValue );
    send_command( command, keyValueType );
}


// This is a function template so we have to define it here in the header.
/**
 *  \tparam ScenarioValueType   type of value to send and get in this scenario command call
 */
template < class ScenarioValueType >
inline void TraCIClient::command_scenario_get( traci::ScenarioRequest::Domain domain, int domainId,
                                               unsigned char variable, ScenarioValueType &value )
{
    command_scenario_get( domain, domainId, variable, value, value );
}


// This is a function template so we have to define it here in the header.
/**
 *  \tparam ScenarioRequestValueType    type of value to send in this scenario command call
 *  \tparam ScenarioResponseValueType   type of value to get in this scenario command call
 */
template < class ScenarioRequestValueType, class ScenarioResponseValueType >
inline void TraCIClient::command_scenario_get( traci::ScenarioRequest::Domain domain,
                           int domainId, unsigned char variable,
                           const ScenarioRequestValueType & requestValue,
                           ScenarioResponseValueType & responseValue )
{
    typename Scenario< ScenarioRequestValueType >::ScenarioTraciType command;
    command.key = ScenarioRequestType( ScenarioRequest(domain, domainId, variable) );
    command.value = requestValue;
    // withTypeId property of command.value will be automatically added by ScenarioTraciType

    tcpip::Storage in;
    const unsigned int endStoragePosition = command_scenario_getset_common( command, in );

    // read answer command
    const typename Scenario< ScenarioResponseValueType >::ScenarioTraciType answer( in );
    if( answer.key != command.key )
    {
        std::ostringstream msg;
        msg << std::hex << std::setfill('0')
            << "traci::TraCIClient::command_scenario_get: got wrong request value in response" << std::endl;
        msg << "   sent (flag, domain, domain ID, variable): ";
        command.key.print(msg);
        msg << std::endl << "   got  (flag, domain, domain ID, variable): ";
        answer.key.print(msg);
        msg << std::endl;
        throw std::runtime_error( msg.str() );
    }

    check_position( in, endStoragePosition );
    responseValue = answer.value;
}


// This is a function template so we have to define it here in the header.
/**
 *  \tparam ScenarioValueType   type of value to set in this scenario command call
 */
template < class ScenarioValueType >
inline void TraCIClient::command_scenario_set( traci::ScenarioRequest::Domain domain, int domainId,
                                               unsigned char variable, const ScenarioValueType &value )
{
    typedef Scenario< ScenarioValueType > MyScenario;
    typename MyScenario::ScenarioTraciType command;
    command.key = ScenarioRequestType( ScenarioRequest(domain, domainId, variable) );
    command.value = value;

    tcpip::Storage in;
    const unsigned int endStoragePosition = command_scenario_getset_common( command, in );

    check_position( in, endStoragePosition );
}


// This is a function template so we have to define it here in the header.
/**
 *  \tparam ScenarioValueType   type of value for the special scenario command function
 */
template < class ScenarioValueType >
inline void TraCIClient::command_scenario_special_common( bool write, int domain, int domainId, int variable, ScenarioValueType & value )
{
    try
    {
        if ( write )
        {
            command_scenario_set<ScenarioValueType>( static_cast<ScenarioRequest::Domain>(domain), domainId, variable, value );
        }
        else
        {
            command_scenario_get<ScenarioValueType>( static_cast<ScenarioRequest::Domain>(domain), domainId, variable, value );
        }
    }
    catch ( std::exception &e )
    {
        std::cerr << "Error in method TraCIClient::command_scenario_special_common: catched std::exception: " << e.what() << std::endl;
        abort();
    }
}


struct TraCIClient::TraCIID
{
    TraCIID();
    TraCIID( int d, int i );
    TraCIID( const TraCIID& );
    virtual ~TraCIID();
    int domain() const;
    int id() const;
    TraCIClient::TraCIID& operator= ( const TraCIClient::TraCIID& rv );
private:
    //friend class traci::TraCIClient;
    friend int operator< ( const TraCIClient::TraCIID&,
                           const TraCIClient::TraCIID& );
    friend int operator> ( const TraCIClient::TraCIID&,
                           const TraCIClient::TraCIID& );
    friend int operator== ( const TraCIClient::TraCIID&,
                            const TraCIClient::TraCIID& );

    int domain_;
    int id_;
};


class TraCIClient::TraCINodeChangeListener
{
public:
    virtual ~TraCINodeChangeListener() {}
    virtual void node_added( shawn::Node& node, const TraCIClient::TraCIID& traci_id ) throw() = 0;
    virtual void node_removed( shawn::Node& node, const TraCIClient::TraCIID& traci_id ) throw() = 0;
protected:
    TraCINodeChangeListener() {}
private:
    TraCINodeChangeListener( const TraCINodeChangeListener& );
};


struct TraCIClient::TLSwitchInfo
{
    double switchTime;
    std::string fromEdge;
    float fromEdgePos;
    std::string toEdge;
    int newStatus;
    double yellowTime;
};


}   // namespace traci

#endif  // #ifdef ENABLE_TRACICLIENT
#endif  // #ifndef SHAWN_APPS_TRACI_CLIENT_TRACI_NODE_MOVEMENT_CREATOR_H

