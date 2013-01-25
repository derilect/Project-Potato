#include "D3DClass.h"

D3DClass::D3DClass()
{
	m_SwapChain = 0;
	m_Device = 0;
	m_DeviceContext = 0;
	m_RenderTargetView = 0;
	m_DepthStencilBuffer = 0;
	m_DepthStencilState = 0;
	m_DepthStencilView = 0;
	m_RasterState = 0;

}

D3DClass::D3DClass(const D3DClass &_Other)
{
}

D3DClass::~D3DClass()
{
}

bool D3DClass::Initialize(int _ScreenWidth, int _ScreenHeight, bool _VSync, HWND _Handle, bool _FullScreen, float _ScreenDepth, float _ScreenNear)
{
	HRESULT Result;
	IDXGIFactory *Factory;
	IDXGIAdapter *Adaptor;
	IDXGIOutput *AdapterOutput;
	unsigned int NumberModes, i, Numerator, Denominator, StringLength;
	DXGI_MODE_DESC *DisplayModeList;
	DXGI_ADAPTER_DESC AdapterDesc;
	int Error;
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	D3D_FEATURE_LEVEL FeatureLevel;
	ID3D11Texture2D *BackBufferPtr;
	D3D11_TEXTURE2D_DESC DepthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC DepthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;
	D3D11_RASTERIZER_DESC RasterDesc;
	D3D11_VIEWPORT ViewPort;
	float FOV, ScreenAspect;

	m_VSync = _VSync;

	Result = CreateDXGIFactory(_uuidof(IDXGIFactory), (void **)&Factory);
	if(FAILED(Result))
	{
		return false;
	}

	Result = Factory->EnumAdapters(0, &Adaptor);
	if(FAILED(Result))
	{
		return false;
	}

	Result = Adaptor->EnumOutputs(0, &AdapterOutput);
	if(FAILED(Result))
	{
		return false;
	}

	Result = AdapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &NumberModes, NULL);
	if(FAILED(Result))
	{
		return false;
	}

	DisplayModeList = new DXGI_MODE_DESC[NumberModes];
	if(!DisplayModeList)
	{
		return false;
	}

	Result = AdapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &NumberModes, DisplayModeList);
	if(FAILED(Result))
	{
		return false;
	}

	for(i = 0; i < NumberModes; i++)
	{
		if(DisplayModeList[i].Width == (unsigned int) _ScreenWidth)
		{
			if(DisplayModeList[i].Height == (unsigned int) _ScreenHeight)
			{
				Numerator = DisplayModeList[i].RefreshRate.Numerator;
				Denominator = DisplayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	Result = Adaptor->GetDesc(&AdapterDesc);
	if(FAILED(Result))
	{
		return false;
	}

	m_VideoCardMemory = (int)(AdapterDesc.DedicatedVideoMemory / 1024 / 1024);

	Error = wcstombs_s(&StringLength, m_VideoCardDescription, 128, AdapterDesc.Description, 128);
	if(Error != 0)
	{
		return false;
	}

	delete [] DisplayModeList;
	DisplayModeList = 0;

	AdapterOutput->Release();
	AdapterOutput = 0;

	Adaptor->Release();
	Adaptor = 0;

	Factory->Release();
	Factory = 0;

	ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));
	SwapChainDesc.BufferCount = 1;

	SwapChainDesc.BufferDesc.Width = _ScreenWidth;
	SwapChainDesc.BufferDesc.Height = _ScreenHeight;

	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

	if(m_VSync)
	{
		SwapChainDesc.BufferDesc.RefreshRate.Numerator = Numerator;
		SwapChainDesc.BufferDesc.RefreshRate.Denominator = Denominator;
	}
	else
	{
		SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.OutputWindow = _Handle;

	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.SampleDesc.Quality = 0;

	if(_FullScreen)
	{
		SwapChainDesc.Windowed = false;
	}
	else
	{
		SwapChainDesc.Windowed = true;
	}

	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	SwapChainDesc.Flags = 0;

	FeatureLevel = D3D_FEATURE_LEVEL_11_0;

	Result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &FeatureLevel, 1, D3D11_SDK_VERSION, &SwapChainDesc, &m_SwapChain, &m_Device, NULL, &m_DeviceContext);
	if(FAILED(Result))
	{
		return false;
	}

	Result = m_SwapChain->GetBuffer(0,__uuidof(ID3D11Texture2D),(LPVOID *)&BackBufferPtr);
	if(FAILED(Result))
	{
		return false;
	}

	Result = m_Device->CreateRenderTargetView(BackBufferPtr, NULL, &m_RenderTargetView);
	if(FAILED(Result))
	{
		return false;
	}

	BackBufferPtr->Release();
	BackBufferPtr = 0;

	ZeroMemory(&DepthBufferDesc, sizeof(DepthBufferDesc));

	DepthBufferDesc.Width = _ScreenWidth;
	DepthBufferDesc.Height = _ScreenHeight;
	DepthBufferDesc.MipLevels = 1;
	DepthBufferDesc.ArraySize = 1;
	DepthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthBufferDesc.SampleDesc.Count = 1;
	DepthBufferDesc.SampleDesc.Quality = 0;
	DepthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	DepthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthBufferDesc.CPUAccessFlags = 0;
	DepthBufferDesc.MiscFlags = 0;

	Result = m_Device->CreateTexture2D(&DepthBufferDesc, NULL, &m_DepthStencilBuffer);
	if(FAILED(Result))
	{
		return false;
	}

	ZeroMemory(&DepthStencilDesc, sizeof(DepthStencilDesc));

	DepthStencilDesc.DepthEnable = true;
	DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	DepthStencilDesc.StencilEnable = true;
	DepthStencilDesc.StencilReadMask = 0xFF;
	DepthStencilDesc.StencilWriteMask = 0xFF;

	DepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	DepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	DepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	DepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	Result = m_Device->CreateDepthStencilState(&DepthStencilDesc, &m_DepthStencilState);
	if(FAILED(Result))
	{
		return false;
	}

	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState, 1);
	
	ZeroMemory(&DepthStencilViewDesc, sizeof(DepthStencilViewDesc));
	DepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DepthStencilViewDesc.Texture2D.MipSlice = 0;

	Result = m_Device->CreateDepthStencilView(m_DepthStencilBuffer, &DepthStencilViewDesc, &m_DepthStencilView);
	if(FAILED(Result))
	{
		return false;
	}

	m_DeviceContext->OMSetRenderTargets(1,&m_RenderTargetView, m_DepthStencilView);

	RasterDesc.AntialiasedLineEnable = false;
	RasterDesc.CullMode = D3D11_CULL_BACK;
	RasterDesc.DepthBias = 0;
	RasterDesc.DepthBiasClamp = 0.0f;
	RasterDesc.DepthClipEnable = true;
	RasterDesc.FillMode = D3D11_FILL_SOLID;
	RasterDesc.FrontCounterClockwise = false;
	RasterDesc.MultisampleEnable = false;
	RasterDesc.ScissorEnable = false;
	RasterDesc.SlopeScaledDepthBias = 0.0f;

	Result = m_Device->CreateRasterizerState(&RasterDesc, &m_RasterState);
	if(FAILED(Result))
	{
		return false;
	}

	m_DeviceContext->RSSetState(m_RasterState);

	ViewPort.Width = (float)_ScreenWidth;
	ViewPort.Height = (float)_ScreenHeight;
	ViewPort.MinDepth = 0.0f;
	ViewPort.MaxDepth = 1.0f;
	ViewPort.TopLeftX = 0.0f;
	ViewPort.TopLeftY = 0.0f;

	m_DeviceContext->RSSetViewports(1, &ViewPort);

	FOV = (float)D3DX_PI / 4.0f;
	ScreenAspect = (float)_ScreenWidth / (float) _ScreenHeight);
	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, FOV, ScreenAspect, _ScreenNear, _ScreenDepth);
	D3DXMatrixIdentity(&m_WorldMatrix);
	D3DXMatrixOrthoLH(&m_OrthoMatrix, (float)_ScreenWidth, (float)_ScreenHeight, _ScreenNear, _ScreenDepth);
	return true;
}


D3DClass::Shutdown()
{
	if(m_SwapChain)
	{
		m_SwapChain->SetFullscreenState(false, NULL);
	}

	if(m_RasterState)
	{
		m_RasterState->Release();
		m_RasterState = 0;
	}

	if(m_DepthStencilView)
	{
		m_DepthStencilView->Release();
		m_DepthStencilView = 0;
	}

	if(m_DepthStencilState)
	{
		m_DepthStencilState->Release();
		m_DepthStencilState = 0;
	}

	if(m_DepthStencilBuffer)
	{
		m_DepthStencilBuffer->Release();
		m_DepthStencilBuffer = 0;
	}

	if(m_RenderTargetView)
	{
		m_RenderTargetView->Release();
		m_RenderTargetView = 0;
	}

	if(m_DeviceContext)
	{
		m_DeviceContext->Release();
		m_DeviceContext = 0;
	}

	if(m_Device)
	{
		m_Device->Release();
		m_Device = 0;
	}

	if(m_SwapChain)
	{
		m_SwapChain->Release();
		m_SwapChain = 0;
	}

	return;
}

bool D3DClass::BeginScene(float _Red, float _Green, float _Blue, float _Alpha)
{
	float Color[4];
	Color[0] = _Red;
	Color[1] = _Green;
	Color[2] = _Blue;
	Color[3] = _Alpha;

	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, Color);
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

void D3DClass::EndScene()
{
	if(m_VSync)
	{
		m_SwapChain->Present(1,0);
	}

	else
	{
		m_SwapChain->Present(0,0,);
	}
	return;
}

ID3D11Device *D3DClass::GetDevice()
{
	return m_Device;
}

ID3D11DeviceContext *D3DClass::GetDeviceContext()
{
	return m_DeviceContext;
}

void D3DClass::GetProjectionmatrix(D3DMATRIX &_ProjectionMatrix)
{
	_ProjectionMatrix = m_ProjectionMatrix;
	return;
}

void D3DClass::GetWorldMatrix(D3DMATRIX &_WorldMatrix)
{
	_WorldMatrix = m_WorldMatrix;
	return;
}

void D3DClass::GetOrthoMatrix(D3DMATRIX &_OrthoMatrix)
{
	_OrthoMatrix = m_OrthoMatrix;
	return;
}

void D3DClass::GetVideoCardInfo(char *_CardName, int &_Memory)
{
	strcpy_s(_CardName, 128, m_VideoCardDescription);
	_Memory = m_VideoCardMemory;
	return;
}

