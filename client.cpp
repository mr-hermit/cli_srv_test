#include <iostream>
#include <fstream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <netdb.h>
#include <cstdlib>

#include <boost/integer.hpp>
#include "person.hpp"
#include "crc.hpp"

/* Procedure that print error message and terminate the program */
void s_error(const char * msg) { 
	perror(msg);
	exit(1);
}

/* Main program */
int main() {
	/* Creating scope of PERSON objects and serialize it to XML */
	std::string filename = "demo_client.xml";

	student *po1 = new student("Ilya Zolotko","Male",26,"RTS-41","4.1");
	student *po2 = new student("Alexaner Gaar","Male",28,"RTS-41","4.1");
	student *po3 = new student("Galina Lipatova","Female",28,"RTS-41","4.1");
	teacher *po4 = new teacher("Ludmila Zolotko","Female",24,"Wife","candidate");
	teacher *po5 = new teacher("Elena Kovsharova","Female",32,"Friend","professor");

	person_list pl;
	pl.append(po1);
	pl.append(po2);
	pl.append(po3);
	pl.append(po4);
	pl.append(po5);

	/* Print person list */
	std::cout << pl << std::endl;

	std::cout << "Serialize PERSON_LIST to XML file" << std::endl;
	save_pl(pl, filename.c_str());

	/* Initialization socket connection and try connect to server */
	int sockfd;
	struct sockaddr_in server;
	struct addrinfo *serverinfo;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) 
		s_error("socket");
	
	if (getaddrinfo("localhost", NULL, NULL, &serverinfo) != 0) 
		s_error("getaddrinfo");

	memcpy(&server, serverinfo->ai_addr, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(1234);
	freeaddrinfo(serverinfo);

	/* Connect to the server */ 
	if (connect(sockfd, (const struct sockaddr *) &server, sizeof(struct sockaddr_in)) != -1) {
		std::cout << "Connect to server established!!!";

		/* Send XML file */
		std::ifstream xmlfd;
		const int r_buff_len = 1024;
		char * r_buff = new char [r_buff_len];

		xmlfd.open(filename.c_str(), std::ios::binary);
		if (xmlfd.is_open()) {
			while (!xmlfd.eof()) {
				xmlfd.read(r_buff, r_buff_len);

				if (send(sockfd, r_buff, (size_t) xmlfd.gcount(), 0) == -1)
					s_error("send r_buff");
			}
		}	
		else s_error("XML file");

		xmlfd.close();

		delete[] r_buff;
		close(sockfd);
	} else s_error("connect");

	std::remove(filename.c_str());

	delete po1;
	delete po2;
	delete po3;
	delete po4;
	delete po5;

	return 0;
}
