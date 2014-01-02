/*
Copyright 2006 Pieter van Ginkel (pvginkel@gmail.com)

This file is part of TrayRun.

TrayRun is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

TrayRun is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with TrayRun; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

// Windows NT4 minimum

#define _WIN32_WINNT 0x0400

// Windows 98 minimum

#define _WIN32_WINDOWS 0x0400
#define WINVER 0x0400

// Internet Explorer 5.0 minimum

/* TTS_BALLOON required IE5 and up. */

#define _WIN32_IE 0x0500

// Lean and mean

#define WIN32_LEAN_AND_MEAN

// And strict

#define STRICT

// Include standard libraries

#define _CRT_SECURE_NO_DEPRECATE

#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <shellapi.h>
#include <htmlhelp.h>

#include "TrayIcon.h"
#include "resource.h"

// These are for compiling under W2K / VS6

#ifndef NIN_SELECT
#define NIN_SELECT          (WM_USER + 0)
#endif

#ifndef NINF_KEY
#define NINF_KEY            0x1
#endif

#ifndef NIN_KEYSELECT
#define NIN_KEYSELECT       (NIN_SELECT | NINF_KEY)
#endif

#ifndef _ARRAYSIZE
#define _ARRAYSIZE(ary) (sizeof(ary) / sizeof(ary[0]))
#endif

BOOL GetClientWindow();

extern HWND g_hWnd;
extern HINSTANCE g_hInstance;
extern HICON g_hClientIcon;
extern HICON g_hClientSmIcon;
extern HWND g_hClientWindow;

#define WM_TRAYICON_NOTIFY WM_USER + 1

#define CLASS_APP "TrayRun"
#define APP_NAME "TrayRun"
#define TRAYICON_TIP "TrayRun"

#define POPUP_TRAYICON		0
