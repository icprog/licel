#ifdef _WIN32
#include <winsock2.h>
#define SHUT_WR SD_SEND
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/timeb.h>
#define SOCKET int
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#endif
#include <stdio.h>

#ifdef _WIN32
#pragma comment(lib, "wsock32.lib")
#endif

/** Enter your values here*/
#define HOST "192.168.69.182"
#define PORTNUM 2055

/**
 * Millisecond delay
 * @param milliSecond  - time to wait
 * @return             - none
 */
void mDelay(int milliSecond);

/** 
 * Open a connection to a port on the Ethernet controller. If the call fails it
 * will return INVALID_SOCKET.
 * @param host - string with the remote controller IP address
 * @param port - the remote port to be opened.
 * @return     - the connection identifier
 */
SOCKET openConnection(const char* host, int port);

/** 
 * Open a connection to a port on the Ethernet controller. If the call fails it
 * will return INVALID_SOCKET.
 * @param host             - string with the remote controller IP address
 * @param port             - the remote port to be opened.
 * @param connectionPasswd - password for blowfish encryption.
 * @return     - the connection identifier
 */
SOCKET openSecureConnection(const char* host, int port, const char* connectionPasswd);

/** 
 * Close the  connection to the Ethernet controller
 * @param s - the connection identifier.
 * @return  - will return SOCKET_ERROR on failure, 0 otherwise.
 */
int closeConnection(SOCKET s);

/**
 * Write a command string to the Ethernet controller. 
 * Each command should be terminated by a CRLF
 * @param s       - the connection identifier
 * @param command - the command string
 */
int writeCommand(SOCKET s, const char* command);

/**
 * Read a ASCII response from the controller. Except for binary data transfer
 * the controller response is a short string indicating whether the action 
 * could be performed or not. This response is terminated by a CRLF. This 
 * routine will read till it encounters a CRLF or if the amount of chars would
 * exceed maxLength.
 * @param s                - the connection identifier
 * @param response         - buffer containing the command response
 * @param maxLength        - buffer capacity
 * @param nTimeOutMillisec - max. time to wait for a response
 * @return                 - number of received bytes
 */
int readResponse(SOCKET s, char* response, int maxLength, int nTimeOutMillisec);

/**
 * Read a binary response from the controller. 
 * @param s                - the connection identifier
 * @param array            - buffer containing binary data
 * @param points           - number of bytes to read
 * @param nTimeOutMillisec - max. time to wait for a response
 * @return                 - number of received bytes
 */
int ReadArray(SOCKET s, unsigned char *array, unsigned long points, int nTimeOutMillisec);
