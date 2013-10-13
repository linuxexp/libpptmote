/*
    Copyright (c) Raja Jamwal, <linux1@zoho.com>

    This file is part of PPTMote.

    PPTMote is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PPTMote is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with PPTMote.  If not, see <http://www.gnu.org/licenses/>.*/

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <ws2bth.h>
#include <initguid.h>
#include <stdlib.h>

#include <strsafe.h>
#include <intsafe.h>

#pragma comment(lib, "Ws2_32.lib")

#define CXN_SUCCESS                       0
#define CXN_ERROR                         1
#define CXN_MAX_INQUIRY_RETRY             3
#define CXN_DELAY_NEXT_INQUIRY            15
#define CTX_WAIT						  0
#define CTX_RESP_LENGTH					  500
#define CTX_ICON						  9

int name_to_bt_address (const LPCSTR pszRemoteName, PSOCKADDR_BTH pRemoteBtAddr);

void
ba2str( BTH_ADDR ba, char *addr );

int
str2uuid( const char *uuid_str, GUID *uuid);

