 /* BoCA - BonkEnc Component Architecture
  * Copyright (C) 2007-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "dllinterface.h"

Array<DynamicLoader *>	 winamp_out_plugins;
Array<Out_Module *>	 winamp_out_modules;

Bool LoadWinampDLLs()
{
	MoveFileA(GUI::Application::GetApplicationDirectory().Append("plugins\\plugins.ini"), GUI::Application::GetApplicationDirectory().Append("BonkEnc.ini"));

	Directory		 directory = GUI::Application::GetApplicationDirectory().Append("plugins\\");
	const Array<File>	&out_dlls   = directory.GetFilesByPattern("out_*.dll");

	for (Int i = 0; i < out_dlls.Length(); i++)
	{
		DynamicLoader	*dll = new DynamicLoader(out_dlls.GetNth(i));

		if (dll != NIL)
		{
			Out_Module *(*proc)() = (Out_Module *(*)()) dll->GetFunctionAddress("winampGetOutModule");

			if (proc != NIL)
			{
				winamp_out_plugins.Add(dll);
				winamp_out_modules.Add(proc());

				proc()->hDllInstance = (HINSTANCE) dll->GetSystemModuleHandle();
				proc()->Init();
			}
			else
			{
				Object::DeleteObject(dll);
			}
		}
	}

	return True;
}

Void FreeWinampDLLs()
{
	for (Int j = 0; j < winamp_out_plugins.Length(); j++)
	{
		winamp_out_modules.GetNth(j)->Quit();

		Object::DeleteObject(winamp_out_plugins.GetNth(j));
	}

	winamp_out_plugins.RemoveAll();
	winamp_out_modules.RemoveAll();

	MoveFileA(GUI::Application::GetApplicationDirectory().Append("BonkEnc.ini"), GUI::Application::GetApplicationDirectory().Append("plugins\\plugins.ini"));
}
