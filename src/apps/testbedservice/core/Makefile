all: soaps

client_header:
	wsdl2h -N shawnts -s -o client_description.h ControllerService.wsdl

server_header:
	wsdl2h -N shawnts -s -o server_description.h WSNService.wsdl

headers:
	make client_header
	make server_header

soaps:
	soapcpp2 -i -C -L -n -pshawn_client client_description.h
	soapcpp2    -S -L -n -pshawn_server server_description.h

clean:
	rm -f \
		ControllerPortBinding* shawn_client* \
		shawn_server* *.xml
