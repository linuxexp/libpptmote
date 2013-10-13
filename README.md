libpptmote
==========

Use BT OBEX object push prompt as remote signaling

libpptmote API
===============

Configure the connection using following function

char * set_device (char *name);
===================================================
The device name of the bluetooth device, if successful it sets the BT device name and return its pointer.

int set_key_code (unsigned int keybd);
===================================================
The program supports emulating keyboard's keys, keys are mapped from 0 to 255, search MSDN for "Virtual Key Codes"

int set_callback (__in int (*func) (char *));
===================================================
Specify a callback function, that will be called with readable error and notification messages

i.e

int log (char msg)
{
	// process your msg here
	return 0;
} 

set_callback (log);

Default Configuration
========================

if something isn't configured, then default is used

device_name = "pptmote"
keycode "VK_SPACE"
callback = NULL

int init_pptmote ()
====================
init_pptmote must be called before you call connect_show_prompt() which pushes the OBEX object, init_pptmote queries the system bluetooth cache, to convert the readable device name to its 48 bit long bluetooth address.

ULONG connect_show_prompt()
==================================
this pushes a OBEX object, if your phone shows prompts for OBEX push, this will show a prompt, if the user accepts the prompt, the return is 1 otherwise 0

int key_emulate ()
==================================
emulate the specified key on the keyboard

