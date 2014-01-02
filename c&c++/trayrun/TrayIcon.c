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

#include "header.h"

#define TRAYICON_ID	1

static BOOL TrayIconShowPopupMenu();

HMENU g_hPopupMenus = NULL;

BOOL TrayIconCreate() {
	NOTIFYICONDATA nid;

	memset(&nid, 0, sizeof(nid));

	nid.cbSize = sizeof(nid);
	nid.hWnd = g_hWnd;
	nid.uID = TRAYICON_ID;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_TRAYICON_NOTIFY;
	nid.hIcon = g_hClientSmIcon;
	strcpy(nid.szTip, TRAYICON_TIP);

	Shell_NotifyIcon(NIM_ADD, &nid);

	return TRUE;
}

BOOL TrayIconDestroy() {
	NOTIFYICONDATA nid;

	memset(&nid, 0, sizeof(nid));

	nid.cbSize = sizeof(nid);
	nid.hWnd = g_hWnd;
	nid.uID = TRAYICON_ID;

	Shell_NotifyIcon(NIM_DELETE, &nid);

	return TRUE;
}

BOOL TrayIconReCreate() {
	TrayIconDestroy();
	TrayIconCreate();

	return TRUE;
}

BOOL TrayIconMessage(INT nTrayIconId, UINT uMsg) {
	switch (uMsg) {
	case WM_CONTEXTMENU:
	case WM_RBUTTONDOWN:
	case NIN_SELECT:
	case NIN_KEYSELECT:
		TrayIconShowPopupMenu();
		break;

	case WM_LBUTTONUP:
		GetClientWindow();

		if (g_hClientWindow) {
			UINT nMsg;

			if (IsWindowVisible(g_hClientWindow)) {
				if (IsZoomed(g_hClientWindow) || IsIconic(g_hClientWindow)) {
					nMsg = ID_TRAYICON_RESTORE;
				} else {
					nMsg = ID_TRAYICON_HIDETOTRAY;
				}
			} else {
				nMsg = ID_TRAYICON_RESTORE;
			}

			SendMessage(g_hWnd, WM_COMMAND, nMsg, 0);
		}
		break;
	}

	return TRUE;
}

static BOOL TrayIconShowPopupMenu() {
	HMENU hSubMenu;
	POINT p;

	// The GetMessagePos() should be a better replacement for
	// using the GetCursorPos. Sadly enough, when the popup menu
	// is shown with the application minimized, this returns the
	// location of the taskbar button of the application instead
	// of the actual position of the cursor when the context menu
	// was requested. So, we use GetCursorPos().

	if (g_hPopupMenus) {
		DestroyMenu(g_hPopupMenus);
	}

	g_hPopupMenus = LoadMenu(g_hInstance, MAKEINTRESOURCE(IDR_POPUPMENUS));
	hSubMenu = GetSubMenu(g_hPopupMenus, POPUP_TRAYICON);

	// We modify the menu so it reflects whether we found a hWnd
	// for the client process

	GetClientWindow();

	if (g_hClientWindow) {
		BOOL fVisible = IsWindowVisible(g_hClientWindow);
		BOOL fIconic = ( fVisible ? IsIconic(g_hClientWindow) : FALSE );
		BOOL fZoomed = ( fVisible ? IsZoomed(g_hClientWindow) : FALSE );

		EnableMenuItem(hSubMenu, ID_TRAYICON_HIDETOTRAY,
			( fVisible ? MF_ENABLED : MF_GRAYED ));
		EnableMenuItem(hSubMenu, ID_TRAYICON_RESTORE,
			( !fVisible || fIconic || fZoomed ? MF_ENABLED : MF_GRAYED ));
		EnableMenuItem(hSubMenu, ID_TRAYICON_MINIMIZE,
			( fVisible && !fIconic ? MF_ENABLED : MF_GRAYED ));
		EnableMenuItem(hSubMenu, ID_TRAYICON_MAXIMIZE,
			( !fVisible || !fZoomed ? MF_ENABLED : MF_GRAYED ));
	} else {
		EnableMenuItem(hSubMenu, ID_TRAYICON_HIDETOTRAY, MF_GRAYED);
		EnableMenuItem(hSubMenu, ID_TRAYICON_RESTORE, MF_GRAYED);
		EnableMenuItem(hSubMenu, ID_TRAYICON_MINIMIZE, MF_GRAYED);
		EnableMenuItem(hSubMenu, ID_TRAYICON_MAXIMIZE, MF_GRAYED);
	}

	GetCursorPos(&p);

	SetForegroundWindow(g_hWnd);

	TrackPopupMenu(hSubMenu, TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
		p.x, p.y, 0, g_hWnd, NULL);

	PostMessage(g_hWnd, WM_NULL, 0, 0);

	return TRUE;
}
