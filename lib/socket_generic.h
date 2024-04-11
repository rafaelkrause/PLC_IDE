
#ifndef _SOCKET_LIB_H_
#define _SOCKET_LIB_H_

//==============================================================================
//								HEADERS
//==============================================================================

#if !defined(__WINDOWS__) && (defined(WIN32) || defined(WIN64) || defined(_MSC_VER) || defined(_WIN32))
#define __WINDOWS__
#endif

#if __cplusplus
#define SOCKET_API extern "C" static
#else
#define SOCKET_API static
#endif


#ifdef __WINDOWS__
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601  /* Windows 7. */
#endif
#include <winsock2.h>
#include <Ws2tcpip.h>

#pragma comment(lib,"ws2_32.lib") /* Winsock Library */

#else
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>	/* for Blocking /no Blocking */
#include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
#include <unistd.h> /* Needed for close() */
#include <errno.h>  /* Get socket Erros Msgs */
#endif

//Common to all system
#include "os_generic.h"


//https://www.apriorit.com/dev-blog/221-crossplatform-linux-windows-sockets


//==============================================================================
//                       Structs,Types and Defins
//==============================================================================

#ifdef __WINDOWS__

typedef SOCKET socket_t;
typedef SOCKADDR_IN socket_addr_in_t;
typedef SOCKADDR socket_addr_t;
typedef WSAEVENT socket_event_t;

#define SOCKET_ERROR_EWOULDBLOCK	WSAEWOULDBLOCK

#else
typedef  int  						socket_t;
typedef struct sockaddr_in 			socket_addr_in_t;
typedef struct sockaddr				socket_addr_t;
#define SOCKET_ERROR_EWOULDBLOCK	EWOULDBLOCK
#endif

#define SOCKET_CON_TYPE_TCP			SOCK_STREAM
#define SOCKET_CON_TYPE_UDP			SOCK_DGRAM 

#define SOCKET_EVENT_TYPE_READ		FD_READ
#define SOCKET_EVENT_TYPE_WRITE		FD_WRITE
#define SOCKET_EVENT_TYPE_ACCEPT	FD_ACCEPT
#define SOCKET_EVENT_TYPE_CONNECT	FD_CONNECT
#define SOCKET_EVENT_TYPE_CLOSE		FD_CLOSE

#define SOCKET_ERROR_TIMEOUT		-10

//==============================================================================
//                       SOCKET FUNCTIONS FUNCTIONS                                
//==============================================================================

SOCKET_API int socket_init()
{
#ifdef __WINDOWS__
	WSADATA wsa_data;
	return (WSAStartup(MAKEWORD(1, 1), &wsa_data) == 0);
#else
	return 1;
#endif
}

SOCKET_API int socket_finish()
{
#ifdef __WINDOWS__
	return WSACleanup();
#else
	return 1;
#endif
}

SOCKET_API int socket_create(socket_t *s, int conntype)
{
#ifdef __WINDOWS__
	*s = socket(AF_INET, conntype, 0);
	return (*s == INVALID_SOCKET) ? 0 : 1;
#else
	*s = socket(AF_INET, conntype, 0);
	return (*s >= 0);
#endif
	return -1;
}

SOCKET_API int socket_set_non_blocking(socket_t s)
{
#ifdef __WINDOWS__
	unsigned long non_blocking = 1;
	return ioctlsocket(s, (long)FIONBIO, &non_blocking);
#else
	int flags = fcntl(s, F_GETFL, 0);
	if (flags == -1) return 0;
	flags = (flags | O_NONBLOCK);
	return (fcntl(s, F_SETFL, flags) == 0) ? 1 : 0;
#endif
}

SOCKET_API int socket_set_blocking(socket_t s)
{
#ifdef __WINDOWS__
	unsigned long non_blocking = 0;
	return ioctlsocket(s, (long)FIONBIO, &non_blocking);
#else
	int flags = fcntl(s, F_GETFL, 0);
	if (flags == -1) return 0;
	flags = (flags | ~O_NONBLOCK);
	return (fcntl(s, F_SETFL, flags) == 0) ? 1 : 0;
#endif
}

SOCKET_API int socket_set_nodelay(socket_t s)
{
#ifdef __WINDOWS__
	unsigned long non_delay = 1;
	return ioctlsocket(s, (long)TCP_NODELAY, &non_delay);
#else
	int i = 1;
	return setsockopt(s, IPPROTO_TCP, TCP_NODELAY, (void *)&i, sizeof(i));
#endif
}

SOCKET_API int socket_connect(socket_t s, const char* ip, int port)
{
#ifdef __WINDOWS__

	if (s == INVALID_SOCKET)
		return -2;

	if (port < 0)
		return -3;

	// Set Address and Port
	SOCKADDR_IN        addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	if (!ip || strlen(ip) <= 1)
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	else
		addr.sin_addr.s_addr = inet_addr(ip);

	//Connect Socket    
	return (connect(s, (struct sockaddr *) &addr, sizeof(addr)) != SOCKET_ERROR);

#else
	if (s < 0)
		return -2;

	if (port < 0)
		return -3;

	// Set Address and Port
	socket_addr_in_t addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	if (!ip || strlen(ip) <= 1)
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	else
		addr.sin_addr.s_addr = inet_addr(ip);

	//Connect Socket    
	return (connect(s, (struct sockaddr *) &addr, sizeof(addr)) >= 0);

#endif
	return -1;

}

SOCKET_API int socket_connect_timeout(socket_t s, const char* ip, int port, int timeout_ms)
{
#ifdef __WINDOWS__

	int res, error;

	struct timeval tm;
	tm.tv_sec = timeout_ms / 1000;
	tm.tv_usec = (timeout_ms % 1000) * 1000;


	if (s == INVALID_SOCKET)
		return -2;

	if (port < 0)
		return -3;

	// Set Address and Port
	SOCKADDR_IN        addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	if (!ip || strlen(ip) <= 1)
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	else
		addr.sin_addr.s_addr = inet_addr(ip);

	//Put Socket in no_blocking mode
	socket_set_non_blocking(s);

	//Connect Socket    
	res = connect(s, (struct sockaddr *) &addr, sizeof(addr));

	if (res <= SOCKET_ERROR)
	{
		error = WSAGetLastError();
		//check if error was WSAEWOULDBLOCK, where we'll wait
		if (error == SOCKET_ERROR_EWOULDBLOCK)
		{
			fd_set Write, Err;
			TIMEVAL Timeout;
			//int TimeoutSec = 10; // timeout after 10 seconds

			FD_ZERO(&Write);
			FD_ZERO(&Err);
			FD_SET(s, &Write);
			FD_SET(s, &Err);

			Timeout.tv_sec = timeout_ms / 1000;
			Timeout.tv_usec = (timeout_ms % 1000) * 1000;
			if (Timeout.tv_usec <= 0)
				Timeout.tv_usec = 100;

			res = select(0,			//ignored
				NULL,				//read
				&Write,				//Write Check
				&Err,				//Error Check
				&Timeout);
			if (res == 0)
			{
				res = SOCKET_ERROR_TIMEOUT; // TIMEOUT
			}
			else
			{
				if (FD_ISSET(s, &Write))
				{
					res = 1;
				}
				if (FD_ISSET(s, &Err))
				{
					res = -1;
				}
			}
		}
		else
		{
			res = -1;
		}
	}
	else
	{
		//connected without waiting (will never execute)
		res = -1;
	}

	//return to blocking mode
	socket_set_blocking(s);

	return res;

#else
	int res, error;

	struct timeval tm;
	tm.tv_sec = timeout_ms / 1000;
	tm.tv_usec = (timeout_ms % 1000) * 1000;


	if (s < 0)
		return -2;

	if (port < 0)
		return -3;

	// Set Address and Port
	socket_addr_in_t        addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	if (!ip || strlen(ip) <= 1)
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	else
		addr.sin_addr.s_addr = inet_addr(ip);

	//Put Socket in no_blocking mode
	socket_set_non_blocking(s);

	//Connect Socket    
	res = connect(s, (struct sockaddr *) &addr, sizeof(addr));

	if (res < 0)
	{
		error = errno;
		//check if error was WSAEWOULDBLOCK, where we'll wait
		if (error == SOCKET_ERROR_EWOULDBLOCK)
		{
			fd_set Write, Err;
			struct timeval Timeout;
			//int TimeoutSec = 10; // timeout after 10 seconds

			FD_ZERO(&Write);
			FD_ZERO(&Err);
			FD_SET(s, &Write);
			FD_SET(s, &Err);

			Timeout.tv_sec = timeout_ms / 1000;
			Timeout.tv_usec = (timeout_ms % 1000) * 1000;
			if (Timeout.tv_usec <= 0)
				Timeout.tv_usec = 100;

			res = select(0,			//ignored
				NULL,				//read
				&Write,				//Write Check
				&Err,				//Error Check
				&Timeout);
			if (res == 0)
			{
				res = SOCKET_ERROR_TIMEOUT; // TIMEOUT
			}
			else
			{
				if (FD_ISSET(s, &Write))
				{
					res = 1;
				}
				if (FD_ISSET(s, &Err))
				{
					res = -1;
				}
			}
		}
		else
		{
			res = -1;
		}
	}
	else
	{
		//connected without waiting (will never execute)
		res = -1;
	}

	//return to blocking mode
	socket_set_blocking(s);

	return res;

#endif
	return -1;

}

SOCKET_API int socket_bind(socket_t s, const char* ip, int port)
{
#ifdef __WINDOWS__

	if (s == INVALID_SOCKET)
		return -2;

	if (port < 0)
		return -3;

	// Set Address and Port
	SOCKADDR_IN        addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	if (!ip || strlen(ip) <= 1)
		addr.sin_addr.s_addr = INADDR_ANY;
	else
		addr.sin_addr.s_addr = inet_addr(ip);

	//Bind Socket    
	return (bind(s, (struct sockaddr *) &addr, sizeof(addr)) != SOCKET_ERROR);

#else
	if (s < 0)
		return -2;

	if (port < 0)
		return -3;

	// Set Address and Port
	socket_addr_in_t        addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	if (!ip || strlen(ip) <= 1)
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	else
		addr.sin_addr.s_addr = inet_addr(ip);

	//Bind Socket    
	return (bind(s, (struct sockaddr *) &addr, sizeof(addr)) >= 0);

#endif
	return -1;

}

SOCKET_API int socket_recv(socket_t s, char* data, int data_size)
{
#ifdef __WINDOWS__

	if (s == INVALID_SOCKET)
		return -2;

	if (!data)
		return -3;

	if (data_size <= 0)
		return -4;

	return recv(s, data, data_size, 0);

#else
	if (s < 0)
		return -2;

	if (!data)
		return -3;

	if (data_size <= 0)
		return -4;

	return recv(s, data, data_size, 0);

#endif
	return -1;
}

SOCKET_API int socket_recvfrom(socket_t s, char* data, int data_size, socket_addr_t *addr)
{
#ifdef __WINDOWS__

	if (s == INVALID_SOCKET)
		return -2;

	if (!data)
		return -3;

	if (data_size <= 0)
		return -4;
	int fromsize = sizeof(socket_addr_t);
	return recvfrom(s, data, data_size, 0, addr, &fromsize);

#else
	if (s < 0)
		return -2;

	if (!data)
		return -3;

	if (data_size <= 0)
		return -4;

	int fromsize = sizeof(socket_addr_t);
	return recvfrom(s, data, data_size, 0, addr, &fromsize);

#endif
	return -1;
}

SOCKET_API int socket_send(socket_t s, const char* data, int data_size)
{
#ifdef __WINDOWS__

	if (s == INVALID_SOCKET)
		return -2;

	if (!data)
		return -3;

	if (data_size <= 0)
		return -4;

	return send(s, data, data_size, 0);

#else
	if (s < 0)
		return -2;

	if (!data)
		return -3;

	if (data_size <= 0)
		return -4;

	return send(s, data, data_size, 0);

#endif
	return -1;
}

SOCKET_API int socket_send_timeout(socket_t s, const char* data, int data_size, int timeout_ms)
{
#ifdef __WINDOWS__

	if (s == INVALID_SOCKET)
		return -2;

	if (!data)
		return -3;

	if (data_size <= 0)
		return -4;

	int res, error;

	struct timeval tm;
	tm.tv_sec = timeout_ms / 1000;
	tm.tv_usec = (timeout_ms % 1000) * 1000;

	res = send(s, data, data_size, 0);

	if (res <= SOCKET_ERROR)
	{
		error = WSAGetLastError();
		//check if error was WSAEWOULDBLOCK, where we'll wait
		if (error == SOCKET_ERROR_EWOULDBLOCK)
		{
			fd_set Write, Err;
			TIMEVAL Timeout;

			FD_ZERO(&Write);
			FD_ZERO(&Err);
			FD_SET(s, &Write);
			FD_SET(s, &Err);

			Timeout.tv_sec = timeout_ms / 1000;
			Timeout.tv_usec = (timeout_ms % 1000) * 1000;
			res = select(0,			//ignored
				NULL,				//read
				&Write,				//Write Check
				&Err,				//Error Check
				&Timeout);

			if (res == 0)
			{
				res = SOCKET_ERROR_TIMEOUT; // TIMEOUT
			}
			else
			{
				if (FD_ISSET(s, &Write))
				{
					res = 1;
				}
				if (FD_ISSET(s, &Err))
				{
					res = -1;
				}
			}
		}
		else
		{
			res = -1;
		}
	}
	else
	{
		res = 1;
	}

	return res;

#else
	if (s < 0)
		return -2;

	if (!data)
		return -3;

	if (data_size <= 0)
		return -4;

	int res, error;

	struct timeval tm;
	tm.tv_sec = timeout_ms / 1000;
	tm.tv_usec = (timeout_ms % 1000) * 1000;

	res = send(s, data, data_size, 0);

	if (res < 0)
	{
		error = WSAGetLastError();
		//check if error was WSAEWOULDBLOCK, where we'll wait
		if (error == SOCKET_ERROR_EWOULDBLOCK)
		{
			fd_set Write, Err;
			struct timeval Timeout;

			FD_ZERO(&Write);
			FD_ZERO(&Err);
			FD_SET(s, &Write);
			FD_SET(s, &Err);

			Timeout.tv_sec = timeout_ms / 1000;
			Timeout.tv_usec = (timeout_ms % 1000) * 1000;
			res = select(0,			//ignored
				NULL,				//read
				&Write,				//Write Check
				&Err,				//Error Check
				&Timeout);

			if (res == 0)
			{
				res = SOCKET_ERROR_TIMEOUT; // TIMEOUT
			}
			else
			{
				if (FD_ISSET(s, &Write))
				{
					res = 1;
				}
				if (FD_ISSET(s, &Err))
				{
					res = -1;
				}
			}
		}
		else
		{
			res = -1;
		}
	}
	else
	{
		res = 1;
	}

	return res;

#endif
	return -1;
}

SOCKET_API int socket_sendto(socket_t s, const char* data, int data_size, socket_addr_t *addr)
{
#ifdef __WINDOWS__

	if (s == INVALID_SOCKET)
		return -2;

	if (!data)
		return -3;

	if (data_size <= 0)
		return -4;

	return sendto(s, data, data_size, 0, addr, sizeof(socket_addr_t));
#else
	if (s < 0)
		return -2;

	if (!data)
		return -3;

	if (data_size <= 0)
		return -4;
	
	return sendto(s, data, data_size, 0, addr, sizeof(socket_addr_t));

#endif
	return -1;
}

SOCKET_API int socket_close(socket_t s)
{
	int status = 0;

#ifdef __WINDOWS__
	status = shutdown(s, SD_BOTH);
	if (status == 0) { status = closesocket(s); }
#else
	status = shutdown(s, SHUT_RDWR);
	if (status == 0) { status = close(s); }
#endif

	return status;
}

SOCKET_API int socket_check_result(int result)
{

#ifdef __WINDOWS__

	//Got from: https://stackoverflow.com/questions/17824865/non-blocking-recv-returns-0-when-disconnected

	if (result > 0)
	{
		return 1;
	}
	else if (result == 0)
	{
		return 0;
	}
	else {
		// there is an error, let's see what it is
		int error = WSAGetLastError(); // or errornr on unix
		switch (error)
		{
		case WSAEALREADY:
		case WSAEINPROGRESS:
		case WSAEWOULDBLOCK:
			// Socket is O_NONBLOCK and there is no data available
			return 1;
			break;
		case EINTR:
			// an interrupt (signal) has been catched
			// should be ingore in most cases
			return 1;
			break;
		case WSAENETDOWN:
		case WSAENETUNREACH:
		case WSAENETRESET:
		case WSAECONNABORTED:
		case WSAECONNRESET:
		case WSAENOTCONN:
		case WSAESHUTDOWN:
			return 0;

		default:
			// socket has an error, no valid anymore
			return -1;
		}
	}

#else
	//Got from: https://stackoverflow.com/questions/17824865/non-blocking-recv-returns-0-when-disconnected

	if (result > 0)
	{
		return 1;
	}
	else if (result == 0)
	{
		return 0;
	}
	else {
		// there is an error, let's see what it is
		switch (errno)
		{
		case EALREADY:
		case EINPROGRESS:
		case EWOULDBLOCK:
			// Socket is O_NONBLOCK and there is no data available
			return 1;
			break;
		case EINTR:
			// an interrupt (signal) has been catched
			// should be ingore in most cases
			return 1;
			break;
		case ENETDOWN:
		case ENETUNREACH:
		case ENETRESET:
		case ECONNABORTED:
		case ECONNRESET:
		case ENOTCONN:
		case ESHUTDOWN:
			return 0;

		default:
			// socket has an error, no valid anymore
			return -1;
		}
	}

#endif

	return -1;
}

/*
SOCKET_API socket_event_t socket_create_event()
{
#ifdef __WINDOWS__
	return WSACreateEvent();
#else

#endif
}

SOCKET_API int socket_set_event(socket_t s, socket_event_t h, long CMDs)
{
#ifdef __WINDOWS__
	return WSAEventSelect(s,h, CMDs);
#else

#endif
}

SOCKET_API int socket_wait_event(socket_event_t h, int timeout_ms)
{
#ifdef __WINDOWS__
	int failed = WAIT_OBJECT_0 != WaitForSingleObject(h, timeout_ms < 0 ? INFINITE : timeout_ms);
	return !failed;
#else

#endif

}

SOCKET_API int socket_reset_event(socket_event_t h)
{
#ifdef __WINDOWS__
	return WSAResetEvent(h);
#else

#endif

}

SOCKET_API int socket_cancel_event(socket_t s, socket_event_t h)
{
#ifdef __WINDOWS__
	return WSAEventSelect(s, h, 0);
#else

#endif
}
*/

SOCKET_API void socket_get_error_msg(char *msg)
{
#ifdef __WINDOWS__
	wchar_t *s = NULL;
	FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&s, 0, NULL);
	sprintf(msg, "%s", s);
	LocalFree(s);
#else
	sprintf(msg, "%s", strerror(errno));
#endif

}

SOCKET_API int socket_get_error()
{
#ifdef __WINDOWS__
	return WSAGetLastError();
#else
	return errno;
#endif
	return 0;
}


#endif