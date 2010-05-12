all: soaps

client_header:
	wsdl2h -N shawnts -s -o client_description.h ControllerService.wsdl

server_header:
	wsdl2h -N shawnts -s -o server_description.h WSNService.wsdl

session_header:
	wsdl2h -N shawnts -s -o session_description.h SessionManagementService.wsdl

headers:
	make client_header
	make server_header
	make session_header

soaps:
	soapcpp2 -i -C -L -n -pshawn_client client_description.h
	soapcpp2    -S -L -n -pshawn_server server_description.h
	soapcpp2    -S -L -n -pshawn_server session_description.h

clean:
	rm -f \
		ControllerPortBinding* shawn_client* \
		shawn_server* *.xml
