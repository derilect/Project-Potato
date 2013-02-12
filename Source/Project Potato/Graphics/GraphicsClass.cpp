#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_TextureShader = 0;
	m_LightShader = 0;
	m_Light = 0;
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

	Result = m_Model->Initialize(m_D3D->GetDevice(), "./data/cube.txt", L"./data/seafloor.dds");
	if(!Result)
	{
		MessageBox(_Handle, L"Could not initialize Model Object.", L"Error", MB_OK);
		return false;
	}
	

	m_LightShader = new LightShaderClass;
	if(!m_LightShader)
	{
		return false;
	}

	Result = m_LightShader->Initialize(m_D3D->GetDevice(), _Handle);
	if(!Result)
	{
		MessageBox(_Handle, L"Could not initialize light shader object.", L"Error", MB_OK);
		return false;
	}

	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	m_Light->SetDiffuseColor(1.0f,1.0,1.0f,0.0f);
	m_Light->SetLightDirection(0.0f,0.0f,1.0f);
	m_Light->SetAmbientColor(0.15f,0.15f,0.15f,1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);
	

	return true;
}

void GraphicsClass::Shutdown()
{
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	if(m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

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

	static float Rotation = 0.0f;

	Rotation += (float)D3DX_PI * 0.001f;

	if(Rotation > 360.0f)
	{
		Rotation -= 360.0f;
	}

	Result = Render(Rotation);
	if(!Result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render(float _Rotation)
{
	D3DXMATRIX ViewMatrix, ProjectionMatrix, WorldMatrix;
	bool Result;

	m_D3D->BeginScene(0.0f,0.0f,0.0f,1.0f);
	m_Camera->Render();

	m_Camera->GetViewMatrix(ViewMatrix);
	m_D3D->GetWorldMatrix(WorldMatrix);
	m_D3D->GetProjectionMatrix(ProjectionMatrix);

	D3DXMatrixRotationY(&WorldMatrix, _Rotation);

	m_Model->Render(m_D3D->GetDeviceContext());

	Result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), WorldMatrix, ViewMatrix, ProjectionMatrix, m_Model->GetTexture(), m_Light->GetDirection(),m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), 
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if(!Result)
	{
		return false;
	}

	m_D3D->EndScene();

	return true;
}