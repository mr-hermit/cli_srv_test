#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <netdb.h>

int main() {
	int sockfd;
	struct sockaddr_in server;
	struct addrinfo *serverinfo;
	const size_t bufflen = 4096;
	ssize_t bytesreceived = 0;
	char buff[bufflen];
	const char * question = "Why r u doing this?";

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		perror("socket");
		return 1;
	}
	
	if (getaddrinfo("localhost", NULL, NULL, &serverinfo) != 0) {
		perror("getaddrinfo");
		return 1;
	}

	memcpy(&server, serverinfo->ai_addr, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(1234);
	freeaddrinfo(serverinfo);

	if (connect(sockfd, (const struct sockaddr *) &server, sizeof(struct sockaddr_in)) == -1) {
		perror("connect");
		return 1;
	}

	if (send(sockfd, question, (size_t) strlen(question)+1, 0) == -1) {
		perror("send");
		return 1;
	}

	bytesreceived = recv(sockfd, buff, bufflen, 0);
	if (bytesreceived == -1) {
		perror("recv");
		return 1;
	}
	printf("%s\n", buff);

	close(sockfd);
	return 0;
}
