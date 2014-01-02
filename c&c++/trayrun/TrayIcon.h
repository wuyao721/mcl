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

#pragma once

#ifndef H_TRAYICON
#define H_TRAYICON

BOOL TrayIconCreate();
BOOL TrayIconDestroy();
BOOL TrayIconReCreate();
BOOL TrayIconMessage(INT nTrayIconId, UINT uMsg);

#endif // H_TRAYICON
