#ifndef _MYLSP_H
#define _MYLSP_H

#ifndef _UNICODE
#define _UNICODE
#endif

#ifndef UNICODE
#define UNICODE
#endif

#include <Winsock2.h>
#include <Ws2spi.h>
#include <Windows.h>
#include <tchar.h>
#include "Debug.h"
#include <stdio.h>
#pragma comment(lib, "Ws2_32.lib")

#define IP_MAX_LENGTH 30


static WSPUPCALLTABLE g_pUpCallTable;
static WSPPROC_TABLE g_NextProcTable;
static TCHAR g_szCurrentApp[MAX_PATH];

static char ChangedIP[IP_MAX_LENGTH] = "23.88.59.59";
static char OrigIP[IP_MAX_LENGTH] = "";
static char NowIP[IP_MAX_LENGTH];
static unsigned short ChangedPort = 80, origPort = 80, nowPort = 0;
static char lanjie = 0;


DWORD GetInformation();
SOCKET
WSPAPI  WSPAccept(
	SOCKET s,
	struct sockaddr FAR * addr,
	LPINT addrlen,
	LPCONDITIONPROC lpfnCondition,
	DWORD_PTR dwCallbackData,
	LPINT lpErrno );
INT
WSPAPI WSPAddressToString(
						  LPSOCKADDR lpsaAddress,
						  DWORD dwAddressLength,
						  LPWSAPROTOCOL_INFOW lpProtocolInfo,
						  LPWSTR lpszAddressString,
						  LPDWORD lpdwAddressStringLength,
						  LPINT lpErrno
						  );
int
WSPAPI WSPAsyncSelect(
					  SOCKET s,
					  HWND hWnd,
					  unsigned int wMsg,
					  long lEvent,
					  LPINT lpErrno
					  );
int
WSPAPI WSPBind(
			   SOCKET s,
			   const struct sockaddr FAR * name,
			   int namelen,
			   LPINT lpErrno
			   );
int
WSPAPI WSPCancelBlockingCall(
							 LPINT lpErrno
							 );
int
WSPAPI WSPCleanup(
				  LPINT lpErrno
				  );
int
WSPAPI WSPCloseSocket(
					  SOCKET s,
					  LPINT lpErrno
					  );
int
WSPAPI WSPConnect(
				  SOCKET s,
				  const struct sockaddr FAR * name,
				  int namelen,
				  LPWSABUF lpCallerData,
				  LPWSABUF lpCalleeData,
				  LPQOS lpSQOS,
				  LPQOS lpGQOS,
				  LPINT lpErrno
				  );
int
WSPAPI WSPDuplicateSocket(
						  SOCKET s,
						  DWORD dwProcessId,
						  LPWSAPROTOCOL_INFOW lpProtocolInfo,
						  LPINT lpErrno
						  );
int
WSPAPI WSPEnumNetworkEvents(
							SOCKET s,
							WSAEVENT hEventObject,
							LPWSANETWORKEVENTS lpNetworkEvents,
							LPINT lpErrno
							);
int
WSPAPI WSPEventSelect(
					  SOCKET s,
					  WSAEVENT hEventObject,
					  long lNetworkEvents,
					  LPINT lpErrno
					  );
BOOL
WSPAPI WSPGetOverlappedResult(
							  SOCKET s,
							  LPWSAOVERLAPPED lpOverlapped,
							  LPDWORD lpcbTransfer,
							  BOOL fWait,
							  LPDWORD lpdwFlags,
							  LPINT lpErrno
							  );
int
WSPAPI WSPGetPeerName(
	SOCKET s,
	struct sockaddr FAR * name,
	LPINT namelen,
	LPINT lpErrno
	);
int
WSPAPI WSPGetSockName(
	SOCKET s,
	struct sockaddr FAR * name,
	LPINT namelen,
	LPINT lpErrno
	);
int
WSPAPI WSPGetSockOpt(
					 SOCKET s,
					 int level,
					 int optname,
					 char FAR * optval,
					 LPINT optlen,
					 LPINT lpErrno
					 );
BOOL
WSPAPI WSPGetQOSByName(
					   SOCKET s,
					   LPWSABUF lpQOSName,
					   LPQOS lpQOS,
					   LPINT lpErrno
					   );
int
WSPAPI WSPIoctl(
				SOCKET s,
				DWORD dwIoControlCode,
				LPVOID lpvInBuffer,
				DWORD cbInBuffer,
				LPVOID lpvOutBuffer,
				DWORD cbOutBuffer,
				LPDWORD lpcbBytesReturned,
				LPWSAOVERLAPPED lpOverlapped,
				LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
				LPWSATHREADID lpThreadId,
				LPINT lpErrno
				);
SOCKET
WSPAPI WSPJoinLeaf(
				   SOCKET s,
				   const struct sockaddr FAR * name,
				   int namelen,
				   LPWSABUF lpCallerData,
				   LPWSABUF lpCalleeData,
				   LPQOS lpSQOS,
				   LPQOS lpGQOS,
				   DWORD dwFlags,
				   LPINT lpErrno
				   );
int
WSPAPI WSPListen(
				 SOCKET s,
				 int backlog,
				 LPINT lpErrno
				 );
int
WSPAPI WSPRecv(
			   SOCKET s,
			   LPWSABUF lpBuffers,
			   DWORD dwBufferCount,
			   LPDWORD lpNumberOfBytesRecvd,
			   LPDWORD lpFlags,
			   LPWSAOVERLAPPED lpOverlapped,
			   LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
			   LPWSATHREADID lpThreadId,
			   LPINT lpErrno
			   );
int
WSPAPI WSPRecvDisconnect(
						 SOCKET s,
						 LPWSABUF lpInboundDisconnectData,
						 LPINT lpErrno
						 );
int
WSPAPI WSPRecvFrom(
	SOCKET s,
	LPWSABUF lpBuffers,
	DWORD dwBufferCount,
	LPDWORD lpNumberOfBytesRecvd,
	LPDWORD lpFlags,
	struct sockaddr FAR * lpFrom,
	LPINT lpFromlen,
	LPWSAOVERLAPPED lpOverlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
	LPWSATHREADID lpThreadId,
	LPINT lpErrno
	);
int
WSPAPI WSPSelect(
				 int nfds,
				 fd_set FAR * readfds,
				 fd_set FAR * writefds,
				 fd_set FAR * exceptfds,
				 const struct timeval FAR * timeout,
				 LPINT lpErrno
				 );
int
WSPAPI WSPSend(
			   SOCKET s,
			   LPWSABUF lpBuffers,
			   DWORD dwBufferCount,
			   LPDWORD lpNumberOfBytesSent,
			   DWORD dwFlags,
			   LPWSAOVERLAPPED lpOverlapped,
			   LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
			   LPWSATHREADID lpThreadId,
			   LPINT lpErrno
			   );
int
WSPAPI WSPSendDisconnect(
						 SOCKET s,
						 LPWSABUF lpOutboundDisconnectData,
						 LPINT lpErrno
						 );
int WSPAPI WSPSendTo(
		SOCKET    s,
		LPWSABUF   lpBuffers,
		DWORD    dwBufferCount,
		LPDWORD    lpNumberOfBytesSent,
		DWORD    dwFlags,
		const struct sockaddr FAR * lpTo,
		int     iTolen,
		LPWSAOVERLAPPED lpOverlapped,
		LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
		LPWSATHREADID lpThreadId,
		LPINT    lpErrno );
int
WSPAPI WSPSetSockOpt(
					 SOCKET s,
					 int level,
					 int optname,
					 const char FAR * optval,
					 int optlen,
					 LPINT lpErrno
					 );
int
WSPAPI WSPShutdown(
				   SOCKET s,
				   int how,
				   LPINT lpErrno
				   );
SOCKET
WSPAPI WSPSocket(
	int af,
	int type,
	int protocol,
	LPWSAPROTOCOL_INFOW lpProtocolInfo,
	GROUP g,
	DWORD dwFlags,
	LPINT lpErrno
);
INT
WSPAPI WSPStringToAddress(
	LPWSTR AddressString,
	INT AddressFamily,
	LPWSAPROTOCOL_INFOW lpProtocolInfo,
	PSOCKADDR lpAddress,
	LPINT lpAddressLength,
	LPINT lpErrno
);


#endif
