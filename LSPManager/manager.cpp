#include "manager.h"
#include "stdio.h"

static GUID ProviderGuid = {0x8a, 0x88b, 0x888c,{0x8a,0x8a,0x8a,0x8a,0x8a,0x8a,0x8a,0x8a}};

LPWSAPROTOCOL_INFOW GetProvider(LPINT lpnTotalProtocols);
void FreeProvider(LPWSAPROTOCOL_INFOW pProtoInfo);
BOOL InstallProvider(WCHAR *pwszPathName);
BOOL RemoveProvider();

LSPERROR InstallLSP()
{
	TCHAR szPathName[256];
	TCHAR* p;
	if(::GetFullPathName(L"MyLSP.dll", 256, szPathName, &p) != 0)
	{
		if(InstallProvider(szPathName))
		{
			return LSP_SUCCESS;
		} else
		{
			return LSP_ACCESS_DENIED;
		}
	}
	return LSP_NO_SUCH_FILE;
}

LSPERROR RemoveLSP()
{
	if (RemoveProvider())
		return LSP_SUCCESS;
	else
		return LSP_ACCESS_DENIED;
}

LSPERROR AddRule(const char* name, const char* value)
{
	HKEY hKey = 0;
	DWORD Count = 0, keyType = REG_DWORD, disp = REG_CREATED_NEW_KEY;
	DWORD len = sizeof(DWORD);
	char temp[REG_MAX_LENGTH];

	if (strlen(name) > REG_MAX_LENGTH || strlen(value) > REG_MAX_LENGTH)
	{
		return LSP_TOO_LONG;
	}

	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\LSPManager",0, KEY_ALL_ACCESS, &hKey)!= ERROR_SUCCESS)
	{
		if (RegCreateKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\LSPManager", 0, NULL, 0, KEY_ALL_ACCESS, NULL,&hKey, &disp))
			return LSP_ACCESS_DENIED;
		Count = 0;
	}
	else
	{
		if (RegQueryValueExA(hKey, "NumberOfRules", 0, &keyType, (BYTE*)&Count, &len))
		{
			RegCloseKey(hKey);
			return LSP_ACCESS_DENIED;
		}
	}

	keyType = REG_SZ;
	len = strlen(temp);
	RegQueryValueExA(hKey, name, 0, &keyType, (BYTE*)temp, &len);
	if (len != strlen(temp))
	{
		RegSetValueExA(hKey, name, 0, REG_SZ, (BYTE*)value, strlen(value));
	}
	else
	{
		Count = Count + 1;
		RegSetValueExA(hKey, "NumberOfRules", 0, REG_DWORD,(BYTE*)&Count, sizeof(Count));
		RegSetValueExA(hKey, name, 0, REG_SZ, (BYTE*)value, strlen(value));
	}
	RegCloseKey(hKey);
	return LSP_SUCCESS;
}

LSPERROR DeleteRule(const char* name)
{
	HKEY hKey = 0;
	DWORD Count = 0, keyType = REG_SZ;
	DWORD len = sizeof(DWORD);

	if (strlen(name) > REG_MAX_LENGTH)
		return LSP_TOO_LONG;

	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\LSPManager",0, KEY_ALL_ACCESS, &hKey)!= ERROR_SUCCESS)
		return LSP_ACCESS_DENIED;

	RegQueryValueExA(hKey, "NumberOfRules", 0, &keyType, (BYTE*)&Count, &len);
	if (Count == 0) 
	{
		RegCloseKey(hKey);
		return LSP_NO_SUCH_FILE;
	}
	if (RegDeleteValueA(hKey, name)) 
	{
		RegCloseKey(hKey);
		return LSP_NO_SUCH_FILE;
	}
	Count = Count - 1;
	RegSetValueExA(hKey, "NumberOfRules", 0, REG_DWORD,(BYTE*)&Count, sizeof(Count));
	RegCloseKey(hKey);
	return LSP_SUCCESS;
}

LPWSAPROTOCOL_INFOW GetProvider(LPINT lpnTotalProtocols)
{
	DWORD dwSize = 0;
	int dwError;
	LPWSAPROTOCOL_INFOW pProtoInfo = NULL;

	if(::WSCEnumProtocols(NULL, pProtoInfo, &dwSize, &dwError) == SOCKET_ERROR)
	{
		if(dwError != WSAENOBUFS) return NULL;
	}

	pProtoInfo = (LPWSAPROTOCOL_INFOW)::GlobalAlloc(GPTR, dwSize);
	*lpnTotalProtocols = ::WSCEnumProtocols(NULL, pProtoInfo, &dwSize, &dwError);
	return pProtoInfo;
}

void FreeProvider(LPWSAPROTOCOL_INFOW pProtoInfo)
{
	::GlobalFree(pProtoInfo);
}

BOOL InstallProvider(WCHAR *pwszPathName)
{
	WCHAR wszLSPName[] = L"MyLSP.dll";
	LPWSAPROTOCOL_INFOW pProtoInfo;
	int nProtocols;
	WSAPROTOCOL_INFOW OriginalProtocolInfo[3];
	DWORD    dwOrigCatalogId[3];
	int nArrayCount = 0;
	DWORD dwLayeredCatalogId;  
	int dwError;

	pProtoInfo = GetProvider(&nProtocols);
	BOOL bFindUdp = FALSE;
	BOOL bFindTcp = FALSE;
	BOOL bFindRaw = FALSE;
	for (int i=0; i<nProtocols; i++)
	{
		if (pProtoInfo[i].iAddressFamily == AF_INET)
		{
			if (!bFindUdp && pProtoInfo[i].iProtocol == IPPROTO_UDP)
			{
				memcpy(&OriginalProtocolInfo[nArrayCount], &pProtoInfo[i], sizeof(WSAPROTOCOL_INFOW));
				OriginalProtocolInfo[nArrayCount].dwServiceFlags1 = OriginalProtocolInfo[nArrayCount].dwServiceFlags1 & (~XP1_IFS_HANDLES); 
				dwOrigCatalogId[nArrayCount++] = pProtoInfo[i].dwCatalogEntryId;
				bFindUdp = TRUE;
			}
			if (!bFindTcp && pProtoInfo[i].iProtocol == IPPROTO_TCP)
			{
				memcpy(&OriginalProtocolInfo[nArrayCount], &pProtoInfo[i], sizeof(WSAPROTOCOL_INFOW));
				OriginalProtocolInfo[nArrayCount].dwServiceFlags1 = OriginalProtocolInfo[nArrayCount].dwServiceFlags1 & (~XP1_IFS_HANDLES); 
				dwOrigCatalogId[nArrayCount++] = pProtoInfo[i].dwCatalogEntryId;
				bFindTcp = TRUE;
			}
			if (!bFindRaw && pProtoInfo[i].iProtocol == IPPROTO_IP)
			{
				memcpy(&OriginalProtocolInfo[nArrayCount], &pProtoInfo[i], sizeof(WSAPROTOCOL_INFOW));
				OriginalProtocolInfo[nArrayCount].dwServiceFlags1 = OriginalProtocolInfo[nArrayCount].dwServiceFlags1 & (~XP1_IFS_HANDLES); 
				dwOrigCatalogId[nArrayCount++] = pProtoInfo[i].dwCatalogEntryId;
				bFindRaw = TRUE;
			}
		}
	}

	WSAPROTOCOL_INFOW LayeredProtocolInfo;
	memcpy(&LayeredProtocolInfo, &OriginalProtocolInfo[0], sizeof(WSAPROTOCOL_INFOW));
	wcscpy_s(LayeredProtocolInfo.szProtocol, wszLSPName);
	LayeredProtocolInfo.ProtocolChain.ChainLen = LAYERED_PROTOCOL;
	LayeredProtocolInfo.dwProviderFlags |= PFL_HIDDEN;

	if (::WSCInstallProvider(&ProviderGuid, pwszPathName, &LayeredProtocolInfo, 1, &dwError) == SOCKET_ERROR)
	{
		return FALSE;
	}

	FreeProvider(pProtoInfo);
	pProtoInfo = GetProvider(&nProtocols);
	for(int i=0; i<nProtocols; i++)
	{
		if (memcmp(&pProtoInfo[i].ProviderId, &ProviderGuid, sizeof(ProviderGuid)) == 0)
		{
			dwLayeredCatalogId = pProtoInfo[i].dwCatalogEntryId;
			break;
		}
	}

	WCHAR wszChainName[WSAPROTOCOL_LEN + 1];
	for(int i=0; i<nArrayCount; i++)
	{
		swprintf_s(wszChainName, L"%ws over %ws", wszLSPName, OriginalProtocolInfo[i].szProtocol);
		wcscpy_s(OriginalProtocolInfo[i].szProtocol, wszChainName);
		if (OriginalProtocolInfo[i].ProtocolChain.ChainLen == 1)
		{
			OriginalProtocolInfo[i].ProtocolChain.ChainEntries[1] = dwOrigCatalogId[i];
		}
		else
		{
			for (int j = OriginalProtocolInfo[i].ProtocolChain.ChainLen; j>0; j--)
			{
				OriginalProtocolInfo[i].ProtocolChain.ChainEntries[j] = OriginalProtocolInfo[i].ProtocolChain.ChainEntries[j-1];
			}
		}
		OriginalProtocolInfo[i].ProtocolChain.ChainLen ++;
		OriginalProtocolInfo[i].ProtocolChain.ChainEntries[0] = dwLayeredCatalogId; 
	}

	GUID ProviderChainGuid;
	if (::UuidCreate(&ProviderChainGuid) == RPC_S_OK)
	{
		if (::WSCInstallProvider(&ProviderChainGuid, pwszPathName, OriginalProtocolInfo, nArrayCount, &dwError) == SOCKET_ERROR)
		{
			return FALSE; 
		}
	}
	else
	{
		return FALSE;
	}
	FreeProvider(pProtoInfo);
	pProtoInfo = GetProvider(&nProtocols);
	DWORD dwIds[20];
	int nIndex = 0;

	for(int i=0; i<nProtocols; i++)
	{
		if((pProtoInfo[i].ProtocolChain.ChainLen > 1) && (pProtoInfo[i].ProtocolChain.ChainEntries[0] == dwLayeredCatalogId))
			dwIds[nIndex++] = pProtoInfo[i].dwCatalogEntryId;
	}

	for(int i=0; i<nProtocols; i++)
	{
		if((pProtoInfo[i].ProtocolChain.ChainLen <= 1) || (pProtoInfo[i].ProtocolChain.ChainEntries[0] != dwLayeredCatalogId))
			dwIds[nIndex++] = pProtoInfo[i].dwCatalogEntryId;
	}

	if((dwError = ::WSCWriteProviderOrder(dwIds, nIndex)) != ERROR_SUCCESS)
	{
		return FALSE;
	}
	FreeProvider(pProtoInfo);
	return TRUE;
}



BOOL RemoveProvider()
{
	LPWSAPROTOCOL_INFOW pProtoInfo;
	int nProtocols, i;
	DWORD dwLayeredCatalogId;

	pProtoInfo = GetProvider(&nProtocols);
	int dwError;
	for (i=0; i<nProtocols; i++)
	{
		if (memcmp(&ProviderGuid, &pProtoInfo[i].ProviderId, sizeof(ProviderGuid)) == 0)
		{
			dwLayeredCatalogId = pProtoInfo[i].dwCatalogEntryId;
			break;
		}
	}
	if (i < nProtocols)
	{
		for (i=0; i<nProtocols; i++)
		{
			if ((pProtoInfo[i].ProtocolChain.ChainLen > 1) && (pProtoInfo[i].ProtocolChain.ChainEntries[0] == dwLayeredCatalogId))
			{
				::WSCDeinstallProvider(&pProtoInfo[i].ProviderId, &dwError);
			}
		}
		::WSCDeinstallProvider(&ProviderGuid, &dwError);
	}
	return TRUE;
}
