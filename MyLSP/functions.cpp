#include "debug.h"
#include "MyLSP.h"

DWORD GetInformation()
{
	HKEY hKey = 0;
	DWORD Count = 0, keyType = REG_SZ, i, j, k;
	DWORD len = sizeof(DWORD);
	char lpName[30], lpValue[30];
	DWORD lpNameLen, lpValueLen;

	char ipName[30], ipValue[30];
	unsigned short portName, portValue;

	lpNameLen = sizeof(lpName);
	lpValueLen = sizeof(lpValue);

	lanjie = 0;

	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\LSPManager",0, KEY_ALL_ACCESS, &hKey)!= ERROR_SUCCESS)
	{
		return 0;
	}
	RegQueryValueExA(hKey, "NumberOfRules", 0, &keyType, (BYTE*)&Count, &len);
	if (Count == 0)
	{
		RegCloseKey(hKey);
		return 0;
	}
	for (i=0; i<=Count; i++)
	{
		memset(lpName, 0, sizeof(lpName));
		memset(lpValue,0, sizeof(lpValue));
		memset(ipName,0, sizeof(ipName));
		memset(ipValue,0, sizeof(ipValue));
		lpNameLen = sizeof(lpName);
		lpValueLen = sizeof(lpValue);
		portName = portValue = 0;
		RegEnumValueA(hKey, i, lpName, &lpNameLen, NULL, &keyType, (unsigned char*)lpValue, &lpValueLen);
		if (strcmp(lpName, "NumberOfRules") == 0) continue;
		k = 0;
		for (j=0; lpName[j]!=':'; j++)
		{
			ipName[k++] = lpName[j];
		}
		j++;
		k=0;
		for (j; j<strlen(lpName); j++)
		{
			if (lpName[j] == '*')
			{
				portName = 0;
				break;
			}
			portName = portName*10 + lpName[j] - '0';
		}

		k = 0;
		for (j=0; lpValue[j]!=':'; j++)
		{
			ipValue[k++] = lpValue[j];
		}
		j++;
		k=0;
		for (j; j<strlen(lpValue); j++)
		{
			portValue = portValue*10 + lpValue[j] - '0';
		}
		if (strlen(ipName) <=5 )
		{
			if (portName == 0 ||  nowPort == portName)
			{
				lanjie = 1;
				ChangedPort = portValue;
				strcpy_s(ChangedIP, ipValue);
			} else
			{
				lanjie = 0;
			}
		} else
		{
			if (strcmp(ipName, NowIP) == 0)
			{
				if (portName == 0 ||  nowPort == portName)
				{
					lanjie = 1;
					ChangedPort = portValue;
					strcpy_s(ChangedIP, ipValue);
				} else
				{
					lanjie = 0;
				}
			} else
			{
				lanjie = 0;
			}
		}
		if (lanjie) break;
	}
	RegCloseKey(hKey);
	return 0;
}


SOCKET
WSPAPI  WSPAccept(
	SOCKET s,
	struct sockaddr FAR * addr,
	LPINT addrlen,
	LPCONDITIONPROC lpfnCondition,
	DWORD_PTR dwCallbackData,
	LPINT lpErrno )
{
	sockaddr_in *ConnectAddress = (sockaddr_in*)addr;
	ODS(L"WSPAccept called! ");
	return g_NextProcTable.lpWSPAccept(s, addr, addrlen, lpfnCondition, dwCallbackData, lpErrno);
}


INT
WSPAPI WSPAddressToString(
						  LPSOCKADDR lpsaAddress,
						  DWORD dwAddressLength,
						  LPWSAPROTOCOL_INFOW lpProtocolInfo,
						  LPWSTR lpszAddressString,
						  LPDWORD lpdwAddressStringLength,
						  LPINT lpErrno
						  )
{
	ODS(L"AddressToString called! ");
	return g_NextProcTable.lpWSPAddressToString(lpsaAddress,
		dwAddressLength,
		lpProtocolInfo,
		lpszAddressString,
		lpdwAddressStringLength,
		lpErrno);
}

int
WSPAPI WSPAsyncSelect(
					  SOCKET s,
					  HWND hWnd,
					  unsigned int wMsg,
					  long lEvent,
					  LPINT lpErrno
					  )
{
	ODS(L"WSPAsyncSelect called! ");
	return g_NextProcTable.lpWSPAsyncSelect(
		s,
		hWnd,
		wMsg,
		lEvent,
		lpErrno);
}


int
WSPAPI WSPBind(
			   SOCKET s,
			   const struct sockaddr FAR * name,
			   int namelen,
			   LPINT lpErrno
			   )
{
	ODS(L"WSPBind called! ");
	sockaddr_in *ConnectAddress = (sockaddr_in*)name;
	return g_NextProcTable.lpWSPBind(s, name, namelen, lpErrno);
}

int
WSPAPI WSPCancelBlockingCall(
							 LPINT lpErrno
							 )
{
	ODS(L"WSPCancelBlockingCall called! ");
	return g_NextProcTable.lpWSPCancelBlockingCall(
		lpErrno);
}


int
WSPAPI WSPCleanup(
				  LPINT lpErrno
				  )
{
	ODS(L"WSPCleanup called! ");
	return g_NextProcTable.lpWSPCleanup(
		lpErrno);
}


int
WSPAPI WSPCloseSocket(
					  SOCKET s,
					  LPINT lpErrno
					  )
{
	ODS(L"CloseSocket called! ");
	return g_NextProcTable.lpWSPCloseSocket(s, lpErrno);
}


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
				  )
{
	WCHAR temp[1024];
	sockaddr_in *ConnectAddress = (sockaddr_in*)name;
	ODS(L"WSPConnect called! ");
	_stprintf_s(temp, L"Connect to  %s:%d\n", inet_ntoa(ConnectAddress->sin_addr), ntohs(ConnectAddress->sin_port));
	ODS(temp);

	strcpy_s(NowIP,inet_ntoa(ConnectAddress->sin_addr));
	nowPort = ntohs(ConnectAddress->sin_port);
	GetInformation();

	ODS(L"After GetInformation!\n")
	if (lanjie == 0)
	{
		return g_NextProcTable.lpWSPConnect(s,
			name,
			namelen,
			lpCallerData,
			lpCalleeData,
			lpSQOS,
			lpGQOS,
			lpErrno);
	}
	ODS(L"lanjie begin");
	((sockaddr_in*)name)->sin_addr.S_un.S_addr = inet_addr(ChangedIP);
	((sockaddr_in*)name)->sin_port = htons(ChangedPort);
	_stprintf_s(temp, L"After change! Connect to  %s:%d\n", inet_ntoa(((sockaddr_in*)name)->sin_addr), ntohs(ConnectAddress->sin_port));
	ODS(temp);
	return g_NextProcTable.lpWSPConnect(s,
		name,
		namelen,
		lpCallerData,
		lpCalleeData,
		lpSQOS,
		lpGQOS,
		lpErrno);
}


int
WSPAPI WSPDuplicateSocket(
						  SOCKET s,
						  DWORD dwProcessId,
						  LPWSAPROTOCOL_INFOW lpProtocolInfo,
						  LPINT lpErrno
						  )
{
	ODS(L"DuplicateSocket called! ");
	return g_NextProcTable.lpWSPDuplicateSocket(
		s,
		dwProcessId,
		lpProtocolInfo,
		lpErrno);
}


int
WSPAPI WSPEnumNetworkEvents(
							SOCKET s,
							WSAEVENT hEventObject,
							LPWSANETWORKEVENTS lpNetworkEvents,
							LPINT lpErrno
							)
{
	return g_NextProcTable.lpWSPEnumNetworkEvents(
		s,
		hEventObject,
		lpNetworkEvents,
		lpErrno);
}


int
WSPAPI WSPEventSelect(
					  SOCKET s,
					  WSAEVENT hEventObject,
					  long lNetworkEvents,
					  LPINT lpErrno
					  )
{
	ODS(L"EventSelect called! ");
	return g_NextProcTable.lpWSPEventSelect(
		s,
		hEventObject,
		lNetworkEvents,
		lpErrno);
}


BOOL
WSPAPI WSPGetOverlappedResult(
							  SOCKET s,
							  LPWSAOVERLAPPED lpOverlapped,
							  LPDWORD lpcbTransfer,
							  BOOL fWait,
							  LPDWORD lpdwFlags,
							  LPINT lpErrno
							  )
{
	return g_NextProcTable.lpWSPGetOverlappedResult(
		s,
		lpOverlapped,
		lpcbTransfer,
		fWait,
		lpdwFlags,
		lpErrno);
}

int
WSPAPI WSPGetPeerName(
					  SOCKET s,
struct sockaddr FAR * name,
	LPINT namelen,
	LPINT lpErrno
	)
{
	return g_NextProcTable.lpWSPGetPeerName(
		s,
		name,
		namelen,
		lpErrno);
}


int
WSPAPI WSPGetSockName(
					  SOCKET s,
struct sockaddr FAR * name,
	LPINT namelen,
	LPINT lpErrno
	)
{
	return g_NextProcTable.lpWSPGetSockName(
		s,
		name,
		namelen,
		lpErrno);
}


int
WSPAPI WSPGetSockOpt(
					 SOCKET s,
					 int level,
					 int optname,
					 char FAR * optval,
					 LPINT optlen,
					 LPINT lpErrno
					 )
{
	return g_NextProcTable.lpWSPGetSockOpt(
		s,
		level,
		optname,
		optval,
		optlen,
		lpErrno
		);
}


BOOL
WSPAPI WSPGetQOSByName(
					   SOCKET s,
					   LPWSABUF lpQOSName,
					   LPQOS lpQOS,
					   LPINT lpErrno
					   )
{
	return g_NextProcTable.lpWSPGetQOSByName(
		s,
		lpQOSName,
		lpQOS,
		lpErrno);
}


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
				)
{
	ODS(L"WSPIoctl Called!\n");
	return g_NextProcTable.lpWSPIoctl(
		s,
		dwIoControlCode,
		lpvInBuffer,
		cbInBuffer,
		lpvOutBuffer,
		cbOutBuffer,
		lpcbBytesReturned,
		lpOverlapped,
		lpCompletionRoutine,
		lpThreadId,
		lpErrno);
}


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
				   )
{
	ODS(L"WSPJoinLeaf CALLED\n");
	return g_NextProcTable.lpWSPJoinLeaf(
		s,
		name,
		namelen,
		lpCallerData,
		lpCalleeData,
		lpSQOS,
		lpGQOS,
		dwFlags,
		lpErrno);
}


int
WSPAPI WSPListen(
				 SOCKET s,
				 int backlog,
				 LPINT lpErrno
				 )
{
	ODS(L"WSPListen called!\n");
	return g_NextProcTable.lpWSPListen(
		s,
		backlog,
		lpErrno);
}


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
			   )
{
	ODS(L"WSPRecv called!\n");
	return g_NextProcTable.lpWSPRecv(
		s,
		lpBuffers,
		dwBufferCount,
		lpNumberOfBytesRecvd,
		lpFlags,
		lpOverlapped,
		lpCompletionRoutine,
		lpThreadId,
		lpErrno);
}

int
WSPAPI WSPRecvDisconnect(
						 SOCKET s,
						 LPWSABUF lpInboundDisconnectData,
						 LPINT lpErrno
						 )
{
	ODS(L"WSPRecvDisconnect called!\n");
	return g_NextProcTable.lpWSPRecvDisconnect(s, lpInboundDisconnectData, lpErrno);
}

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
	)
{
	ODS(L"WSPRecvFrom called!\n");
	WCHAR temp[1024];
	char *ip = inet_ntoa(((SOCKADDR_IN*)lpFrom)->sin_addr);
	USHORT port = ntohs(((SOCKADDR_IN*)lpFrom)->sin_port);
	_stprintf_s(temp, L"IP is %s, PORT is %d\n", ip, port);
	ODS(temp);
	return g_NextProcTable.lpWSPRecvFrom(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags,
		lpFrom, lpFromlen, lpOverlapped, lpCompletionRoutine, lpThreadId, lpErrno);
}


int
WSPAPI WSPSelect(
				 int nfds,
				 fd_set FAR * readfds,
				 fd_set FAR * writefds,
				 fd_set FAR * exceptfds,
				 const struct timeval FAR * timeout,
				 LPINT lpErrno
				 )
{
	ODS(L"WSPSelect called!\n");
	return g_NextProcTable.lpWSPSelect(nfds, readfds, writefds,
		exceptfds, timeout, lpErrno);
}

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
			   )
{
	ODS(L"WSPSend called!\n");
	return g_NextProcTable.lpWSPSend(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent,
		dwFlags, lpOverlapped, lpCompletionRoutine, lpThreadId, lpErrno);
}

int
WSPAPI WSPSendDisconnect(
						 SOCKET s,
						 LPWSABUF lpOutboundDisconnectData,
						 LPINT lpErrno
						 )
{
	ODS(L"WSPSendDisconnect called!\n");
	return g_NextProcTable.lpWSPSendDisconnect(s, lpOutboundDisconnectData, lpErrno);
}


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
		LPINT    lpErrno )
{
	ODS(L"SendTo called\n");
	sockaddr_in *sa = (sockaddr_in *)lpTo;
	strcpy_s(NowIP,inet_ntoa(sa->sin_addr));
	nowPort = ntohs(sa->sin_port);
	GetInformation();
	if (!lanjie)
	{
		return g_NextProcTable.lpWSPSendTo(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpTo,
			iTolen, lpOverlapped, lpCompletionRoutine, lpThreadId, lpErrno);
	}
	((sockaddr_in*)lpTo)->sin_addr.S_un.S_addr = inet_addr(ChangedIP);
	((sockaddr_in*)lpTo)->sin_port = htons(ChangedPort);
	return g_NextProcTable.lpWSPSendTo(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpTo,
		iTolen, lpOverlapped, lpCompletionRoutine, lpThreadId, lpErrno);
}


int
WSPAPI WSPSetSockOpt(
					 SOCKET s,
					 int level,
					 int optname,
					 const char FAR * optval,
					 int optlen,
					 LPINT lpErrno
					 )
{
	ODS(L"WSPSetSockOpt called\n");
	return g_NextProcTable.lpWSPSetSockOpt(s, level, optname, optval, optlen, lpErrno);
}


int
WSPAPI WSPShutdown(
				   SOCKET s,
				   int how,
				   LPINT lpErrno
				   )
{
	return g_NextProcTable.lpWSPShutdown(s, how, lpErrno);
}

SOCKET
WSPAPI WSPSocket(
				 int af,
				 int type,
				 int protocol,
				 LPWSAPROTOCOL_INFOW lpProtocolInfo,
				 GROUP g,
				 DWORD dwFlags,
				 LPINT lpErrno
				 )
{
	ODS(L"Socket called! ");
	return g_NextProcTable.lpWSPSocket(af, type,protocol, 
		lpProtocolInfo, g, dwFlags, lpErrno);
}

INT
WSPAPI WSPStringToAddress(
						  LPWSTR AddressString,
						  INT AddressFamily,
						  LPWSAPROTOCOL_INFOW lpProtocolInfo,
						  LPSOCKADDR lpAddress,
						  LPINT lpAddressLength,
						  LPINT lpErrno
						  )
{
	return g_NextProcTable.lpWSPStringToAddress(AddressString, AddressFamily, 
		lpProtocolInfo, lpAddress, lpAddressLength, lpErrno);
}
