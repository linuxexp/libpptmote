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

#include "common.h"

void
ba2str( BTH_ADDR ba, char *addr )
{
    int i;
    unsigned char bytes[6];
    for( i=0; i<6; i++ ) {
        bytes[5-i] = (unsigned char) ((ba >> (i*8)) & 0xff);
    }
    sprintf(addr, "%02X:%02X:%02X:%02X:%02X:%02X",
            bytes[0], bytes[1], bytes[2], bytes[3], bytes[4], bytes[5] );
}

int
str2uuid( const char *uuid_str, GUID *uuid)
{
    // Parse uuid128 standard format: 12345678-9012-3456-7890-123456789012
    int i;
    char buf[20] = { 0 };

    strncpy(buf, uuid_str, 8);
    uuid->Data1 = strtoul( buf, NULL, 16 );
    memset(buf, 0, sizeof(buf));

    strncpy(buf, uuid_str+9, 4);
    uuid->Data2 = (unsigned short) strtoul( buf, NULL, 16 );
    memset(buf, 0, sizeof(buf));
   
    strncpy(buf, uuid_str+14, 4);
    uuid->Data3 = (unsigned short) strtoul( buf, NULL, 16 );
    memset(buf, 0, sizeof(buf));

    strncpy(buf, uuid_str+19, 4);
    strncpy(buf+4, uuid_str+24, 12);
    for( i=0; i<8; i++ ) {
        char buf2[3] = { buf[2*i], buf[2*i+1], 0 };
        uuid->Data4[i] = (unsigned char)strtoul( buf2, NULL, 16 );
    }

    return 0;
}
