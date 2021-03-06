#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <netdb.h>

#include <iostream>
#include <cstdlib>
#include <fstream>

#include "person.hpp"
#include "crc.hpp"

/* Procedure that print error message and terminate the program */
void s_error(const char * msg) {
	perror(msg);
	exit(1);
}

/* Main program */
int main() {
	int sockfd;
	int acceptfd;
	int forceRebind = 1;
	struct sockaddr_in server;
	struct sockaddr_in client;
	socklen_t clientaddrlen = 0;

	std::string filename = "demo_server.xml";

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) 
		s_error("socket");

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &forceRebind, sizeof(int)) == -1) 
		s_error("setsockopt");

	memset((void *) &server, 0, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(1234);
	server.sin_addr.s_addr = INADDR_ANY;

	if (bind(sockfd, (const struct sockaddr *) &server, sizeof(struct sockaddr_in)) == -1)
		s_error("bind");

	if (listen(sockfd, 0) == -1) 
		s_error("listen");

	acceptfd = accept(sockfd, (struct sockaddr *) &client, &clientaddrlen);
	if (acceptfd == -1) 
		s_error("accept");

	// Receive XML file and save it as demo_recv.xml
	std::ofstream xmlfd;
	const int r_buff_len = 1024;
	char * r_buff = new char [r_buff_len];
	ssize_t r_bytes = 0;

	xmlfd.open(filename.c_str(), std::ios::binary);
	if (xmlfd.is_open()) {
		while ((r_bytes = recv(acceptfd, r_buff, r_buff_len, 0)) > 0) {
			xmlfd.write(r_buff, r_bytes);
		}	
	}
	else s_error("XML File");

	xmlfd.close();

	person_list pl;
	load_pl(pl, filename.c_str());

	std::cout << pl << std::endl;
	
	std::remove(filename.c_str());

	if (close(acceptfd) == -1)
		s_error("Close AcceptFD");

	if (close(sockfd) == -1) 
		s_error("Close SockFD");

	return 0;
}
