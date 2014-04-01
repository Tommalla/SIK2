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

const char* usage_msg = "Wrong arguments! Correct usage is: mierniczys <port>";

int main(int argc, char** argv) {
	if (argc != 2)
		fatal(usage_msg);

	short int tcp_port;
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
	server_address.sin_port = htons(atoi(argv[1])); // listening on port from args

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
				int sflags;

				size_t len;
				ssize_t snd_len, rcva_len;
				struct sockaddr_in my_address;

				my_address.sin_family = AF_INET; // IPv4
				my_address.sin_addr.s_addr =
				client_address.sin_addr.s_addr; // address IP
				my_address.sin_port = htons((uint16_t) atoi(buffer));

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
