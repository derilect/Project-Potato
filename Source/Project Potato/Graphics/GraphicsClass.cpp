#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
{
}

GraphicsClass::GraphicsClass(const GraphicsClass &)
{
}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(int _ScreenWidth, int _ScreenHeight, HWND _Handle)
{
	bool Result;

	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	Result = m_D3D->Initialize(_ScreenWidth, _ScreenHeight, VSYNC_ENABLED, _Handle, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!Result)
	{
		MessageBox(_Handle, L"Could not initialize Direct3d", L"Error", "MB_OK");
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}

bool GraphicsClass::Frame()
{
	bool Result;

	Result = Render();
	if(!Result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render()
{
	m_D3D->BeginScene(0.5f,0.5f,0.5f,1.0f);

	m_D3D->EndScene();

	return true;
}