#include "stdafx.h"
#include "licel_tcpip.h"
#include "blowfish.h"
#ifdef _MSDOS
#include <dos.h>
#endif

void mDelay(int Milliseconds)
{
#ifdef __LINUX__
  usleep(Milliseconds*1000);
#else
#ifndef _MSDOS
  Sleep(Milliseconds);
#else
  delay(Milliseconds);
#endif
#endif
}
u_long LookupAddress(const char* pcHost)
{
  u_long nRemoteAddress = inet_addr(pcHost);
  if (INADDR_NONE == nRemoteAddress) {
    hostent* pHE = gethostbyname(pcHost);
    if (!pHE) {
      return INADDR_NONE;
    }
    nRemoteAddress = *((u_long*)pHE->h_addr_list[0]);
  }
  return nRemoteAddress;
}

SOCKET EstablishConnection(u_long nRemoteAddr, u_short nPort)
{
  SOCKET sd = socket(PF_INET, SOCK_STREAM, 0);
  if (INVALID_SOCKET != sd) {
    sockaddr_in sinRemote;
    sinRemote.sin_family = AF_INET;
#ifdef WIN32
    sinRemote.sin_addr.S_un.S_addr = nRemoteAddr;
#else
    sinRemote.sin_addr.s_addr = nRemoteAddr;
#endif
    sinRemote.sin_port = htons(nPort);
    sockaddr_in sinLocal;
    sinLocal.sin_family = AF_INET;
    sinLocal.sin_port = 0;
    if (connect(sd,(sockaddr*) &sinRemote, sizeof(sockaddr_in)) == SOCKET_ERROR) {
#ifdef WIN32
      fprintf(stderr, "connect failed: %u\n", GetLastError ());
      closesocket (sd);
#else
      fprintf(stderr, "connect failed: %u\n", errno);
      close(sd);
#endif
      return INVALID_SOCKET;
    }
  }
  //int sv=200000;
  //
  //setsockopt(sd,SOL_SOCKET,SO_RCVBUF,(const char *)&sv, sizeof(sv));
  return sd;
}

int ShutdownConnection(SOCKET sd)
{
  if (shutdown(sd, SHUT_WR) == SOCKET_ERROR) {
    return -1;
  }
#ifdef EMPTY_SOCKET
  char acReadBuffer[100];
  while (1) {
    int nNewBytes = recv(sd,acReadBuffer, 99,0);
    if (nNewBytes == SOCKET_ERROR) {
      return -1;
    }
    else if (nNewBytes) {
    }
    else {
      break;
    }
  }
#endif
#ifdef WIN32
  if (closesocket(sd) == SOCKET_ERROR) {
#else
  if (close(sd) == SOCKET_ERROR) {
#endif
    return -1;
  }
  return 0;
}

SOCKET openConnection(const char* host, int port)
{
#ifdef WIN32
  WSAData wsaData;
  int nCode;
  if ((nCode = WSAStartup(MAKEWORD(1,1), &wsaData)) != 0) {
    fprintf(stderr, "Network Error %d received\r\n", nCode);
    return 1;
  }
#endif
  u_long nRemoteAddress = LookupAddress(host);
  if (INADDR_NONE == nRemoteAddress) {
    fprintf(stderr, "host %s not found\r\n", host);
    return 1;
  }
  in_addr Address;
  memcpy(&Address, &nRemoteAddress, sizeof(u_long));
  return EstablishConnection(nRemoteAddress, port);
}

SOCKET openSecureConnection(const char* host, int port, const char* connectionPasswd)
{
  SOCKET sd = openConnection(host, port);
  if (sd != INVALID_SOCKET) {
    char buffer[255];
    readResponse(sd, buffer, 255, 800);
    unsigned long xl, xr;
    sscanf(buffer,"%08lX%08lX", &xl, &xr);
    InitializeBlowfish(connectionPasswd, strlen(connectionPasswd));
    Blowfish_encipher(&xl, &xr);
    sprintf(buffer, "LOGON \"%08lX%08lX\"\r\n", xl, xr);
    writeCommand(sd, buffer);
  }
  return sd;
}

int closeConnection(SOCKET s)
{
  int rv = ShutdownConnection(s);
#ifdef __WIN32__
  WSACleanup();
#endif
  return rv;
}

int writeCommand(SOCKET s, const char* command)
{
  if (!command)
    return -1;
  return send(s, command, strlen(command), 0);
}

int readResponse(SOCKET s, char* response, int maxLength, int nTimeOutMilliSec)
{
 #ifdef WIN32
  int receivedBytes = 0;
  if (!response)
    return -1;
  maxLength--; // make place for a terminating zero in string
  int requestLength = 2; // get just CRLF
  int elapsedTime = 0;
  DWORD start = GetTickCount();
  while (receivedBytes < maxLength ) {
   // && nTimeOutMilliSec > elapsedTime) {
    int newBytes = recv(s, response + receivedBytes, requestLength, 0);
    if (SOCKET_ERROR == newBytes) {
      return -1;
    }
    else if (!newBytes) {
      fprintf(stderr, "connection closed by peer\n");
      return 0;
    }
    receivedBytes += newBytes;
    if (receivedBytes < maxLength) {
      response[receivedBytes] = 0;
      if (strstr(response,"\r\n")) {
        if(receivedBytes > 2)
          response[receivedBytes -2] = 0;
        return receivedBytes;
      }
      if (strstr(response,"\r")) {
        requestLength = 1; // wait for the remaining byte
      }
      elapsedTime = GetTickCount() - start;
    }

  }
  return receivedBytes;
#else
  int receivedBytes = 0;
  struct timeval seltime;
  fd_set selset;
  if (!response || (maxLength <= 0) || s == INVALID_SOCKET)
    return -1;
  memset(response, 0, maxLength);
  int timeoutUSec =  nTimeOutMilliSec * 1000;
  maxLength--; // make place for a terminating zero in string
  int requestLength = 2; // get just CRLF
  int elapsedTime = 0;
  int rv;
  while (receivedBytes < maxLength && timeoutUSec > elapsedTime)
  {
    FD_ZERO(&selset);
    FD_SET(s, &selset);
    seltime.tv_sec = 0;
    seltime.tv_usec = 1000;
    rv = select(s+1, &selset, NULL, NULL, &seltime);
    switch (rv) {
      case -1:            /* error occured! */
        return errno;
      case 0:             /* timeout; retry */
        elapsedTime += 1000;
        continue;
      default:            /* ready to read */
        break;
    }
    if (!FD_ISSET(s, &selset))
    {
      continue;           /* not ready to read; retry */
    }
    int newBytes = read(s, response + receivedBytes, requestLength);
    if (newBytes < 0) {
      return -1;
    }
    else if (!newBytes) {
      fprintf(stderr, "connection closed by peer\n");
      return 0;
    }
    receivedBytes += newBytes;
    if (receivedBytes < maxLength) {
      response[receivedBytes] = 0;
      if (strstr(response,"\r\n")) {
        if(receivedBytes > 2)
          response[receivedBytes -2] = 0;
        return receivedBytes;
      }
      if (strstr(response,"\r")) {
        requestLength = 1; // wait for the remaining byte
      }
    }
  }
  return -1;
#endif
}
int ReadArray(SOCKET s, unsigned char * array, unsigned long points, int nTimeOutMillisec)
{
  unsigned long receivedBytes = 0;
  while (receivedBytes < points ) {
    int newBytes = recv(s, (char* ) array + receivedBytes, points - receivedBytes, 0);
    if (SOCKET_ERROR == newBytes) {
      return -1;
    }
    else if (!newBytes) {
      fprintf(stderr,"connection closed by peer\n");
      return 0;
    }
    receivedBytes += newBytes;
  }
  return 0;
}
