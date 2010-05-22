/* session_description.h
   Generated by wsdl2h 1.2.9l from SessionManagementService.wsdl and typemap.dat
   2010-05-21 13:48:32 GMT
   Copyright (C) 2001-2007 Robert van Engelen, Genivia Inc. All Rights Reserved.
   This part of the software is released under one of the following licenses:
   GPL or Genivia's license for commercial use.
*/

/* NOTE:

 - Compile this file with soapcpp2 to complete the code generation process.
 - Use soapcpp2 option -I to specify paths for #import
   To build with STL, 'stlvector.h' is imported from 'import' dir in package.
 - Use wsdl2h options -c and -s to generate pure C code or C++ code without STL.
 - Use 'typemap.dat' to control schema namespace bindings and type mappings.
   It is strongly recommended to customize the names of the namespace prefixes
   generated by wsdl2h. To do so, modify the prefix bindings in the Namespaces
   section below and add the modified lines to 'typemap.dat' to rerun wsdl2h.
 - Use Doxygen (www.doxygen.org) to browse this file.
 - Use wsdl2h option -l to view the software license terms.

   DO NOT include this file directly into your project.
   Include only the soapcpp2-generated headers and source code files.
*/

//gsoapopt w

/******************************************************************************\
 *                                                                            *
 * urn:SessionManagementService                                               *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Import                                                                     *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Schema Namespaces                                                          *
 *                                                                            *
\******************************************************************************/


/* NOTE:

It is strongly recommended to customize the names of the namespace prefixes
generated by wsdl2h. To do so, modify the prefix bindings below and add the
modified lines to typemap.dat to rerun wsdl2h:

shawnts = "urn:SessionManagementService"
ns1 = ""
ns2 = "urn:CommonTypes"

*/

//gsoap ns1   schema namespace:	
//gsoap shawnts schema namespace:	urn:SessionManagementService
//gsoap ns2   schema namespace:	urn:CommonTypes
//gsoap ns1   schema form:	unqualified
//gsoap shawnts schema form:	unqualified
//gsoap ns2   schema form:	unqualified

/******************************************************************************\
 *                                                                            *
 * Schema Types                                                               *
 *                                                                            *
\******************************************************************************/
namespace session_management {

/// Built-in type "xs:base64Binary".
class xsd__base64Binary
{	unsigned char *__ptr;
	int __size;
	char *id, *type, *options; // NOTE: for DIME and MTOM XOP attachments only
	struct soap *soap;
};


//  Forward declaration of class shawnts__getInstance.
class shawnts__getInstance;

//  Forward declaration of class shawnts__ExperimentNotRunningException.
class shawnts__ExperimentNotRunningException;

//  Forward declaration of class shawnts__UnknownReservationIdException.
class shawnts__UnknownReservationIdException;

//  Forward declaration of class shawnts__free.
class shawnts__free;

//  Forward declaration of class shawnts__freeResponse.
class shawnts__freeResponse;

//  Forward declaration of class shawnts__getInstanceResponse.
class shawnts__getInstanceResponse;

//  Forward declaration of class ns2__message.
class ns2__message;

//  Forward declaration of class ns2__textMessage.
class ns2__textMessage;

//  Forward declaration of class ns2__binaryMessage.
class ns2__binaryMessage;

//  Forward declaration of class ns2__getNetwork.
class ns2__getNetwork;

//  Forward declaration of class ns2__getNetworkResponse.
class ns2__getNetworkResponse;

/// "urn:CommonTypes":requestId is a simpleType restriction of xs:string.
typedef char* ns2__requestId;

/// "urn:CommonTypes":messageType is a simpleType restriction of xs:string.
/// Note: enum values are prefixed with 'ns2__messageType' to avoid name clashes, please use wsdl2h option -e to omit this prefix
enum ns2__messageType
{
	ns2__messageType__TEXT,	///< xs:string value="TEXT"
	ns2__messageType__BINARY,	///< xs:string value="BINARY"
};

/// "urn:CommonTypes":messageLevel is a simpleType restriction of xs:string.
/// Note: enum values are prefixed with 'ns2__messageLevel' to avoid name clashes, please use wsdl2h option -e to omit this prefix
enum ns2__messageLevel
{
	ns2__messageLevel__TRACE,	///< xs:string value="TRACE"
	ns2__messageLevel__DEBUG,	///< xs:string value="DEBUG"
	ns2__messageLevel__INFO,	///< xs:string value="INFO"
	ns2__messageLevel__WARN,	///< xs:string value="WARN"
	ns2__messageLevel__ERROR,	///< xs:string value="ERROR"
	ns2__messageLevel__FATAL,	///< xs:string value="FATAL"
};

/// "urn:CommonTypes":urn is a simpleType restriction of xs:string.
/// Content pattern is "urn:.*"
typedef char* ns2__urn "urn:.*";

/// "urn:SessionManagementService":getInstance is a complexType.
class shawnts__getInstance
{ public:
/// Element reservationId of type xs:string.
    char*                                reservationId                  1;	///< Required element.
/// Element controller of type "urn:CommonTypes":urn.
    ns2__urn                             controller                     1;	///< Required element.
/// A handle to the soap struct that manages this instance (automatically set)
    struct soap                         *soap                          ;
};

/// "urn:SessionManagementService":ExperimentNotRunningException is a complexType.
class shawnts__ExperimentNotRunningException
{ public:
/// Element message of type xs:string.
    char*                                message                        0;	///< Optional element.
/// A handle to the soap struct that manages this instance (automatically set)
    struct soap                         *soap                          ;
};

/// "urn:SessionManagementService":UnknownReservationIdException is a complexType.
class shawnts__UnknownReservationIdException
{ public:
/// Element message of type xs:string.
    char*                                message                        0;	///< Optional element.
/// Element reservationId of type xs:string.
    char*                                reservationId                  1;	///< Required element.
/// A handle to the soap struct that manages this instance (automatically set)
    struct soap                         *soap                          ;
};

/// "urn:SessionManagementService":free is a complexType.
class shawnts__free
{ public:
/// Element reservationId of type xs:string.
    char*                                reservationId                  1;	///< Required element.
/// A handle to the soap struct that manages this instance (automatically set)
    struct soap                         *soap                          ;
};

/// "urn:SessionManagementService":freeResponse is a complexType.
class shawnts__freeResponse
{ public:
/// A handle to the soap struct that manages this instance (automatically set)
    struct soap                         *soap                          ;
};

/// "urn:SessionManagementService":getInstanceResponse is a complexType.
class shawnts__getInstanceResponse
{ public:
/// Element return of type "urn:CommonTypes":urn.
    ns2__urn                             return_                        1;	///< Required element.
/// A handle to the soap struct that manages this instance (automatically set)
    struct soap                         *soap                          ;
};

/// "urn:CommonTypes":message is a complexType.
class ns2__message
{ public:
/// Element sourceNodeId of type xs:string.
    char*                                sourceNodeId                   1;	///< Required element.
/// Element timestamp of type xs:dateTime.
    time_t                               timestamp                      1;	///< Required element.
/// CHOICE OF ELEMENTS FOR choice
    int                                  __union_message               ;	///< Union _ns2__union_message selector: set to SOAP_UNION__ns2__union_message_<fieldname>
    union _ns2__union_message
    {
/// Element textMessage of type "urn:CommonTypes":textMessage.
    ns2__textMessage*                    textMessage                    1;	///< Required element.
/// Element binaryMessage of type "urn:CommonTypes":binaryMessage.
    ns2__binaryMessage*                  binaryMessage                  1;	///< Required element.
    }                                    union_message                 ;
//  END OF CHOICE
/// A handle to the soap struct that manages this instance (automatically set)
    struct soap                         *soap                          ;
};

/// "urn:CommonTypes":textMessage is a complexType.
class ns2__textMessage
{ public:
/// Element msg of type xs:string.
    char*                                msg                            1;	///< Required element.
/// Element messageLevel of type "urn:CommonTypes":messageLevel.
    enum ns2__messageLevel*              messageLevel                   0;	///< Optional element.
/// A handle to the soap struct that manages this instance (automatically set)
    struct soap                         *soap                          ;
};

/// "urn:CommonTypes":binaryMessage is a complexType.
class ns2__binaryMessage
{ public:
/// Element binaryData of type xs:base64Binary.
    xsd__base64Binary                    binaryData                     1;	///< Required element.
/// Element binaryType of type xs:byte.
    short*                               binaryType                     0;	///< Optional element.
/// A handle to the soap struct that manages this instance (automatically set)
    struct soap                         *soap                          ;
};

/// "urn:CommonTypes":getNetwork is a complexType.
class ns2__getNetwork
{ public:
/// A handle to the soap struct that manages this instance (automatically set)
    struct soap                         *soap                          ;
};

/// "urn:CommonTypes":getNetworkResponse is a complexType.
class ns2__getNetworkResponse
{ public:
/// Element return of type xs:string.
    char*                                return_                        1;	///< Required element.
/// A handle to the soap struct that manages this instance (automatically set)
    struct soap                         *soap                          ;
};

/// Element "urn:SessionManagementService":free of type "urn:SessionManagementService":free.
/// Note: use wsdl2h option -g to generate this global element declaration.

/// Element "urn:SessionManagementService":freeResponse of type "urn:SessionManagementService":freeResponse.
/// Note: use wsdl2h option -g to generate this global element declaration.

/// Element "urn:SessionManagementService":getInstance of type "urn:SessionManagementService":getInstance.
/// Note: use wsdl2h option -g to generate this global element declaration.

/// Element "urn:SessionManagementService":ExperimentNotRunningException of type "urn:SessionManagementService":ExperimentNotRunningException.
/// Note: use wsdl2h option -g to generate this global element declaration.

/// Element "urn:SessionManagementService":UnknownReservationIdException of type "urn:SessionManagementService":UnknownReservationIdException.
/// Note: use wsdl2h option -g to generate this global element declaration.

/// Element "urn:SessionManagementService":getInstanceResponse of type xs:string.
/// Note: use wsdl2h option -g to generate this global element declaration.

/// Element "urn:CommonTypes":getNetwork of type "urn:CommonTypes":getNetwork.
/// Note: use wsdl2h option -g to generate this global element declaration.

/// Element "urn:CommonTypes":getNetworkResponse of type "urn:CommonTypes":getNetworkResponse.
/// Note: use wsdl2h option -g to generate this global element declaration.

/// Element "urn:CommonTypes":message of type "urn:CommonTypes":message.
/// Note: use wsdl2h option -g to generate this global element declaration.

/// Element "urn:CommonTypes":urn of type "urn:CommonTypes":urn.
/// Note: use wsdl2h option -g to generate this global element declaration.

/******************************************************************************\
 *                                                                            *
 * Services                                                                   *
 *                                                                            *
\******************************************************************************/


//gsoap shawnts service name:	SessionManagementPortBinding 
//gsoap shawnts service type:	SessionManagement 
//gsoap shawnts service port:	REPLACE_WITH_ACTUAL_URL 
//gsoap shawnts service namespace:	urn:SessionManagementService 
//gsoap shawnts service transport:	http://schemas.xmlsoap.org/soap/http 

/** @mainpage SessionManagementService Definitions

@section SessionManagementService_bindings Bindings
  - @ref SessionManagementPortBinding

*/

/**

@page SessionManagementPortBinding Binding "SessionManagementPortBinding"

@section SessionManagementPortBinding_operations Operations of Binding  "SessionManagementPortBinding"
  - @ref __shawnts__getInstance
  - @ref __shawnts__free
  - @ref __shawnts__getNetwork

@section SessionManagementPortBinding_ports Endpoints of Binding  "SessionManagementPortBinding"
  - REPLACE_WITH_ACTUAL_URL

Note: use wsdl2h option -N to change the service binding prefix name

*/

/******************************************************************************\
 *                                                                            *
 * SOAP Fault Detail                                                          *
 *                                                                            *
\******************************************************************************/

/**

The SOAP Fault is part of the gSOAP context and its content is accessed
through the soap.fault->detail variable (SOAP 1.1) or the
soap.fault->SOAP_ENV__Detail variable (SOAP 1.2).
Use option -j to omit.

*/
struct SOAP_ENV__Detail
{
    int                                  __type                        ;	///< set to SOAP_TYPE_X for a serializable type X
    void                                *fault                         ;	///< points to serializable object X or NULL
    _XML                                 __any                         ;	///< Catch any element content in XML string.
    shawnts__ExperimentNotRunningException*  shawnts__ExperimentNotRunningException_;
///< SOAP Fault element ""urn:SessionManagementService":ExperimentNotRunningException" part "fault"
    shawnts__UnknownReservationIdException*  shawnts__UnknownReservationIdException_;
///< SOAP Fault element ""urn:SessionManagementService":UnknownReservationIdException" part "fault"
};

/******************************************************************************\
 *                                                                            *
 * SessionManagementPortBinding                                               *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * __shawnts__getInstance                                                     *
 *                                                                            *
\******************************************************************************/


/// Operation "__shawnts__getInstance" of service binding "SessionManagementPortBinding"

/**

Operation details:

  - SOAP document/literal style
  - SOAP Fault: "urn:SessionManagementService":ExperimentNotRunningException (literal)
  - SOAP Fault: "urn:SessionManagementService":UnknownReservationIdException (literal)

C stub function (defined in soapClient.c[pp] generated by soapcpp2):
@code
  int soap_call___shawnts__getInstance(
    struct soap *soap,
    NULL, // char *endpoint = NULL selects default endpoint for this operation
    NULL, // char *action = NULL selects default action for this operation
    // request parameters:
    shawnts__getInstance*               shawnts__getInstance_,
    // response parameters:
    char*                              &shawnts__getInstanceResponse_
  );
@endcode

C server function (called from the service dispatcher defined in soapServer.c[pp]):
@code
  int __shawnts__getInstance(
    struct soap *soap,
    // request parameters:
    shawnts__getInstance*               shawnts__getInstance_,
    // response parameters:
    char*                              &shawnts__getInstanceResponse_
  );
@endcode

C++ proxy class (defined in soapSessionManagementPortBindingProxy.h):
  class SessionManagementPortBinding;

Note: use soapcpp2 option '-i' to generate improved proxy and service classes;

*/

//gsoap shawnts service method-style:	getInstance document
//gsoap shawnts service method-encoding:	getInstance literal
//gsoap shawnts service method-action:	getInstance ""
//gsoap shawnts service method-fault:	getInstance shawnts__ExperimentNotRunningException_
//gsoap shawnts service method-fault:	getInstance shawnts__UnknownReservationIdException_
int __shawnts__getInstance(
    shawnts__getInstance*               shawnts__getInstance_,	///< Request parameter
    char*                              &shawnts__getInstanceResponse_	///< Response parameter
);

/******************************************************************************\
 *                                                                            *
 * __shawnts__free                                                            *
 *                                                                            *
\******************************************************************************/


/// Operation "__shawnts__free" of service binding "SessionManagementPortBinding"

/**

Operation details:

  - SOAP document/literal style
  - SOAP Fault: "urn:SessionManagementService":ExperimentNotRunningException (literal)
  - SOAP Fault: "urn:SessionManagementService":UnknownReservationIdException (literal)

C stub function (defined in soapClient.c[pp] generated by soapcpp2):
@code
  int soap_call___shawnts__free(
    struct soap *soap,
    NULL, // char *endpoint = NULL selects default endpoint for this operation
    NULL, // char *action = NULL selects default action for this operation
    // request parameters:
    shawnts__free*                      shawnts__free_,
    // response parameters:
    shawnts__freeResponse*              shawnts__freeResponse_
  );
@endcode

C server function (called from the service dispatcher defined in soapServer.c[pp]):
@code
  int __shawnts__free(
    struct soap *soap,
    // request parameters:
    shawnts__free*                      shawnts__free_,
    // response parameters:
    shawnts__freeResponse*              shawnts__freeResponse_
  );
@endcode

C++ proxy class (defined in soapSessionManagementPortBindingProxy.h):
  class SessionManagementPortBinding;

Note: use soapcpp2 option '-i' to generate improved proxy and service classes;

*/

//gsoap shawnts service method-style:	free document
//gsoap shawnts service method-encoding:	free literal
//gsoap shawnts service method-action:	free ""
//gsoap shawnts service method-fault:	free shawnts__ExperimentNotRunningException_
//gsoap shawnts service method-fault:	free shawnts__UnknownReservationIdException_
int __shawnts__free(
    shawnts__free*                      shawnts__free_,	///< Request parameter
    shawnts__freeResponse*              shawnts__freeResponse_	///< Response parameter
);

/******************************************************************************\
 *                                                                            *
 * __shawnts__getNetwork                                                      *
 *                                                                            *
\******************************************************************************/


/// Operation "__shawnts__getNetwork" of service binding "SessionManagementPortBinding"

/**

Operation details:

  - SOAP document/literal style

C stub function (defined in soapClient.c[pp] generated by soapcpp2):
@code
  int soap_call___shawnts__getNetwork(
    struct soap *soap,
    NULL, // char *endpoint = NULL selects default endpoint for this operation
    NULL, // char *action = NULL selects default action for this operation
    // request parameters:
    ns2__getNetwork*                    ns2__getNetwork_,
    // response parameters:
    ns2__getNetworkResponse*            ns2__getNetworkResponse_
  );
@endcode

C server function (called from the service dispatcher defined in soapServer.c[pp]):
@code
  int __shawnts__getNetwork(
    struct soap *soap,
    // request parameters:
    ns2__getNetwork*                    ns2__getNetwork_,
    // response parameters:
    ns2__getNetworkResponse*            ns2__getNetworkResponse_
  );
@endcode

C++ proxy class (defined in soapSessionManagementPortBindingProxy.h):
  class SessionManagementPortBinding;

Note: use soapcpp2 option '-i' to generate improved proxy and service classes;

*/

//gsoap shawnts service method-style:	getNetwork document
//gsoap shawnts service method-encoding:	getNetwork literal
//gsoap shawnts service method-action:	getNetwork ""
int __shawnts__getNetwork(
    ns2__getNetwork*                    ns2__getNetwork_,	///< Request parameter
    ns2__getNetworkResponse*            ns2__getNetworkResponse_	///< Response parameter
);

}

/* End of session_description.h */
