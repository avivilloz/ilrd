/*****************************************************************************
Title: shared 
Author: aviv illoz
Description: shared api
******************************************************************************/

#include <string.h> /* strlen */
#include <arpa/inet.h> /* inet_ntop */

#include "shared.h" /* api */

/*****************************************************************************/

typedef int (*action_t)(int, const addrinfo_t*);

static addrinfo_t SetHints(int socktype, int is_server);
static int Connect(int sockfd, const addrinfo_t *curr);
static int Bind(int sockfd, const addrinfo_t *curr);

/*****************************************************************************/

int GetSocketFd(
	const char *host, 
	const char *service, 
	int socktype, 
	sockaddr_t *_buff
)
{
	int status = 0;
	int sockfd = 0;
	int is_server = 0;
	addrinfo_t hints;
	addrinfo_t *servinfo = NULL;
	addrinfo_t *curr = NULL;
	action_t actions[2] = {&Connect, &Bind};

	is_server = (NULL == host);

	hints = SetHints(socktype, is_server);

	status = getaddrinfo(host, service, &hints, &servinfo);
	MSG_IF_BAD((0 != status), gai_strerror(status), return -1);

	for (curr = servinfo; NULL != curr; curr = curr->ai_next)
	{
		sockfd = socket(curr->ai_family, curr->ai_socktype, curr->ai_protocol);
		PERROR_IF_BAD((-1 == sockfd), "socket", continue);

		status = actions[is_server](sockfd, curr);
		CHECK_IF_BAD((-1 == status), continue);

		break;
	}

	MSG_IF_BAD((NULL == curr), "failed to bind/connect", return -1);

	if (NULL != _buff)
	{
		memcpy(_buff, curr->ai_addr, sizeof(sockaddr_t));
	}

	freeaddrinfo(servinfo);

	return sockfd;
}

int SetOptions(int sockfd, int optname)
{
	int status = 0;
	int enable = 1;

	status = setsockopt(sockfd, SOL_SOCKET, optname, &enable, sizeof(enable));
	PERROR_IF_BAD((-1 == status), "setsockopt", return -1);

	return 0;
}

/*****************************************************************************/

static addrinfo_t SetHints(int socktype, int is_server)
{
	addrinfo_t hints;

	memset(&hints, 0, sizeof(addrinfo_t));
	if (is_server)
	{
		hints.ai_flags = AI_PASSIVE;
	}

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = socktype;

	return hints;
}

static int Connect(int sockfd, const addrinfo_t *curr)
{
	if (SOCK_STREAM == curr->ai_socktype)
	{
		int status = connect(sockfd, curr->ai_addr, curr->ai_addrlen);
		PERROR_IF_BAD((-1 == status), "connect", return -1);
	}

	return 0;
}

static int Bind(int sockfd, const addrinfo_t *curr)
{
	int status = bind(sockfd, curr->ai_addr, curr->ai_addrlen);
	PERROR_IF_BAD((-1 == status), "bind", return -1);

	return 0;
}

/*****************************************************************************/
