#include "System\SystemClass.h"

int WINAPI WinMain(HINSTANCE _Instance, HINSTANCE _PrevInstance, PSTR _CommandLine, int _CommandShow)
{
	bool Result;

	System = new SystemClass;

	if(!System)
	{
		return 0;
	}

	Result = System->Initialize();
	if(Result)
	{
		System->Run();
	}

	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}