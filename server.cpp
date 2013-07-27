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
	int acceptfd;
	int forceRebind = 1;
	struct sockaddr_in server;
	struct sockaddr_in client;
	socklen_t clientaddrlen = 0;
	const size_t bufflen = 4096;
	ssize_t bytesreceived = 0;
	char buff[bufflen];
	const char * reply = "I want proof I can do more...";

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		perror("socket");
		return 1;
	}

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &forceRebind, sizeof(int)) == -1) {
		perror("setsockopt");
		return 1;
	}

	memset((void *) &server, 0, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(1234);
	server.sin_addr.s_addr = INADDR_ANY;

	if (bind(sockfd, (const struct sockaddr *) &server, sizeof(struct sockaddr_in)) == -1) {
		perror("bind");
		return 1;
	}

	if (listen(sockfd, 0) == -1) {
		perror("listen");
		return 1;
	}

	acceptfd = accept(sockfd, (struct sockaddr *) &client, &clientaddrlen);
	if (acceptfd == -1) {
		perror("accept");
		return 1;
	}

	bytesreceived = recv(acceptfd, buff, bufflen, 0);
	if (bytesreceived == -1) {
		perror("recv");
		return 1;
	}

	printf("%s\n", buff);

	if (send(acceptfd, reply, strlen(reply)+1, 0) == -1) {
		perror("send parent");
		return 1;
	}

	if (close(acceptfd) == -1) {
		perror("close(acceptfd)");
		return 1;
	}

	if (close(sockfd) == -1) {
		perror("close(sockfd)");
		return 1;
	}
	return 0;
}
