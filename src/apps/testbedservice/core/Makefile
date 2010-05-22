all: soaps

controller_client_header:
	wsdl2h -N shawnts -s -o controller_client_description.h ControllerService.wsdl

wsnapi_client_header:
	wsdl2h -N shawnts -s -o wsnapi_client_description.h WSNService.wsdl

wsnapi_server_header:
	wsdl2h -N shawnts -s -o wsnapi_server_description.h WSNService.wsdl

session_server_header:
	wsdl2h -N shawnts -s -o session_description.h SessionManagementService.wsdl

headers:
	make controller_client_header
	make wsnapi_client_header
	make wsnapi_server_header
	make session_server_header

soaps:
	soapcpp2 -i -C -L -n -pcontroller_client controller_client_description.h
	soapcpp2 -i -C -L -n -pwsn_client wsnapi_client_description.h
	soapcpp2    -S -L -n -pwsn_server wsnapi_server_description.h
	soapcpp2    -S -L -n -psession_server session_description.h

clean:
	rm -f \
		WSNPortBinding* wsnapi_clientS* wsnapi_clientC.cpp \
	   wsnapi_clientH.h wsnapi_client.nsmap wsnapi_clientWSNPortBindingProxy* \
	   controllerControllerPortBindingProxy* controllerC.cpp \
	   controller_clientH.h controller_client.nsmap \
		controller_clientStub.h controller_clientC.cpp \
	   controller_clientControllerPortBindingProxy* \
	   ControllerPortBinding* SessionManagementPortBinding* \
	   session_managementC.cpp session_managementH.h session_management.nsmap \
	   session_managementS* \
	   wsnapi_serverWSNPortBindingObject* wsnapi_serverC.cpp wsnapi_serverH.h \
	   wsnapi_server.nsmap wsnapi_serverServer.cpp wsnapi_serverStub.h

