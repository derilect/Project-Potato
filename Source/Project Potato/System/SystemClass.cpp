#include "SystemClass.h"

SystemClass::SystemClass()
{
	m_Input = 0;
	m_Graphics = 0;
}

SystemClass::SystemClass(const SystemClass &_Other)
{
}

SystemClass::~SystemClass()
{
}

bool SystemClass::Initialize()
{
	int ScreenWidth, ScreenHeight;
	bool Result;

	ScreenHeight = 0;
	ScreenWidth = 0;

	InitializeWindows(ScreenWidth, ScreenHeight);
	m_Input = new InputClass;
	
	if(!m_Input)
	{
		return false;
	}

	m_Input->Initialize();

	m_Graphics = new GraphicsClass;
	
	if(!m_Graphics)
	{
		return false;
	}
	Result = m_Graphics->Initialize(ScreenWidth, ScreenHeight, m_WindowHandle);

	if(!Result)
	{
		return false;
	}

	return true;

}

void SystemClass::Shutdown()
{
	if(m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	if(m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	ShutdownWindows();
}


void SystemClass::Run()
{
	MSG Message;
	bool Done, Result;

	ZeroMemory(&Message, sizeof(MSG));

	Done = false;

	while(!Done)
	{
		if(PeekMessage(&Message, NULL, 0,0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		if(Message.message == WM_QUIT)
		{
			Done = true;
		}
		else
		{
			Result = Frame();

			if(!Result)
			{
				Done = true;
			}
		}
	}
	
	return;
}

bool SystemClass::Frame()
{
	bool Result;

	if(m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	Result = m_Graphics->Frame();

	if(!Result)
	{
		return false;
	}

	return true;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND _Handle, UINT _Message, WPARAM _WParam, LPARAM _LParam)
{
	switch (_Message)
	{
	case WM_KEYDOWN:
		{
			m_Input->KeyDown((unsigned int)_WParam);
			return 0;
		}

	case WM_KEYUP:
		{
			m_Input->KeyUp((unsigned int)_WParam);
			return 0;
		}

	default:
		{
			return DefWindowProc(_Handle, _Message, _WParam, _LParam);
		}
	}
}

void SystemClass::InitializeWindows(int &_ScreenWidth, int &_ScreenHeight)
{
	WNDCLASSEX WindowClass;
	DEVMODE ScreenSettings;
	int PosX, PosY;

	ApplicationHandle = this;
	m_Instance = GetModuleHandle(NULL);
	m_ApplicationName = L"Project Potato";

	WindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	WindowClass.lpfnWndProc = WndProc;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = m_Instance;
	WindowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	WindowClass.hIconSm = WindowClass.hIcon;
	WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WindowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WindowClass.lpszMenuName = NULL;
	WindowClass.lpszClassName = m_ApplicationName;
	WindowClass.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&WindowClass);

	_ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	_ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	if(FULL_SCREEN)
	{
		memset(&ScreenSettings, 0, sizeof(ScreenSettings));
		ScreenSettings.dmSize = sizeof(ScreenSettings);
		ScreenSettings.dmPelsWidth = (unsigned long)_ScreenWidth;
		ScreenSettings.dmPelsHeight = (unsigned long)_ScreenHeight;
		ScreenSettings.dmBitsPerPel = 32;
		ScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&ScreenSettings, CDS_FULLSCREEN);

		PosX = PosY = 0;
	}

	else
	{
		_ScreenWidth = 800;
		_ScreenHeight = 600;

		PosX = (GetSystemMetrics(SM_CXSCREEN) - _ScreenWidth) / 2;
		PosY = (GetSystemMetrics(SM_CYSCREEN) - _ScreenHeight) / 2;
	}


	m_WindowHandle = CreateWindowEx(WS_EX_APPWINDOW, m_ApplicationName, m_ApplicationName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, PosX, PosY, _ScreenWidth, _ScreenHeight,
		NULL,NULL, m_Instance, NULL);

	ShowWindow(m_WindowHandle, SW_SHOW);
	SetForegroundWindow(m_WindowHandle);
	SetFocus(m_WindowHandle);

	ShowCursor(false);
}

void SystemClass::ShutdownWindows()
{
	ShowCursor(true);

	if(FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL,0);
	}

	DestroyWindow(m_WindowHandle);
	m_WindowHandle = NULL;

	UnregisterClass(m_ApplicationName, m_Instance);
	m_Instance = NULL;

	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND _Handle, UINT _Message, WPARAM _WParam, LPARAM _LParam)
{
	switch(_Message)
	{
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
	default:
		{
			return ApplicationHandle->MessageHandler(_Handle, _Message, _WParam, _LParam);
		}
	}
}