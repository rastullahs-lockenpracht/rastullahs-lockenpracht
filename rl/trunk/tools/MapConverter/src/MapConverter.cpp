/*
Half-Life MAP to Quake3 Map Conversion Utility
Copyright (C) 2004  Henrik Hinrichs

based on the
Half-Life MAP viewing utility.
Copyright (C) 2003  Ryan Samuel Gregg

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "MapConverterPrerequisites.h"
#include "MainForm.h"

using namespace MapConverter;

// This is the entry point for this application
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	String *sArgs[] = (new String(lpCmdLine))->Replace("\"\"", "\"")->Replace("\" \"", "\n")->Replace("\" ", "\n")->Replace(" \"", "\n")->Replace("\"", "")->Split(S"\n"->ToCharArray());

	MainForm *mainForm = new MainForm(); //(sArgs->Length == 1 ? sArgs[0] : S""), Config);

	Application::Run(mainForm);

    return 0;
}