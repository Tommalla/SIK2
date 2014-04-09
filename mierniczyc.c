/* Tomasz Zakrzewski, tz336079
 * SIK2, client
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "err.h"
#include "utils.h"

#define BUFFER_SIZE   1000
#define QUEUE_LENGTH     5

char buffer[BUFFER_SIZE];
const char* usage_error = "Wrong arguments! Correct usage is: mierniczyc <UDP port> <TCP port> <server address>";

int main(const int argc, char** argv) {
	int udp_sock, tcp_sock;
	unsigned short int udp_port, tcp_port;
	unsigned int res;
	int err;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;

	socklen_t rcv_len;
	ssize_t len;

	struct addrinfo addr_hints;
	struct addrinfo *addr_result;

	if (argc != 4)
		fatal(usage_error);

	//converting input
	udp_port = str_to_short(argv[1], usage_error);
	tcp_port = str_to_short(argv[2], usage_error);

	printf("Client started with port numbers: udp: %d, tcp: %d\n", udp_port, tcp_port);

	//opening udp port
	udp_sock = socket(AF_INET, SOCK_DGRAM, 0); // creating IPv4 UDP socket
	if (udp_sock < 0)
		syserr("UDP socket");

	server_address.sin_family = AF_INET; // IPv4
	server_address.sin_addr.s_addr = htonl(INADDR_ANY); // listening on all interfaces
	server_address.sin_port = htons(udp_port); // default port for receiving is PORT_NUM

	// bind the socket to a concrete address
	if (bind(udp_sock, (struct sockaddr *) &server_address, (socklen_t) sizeof(server_address)) < 0)
		syserr("UDP bind");

	// connecting to TCP server
	// 'converting' host/port in string to struct addrinfo
	memset(&addr_hints, 0, sizeof(struct addrinfo));
	addr_hints.ai_family = AF_INET; // IPv4
	addr_hints.ai_socktype = SOCK_STREAM;
	addr_hints.ai_protocol = IPPROTO_TCP;
	err = getaddrinfo(argv[3], argv[2], &addr_hints, &addr_result);
	if (err != 0)
		syserr("getaddrinfo: %s\n", gai_strerror(err));

	// initialize socket according to getaddrinfo results
	tcp_sock = socket(addr_result->ai_family, addr_result->ai_socktype, addr_result->ai_protocol);
	if (tcp_sock < 0)
		syserr("TCP socket");

	// connect socket to the server
	if (connect(tcp_sock, addr_result->ai_addr, addr_result->ai_addrlen) < 0)
		syserr("TCP connect");

	//send port number to server
	udp_port = htons(udp_port);
	if (write(tcp_sock, &udp_port, sizeof(udp_port)) != sizeof(udp_port))
		syserr("partial / failed TCP write");

	// read from stdin and write to TCP
	while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
		ssize_t len = strlen(buffer);
		if (write(tcp_sock, buffer, len) != len)
			syserr("partial / failed TCP write");
	}

	//close TCP connection
	close(tcp_sock);

	//get UDP data
	rcv_len = (ssize_t) sizeof(client_address);
	do {
		len = recvfrom(udp_sock, &res, sizeof(res), 0,
		       (struct sockaddr *) &client_address, &rcv_len);
		if (len < 0)
			syserr("error on datagram from server");
		res = ntohl(res);
	} while (ntohs(client_address.sin_addr.s_addr) != ntohs(((struct sockaddr_in*)addr_result->ai_addr)->sin_addr.s_addr));

	printf("%u\n", res);

	freeaddrinfo(addr_result);
	close(udp_sock);

	return 0;
}
