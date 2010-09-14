/** \file
 *  Definitions of more complex TraCI data types.
 *
 *  \author Björn Hendriks
 */

#ifndef COMPLEX_DATATYPE_H
#define COMPLEX_DATATYPE_H

#include "_apps_enable_cmake.h"

#ifdef ENABLE_TRACICLIENT

#include "atomic_datatypes.h"
#include "composed_datatypes.h"
#include "dynamic_length_datatypes.h"
#include "mobility_interface_constants.h"


namespace traci
{


/**
 *  Key/value pair of two arbitrary TraciTypes.
 *
 *  This template generalizes the structure of the scenario command and change
 *  state commands. Those commands are implemented as specializations
 *  of this template by deriving from it. See ChangeState for an example.
 *
 *  The template defines a TraciType-value struct which can be used as value of
 *  ComposedValue template. That value struct is used to define a composed
 *  TraciType consisting of a \p TheKeyType and a \p TheValueType.
 *
 *  \tparam TheKeyType      TraciType of the key
 *  \tparam TheValueType    TraciType of the value
 *  \tparam ValueWithType   enforcement of with-type property of value
 */
template < class TheKeyType, class TheValueType, bool ValueWithType = true  >
struct KeyValuePair
{
    /// Give element types a name
    typedef TheKeyType KeyType;
    typedef TheValueType ValueType;

    /// composed key/value value
    struct KeyValuePairValue
    {
    public:
        explicit KeyValuePairValue( const KeyType & aKey = KeyType(),
                                    const ValueType & aValue = ValueType() ) :
            key(aKey),
            value(aValue)
        {
        }

        KeyType key;
        ValueType value;

    protected:
        /// set order of elements in TraCI type
        virtual void init( ComposedContainer &queue )
        {
            queue.push_back(&key);
            queue.push_back(&value);

            value.setWithTypeId(ValueWithType);
        }
    };

    /// TraciType
    typedef CompoundTraciType< MessageHandlerComposed< ComposedValue< KeyValuePairValue > > > KeyValuePairTraciType;
};


/**
 *  Template with all structures to handle a change state command with a particular value type
 *
 *  See http://sourceforge.net/apps/mediawiki/sumo/index.php?title=TraCI/Plane_SUMO_ID_Commands_Structure
 *
 *  \tparam TheChangeStateValueType     type derived from TraciType to hold the new state value
 */
template < class TheChangeStateValueType >
struct ChangeState : KeyValuePair< VariableIdType, TheChangeStateValueType >
{
    /// Base Type
    typedef KeyValuePair< VariableIdType, TheChangeStateValueType > MyKeyValuePair;

    /// Give value type a name
    typedef typename MyKeyValuePair::ValueType ChangeStateValueType;

    /// Name of composed key/value structure
    typedef typename MyKeyValuePair::KeyValuePairValue ChangeStateKeyValuePair;

    /// TraciType for change state command
    typedef typename MyKeyValuePair::KeyValuePairTraciType ChangeStateTraciType;
};


/** \name   Structures for TraCI's scenario command
 *
 *  See http://sourceforge.net/apps/mediawiki/sumo/index.php?title=TraCI/Environment-related_commands
 */
//@{


/// composed value for scenario command's request data
struct ScenarioRequest
{
public:
    /// enumeration of scenario domains
    enum Domain
    {
        DomainRoadMap = 0x00,
        DomainVehicle = 0x01,
        DomainTrafficLight = 0x02,
        DomainPoi = 0x03,
        DomainPolygon = 0x04
    };

    /// default constructor
    ScenarioRequest() :
        flag(0x00),
        domain(),
        domainId(),
        variable()
    {
    }

    /// initialization constructor
    ScenarioRequest(Domain aDomain, int aDomainId, unsigned char aVariable, bool isRead = true) :
        flag(isRead ? 0x00 : 0x01),
        domain(aDomain),
        domainId(aDomainId),
        variable(aVariable)
    {
    }

    UbyteType flag;
    UbyteType domain;
    IntegerType domainId;
    UbyteType variable;

protected:
    /// set order of elements in TraCI type
    virtual void init( ComposedContainer &queue )
    {
        queue.push_back(&flag);
        queue.push_back(&domain);
        queue.push_back(&domainId);
        queue.push_back(&variable);
    }
};
/// TraciType for value structure ScenarioRequest
typedef CompoundTraciType< MessageHandlerComposed< ComposedValue<ScenarioRequest> > > ScenarioRequestType;


/**
 *  Template with all structures to handle a scenario command with a particular value type
 *
 *  See http://sourceforge.net/apps/mediawiki/sumo/index.php?title=TraCI/Environment-related_commands
 *
 *  \tparam TheScenarioValueType    type derived from TraciType to hold the scenario command value
 */
template < class TheScenarioValueType >
struct Scenario : KeyValuePair< ScenarioRequestType, TheScenarioValueType >
{
    /// Base Type
    typedef KeyValuePair< ScenarioRequestType, TheScenarioValueType > MyKeyValuePair;

    /// Make value type accessible
    typedef typename MyKeyValuePair::ValueType ScenarioValueType;

    /// composed value for scenario command
    typedef typename MyKeyValuePair::KeyValuePairValue ScenarioValue;

    /// TraciType for scenario command
    typedef typename MyKeyValuePair::KeyValuePairTraciType ScenarioTraciType;
};


//@}



/** \name   Structures for TraCI compound object links of command get lane variable
 *
 *  See
 *  http://sourceforge.net/apps/mediawiki/sumo/index.php?title=TraCI/Lane_Value_Retrieval
 */
//@{

/**
 *  Composed value type for lanes link as part of result of get lane variable links
 *
 *  This is one link element of the composed result of variable links of lane
 *  value retrieval.
 */
struct SingleLaneLink
{
    /// consecutive not internal lane
    StringTypeWithTypeId consecutiveNotInternal;
    /// consecutive internal lane
    StringTypeWithTypeId consecutiveInternal;
    /// has priority (true = 1, false = 0)
    UbyteTypeWithTypeId hasPriority;
    /// is opened (true = 1, false = 0)
    UbyteTypeWithTypeId isOpened;
    /// has approaching foo (true = 1, false = 0)
    UbyteTypeWithTypeId hasApproachingFoo;
    /// (current) state
    StringTypeWithTypeId currState;
    /// direction
    StringTypeWithTypeId direction;
    /// length [m] (only valid if not using internal lanes)
    FloatTypeWithTypeId length;

protected:
    /// set order of elements in TraCI type
    virtual void init( ComposedContainer &queue )
    {
        queue.push_back(&consecutiveNotInternal);
        queue.push_back(&consecutiveInternal);
        queue.push_back(&hasPriority);
        queue.push_back(&isOpened);
        queue.push_back(&hasApproachingFoo);
        queue.push_back(&currState);
        queue.push_back(&direction);
        queue.push_back(&length);
    }
};
/// GenTraciType of LaneLink
typedef CompoundTraciType< MessageHandlerComposed< ComposedValue<SingleLaneLink> > > SingleLaneLinkType;
/// Container for single lane links
typedef DynamicLengthValue<SingleLaneLinkType, IntegerTypeWithTypeId> LaneLinksContainer;
/// Message handler for dynamic list of LaneLinkType
typedef MessageHandlerDynamicLength< LaneLinksContainer > MessageHandlerLaneLinksList;
/// TraciType for dynamic list of LaneLinkType
typedef CompoundTraciType< MessageHandlerLaneLinksList > LaneLinksListType;


/**
 *  Composed value type for the whole compound object links of command get lane variable
 *
 *  This TraCI type is only used for reading so we don't need a functions to
 *  compute length.
 */
struct LaneLinks
{
    /// length
    IntegerType length;
    /// list of LaneLinkType
    LaneLinksListType laneLinks;

protected:
    /// set order of elements in TraCI type
    virtual void init( ComposedContainer &queue )
    {
        queue.push_back(&length);
        queue.push_back(&laneLinks);
    }
};
/// GenTraciType of the whole compund object LaneLinks
typedef CompoundTraciType< MessageHandlerComposed< ComposedValue<LaneLinks> > > LaneLinksType;

//@}


}   // namespace traci

#endif  // #ifdef ENABLE_TRACICLIENT
#endif // COMPLEX_DATATYPE_H

