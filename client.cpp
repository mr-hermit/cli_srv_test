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

void s_error(const char * msg) { 
	perror(msg);
	exit(1);
}

int main() {
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

	if (connect(sockfd, (const struct sockaddr *) &server, sizeof(struct sockaddr_in)) != -1) {

		std::ifstream xmlfd;
		const int r_buff_len = 1024;
		char * r_buff = new char [r_buff_len];
		
		xmlfd.open("demo.xml", std::ios::in|std::ios::binary);
		if (xmlfd.is_open()) {
			while (!xmlfd.eof()) {
				xmlfd.read(r_buff, r_buff_len);
				std::cout << "Read: " << xmlfd.gcount();
				std::cout << "\tC.Pos: " << xmlfd.tellg() << std::endl;

				if (send(sockfd, r_buff, (size_t) xmlfd.gcount(), 0) == -1)
					s_error("send");
			}
		}	
		else s_error("XML file");

		xmlfd.close();

		delete[] r_buff;
		close(sockfd);
	} else s_error("connect");

	return 0;
}
