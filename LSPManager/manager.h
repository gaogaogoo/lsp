#ifndef _MANAGER_H
#define _MANAGER_H


#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <Ws2spi.h>
#include <Sporder.h>  
#include <windows.h>
#include <stdio.h>
#include <string.h>
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Rpcrt4.lib")

#define REG_MAX_LENGTH 30

typedef enum _LSPERROR
{
	LSP_SUCCESS,
	LSP_NO_SUCH_FILE,
	LSP_ACCESS_DENIED,
	LSP_TOO_SMALL,
	LSP_TOO_LONG,
	LSP_ERROR_UNKNOWN
} LSPERROR, *PLSPERROR;


LSPERROR InstallLSP();
LSPERROR RemoveLSP();
LSPERROR AddRule(const char* name, const char* value);
LSPERROR DeleteRule(const char*name);

#endif
