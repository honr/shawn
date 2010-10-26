/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/node_movements/linear_movement.h"
#include "sys/node_movements/jump_movement.h"
#include "sys/processors/processor_keeper.h"
#include "sys/misc/tokenize.h"

#include "traci_node_movement_creator.h"
#include <iomanip>
#include <sstream>
#include "atomic_datatypes.h"


#ifdef ENABLE_TRACICLIENT


using namespace std;
using namespace tcpip;
using namespace shawn;

namespace traci
{

// Add domains here.
// Make sure that:
// -the counts of elements in both arrays are the same and
// -the order of the domain_ids corresponds to the order of corresponding domain_names
const int TraCIClient::DOMAIN_IDS[] = {0x01, 0x02};
const std::string TraCIClient::DOMAIN_NAMES[] = {"vehicle", "traffic_light"};

TraCIClient::TraCIClient() :
        socket_( NULL ),
        remotehost_( "localhost" ),
        remoteport_( 8888 ),
        time_interval_( 1.0 ),
        target_time_( 0.0 ),
        feed_time_( 0.0 ),
        mi_( NULL )
{
    cout << "TraCIClient ctor" << endl;
}


//---------------------------------------------------------------------
TraCIClient::~TraCIClient( void )
{
    if ( socket_ ) close();
    reset();
    cout << "TraCIClient dtor" << endl;
}


//---------------------------------------------------------------------
TraCIClient* TraCIClient::instance()
{
    static TraCIClient* inst = new TraCIClient();
    return inst;
}


//---------------------------------------------------------------------
void TraCIClient::node_added( Node& )
throw()
{
    // Do nothing here
}


//---------------------------------------------------------------------
void TraCIClient::node_removed( Node& node )
throw()
{
    remove_node_from_map( &node );
}


//---------------------------------------------------------------------
/**
 *  This occurs, for
 *  example, if the method reorganize_nodes() of the world is called,
 *  because the internal NodeVector is optimized (compressed) there
 *  and ids may change.
 *
 *  \param int Old ID
 *  \param int New ID
 */
void TraCIClient::id_changed( int, int )
throw()
{
    //ABORT_NOT_IMPLEMENTED;
}


//---------------------------------------------------------------------
/**
 *  \return     \c true, if the world should delete the listener
 */
bool TraCIClient::invalidate( void )
throw()
{
    // Do nothing here
    return false;
}


//---------------------------------------------------------------------
void TraCIClient::run( SimulationController& sc )
throw()
{
    sc_ = &sc;
    sc_->world_w().add_node_change_listener( *this );
    // Fetch parameters
    remoteport_ = sc_->environment().optional_int_param( "remote_port", remoteport_ );
    remotehost_ = sc_->environment().optional_string_param( "remote_host", remotehost_ );
    feed_time_ =  sc_->environment().optional_double_param( "feed_time", feed_time_ );
    //target_time_ = sc_->environment().optional_double_param("target_time",target_time_);
    time_interval_ = sc_->environment().optional_double_param( "time_interval", time_interval_ );

    fetch_processor_factories();
    // Connect it!
    if ( !connect() ) abort();

    if ( feed_time_ - time_interval_ > 0.0 )
    {
        // Let TraCI server run feed_time_ seconds of simulation time
        Storage s;
        // Lenght of command
        s.writeByte( 11 );
        // CommandID
        s.writeChar( TraciConstants::CmdSimstep );
        // Target time
        s.writeDouble( feed_time_ - time_interval_ );
        // Result type (No positions needed)
        s.writeChar( TraciConstants::PositionNone );

        // Send command
        try
        {
            socket_->sendExact( s );
        }
        catch ( SocketException &e )
        {
            cerr << "Error in method TraCIClient::next_movement() while sending: "
                 << e.what() << endl;
            abort();
        }

        try
        {
            socket_->receiveExact( s );
        }
        catch ( SocketException &e )
        {
            cerr << "Error in method TraCIClient::next_movement while receiving: "
                 << e.what() << endl;
            abort();
        }

        // First result command serves as status information
        string description;
        extract_command_status( s, TraciConstants::CmdSimstep, description );

        bool tln = sc_->environment_w().optional_bool_param( "traffic_light_nodes", false );
        if ( tln ) add_traffic_lights();

        // No node position informations requested
    }
}


//---------------------------------------------------------------------
/**
 *  After executing the traffic simulation further calls will iterate
 *  over the moved vehicles to return their shawn::MovementInfo. After
 *  this iteration it returns a shawn::MovementInfo with
 *  shawn::MovementInfo::Urgency set to shawn::MovementInfo::Delayed.
 *
 *  \return shawn::MovementInfo for current node.
 */
// next_movement = command_simulation_step -> scheduled "magically"
MovementInfo* TraCIClient::next_movement()
throw( runtime_error )
{
    if ( !socket_ )
    {
        cerr << "Error in method TraCIClient::next_movement, socket_ == NULL!" << endl;
        abort();
    }


    // If in-storage is empty ask for new data from movement simulator (e.g. SUMO)
    if ( !in_.valid_pos() )
    {
        // If the right simulation time for sending the next question is not yet arrived ...
        if ( sc_->world().scheduler().current_time() < target_time_ )
        {
            //... wait until then
            mi_ = new MovementInfo();
            mi_->set_urgency( MovementInfo::Delayed );
            mi_->set_dispatch_time( target_time_ );
            mi_->set_node( NULL );
            mi_->set_node_movement( NULL );
            return mi_;
        }
        // If target_time is reached, ask for new movements
        target_time_ = sc_->world().scheduler().current_time() + time_interval_;
        check_for_unused_vehicle_nodes();
        current_vehicle_nodes_.clear();
        // Build command
        Storage out;
        // Lenght of command
        out.writeByte( 11 );
        // CommandID
        out.writeChar( TraciConstants::CmdSimstep );
        // Target time
        out.writeInt( static_cast<int>(1000.0 * target_time_) );
        // Result type (Position representation)
        out.writeChar( TraciConstants::Position2d );

        // Send command
        try
        {
            socket_->sendExact( out );
        }
        catch ( SocketException &e )
        {
            cerr << "Error in method TraCIClient::next_movement() while sending: "
                 << e.what() << endl;
            abort();
        }

        try
        {
            socket_->receiveExact( in_ );
        }
        catch ( SocketException &e )
        {
            cerr << "Error in method TraCIClient::next_movement() while receiving: "
                 << e.what() << endl;
            abort();
        }

        // First result command serves as status information
        string description;
        extract_command_status( in_, TraciConstants::CmdSimstep, description );

    }

    // Now follow the single node movements
    int node_id = -1;
    double dest_time = 0.0;
    double x = 0.0;
    double y = 0.0;
    double now = sc_->world().scheduler().current_time();
    double velocity = 0.0;
    Node* node = NULL;

    unsigned int command_start = 0;
    unsigned int command_length = 0;

    if ( ! in_.valid_pos() )
    {
        // No equipped vehicle is reported
        mi_ = new MovementInfo();
        mi_->set_urgency( MovementInfo::Delayed );
        mi_->set_dispatch_time( target_time_ );
        mi_->set_node( NULL );
        mi_->set_node_movement( NULL );
        return mi_;
    }

    // Extract next movement information
    command_start = in_.position();
    command_length = in_.readByte();

    if ( unsigned int command_id = in_.readChar() != TraciConstants::CmdMovenode )
    {
        // Move node command expected
        cerr << "Error in method TraCIClient::next_movement, move node command "
             "expected. Got command_id " << command_id << endl;
        abort();
    }

    node_id = in_.readInt();
    dest_time = static_cast<double>(in_.readInt()) / 1000.0;

    // PositionType
    if ( in_.readByte() != TraciConstants::Position2d )
    {
        cerr << "Error in method TraCIClient::next_movement, "
             "I can handle only 2D-coordinates!" << endl;
        abort();
    }
    x = in_.readFloat();
    y = in_.readFloat();

    bool is_new_node = false;
    node = find_node_by_traci_id_w( TraCIID( 0x01, node_id ) );
    // If node doesn't exist create new one
    if ( !node )
    {
        node = new_node( TraCIID( 0x01, node_id ) );
        is_new_node = true;
    }
    current_vehicle_nodes_.insert( TraCIID( 0x01, node_id ) );
    JumpMovement* jm = NULL;
    LinearMovement* lm = NULL;
    mi_ = new MovementInfo();
    if ( is_new_node )
    {
        // "Fresh" node
        jm = new JumpMovement();
        jm->set_dimension( JumpMovement::X, x );
        jm->set_dimension( JumpMovement::Y, y );
        jm->set_dimension( JumpMovement::Z, 0.0 );
    }
    else
    {
        // Old node
        // Set node exactly to old destination
        const LinearMovement* old_lm = dynamic_cast<const LinearMovement*>( &node->movement() );
        if ( old_lm ) node->set_real_position( old_lm->destination() );
        lm = new LinearMovement();
        if ( dest_time > now )
        {
            velocity = sqrt(( node->real_position().x() - x ) * ( node->real_position().x() - x ) +
                            ( node->real_position().y() - y ) * ( node->real_position().y() - y ) ) /
                       ( dest_time - now );
            lm->set_parameters( velocity, Vec( x, y, 0.0 ), sc_->world_w() );
        }
    }
    mi_->set_urgency( shawn::MovementInfo::Immediately );
    mi_->set_dispatch_time( now );
    mi_->set_node( node );
    assert( bool ( jm ) ^ bool ( lm ) );
    if ( jm ) mi_->set_node_movement( jm );
    if ( lm ) mi_->set_node_movement( lm );


    // Right length???
    if ( command_start + command_length != in_.position() )
    {
        // Last read command has wrong length
        cerr << "Error in method TraCIClient::next_movement, command at position "
             << command_start << " was read with wrong length." << endl;
        abort();
    }

    return mi_;
}


//---------------------------------------------------------------------
void TraCIClient::add_traffic_lights()
{
    int count = 0;
    Node* node = NULL;
    if ( command_scenario_integer( false, 0x02, -1, 0x01, count ) )
    {
        for ( int i = 0; i < count; ++i )
        {
            node = new_node( TraCIID( 0x02, i ) );
            float x = 0.0, y = 0.0, z = 0.0;
            if ( command_scenario_pos3d( false, 0x02, i, 0x02, x, y, z ) )
            {
                node->set_real_position( Vec( x, y, z ) );
            }
            else
            {
                cerr << "Positioning of traffic light: " << i << " failed!" << endl;
                abort();
            }
        }
    }
    else
    {
        cerr << "Couldn't aquire count of traffic lights!" << endl;
        abort();
    }
}


//---------------------------------------------------------------------
unsigned int TraCIClient::command_scenario_getset_common( const TraciType & command, Storage & in )
{
    send_command( TraciConstants::CmdScenario, command, in );

    if ( !in.valid_pos() )
        throw std::runtime_error( "traci::TraCIClient::command_scenario_common: missing repsonse command" );

    const unsigned int commandStart = in.position();
    const unsigned int commandLength = read_length( in );

    // check answer command ID
    const UbyteType commandId(in);
    checkCommandId( TraciConstants::CmdScenario, commandId, "command_scenario_getset_common" );

    return commandStart + commandLength;
}


//---------------------------------------------------------------------
void TraCIClient::checkCommandId( unsigned char expectedId, unsigned char actualId, const std::string &functionName ) const
{
    if ( expectedId != actualId )
    {
        std::ostringstream msg;
        msg << std::hex << std::setfill('0')
            << "traci::TraCIClient";
        if ( !functionName.empty() )
            msg << "::" << functionName;
        msg << ": got unexpected response command, "
            << "expected 0x" << std::setw(2) << static_cast<unsigned int>( expectedId )
            << "(Scenario), got 0x" << std::setw(2) << static_cast<unsigned int>( actualId );
        throw std::runtime_error( msg.str() );
    }
}


//---------------------------------------------------------------------
void TraCIClient::extract_command_status( Storage& s, unsigned char command_id, string& description )
{
    unsigned int command_start = s.position();
    unsigned int command_length = read_length( s );

    // CommandID needs to fit
    if ( unsigned char rcvd_command_id = s.readChar() != command_id )
    {
        cerr << sc_->world().scheduler().current_time()
             << ": Error in method TraCIClient::extract_command_status, Server "
             "answered to command: " << rcvd_command_id << ". Expected command: " << command_id << endl;
        abort();
    }

    const StatusResponseType response( s );

    if ( !response.isSuccess() )
    {
        cerr << sc_->world().scheduler().current_time()
             << ": Error in method TraCIClient::extract_command_status, Server returned error "
             << "[0x" << hex << setfill('0') << setw(2) << static_cast<unsigned int>(response.result) << "]: "
             << response.description << dec << setfill(' ') << endl;
        abort();
    }

    // Right length?
    if ( command_start + command_length != s.position() )
    {
        // Last read command has wrong length
        cerr << sc_->world().scheduler().current_time()
             << ": Error in method TraCIClient::extract_command_status, command at position "
             << command_start << " was read with wrong length." << endl;
        abort();
    }

}


//---------------------------------------------------------------------
void TraCIClient::reset()
{
    for ( map<int, set<ProcessorFactoryHandle>* >::iterator it = domain_processor_factories_.begin(); it != domain_processor_factories_.end();++it )
    {
        it->second->clear();
        delete it->second;
    }
    node_change_listeners_.clear();
    current_vehicle_nodes_.clear();
    node_to_traci_ids_.clear();
    traci_ids_to_node_.clear();
}


//---------------------------------------------------------------------
bool TraCIClient::connect()
{
    if ( socket_ ) close();

    socket_ = new Socket( remotehost_, remoteport_ );
    socket_->set_blocking( true );
    try
    {
        socket_->connect();
    }
    catch ( SocketException &e )
    {
        cerr << "Error in method TraCIClient::connect(): "
             << e.what() << endl;
        delete socket_;
        socket_ = NULL;
        return false;
    }
    return true;
}


//---------------------------------------------------------------------
void TraCIClient::close()
{
    if ( !socket_ )
    {
        cerr << "Error in method TraCIClient::close(): socket_ == NULL" << endl;
        abort();
    }
    Storage out;
    out.writeByte( 2 );
    out.writeChar( TraciConstants::CmdClose );
    try
    {
        socket_->sendExact( out );
    }
    catch ( SocketException &e )
    {
        cerr << "Error in method TraCIClient::close(): "
             << e.what() << endl;
        delete socket_;
        socket_ = NULL;
        abort();
    }

    delete socket_;
    socket_ = NULL;
}


//---------------------------------------------------------------------
void TraCIClient::fetch_processor_factories()
{
    for ( unsigned int i = 0; i < sizeof( DOMAIN_IDS ) / sizeof( int ); ++i )
    {
        ProcessorFactoryHandle pfh;
        set<ProcessorFactoryHandle>* processor_factories = new set<ProcessorFactoryHandle>();
        StrTok tok( sc_->environment().optional_string_param( DOMAIN_NAMES[i] + "_processors", "" ), ", " );
        for ( StrTok::iterator it = tok.begin(); it != tok.end(); ++it )
        {
            pfh = sc_->processor_keeper_w().find_w( *it );
            assert( pfh != NULL );
            processor_factories->insert( pfh );
        }
        domain_processor_factories_.
        insert( pair<int, set<ProcessorFactoryHandle>*> ( DOMAIN_IDS[i], processor_factories ) );
    }
}


//---------------------------------------------------------------------
void TraCIClient::add_node_change_listener( TraCINodeChangeListener& listener )
throw()
{
    node_change_listeners_.push_back( &listener );
}


//---------------------------------------------------------------------
Node* TraCIClient::new_node( const TraCIID& traci_id, bool attach_processors_by_domain )
{
    // If node exists return NULL
    if ( find_node_by_traci_id_w( traci_id ) )
        return NULL;

    Node* node = new Node();
    add_node( node, traci_id );

    if ( attach_processors_by_domain )
    {
        map<int, set<ProcessorFactoryHandle>* >::const_iterator it1 =
            domain_processor_factories_.find( traci_id.domain() );

        if ( it1 != domain_processor_factories_.end() )
        {
            set<ProcessorFactoryHandle>* processor_factories = it1->second;
            for ( set<ProcessorFactoryHandle>::const_iterator it2 = processor_factories->begin();
                    it2 != processor_factories->end(); ++it2 )
            {
                ProcessorHandle ph = it2->get()->create();
                node->add_processor( ph );
            }
        }
    }
    return node;
}


//---------------------------------------------------------------------
void TraCIClient::check_for_unused_vehicle_nodes()
{
    for ( World::node_iterator ni = sc_->world_w().begin_nodes_w();
            ni != sc_->world_w().end_nodes_w(); ++ni )
    {

        TraCIID ti;
        try
        {
            ti = find_traci_id_by_node( *ni );
        }
        catch ( logic_error &e )
        {
            // Not found
            continue;
        }
        if ( ti.domain() == 0x01 && current_vehicle_nodes_.find( ti ) == current_vehicle_nodes_.end() )
        {
            sc_->world_w().remove_node( *ni );
        }
    }
}


//---Movement-Simulator-Commands---
//---------------------------------------------------------------------
/**
 *  see http://sourceforge.net/apps/mediawiki/sumo/index.php?title=TraCI/Mobility-related_commands
 *
 *  Calls \c abort() in case of problems after sending a message to \c stderr.
 *
 *  \param[in]  node        node to set max speed for
 *  \param[in]  max_speed   max speed to set (negative -> original value)
 */
void TraCIClient::command_set_maximum_speed( const Node& node, double max_speed )
{
    if ( !socket_ )
    {
        cerr << "Error in method TraCIClient::command_set_maximum_speed, "
             "socket_ == NULL" << endl;
        abort();
    }

    // Build command
    Storage out;
    // Length of command
    out.writeUnsignedByte( 10 );
    // CommandID
    out.writeChar( TraciConstants::CmdSetmaxspeed );
    // Node ID
    TraCIID ti;
    try
    {
        ti = find_traci_id_by_node( node );
    }
    catch ( logic_error &e )
    {
        cerr << "Error in method TraCIClient::command_set_maximum_speed: " << e.what() << endl;
        abort();
    }
    check_domain( 0x01, ti, true, "Error in method TraCIClient::command_set_maximum_speed: "
                  "Command not applicable for elements in domain " + number_to_string( ti.domain() ) + "!" );
    out.writeInt( ti.id() );
    // Max speed
    out.writeFloat( static_cast<float>( max_speed ) );

    // Send command

    try
    {
        socket_->sendExact( out );
    }
    catch ( SocketException &e )
    {
        cerr << "Error in method TraCIClient::command_set_maximum_speed "
             "while sending: " << e.what() << endl;
        abort();
    }

    Storage in;
    try
    {
        socket_->receiveExact( in );
    }
    catch ( SocketException &e )
    {
        cerr << "Error in method TraCIClient::command_set_maximum_speed "
             "while receiving: " << e.what() << endl;
        abort();
    }

    // First result command serves as status informatin
    string description;
    extract_command_status( in, TraciConstants::CmdSetmaxspeed, description );
}


//---------------------------------------------------------------------
/**
 *  see http://sourceforge.net/apps/mediawiki/sumo/index.php?title=TraCI/Mobility-related_commands
 *
 *  Calls \c abort() in case of problems after sending a message to \c stderr.
 *
 *  \param[in]  node        node to set stop position for
 *  \param[in]  x           \c x of 2DPosition
 *  \param[in]  y           \c y of 2DPosition
 *  \param[in]  radius      \c Radius of stop node command
 *  \param[in]  wait_time   \c WaitTime of stop node command
 */
void TraCIClient::command_stop( const Node& node, double x, double y, double radius, double wait_time )
{
    if ( !socket_ )
    {
        cerr << "Error in method TraCIClient::command_stop, "
             "socket_ == NULL" << endl;
        abort();
    }

    // Build command
    Storage out;
    // Length of command
    out.writeUnsignedByte( 27 );
    // Command ID
    out.writeChar( TraciConstants::CmdStop );
    // Node ID
    TraCIID ti;
    try
    {
        //out.writeInt(find_traci_id_by_node(node).id());
        ti = find_traci_id_by_node( node );
    }
    catch ( logic_error &e )
    {
        cerr << "Error in method TraCIClient::command_stop: " << e.what() << endl;
        abort();
    }
    check_domain( 0x01, ti, true, "Error in method TraCIClient::command_stop: "
                  "Command not applicable for elements in domain " + number_to_string( ti.domain() ) + "!" );
    out.writeInt( ti.id() );
    // Position type
    out.writeUnsignedByte( TraciConstants::Position2d );
    // Position
    out.writeFloat( static_cast<float>( node.real_position().x() ) );
    out.writeFloat( static_cast<float>( node.real_position().y() ) );
    // Radius
    out.writeFloat( static_cast<float>( radius ) );
    // Wait time
    out.writeDouble( wait_time );

    // Send command
    try
    {
        socket_->sendExact( out );
    }
    catch ( SocketException &e )
    {
        cerr << "Error in method TraCIClient::command_stop while sending: "
             << e.what() << endl;
        abort();
    }

    Storage in;
    try
    {
        socket_->receiveExact( in );
    }
    catch ( SocketException &e )
    {
        cerr << "error in method TraCIClient::command_stop while recieving: "
             << e.what() << endl;
        abort();
    }

    // First result command serves as status information
    string description;
    extract_command_status( in, TraciConstants::CmdStop, description );
}


//---------------------------------------------------------------------
/**
 *  see http://sourceforge.net/apps/mediawiki/sumo/index.php?title=TraCI/Mobility-related_commands
 *
 *  Calls \c abort() in case of problems after sending a message to \c stderr.
 *
 *  \param[in]  node            node (vehicle) to set new travel time for
 *  \param[in]  road_id         edge to set travel time for
 *  \param[in]  travel_time     travel time to set for edge (negative -> reset)
 */
void TraCIClient::command_change_route( const Node& node, string road_id, double travel_time )
{
    if ( !socket_ )
    {
        cerr << "Error in method TraCIClient::command_change_route, "
             "socket == NULL" << endl;
        abort();
    }

    // Build command
    Storage out;
    // Length of command
    out.writeUnsignedByte( 18 + road_id.length() );
    // Command ID
    out.writeChar( TraciConstants::CmdChangeroute );
    // Node ID
    TraCIID ti;
    try
    {
        ti = find_traci_id_by_node( node );
    }
    catch ( logic_error &e )
    {
        cerr << "Error in method TraCIClient::command_change_route: " << e.what() << endl;
        abort();
    }
    check_domain( 0x01, ti, true, "Error in method TraCIClient::command_change_route: "
                  "Command not applicable for elements in domain " + number_to_string( ti.domain() ) + "!" );
    out.writeInt( ti.id() );
    // Road ID
    out.writeString( road_id );
    // Travel time
    out.writeDouble( travel_time );

    // Send command
    try
    {
        socket_->sendExact( out );
    }
    catch ( SocketException &e )
    {
        cerr << "Error in method TraCIClient::coammand_change_route while sending: "
             << e.what() << endl;
        abort();
    }
    Storage in;
    try
    {
        socket_->receiveExact( in );
    }
    catch ( SocketException &e )
    {
        cerr << "Error in method TraCIClient::command_change_route while receiving: "
             << e.what() << endl;
        abort();
    }

    string description;
    extract_command_status( in, TraciConstants::CmdChangeroute, description );
}


//---------------------------------------------------------------------
/**
 *  see http://sourceforge.net/apps/mediawiki/sumo/index.php?title=TraCI/Mobility-related_commands
 *
 *  Calls \c abort() in case of problems after sending a message to \c stderr.
 *
 *  \param[in]  node    node (vehicle) to set lane constraint for
 *  \param[in]  lane    lane to get vehicle to change to (0 for rightmost)
 *  \param[in]  time    duration of lane constraint
 */
void TraCIClient::command_change_lane( const Node& node, char lane, float time )
{
    if ( !socket_ )
    {
        cerr << "Error in method TraCIClient::command_change_lane, "
             "socket == NULL" << endl;
        abort();
    }


    // Build command
    Storage out;
    // Length of command
    out.writeUnsignedByte( 9 );
    // Command ID
    out.writeChar( TraciConstants::CmdChangelane );
    // Node ID
    TraCIID ti;
    try
    {
        ti = find_traci_id_by_node( node );
    }
    catch ( logic_error &e )
    {
        cerr << "Error in method TraCIClient::command_change_lane: " << e.what() << endl;
        abort();
    }
    check_domain( 0x01, ti, true, "Error in method TraCIClient::command_change_lane: "
                  "Command not applicable for elements in domain " + number_to_string( ti.domain() ) + "!" );
    out.writeInt( ti.id() );
    // Lane
    out.writeByte( lane );
    // Time
    out.writeFloat( time );

    // Send command
    try
    {
        socket_->sendExact( out );
    }
    catch ( SocketException &e )
    {
        cerr << "Error in method TraCIClient::coammand_change_lane while sending: "
             << e.what() << endl;
        abort();
    }
    Storage in;
    try
    {
        socket_->receiveExact( in );
    }
    catch ( SocketException &e )
    {
        cerr << "Error in method TraCIClient::command_change_lane while receiving: "
             << e.what() << endl;
        abort();
    }

    string description;
    extract_command_status( in, TraciConstants::CmdChangelane, description );

}


//---------------------------------------------------------------------
/**
 *  Calls \c abort() in case of problems after sending a message to \c stderr.
 *
 *  Out-values will only be set if \p node was found.
 *
 *  \param[in]      node            node which's %position to get
 *  \param[in]      targetTime      target time for simulation step TraCI-command
 *  \param[out]     roadId_         edge of SUMO network containing \p node
 *  \param[out]     relPosition_    %position of \p node on edge \p roadId_
 *  \param[out]     laneId_         lane containing \p node
 *
 *  \return     always \c true
 */
bool TraCIClient::command_roadmap_position( const Node& node, double targetTime,
        string& roadId_, float& relPosition_, string& laneID_ )
{
    // Preconditions
    if ( socket_ == NULL )
    {
        cerr << "Error in method MobilityInterfaceClient::command_roadmap_position, socket_ == NULL" << endl;
        abort();
    }

    target_time_ = targetTime;

    //cout << "Target time to retrieve: " << target_time_ << endl;
    // build command
    Storage out;
    // Length of command
    out.writeByte( 11 );
    // CommandID
    out.writeChar( TraciConstants::CmdSimstep );
    // TargetTime
    out.writeInt( static_cast<int>(1000.0 * targetTime) );
    // ResultType (Position representation)
    out.writeChar( TraciConstants::PositionRoadmap );

    // Send command
    try
    {
        socket_->sendExact( out );
    }
    catch ( SocketException &e )
    {
        cerr << "Error in method MobilityInterfaceClient::command_roadmap_position while sending: " << e.what() << endl;
        abort();
    }

    Storage in;
    try
    {
        socket_->receiveExact( in );
    }
    catch ( SocketException &e )
    {
        cerr << "Error in method MobilityInterfaceClient::command_roadmap_position while receiving: " << e.what() << endl;
        abort();
    }

    // First result command serves as status information
    string description;
    extract_command_status( in, TraciConstants::CmdSimstep, description );

    int id = 0;
    double destTime = 0.0;
    string roadId = "";
    float relPosition = 0.0;
    int laneId = 0;

    unsigned int commandStart = 0;
    unsigned int commandLength = 0;

    while ( in.valid_pos() )
    {
        // Save start and length of next command
        commandStart = in.position();
        commandLength = in.readByte();

        if ( unsigned char commandId = in.readChar() != TraciConstants::CmdMovenode )
        {
            // move node command expected
            cerr << "Error in method TraCIClient::command_roadmap_position, "
                 << "move node command expected. Got commandId 0x"
                 << hex << setfill( '0' ) << setw( 2 ) << static_cast<int>( commandId )
                 << dec << setfill( ' ' ) << endl;
            abort();
        }

        // Read node address, end time and destination position (road)
        id = in.readInt();
        destTime = static_cast<double>(in.readInt()) / 1000.0;

        // PositionType
        if ( in.readByte() == TraciConstants::PositionRoadmap )
        {
            roadId = in.readString();
            relPosition = in.readFloat();
            laneId = in.readByte();

            // Update the node's road map position
            TraCIID ti;
            try
            {
                ti = find_traci_id_by_node( node );
            }
            catch ( logic_error &e )
            {
                cerr << "Error in method TraCIClient::command_roadmap_position: " << e.what() << endl;
                abort();
            }
            //check_domain(0x01,ti,true,"Error in method TraCIClient::command_roadmap_position: "
            //    "Command not applicable for elements in domain " + number_to_string(ti.domain()) + "!");
            if ( ti.id() == id )
            {
                //string laneIdString = "";
                roadId_ = roadId;
                relPosition_ = relPosition;
                ostringstream laneIdStream;
                laneIdStream << roadId_ << '_' << laneId;
                laneID_ = laneIdStream.str();
            }

        }
    }
    return true;
}


//---------------------------------------------------------------------
/**
 *  see http://sourceforge.net/apps/mediawiki/sumo/index.php?title=TraCI/Environment-related_commands
 *
 *  Depending on \p write the \p value will be set or not. The answer from SUMO
 *  will be returned in \p value.
 *
 *  Calls \c abort() in case of problems after sending a message to \c stderr.
 *
 *  \param[in]      write       \c true: \p value will be set
 *  \param[in]      domain      domain of scenario command
 *  \param[in]      domainId    ID of object within domain
 *  \param[in]      variable    ID of variable within domain
 *  \param[in,out]  value       reference to answer value and to value to set if applicable
 *
 *  \return     always \c true
 */
bool TraCIClient::command_scenario_integer( bool write, int domain, int domainId,
        int variable, int& value )
{
    IntegerTypeWithTypeId traciValue(value);
    command_scenario_special_common( write, domain, domainId, variable, traciValue );
    value = traciValue;
    return true;
}


//---------------------------------------------------------------------
/**
 *  see http://sourceforge.net/apps/mediawiki/sumo/index.php?title=TraCI/Environment-related_commands
 *
 *  Depending on \p write the \p value will be set or not. The answer from SUMO
 *  will be returned in \p value.
 *
 *  Calls \c abort() in case of problems after sending a message to \c stderr.
 *
 *  \param[in]      write       \c true: \p value will be set
 *  \param[in]      domain      domain of scenario command
 *  \param[in]      domainId    ID of object within domain
 *  \param[in]      variable    ID of variable within domain
 *  \param[in,out]  value       reference to answer value and to value to set if applicable
 *
 *  \return     always \c true
 */
bool TraCIClient::command_scenario_string( bool write, int domain, int domainId,
        int variable, string& value )
{
    StringTypeWithTypeId traciValue(value);
    command_scenario_special_common( write, domain, domainId, variable, traciValue );
    value = traciValue;
    return true;
}


//---------------------------------------------------------------------
/**
 *  see http://sourceforge.net/apps/mediawiki/sumo/index.php?title=TraCI/Environment-related_commands
 *
 *  Depending on \p write (\p x, \p y, \p z) will be set or not. The answer from SUMO
 *  will be returned in (\p x, \p y, \p z).
 *
 *  Calls \c abort() in case of problems after sending a message to \c stderr.
 *
 *  \param[in]      write       \c true: \p value will be set
 *  \param[in]      domain      domain of scenario command
 *  \param[in]      domainId    ID of object within domain
 *  \param[in]      variable    ID of variable within domain
 *  \param[in,out]  x           reference to answer x-value and to x-value to set if applicable
 *  \param[in,out]  y           reference to answer y-value and to y-value to set if applicable
 *  \param[in,out]  z           reference to answer z-value and to z-value to set if applicable
 *
 *  \return     always \c true
 */
bool TraCIClient::command_scenario_pos3d( bool write, int domain, int domainId,
        int variable, float& x, float& y, float& z )
{
    Position3DTypeWithTypeId traciValue;
    traciValue.x = x;
    traciValue.y = y;
    traciValue.z = z;
    command_scenario_special_common( write, domain, domainId, variable, traciValue );
    x = traciValue.x;
    y = traciValue.y;
    z = traciValue.z;

    return true;
}


//---------------------------------------------------------------------
/**
 *  see http://sourceforge.net/apps/mediawiki/sumo/index.php?title=TraCI/Environment-related_commands
 *
 *  Depending on \p write the bounding box will be set or not. The answer from SUMO
 *  will be returned in (\p x1, \p y1, \p x2, \p y2).
 *
 *  Calls \c abort() in case of problems after sending a message to \c stderr.
 *
 *  \param[in]      write       \c true: \p value will be set
 *  \param[in]      domain      domain of scenario command
 *  \param[in]      domainId    ID of object within domain
 *  \param[in]      variable    ID of variable within domain
 *  \param[in,out]  x1          \c LowerLeftX
 *  \param[in,out]  y1          \c LowerLeftY
 *  \param[in,out]  x2          \c UpperRightX
 *  \param[in,out]  y2          \c UpperRightY
 *
 *  \return     always \c true
 */
bool TraCIClient::command_scenario_bounding_box( bool write, int domain, int domainId,
        int variable, float& x1, float& y1, float& x2, float& y2 )
{
    try
    {
        if ( write )
            throw std::invalid_argument( "setting of a boundary box with scenario command not possible" );

        // SUMO (revision 8508) doesn't accept boundary box as input type for scenario command so provide a dummy
        UbyteTypeWithTypeId inputDummy;
        BoundaryBoxTypeWithTypeId result;
        command_scenario_get( static_cast<ScenarioRequest::Domain>(domain), domainId, variable, inputDummy, result );
        x1 = result.lowerLeftX;
        y1 = result.lowerLeftY;
        x2 = result.upperRightX;
        y2 = result.upperRightY;
    }
    catch ( std::exception &e )
    {
        std::cerr << "Error in method TraCIClient::command_scenario_bounding_box: catched std::exception: " << e.what() << std::endl;
        abort();
    }

    return true;
}


//---------------------------------------------------------------------
/**
 *  see http://sourceforge.net/apps/mediawiki/sumo/index.php?title=TraCI/Environment-related_commands
 *
 *  Calls \c abort() in case of problems after sending a message to \c stderr.
 *
 *  \param[in]      write       flag will be send and checked in the answer
 *  \param[in]      domain      domain of scenario command (should be \c vehicle (0x01))
 *  \param[in]      domainId    ID of object within domain
 *  \param[in]      variable    ID of variable within domain
 *  \param[in]      roadId      SUMO's edge ID
 *  \param[in]      roadPos     position on lane
 *  \param[in]      lane        lane number
 *
 *  \return     distance
 */
float TraCIClient::command_scenario_distance_to_roadmap_position( bool write,
        int domain, int domainId, int variable, string roadId, float roadPos,
        int lane )
{
    try
    {
        RoadMapPositionTypeWithTypeId traciType;
        traciType.road = roadId;
        traciType.pos = roadPos;
        traciType.lane = lane;
        FloatTypeWithTypeId result;

        if ( write )
        {
            command_scenario_set( static_cast<ScenarioRequest::Domain>(domain), domainId, variable, traciType );
        }
        else
        {
            FloatTypeWithTypeId distance;
            command_scenario_get( static_cast<ScenarioRequest::Domain>(domain), domainId, variable, traciType, result );
        }
        return result;
    }
    catch ( std::exception &e )
    {
        std::cerr << "Error in method TraCIClient::command_scenario_distance_to_roadmap_position: catched std::exception: " << e.what() << std::endl;
        abort();
    }
}


//---------------------------------------------------------------------
/**
 *  see http://sourceforge.net/apps/mediawiki/sumo/index.php?title=TraCI/Traffic_Lights
 *
 *  Calls \c abort() in case of problems after sending a message to \c stderr.
 *
 *  \param[in]  tlId            traffic light ID
 *  \param[in]  from            start of queried time interval
 *  \param[in]  until           end of queried time interval
 *  \param[out] switchInfos     reference to data structure to store answer
 *
 *  \return     always \c true
 */
bool TraCIClient::command_get_TL_status( int tlId, double from, double until,
        TLSwitchInfoVec& switchInfos )
{
    // Preconditions
    if ( socket_ == NULL )
    {
        cerr << "Error in method TraCIClient::commandgetTLStatus, socket_ == NULL" << endl;
        abort();
    }

    // build command
    Storage out;
    // Length of command
    out.writeByte( 1 + 1 + 4 + 8 + 8 );
    // CommandID
    out.writeChar( TraciConstants::CmdGettlstatus );
    // traffic light id
    out.writeInt( tlId );
    // timefrom
    out.writeDouble( from + feed_time_ );
    // timeto
    out.writeDouble( until + feed_time_ );

    // Send command
    try
    {
        socket_->sendExact( out );
    }
    catch ( SocketException &e )
    {
        cerr << "Error in method TraCIClient::commandgetTLStatus while sending: "
             << e.what() << endl;
        abort();
    }

    Storage in;
    try
    {
        socket_->receiveExact( in );
    }
    catch ( SocketException &e )
    {
        cerr << "Error in method TraCIClient::commandgetTLStatus while receiving: "
             << e.what() << endl;
        abort();
    }
    // First result command serves as status information
    string description;
    extract_command_status( in, TraciConstants::CmdGettlstatus, description );

    unsigned int commandStart = 0;
    unsigned int commandLength = 0;

    while ( in.valid_pos() )
    {
        // Save start and length of next command
        commandStart = in.position();
        commandLength = in.readByte();
        TLSwitchInfo tlswitch;

        if ( unsigned char commandId = in.readChar() != TraciConstants::CmdTlswitch )
        {
            // move node command expected
            cerr << "Error in method TraCIClient::commandgetTLStatus, TLSWITCH command expected. Got commandId "
                 << showbase << hex << commandId << endl;
            abort();
        }

        // Read switchTime, fromEdge, toEdge, newStatus (TLPHASE_GREEN or TLPHASE_RED) and yellowTime
        tlswitch.switchTime = in.readDouble() - feed_time_;
        tlswitch.fromEdge = in.readString();
        tlswitch.fromEdgePos = in.readFloat();
        tlswitch.toEdge = in.readString();
        tlswitch.newStatus = in.readUnsignedByte();
        if ( tlswitch.newStatus != TraciConstants::TlphaseRed && tlswitch.newStatus != TraciConstants::TlphaseGreen )
        {
            cerr << "Error in method TraCIClient::commandgetTLStatus while parsing answer command, new phase <"
                 << tlswitch.newStatus << "> unknown." << endl;
            abort();
        }
        tlswitch.yellowTime = in.readDouble();

        // Correct length???
        if ( commandStart + commandLength != in.position() )
        {
            // Last read command has wrong length
            cerr << "Error in method TraCIClient::commandgetTLStatus, command at position "
                 << commandStart << " was read with wrong length - expected " << commandLength << " but was "
                 << in.position() - commandStart << endl;
            abort();
        }
        switchInfos.push_back( tlswitch );
    }
    return true;
}


//---------------------------------------------------------------------
/**
 *  Compare actual position in \p storage with \p expected_position and throw
 *  a std::runtime_error with an appropriate message if not.
 *
 *  \param[in]  storage             storage to check_domain
 *  \param[in]  expected_position   expected position
 */
void TraCIClient::check_position( const tcpip::Storage &storage, unsigned int expected_position ) const
{
    if ( expected_position != storage.position() )
    {
        ostringstream msg;
        msg << "traci::TraCIClient::check_position: unexpected position, "
            << "expected = " << expected_position
            << ", actual = " << storage.position();
        throw runtime_error( msg.str() );
    }
}


//---------------------------------------------------------------------
/**
 *  Throws an exceptions in case of problems.
 *
 *  \param[in]  command             command ID to send
 *  \param[in]  variableId          variable and element ID to request
 *  \param[out] response            tcpip::Storage for reponse, will be positioned
 *                                  to start of returned value (behind ID)
 *  \param[in]  response_command    (optional) command ID of response (if 0 it
 *                                  will be computed from \p command)
 *
 *  \return     expected position of \p response after reading the whole answer command
 */
void TraCIClient::command_get_value( unsigned char command, const VariableId &variableId,
                            traci::TraciType &value, unsigned char response_command )
{
    if ( 0 == response_command )
        response_command = command | 0x10;

    Storage in;

    const VariableIdType variableIdType(variableId);
    send_command( command, variableIdType, in );

    if ( !in.valid_pos() )
        throw runtime_error( "traci::TraCIClient::command_get_value: missing response command" );

    const unsigned int commandStart = in.position();
    const unsigned int commandLength = read_length( in );

    // check answer command
    const UbyteType commandId(in);
    checkCommandId( response_command, commandId, "command_get_value" );

    const VariableIdType answerVarId(in);
    if ( answerVarId != variableIdType )
    {
        ostringstream msg;
        msg << "traci::TraCIClient::command_get_value: got wrong answer (variable, ID), expected ";
        variableIdType.print(msg);
        msg << ", got ";
        answerVarId.print(msg);
        throw runtime_error( msg.str() );
    }

    // value retrieval answers are always with type info
    const bool oldWithTypeId = value.setWithTypeId( true );
    value.read(in);
    value.setWithTypeId( oldWithTypeId );   // reset withTypeId
    check_position( in, commandStart + commandLength );
}


//---------------------------------------------------------------------
/**
 *  Also considers protocol definition for lengths > 255.
 *
 *  \param[in,out]  storage     storage to read from (position will be advanced)
 *
 *  \return     read length
 */
unsigned int TraCIClient::read_length( tcpip::Storage &storage ) const
{
    const UbyteType short_length( storage );
    if ( 0 == short_length )
        return IntegerType( storage );
    return short_length;
}


//---------------------------------------------------------------------
/**
 *  see http://sourceforge.net/apps/mediawiki/sumo/index.php?title=TraCI/Protocol
 *
 *  For historical reasons subfunctions might call \c abort() in case of problems
 *  after sending a message to \c stderr instead of throwing an exception.
 *
 *  \param[in]  command         TraCI command ID to send
 *  \param[in]  command_content part of TraCI command after the command ID
 *  \param[out] answer          tcpip::Storage for command answer, position will
 *                              be set behind status response at first answer command
 */
void TraCIClient::send_command( unsigned char command,
                                tcpip::Storage &command_content,
                                tcpip::Storage &answer )
throw(SocketException)
{
    // Preconditions
    if ( socket_ == NULL )
        throw SocketException( "TraCIClient::send_command: socket_ == NULL" );

    // build command
    Storage out;
    // Length of command
    out.writeByte( 2 + static_cast<int>(command_content.size()) );
    // CommandID
    out.writeChar( command );
    out.writeStorage( command_content );

    // Send command
    socket_->sendExact( out );
    socket_->receiveExact( answer );

    // First result command serves as status information
    string description;
    extract_command_status( answer, command, description );
}


void TraCIClient::send_command( unsigned char commandId,
                                const traci::TraciType &content,
                                tcpip::Storage &answer )
throw( tcpip::SocketException )
{
    // Preconditions
    if ( socket_ == NULL )
        throw SocketException( "TraCIClient::send_command: socket_ == NULL" );

    // build command
    Storage out;
    // Length of command

    if ( content.getLength() <= 253 )
    {
        UbyteType( content.getLength() + 2 ).write( out );
    }
    else
    {
        UbyteType( 0 ).write( out );
        IntegerType( content.getLength() + 6 ).write( out );
    }

    // CommandID
    UbyteType command( commandId );
    command.write( out );
    content.write( out );

    // Send command
    socket_->sendExact( out );
    socket_->receiveExact( answer );

    // First result command serves as status information
    string description;
    extract_command_status( answer, command, description );
}


void TraCIClient::send_command( unsigned char commandId, const traci::TraciType &content )
throw( tcpip::SocketException )
{
    Storage answer;
    send_command( commandId, content, answer );

    // more data in answer?
    if ( answer.valid_pos() )
        throw std::runtime_error( "traci::TraCIClient::send_command: unexpected response content" );
}


//---------------------------------------------------------------------
const Node* TraCIClient::find_node_by_traci_id( const TraCIID& traci_id )
{
    return find_node_by_traci_id_w( traci_id );
}


//---------------------------------------------------------------------
Node* TraCIClient::find_node_by_traci_id_w( const TraCIID& traci_id )
{
    TraCIIDToNodeMap::iterator it = traci_ids_to_node_.find( traci_id );
    return it != traci_ids_to_node_.end() ? it->second : NULL;
}


//---------------------------------------------------------------------
const TraCIClient::TraCIID& TraCIClient::find_traci_id_by_node( const Node& node )
throw( logic_error )
{
    NodeToTraCIIDMap::const_iterator it = node_to_traci_ids_.find( const_cast<Node*>( &node ) );
    if ( it == node_to_traci_ids_.end() ) throw logic_error( "node not found" );
    return it->second;
}


//---------------------------------------------------------------------
void TraCIClient::add_node( Node* node, const TraCIID& traci_id )
{
    assert( node );
    sc_->world_w().add_node( *node );
    assert( node_to_traci_ids_.find( node ) == node_to_traci_ids_.end() );
    assert( traci_ids_to_node_.find( traci_id ) == traci_ids_to_node_.end() );

    node_to_traci_ids_.insert( make_pair<Node*, TraCIID> ( node, traci_id ) );
    traci_ids_to_node_.insert( make_pair<TraCIID, Node*> ( traci_id, node ) );
    assert( traci_ids_to_node_.size() == node_to_traci_ids_.size() );

    // Inform the listeners
    for ( TraCINodeChangeListenerList::iterator it = node_change_listeners_.begin();
            it != node_change_listeners_.end(); ++it )
    {
        ( *it )->node_added( *node, traci_id );
    }
}
//---------------------------------------------------------------------
void TraCIClient::remove_node_from_map( Node* node )
{
    assert( node );

    TraCIID ti;
    try
    {
        ti = find_traci_id_by_node( *node );
    }
    catch ( logic_error )
    {
        assert( traci_ids_to_node_.size() == node_to_traci_ids_.size() );
        // Not found, nothing to remove!
        return;
    }

    // Inform the listeners
    for ( TraCINodeChangeListenerList::iterator it = node_change_listeners_.begin();
            it != node_change_listeners_.end(); ++it )
    {
        ( *it )->node_removed( *node, ti );
    }

    TraCIIDToNodeMap::iterator ttn = traci_ids_to_node_.find( ti );
    NodeToTraCIIDMap::iterator ntt = node_to_traci_ids_.find( node );
    if ( ttn != traci_ids_to_node_.end() )
    {
        //assert(ttn != traci_ids_to_node_.end());
        traci_ids_to_node_.erase( ttn );
    }
    if ( ntt != node_to_traci_ids_.end() )
    {
        //assert(ntt != node_to_traci_ids_.end());
        node_to_traci_ids_.erase( ntt );
    }
    assert( traci_ids_to_node_.size() == node_to_traci_ids_.size() );
}


//---------------------------------------------------------------------
bool TraCIClient::check_domain( int domain, const TraCIID& traci_id, bool abrt,
                                string err_msg )
{
    if ( domain == traci_id.domain() ) return true;
    if ( abrt )
    {
        cerr << err_msg << endl;
        abort();
    }
    return false;
}


//---------------------------------------------------------------------
std::string TraCIClient::number_to_string( double n )
{
    stringstream oss;
    oss << n;
    return oss.str();
}


//---------------------------------------------------------------------
TraCIClient::NodeToTraCIIDMap::const_iterator TraCIClient::begin_traci_nodes() const
throw()
{
    return node_to_traci_ids_.begin();
}


//---------------------------------------------------------------------
TraCIClient::NodeToTraCIIDMap::const_iterator TraCIClient::end_traci_nodes() const
throw()
{
    return node_to_traci_ids_.end();
}


//---------------------------------------------------------------------
//---TraCIID---
//---------------------------------------------------------------------
TraCIClient::TraCIID::TraCIID() :
        domain_( -1 ),
        id_( -1 )
{
}


//---------------------------------------------------------------------
TraCIClient::TraCIID::TraCIID( int d, int i ) :
        domain_( d ),
        id_( i )
{
}


//---------------------------------------------------------------------
TraCIClient::TraCIID::TraCIID( const TraCIID& o ) :
        domain_( o.domain() ),
        id_( o.id() )
{
}


//---------------------------------------------------------------------
TraCIClient::TraCIID::~TraCIID()
{
}


//---------------------------------------------------------------------
int TraCIClient::TraCIID::domain() const
{
    return domain_;
}


//---------------------------------------------------------------------
int TraCIClient::TraCIID::id() const
{
    return id_;
}


//---------------------------------------------------------------------
TraCIClient::TraCIID& TraCIClient::TraCIID::operator= ( const TraCIClient::TraCIID& rv )
{
    if ( this == & rv ) return *this;
    id_ = rv.id();
    domain_ = rv.domain();
    return *this;
}


//---------------------------------------------------------------------
int operator< ( const TraCIClient::TraCIID& left,
                const TraCIClient::TraCIID& right )
{
    if ( left.domain_ < right.domain_ ) return 1;
    if ( left.domain_ > right.domain_ ) return 0;
    if ( left.id_ < right.id_ ) return 1;
    if ( left.id_ > right.id_ ) return 0;
    return 0;
}


//---------------------------------------------------------------------
int operator> ( const TraCIClient::TraCIID& left,
                const TraCIClient::TraCIID& right )
{
    return ( right < left );
}


//---------------------------------------------------------------------
int operator== ( const TraCIClient::TraCIID& left,
                 const TraCIClient::TraCIID& right )
{
    if ( left.domain_ == right.domain_ && left.id_ == right.id_ ) return 1;
    return 0;
}


}   // namespace traci

#endif  // #ifdef ENABLE_TRACICLIENT

