/* Tomasz Zakrzewski, tz336079
 *  SIK2, server
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "err.h"

#define BUFFER_SIZE   1000
#define QUEUE_LENGTH     5
#define PORT_NUM     1625

int main() {
	int tcp_sock, msg_sock;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	socklen_t client_address_len;

	char buffer[BUFFER_SIZE];
	ssize_t tcp_rcv_len;

	tcp_sock = socket(PF_INET, SOCK_STREAM, 0); // creating IPv4 TCP socket
	if (tcp_sock <0)
		syserr("socket");

	server_address.sin_family = AF_INET; // IPv4
	server_address.sin_addr.s_addr = htonl(INADDR_ANY); // listening on all interfaces
	server_address.sin_port = htons(PORT_NUM); // listening on port PORT_NUM

	// bind the socket to a concrete address
	if (bind(tcp_sock, (struct sockaddr *) &server_address, sizeof(server_address)) < 0)
		syserr("bind");

	// switch to listening (passive open)
	if (listen(tcp_sock, QUEUE_LENGTH) < 0)
		syserr("listen");

	printf("accepting client connections on port %hu\n", ntohs(server_address.sin_port));

	for (;;) {
		client_address_len = sizeof(client_address);
		// get client connection from the socket
		msg_sock = accept(tcp_sock, (struct sockaddr *) &client_address, &client_address_len);
		if (msg_sock < 0)
			syserr("accept");
		do {
			tcp_rcv_len = read(msg_sock, buffer, sizeof(buffer));
			if (tcp_rcv_len < 0)
				syserr("reading from client socket");
			else if (tcp_rcv_len > 0) {
				printf("read from socket: %zd bytes: %.*s\n", tcp_rcv_len, (int) tcp_rcv_len, buffer);
				int sock;
				struct addrinfo addr_hints;
				struct addrinfo *addr_result;

				int i, flags, sflags;
				size_t len;
				ssize_t snd_len, rcva_len;
				struct sockaddr_in my_address;
				struct sockaddr_in srvr_address;

				// 'converting' host/port in string to struct addrinfo
				(void) memset(&addr_hints, 0, sizeof(struct addrinfo));
				addr_hints.ai_family = AF_INET; // IPv4
				addr_hints.ai_socktype = SOCK_DGRAM;
				addr_hints.ai_protocol = IPPROTO_UDP;
				addr_hints.ai_flags = 0;
				addr_hints.ai_addrlen = 0;
				addr_hints.ai_addr = NULL;
				addr_hints.ai_canonname = NULL;
				addr_hints.ai_next = NULL;
				if (getaddrinfo("localhost", NULL, &addr_hints, &addr_result) != 0) {
					syserr("getaddrinfo");
				}

				my_address.sin_family = AF_INET; // IPv4
				my_address.sin_addr.s_addr =
				((struct sockaddr_in*) (addr_result->ai_addr))->sin_addr.s_addr; // address IP
				my_address.sin_port = htons((uint16_t) atoi(buffer));

				freeaddrinfo(addr_result);

				sock = socket(PF_INET, SOCK_DGRAM, 0);
				if (sock < 0)
					syserr("socket");

				const char* msg = "dupa";
				len = strlen(msg);
				(void) printf("sending to socket: %s\n", msg);
				sflags = 0;
				rcva_len = (socklen_t) sizeof(my_address);
				snd_len = sendto(sock, msg, len, sflags,
						 (struct sockaddr *) &my_address, rcva_len);
				if (snd_len != (ssize_t) len) {
					syserr("partial / failed write");
				}

				if (close(sock) == -1) { //very rare errors can occur here, but then
					syserr("close"); //it's healthy to do the check
				};

			}
		} while (tcp_rcv_len > 0);
		printf("ending connection\n");
		if (close(msg_sock) < 0)
			syserr("close");
	}
	close(tcp_sock);
	return 0;
}
