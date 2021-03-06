#ifndef _SYSTEMCLASS_H
#define _SYSTEMCLASS_H

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "../Input/InputClass.h"
#include "../Graphics/GraphicsClass.h"


class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass &);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_ApplicationName;
	HINSTANCE m_Instance;
	HWND m_WindowHandle;

	InputClass *m_Input;
	GraphicsClass *m_Graphics;

};

// Globals
static SystemClass *ApplicationHandle = 0;

// Function Prototypes0
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


#endif