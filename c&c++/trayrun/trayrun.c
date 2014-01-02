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

HINSTANCE g_hInstance = NULL;
HWND g_hWnd = NULL;
LPSTR g_lpCmdLine = NULL;
HICON g_hClientIcon = NULL;
HICON g_hClientSmIcon = NULL;
HWND g_hClientWindow = NULL;
LPSTR g_szClientAppName = NULL;
PROCESS_INFORMATION g_piClient;

static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static BOOL WmCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
static BOOL ExecuteClient();
static BOOL LoadClientIcons();
static BOOL EnumNamesFunc(HANDLE hModule, LPCTSTR lpType, LPTSTR lpName, LONG lParam);
static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
static BOOL WorkoutCmdLine(LPSTR lpCmdLine);
static BOOL ShowManual();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc;
	MSG msg;
	HANDLE aHandles[1];

	g_hInstance = hInstance;

	if (!WorkoutCmdLine(lpCmdLine)) {
		ShowManual();
		return 0;
	}

	if (!ExecuteClient()) {
		ShowManual();
		return 0;
	}

	LoadClientIcons();

	memset(&wc, 0, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = g_hInstance;
	wc.hIcon = g_hClientIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = 0;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = CLASS_APP;
	wc.hIconSm = g_hClientSmIcon;
	
	if (RegisterClassEx(&wc) == 0) {
		ShowManual();
		return 0;
	}

	CreateWindowEx(
		WS_EX_APPWINDOW,
		CLASS_APP,
		APP_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		g_hInstance,
		NULL);

	if (g_hWnd == 0) {
		ShowManual();
		return 0;
	}

	aHandles[0] = g_piClient.hProcess;

	for (;;) {
		DWORD dwResult;
		BOOL fFinished = FALSE;

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				fFinished = TRUE;
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (fFinished) {
			break;
		}

		dwResult = MsgWaitForMultipleObjects(1, aHandles, FALSE, INFINITE, QS_ALLINPUT);

		if (dwResult == WAIT_OBJECT_0) {
			DWORD dwExitCode;

			if (!GetExitCodeProcess(g_piClient.hProcess, &dwExitCode)
				|| dwExitCode != STILL_ACTIVE) {
				PostMessage(g_hWnd, WM_COMMAND, ID_TRAYICON_CLOSE, 0);
			}
		}
	}

	DestroyWindow(g_hWnd);

	free(g_szClientAppName);
	free(g_lpCmdLine);

	GetClientWindow();

	if (g_hClientWindow) {
		PostMessage(g_hClientWindow, WM_CLOSE, 0, 0);
	} else {
		TerminateProcess(g_piClient.hProcess, 0);
	}

	return (int)msg.wParam;
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static UINT uTaskbarRestart = 0;

	switch (uMsg) {
	case WM_CREATE:
		g_hWnd = hWnd;

		uTaskbarRestart = RegisterWindowMessage("TaskbarCreated");

		TrayIconCreate();
		break;

	case WM_DESTROY:
		TrayIconDestroy();
		PostQuitMessage(0);
		break;

	case WM_TRAYICON_NOTIFY:
		TrayIconMessage((INT)wParam, (UINT)lParam);
		break;

	case WM_COMMAND:
		if (WmCommand(hWnd, wParam, lParam)) {
			return 0;
		}
		break;

	default:
		// This is so our task tray button reappears when
		// the explorer taskbar / process restarts.

		if (uMsg == uTaskbarRestart) {
			TrayIconReCreate();
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

static BOOL WmCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	INT wID = LOWORD(wParam);
	INT wEvent = HIWORD(wParam);

	switch (wID) {
	case ID_TRAYICON_HIDETOTRAY:
		GetClientWindow();

		if (g_hClientWindow) {
			ShowWindow(g_hClientWindow, SW_HIDE);
		}
		break;

	case ID_TRAYICON_RESTORE:
		GetClientWindow();

		if (g_hClientWindow) {
			ShowWindow(g_hClientWindow, SW_RESTORE);
		}
		break;

	case ID_TRAYICON_MINIMIZE:
		GetClientWindow();

		if (g_hClientWindow) {
			ShowWindow(g_hClientWindow, SW_MINIMIZE);
		}
		break;

	case ID_TRAYICON_MAXIMIZE:
		GetClientWindow();

		if (g_hClientWindow) {
			ShowWindow(g_hClientWindow, SW_MAXIMIZE);
		}
		break;

	case ID_TRAYICON_CLOSE:
		PostQuitMessage(0);
		break;

	default:
		return FALSE;
	}

	return TRUE;
}

static BOOL ExecuteClient() {
	BOOL fResult;
	STARTUPINFO si;

	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;

	memset(&g_piClient, 0, sizeof(g_piClient));

	fResult = CreateProcess(
		NULL,
		g_lpCmdLine,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
		&g_piClient);

	if (!fResult) {
		return FALSE;
	}

	return TRUE;
}

static BOOL LoadClientIcons() {
	HMODULE hModule;
	
	hModule = LoadLibrary(g_szClientAppName);
	if (hModule) {
		EnumResourceNames(hModule, RT_ICON, (ENUMRESNAMEPROC)EnumNamesFunc, 0); 
		FreeLibrary(hModule);
	}

	if (!g_hClientIcon || !g_hClientSmIcon) {
		g_hClientIcon = LoadIcon(g_hInstance,
			MAKEINTRESOURCE(IDI_APPLICATION_ICON));
		g_hClientSmIcon = LoadImage(g_hInstance,
			MAKEINTRESOURCE(IDI_APPLICATION_ICON),
			IMAGE_ICON, GetSystemMetrics(SM_CXSMICON),
			GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	}

	return TRUE;
}

static BOOL EnumNamesFunc(HANDLE hModule, LPCTSTR lpType, LPTSTR lpName, LONG lParam) {
	HRSRC hResource;
	HGLOBAL hGlobal;
	LPVOID lpResource;
	
	hResource = FindResource(hModule, lpName, lpType);
	hGlobal = LoadResource(hModule, hResource);
	lpResource = LockResource(hGlobal);

	g_hClientIcon = CreateIconFromResourceEx(lpResource,
		SizeofResource(hModule, hResource), TRUE, 0x00030000,
		GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON),
		LR_DEFAULTCOLOR);

	g_hClientSmIcon = CreateIconFromResourceEx(lpResource,
		SizeofResource(hModule, hResource), TRUE, 0x00030000,
		GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON),
		LR_DEFAULTCOLOR);

	return FALSE;
}

static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam) {
	DWORD dwProcessId;

	GetWindowThreadProcessId(hWnd, &dwProcessId);

	if (dwProcessId == g_piClient.dwProcessId && GetWindowTextLength(hWnd)) {
		g_hClientWindow = hWnd;
		return FALSE;
	}

	return TRUE;
}

BOOL GetClientWindow() {
	if (!g_hClientWindow) {
		EnumWindows((WNDENUMPROC)EnumWindowsProc, 0);
	}

	return TRUE;
}

static BOOL WorkoutCmdLine(LPSTR lpCmdLine) {
	LPSTR szApp;
	LPSTR szParams;
	INT nLength = (INT)strlen(lpCmdLine);
	LPSTR p;
	LPSTR p2;
	BOOL fHadQuotes;
	DWORD dwResult;
	LPSTR szAppExp;

	szApp = (LPSTR)malloc(nLength + 1);

	if (!szApp) {
		return FALSE;
	}

	p = lpCmdLine;
	fHadQuotes = FALSE;

	while (isspace(*p)) {
		p++;
	}

	if (!*p) {
		return FALSE;
	}

	if (*p == '"') {
		p++;
		fHadQuotes = TRUE;
	}

	p2 = szApp;

	if (fHadQuotes) {
		while (*p != '\0' && *p != '"') {
			*p2++ = *p++;
		}

		if (!*p) {
			return FALSE;
		}

		p++;
	} else {
		while (*p != '\0' && !isspace(*p)) {
			*p2++ = *p++;
		}
	}

	*p2 = '\0';

	if (!*szApp) {
		return FALSE;
	}

	while (isspace(*p)) {
		p++;
	}

	szParams = p;

	szAppExp = (LPSTR)malloc(MAX_PATH + 1);
	dwResult = ExpandEnvironmentStrings(szApp, szAppExp, MAX_PATH + 1);

	if (dwResult > MAX_PATH + 1) {
		szAppExp = (LPSTR)realloc(szAppExp, dwResult + 1);
		dwResult = ExpandEnvironmentStrings(szApp, szAppExp, dwResult + 1);
	}

	free(szApp);

	if (dwResult == 0) {
		return FALSE;
	}

	g_szClientAppName = szAppExp;

	g_lpCmdLine = (LPSTR)malloc(strlen(szAppExp) + strlen(szParams) + 4);

	wsprintf(g_lpCmdLine, "\"%s\" %s", szAppExp, szParams);

	return TRUE;
}

static BOOL ShowManual() {
	CHAR szPath[MAX_PATH + 1];

	ExpandEnvironmentStrings("%WINDIR%\\TrayRun.chm", szPath, _ARRAYSIZE(szPath));
	ShellExecute(g_hWnd, "open", szPath, NULL, NULL, SW_SHOW);

	return TRUE;
}
