#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_TextureShader = 0;
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
		MessageBox(_Handle, L"Could not initialize Direct3d", L"Error", MB_OK);
		return false;
	}

	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	m_Camera->SetPosition(0.0f,0.0f,-10.0f);

	m_Model = new ModelClass;
	if(!m_Model)
	{
		return false;
	}

	Result = m_Model->Initialize(m_D3D->GetDevice(), L"./data/seafloor.dds");
	if(!Result)
	{
		MessageBox(_Handle, L"Could not initialize Model Object.", L"Error", MB_OK);
		return false;
	}
	
	m_TextureShader = new TextureShaderClass;
	if(!m_TextureShader)
	{
		return false;
	}
	
	Result = m_TextureShader->Initialize(m_D3D->GetDevice(), _Handle);
	if(!Result)
	{
		MessageBox(_Handle, L"Could not Initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}
	return true;
}

void GraphicsClass::Shutdown()
{
	if(m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	if(m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

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
	D3DXMATRIX ViewMatrix, ProjectionMatrix, WorldMatrix;
	bool Result;

	m_D3D->BeginScene(0.0f,0.0f,0.0f,1.0f);
	m_Camera->Render();

	m_Camera->GetViewMatrix(ViewMatrix);
	m_D3D->GetWorldMatrix(WorldMatrix);
	m_D3D->GetProjectionMatrix(ProjectionMatrix);

	m_Model->Render(m_D3D->GetDeviceContext());

	Result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), WorldMatrix, ViewMatrix, ProjectionMatrix, m_Model->GetTexture());
	if(!Result)
	{
		return false;
	}

	m_D3D->EndScene();

	return true;
}