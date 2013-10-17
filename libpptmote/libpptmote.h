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

#ifndef LIBPPTMOTE_H
#define LIBPPTMOTE_H

#ifdef LIBPPTMOTE_EXPORTS
#define LIBPPTMOTE_API __declspec(dllexport) 
#else
#define LIBPPTMOTE_API __declspec(dllimport) 
#endif

LIBPPTMOTE_API ULONG connect_show_prompt (SOCKADDR_BTH sock_remote_address);
LIBPPTMOTE_API int set_callback (int (*func) (char *));
LIBPPTMOTE_API char * set_device (char *name);
LIBPPTMOTE_API int set_key_code (unsigned int keybd);
LIBPPTMOTE_API int init_pptmote ();
LIBPPTMOTE_API ULONG connect_show_prompt();
LIBPPTMOTE_API int key_emulate ();
LIBPPTMOTE_API BOOL set_file_send (BOOL val);
#endif
