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

#include <winsock2.h>
#include <windows.h>
#include <ws2bth.h>
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib,"libpptmote.lib")

#ifndef LIBPPTMOTE_H
#define LIBPPTMOTE_H

int set_callback (int (*func) (char *));
char * set_device (char *name);
int set_key_code (unsigned int keybd);
int init_pptmote ();
ULONG connect_show_prompt();
int key_emulate ();

#endif
