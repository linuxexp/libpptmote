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
#include "libpptmote.h"

char obex_push_uid[] = "00001105-0000-1000-8000-00805f9b34fb";
unsigned int key_code = VK_SPACE;
int (*log_func) (char * msg) = NULL;

char 	PPT_DEVICE[] 			= "pptmote";
char 	buf[40] 	  			= {0};
int		obex_port				= 0;

char 	obex_connect[] 			= {0x80, 0x00, 0x1a, 0x10, 0x00, 0xff, 0xff, 0x46, 0x00, 0x13, 0xf9, 0xec, 0x7b, 0xc4, 0x95, 0x3c, 
  0x11, 0xd2, 0x98, 0x4e, 0x52, 0x54, 0x00, 0xdc, 0x9e, 0x09};

char 	obex_send_file[] 		= {0x02, 0x00, 0x1b, 0x01, 0x00, 0x13, 0x00, 0x70, 0x00, 0x70, 0x00, 0x74, 0x00, 0x6d, 0x00, 0x6f, 
0x00, 0x74, 0x00, 0x65, 0x00, 0x00, 0xc3, 0x00, 0x00, 0x00, 0x07};

BOOL send_file = FALSE;

char obex_file [] = {0x82, 0x00, 0x0e, 0x49, 0x00, 0x0b, 0x0a, 0x70, 0x70, 0x74, 0x6d, 0x6f, 0x74, 0x65 };

char 	obex_disconnect[] 		= {0x81, 0x00, 0x03};

char 	*obex_headers[] 		= {obex_connect, obex_send_file, obex_file, obex_disconnect};

char start_notice [] = "PPTMote (c) Raja Jamwal <linux1@zoho.com>\nTrivial remote control using Bluetooth OBEX\n\nThis program is free software: you can redistribute it and/or modify\nit under the terms of the GNU General Public License as published by\nthe Free Software Foundation, either version 3 of the License, or\n(at your option) any later version.\n\nThis program is distributed in the hope that it will be useful,\nbut WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\nGNU General Public License for more details.\n\nYou should have received a copy of the GNU General Public License\nalong with this program.  If not, see <http://www.gnu.org/licenses/>.\n\n";

BOOL set_file_send (BOOL val)
{
		send_file = val;
		return send_file;
}

int log (char * msg)
{
    printf (msg);
    if (log_func != NULL){ log_func (msg);}
    return 0;
}

int set_callback (int (*func) (char *))
{
    log_func = func;
    return 0;
}

char * set_device (char *name)
{
    strcpy (PPT_DEVICE, name);
    return PPT_DEVICE; 
}

int set_key_code (unsigned int keybd)
{
    key_code = keybd;
    return key_code;
}

int key_emulate ()
{
    keybd_event (key_code, 0, 0, 0);
    keybd_event (key_code, 0, 2, 0);
    return 0;
}

SOCKADDR_BTH remote_bth_address = {0};

int init_pptmote ()
{
    int       return_code 			= 	CXN_SUCCESS;

    WSADATA   wsa_data 				= {0};

    return_code 					= WSAStartup (MAKEWORD (2, 2), &wsa_data);
    
    if (CXN_SUCCESS != return_code) {
    
        char msg[512]; sprintf (msg, "-FATAL- | Unable to initialize Winsock version 2.2\n");
        log (msg);
    }
    
    if ( CXN_SUCCESS == return_code) {

        return_code = name_to_bt_address (PPT_DEVICE, &remote_bth_address);

        if ( CXN_SUCCESS != return_code ) {
            char msg[512]; sprintf (msg, "-FATAL- | Unable to get address of the remote radio having name %s\n", PPT_DEVICE);	
            log (msg);
        }
    }

    char msg[512];
    sprintf (msg, "* Using device %s\n", buf);
    log (msg);

    return(int)return_code;
}



ULONG connect_show_prompt()
{
    ULONG           return_code          = CXN_SUCCESS;
    SOCKET          local_socket 	     = INVALID_SOCKET;
    SOCKADDR_BTH    sock_addr_bth_server = remote_bth_address;
    HRESULT         res;
        
    //
    // Setting address family to AF_BTH indicates winsock2 to use Bluetooth sockets
    // specify obex uuid, instead of the port
    //

    GUID	  uuid;
    str2uuid (obex_push_uid, &uuid );
    
    sock_addr_bth_server.addressFamily   = AF_BTH;
    sock_addr_bth_server.serviceClassId  = uuid;
    sock_addr_bth_server.port            = 0;

    //while (1)
    //{
        //
        // Open a bluetooth socket using RFCOMM protocol
        //

        local_socket = socket (AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);

        if (INVALID_SOCKET == local_socket ) {
            char msg[512]; sprintf (msg, "=CRITICAL= | socket() call failed. WSAGetLastError = [%d]\n", WSAGetLastError());
            log (msg);
            return_code = CXN_ERROR;   
        }
        
        // connect
        
        if ( SOCKET_ERROR == connect (local_socket,
                                      (struct sockaddr *) &sock_addr_bth_server,
                                      sizeof(SOCKADDR_BTH)) ) {
            char msg[512];
            sprintf (msg, "=CRITICAL= | connect() call failed. WSAGetLastError=[%d]\n", WSAGetLastError());
            log (msg);
            return_code = CXN_ERROR;
            //continue;
        }

        unsigned char recd [512];
        int key_pressed = 0;

        for (int i=0;i<(sizeof(obex_headers)/sizeof(obex_headers[0]));i++)
        {
        
            // lets send OBEX object

			if (send_file == FALSE && i==2) continue; 
        
            if (SOCKET_ERROR == send(local_socket,
                                     (char *)obex_headers[i],
                                     (int)obex_headers[i][2],
                                     0) ) {
                    
                    char msg[512]; sprintf (msg, "failed to send, header");					  
                    return_code = CXN_ERROR;
                    
                    if ( SOCKET_ERROR == closesocket(local_socket) ) {
                        char msg[512];sprintf(msg,"=CRITICAL= | closesocket() call failed w/socket = [0x%I64X]. WSAGetLastError=[%d]\n", local_socket, WSAGetLastError());
                        log (msg);
                        return_code = CXN_ERROR;
                
                    }
                    return 0;
            }

            recv (local_socket, (char*) recd, 512, 0);
            if (recd[0] == 0x90 ) key_pressed = 1;
			if (recd[0] == 0x81 && i==2 && send_file==TRUE ) key_pressed = 1;
        }
            
        //
        // Close the socket
        //
        if ( SOCKET_ERROR == closesocket(local_socket) ) {
                
            char msg[512];sprintf(msg,"=CRITICAL= | closesocket() call failed w/socket = [0x%I64X]. WSAGetLastError=[%d]\n", local_socket, WSAGetLastError());
            log (msg);
            return_code = CXN_ERROR;
                
        }

        local_socket = INVALID_SOCKET;
    //}

    
    return (key_pressed);
}


//
// name_to_bt_address converts a bluetooth device name to a bluetooth address,
// if required by performing inquiry with remote name requests.
//
int name_to_bt_address (const LPCSTR remote_name, PSOCKADDR_BTH premote_bt_address)
{
    INT             res_code 		= CXN_SUCCESS;
    BOOL            continue_lookup = FALSE, remote_device_found = FALSE;
    ULONG           query_flags 	= 0, query_size = sizeof(WSAQUERYSET);
    HANDLE          hlookup 		= NULL;
    PWSAQUERYSET    wsa_query_set 	= NULL;

    wsa_query_set 					= (PWSAQUERYSET) HeapAlloc(GetProcessHeap(),
                                                               HEAP_ZERO_MEMORY,
                                                               query_size);
    if (NULL == wsa_query_set ) {
        res_code = STATUS_NO_MEMORY;
        char msg[512];
        sprintf	(msg, "!ERROR! | Unable to allocate memory for WSAQUERYSET\n");
        log (msg);
    }

    //
    // Search for the device with the correct name
    //
    
    if ( CXN_SUCCESS == res_code) {

        for ( INT retry_count = 0;
            !remote_device_found && (retry_count < CXN_MAX_INQUIRY_RETRY);
            retry_count++ ) {
            
            //
            // WSALookupService is used for both service search and device inquiry
            // LUP_CONTAINERS is the flag which signals that we're doing a device inquiry.
            //
            
            query_flags = LUP_CONTAINERS;

            //
            // Friendly device name (if available) will be returned in lpszServiceInstanceName
            //
            query_flags |= LUP_RETURN_NAME;

            //
            // BTH_ADDR will be returned in lpcsaBuffer member of WSAQUERYSET
            //
            query_flags |= LUP_RETURN_ADDR;

            if ( 0 == retry_count ) {
                char msg[512]; sprintf (msg, "*INFO* | Inquiring device from cache...\n");
                log (msg);
            } else {
            
                //
                // Flush the device cache for all inquiries, except for the first inquiry
                //
                // By setting LUP_FLUSHCACHE flag, we're asking the lookup service to do
                // a fresh lookup instead of pulling the information from device cache.
                //
                query_flags |= LUP_FLUSHCACHE;

                //
                // Pause for some time before all the inquiries after the first inquiry
                //
                // Remote Name requests will arrive after device inquiry has
                // completed.  Without a window to receive IN_RANGE notifications,
                // we don't have a direct mechanism to determine when remote
                // name requests have completed.
                //
                char msg[512];
                sprintf(msg, "*INFO* | Unable to find device.  Waiting for %d seconds before re-inquiry...\n", CXN_DELAY_NEXT_INQUIRY);
                log (msg);
                Sleep(CXN_DELAY_NEXT_INQUIRY * 1000);

                sprintf(msg, "*INFO* | Inquiring device ...\n");
                log (msg);
            }

            //
            // Start the lookup service
            //
            res_code = CXN_SUCCESS;
            hlookup = 0;
            continue_lookup = FALSE;
            ZeroMemory(wsa_query_set, query_size);
            wsa_query_set->dwNameSpace = NS_BTH;
            wsa_query_set->dwSize = sizeof(WSAQUERYSET);
            res_code = WSALookupServiceBegin(wsa_query_set, query_flags, &hlookup);

            //
            // Even if we have an error, we want to continue until we
            // reach the CXN_MAX_INQUIRY_RETRY
            //
            if ( (NO_ERROR == res_code) && (NULL != hlookup) ) {
                continue_lookup = TRUE;
            } else if ( 0 < retry_count ) {
                char msg[512];
                sprintf(msg, "=CRITICAL= | WSALookupServiceBegin() failed with error code %d, WSAGetLastError = %d\n", res_code, WSAGetLastError());
                log (msg);
                break;
            }

            while ( continue_lookup ) {
                //
                // Get information about next bluetooth device
                //
                // Note you may pass the same WSAQUERYSET from LookupBegin
                // as long as you don't need to modify any of the pointer
                // members of the structure, etc.
                //
                // ZeroMemory(wsa_query_set, query_size);
                // wsa_query_set->dwNameSpace = NS_BTH;
                // wsa_query_set->dwSize = sizeof(WSAQUERYSET);
                if ( NO_ERROR == WSALookupServiceNext(hlookup,
                                                      query_flags,
                                                      &query_size,
                                                      wsa_query_set) ) {
                    
                    //
                    // Compare the name to see if this is the device we are looking for.
                    //
                    if ( ( wsa_query_set->lpszServiceInstanceName != NULL ) &&
                         ( CXN_SUCCESS == stricmp(wsa_query_set->lpszServiceInstanceName, remote_name) ) ) {
                        //
                        // Found a remote bluetooth device with matching name.
                        // Get the address of the device and exit the lookup.

                             
                        //
                        CopyMemory(premote_bt_address,
                                   (PSOCKADDR_BTH) wsa_query_set->lpcsaBuffer->RemoteAddr.lpSockaddr,
                                   sizeof(*premote_bt_address));

                        BTH_ADDR result = 
                                ((SOCKADDR_BTH*)wsa_query_set->lpcsaBuffer->RemoteAddr.lpSockaddr)->btAddr;
                        ba2str( result, buf );

                        remote_device_found = TRUE;
                        continue_lookup = FALSE;
                    }
                } else {
                    res_code = WSAGetLastError();
                    if ( WSA_E_NO_MORE == res_code ) { //No more data
                        //
                        // No more devices found.  Exit the lookup.
                        //
                        continue_lookup = FALSE;
                    } else if ( WSAEFAULT == res_code ) {
                        //
                        // The buffer for QUERYSET was insufficient.
                        // In such case 3rd parameter "query_size" of function "WSALookupServiceNext()" receives
                        // the required size.  So we can use this parameter to reallocate memory for QUERYSET.
                        //
                        HeapFree(GetProcessHeap(), 0, wsa_query_set);
                        wsa_query_set = (PWSAQUERYSET) HeapAlloc(GetProcessHeap(),
                                                                HEAP_ZERO_MEMORY,
                                                                query_size);
                        if ( NULL == wsa_query_set ) {
                            char msg[512];
                            sprintf(msg,"!ERROR! | Unable to allocate memory for WSAQERYSET\n");
                            log (msg);
                            continue_lookup = FALSE;
                        }
                    } else {
                        char msg[512];
                        sprintf(msg,"=CRITICAL= | WSALookupServiceNext() failed with error code %d\n", res_code);
                        log (msg);
                        continue_lookup = FALSE;
                    }
                }
            }

            //
            // End the lookup service
            //
            WSALookupServiceEnd(hlookup);
        }
    }

    if ( NULL != wsa_query_set ) {
        HeapFree(GetProcessHeap(), 0, wsa_query_set);
        wsa_query_set = NULL;
    }

    if ( remote_device_found ) {
        res_code = CXN_SUCCESS;
    } else {
        res_code = CXN_ERROR;
    }

    return res_code;
}
