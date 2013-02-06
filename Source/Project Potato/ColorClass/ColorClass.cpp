#include "ColorClass.h"

ColorShaderClass::ColorShaderClass()
{
	m_VertexShader = 0;
	m_PixelShader = 0;
	m_Layout = 0;
	m_MatrixBuffer = 0;
}

ColorShaderClass::ColorShaderClass(const ColorShaderClass &_Other)
{
}

ColorShaderClass::~ColorShaderClass()
{
}

bool ColorShaderClass::Initialize(ID3D11Device *_Device, HWND _Hwnd)
{
	bool Result;

	Result = InitializeShader(_Device, _Hwnd, L"./Shaders/color.vsh", L"./Shaders/color.psh");
	if(!Result)
	{
		return false;
	}
	
	return true;
}

void ColorShaderClass::Shutdown()
{
	ShutdownShader();
	return;
}

bool ColorShaderClass::Render(ID3D11DeviceContext *_DeviceContext, int _IndexCount, D3DXMATRIX _WorldMatrix, D3DXMATRIX _ViewMatrix, D3DXMATRIX _ProjectionMatrix)
{
	bool Result;

	Result = SetShaderParameters(_DeviceContext, _WorldMatrix, _ViewMatrix, _ProjectionMatrix);
	if(!Result)
	{
		return false;
	}

	RenderShader(_DeviceContext, _IndexCount);

	return true;
}

bool ColorShaderClass::InitializeShader(ID3D11Device *_Device, HWND _Hwnd, WCHAR *_VSFilename, WCHAR *_PSFilename)
{
	HRESULT Result;
	ID3D10Blob *ErrorMessage;
	ID3D10Blob *VertexShaderBuffer;
	ID3D10Blob *PixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC PolygonLayout[2];
	unsigned int NumElements;
	D3D11_BUFFER_DESC MatrixBufferDesc;

	ErrorMessage = 0;
	VertexShaderBuffer = 0;
	PixelShaderBuffer = 0;

	Result = D3DX11CompileFromFile(_VSFilename, NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS,0,NULL, &VertexShaderBuffer, &ErrorMessage, NULL);
	if(FAILED(Result))
	{
		if(ErrorMessage)
		{
			OutputShaderErrorMessage(ErrorMessage, _Hwnd, _VSFilename);
		}
		else
		{
			MessageBox(_Hwnd, _VSFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	Result = D3DX11CompileFromFile(_PSFilename, NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &PixelShaderBuffer, &ErrorMessage, NULL);

	if(FAILED(Result))
	{
		if(ErrorMessage)
		{
			OutputShaderErrorMessage(ErrorMessage, _Hwnd, _PSFilename);
		}
		else
		{
			MessageBox(_Hwnd, _VSFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	Result = _Device->CreateVertexShader(VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), NULL, &m_VertexShader);
	if(FAILED(Result))
	{
		return false;
	}

	Result = _Device->CreatePixelShader(PixelShaderBuffer->GetBufferPointer(), PixelShaderBuffer->GetBufferSize(), NULL, &m_PixelShader);
	if(FAILED(Result))
	{
		return false;
	}

	PolygonLayout[0].SemanticName = "POSITION";
	PolygonLayout[0].SemanticIndex = 0;
	PolygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	PolygonLayout[0].InputSlot = 0;
	PolygonLayout[0].AlignedByteOffset = 0;
	PolygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLayout[0].InstanceDataStepRate = 0;

	PolygonLayout[0].SemanticName = "COLOR";
	PolygonLayout[0].SemanticIndex = 0;
	PolygonLayout[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	PolygonLayout[0].InputSlot = 0;
	PolygonLayout[0].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	PolygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLayout[0].InstanceDataStepRate = 0;

	NumElements = sizeof(PolygonLayout) / sizeof(PolygonLayout[0]);
	Result = _Device->CreateInputLayout(PolygonLayout, NumElements, VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), &m_Layout);
	if(FAILED(Result))
	{
		return false;
	}

	VertexShaderBuffer->Release();
	VertexShaderBuffer = 0;

	PixelShaderBuffer->Release();
	PixelShaderBuffer = 0;

	MatrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	MatrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	MatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	MatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	MatrixBufferDesc.MiscFlags = 0;
	MatrixBufferDesc.StructureByteStride = 0;

	Result = _Device->CreateBuffer(&MatrixBufferDesc, NULL, &m_MatrixBuffer);

	if(FAILED(Result))
	{
		return false;
	}


	return true;
}

void ColorShaderClass::ShutdownShader()
{
	if(m_MatrixBuffer)
	{
		m_MatrixBuffer->Release();
		m_MatrixBuffer = 0;
	}

	if(m_Layout)
	{
		m_Layout->Release();
		m_Layout = 0;
	}

	if(m_PixelShader)
	{
		m_PixelShader->Release();
		m_PixelShader = 0;
	}

	if(m_VertexShader)
	{
		m_VertexShader->Release();
		m_VertexShader = 0;
	}
}

void ColorShaderClass::OutputShaderErrorMessage(ID3D10Blob *_ErrorMessage, HWND _Hwnd, WCHAR *_ShaderFilename)
{
	char *CompileErrors;
	unsigned long BufferSize, i;
	ofstream FOut;

	CompileErrors = (char * )(_ErrorMessage->GetBufferPointer());
	BufferSize = _ErrorMessage->GetBufferSize();

	FOut.open("Shader-Errors.txt");
	for(i=0; i < BufferSize; i++)
	{
		FOut << CompileErrors[i];
	}
	FOut.close();
	_ErrorMessage->Release();
	_ErrorMessage = 0;

	MessageBox(_Hwnd, L"Error compiling shader. Check Shader-Errors.txt", _ShaderFilename, MB_OK);
	return;
}

bool ColorShaderClass::SetShaderParameters(ID3D11DeviceContext *_DeviceContext, D3DXMATRIX _WorldMatrix, D3DXMATRIX _ViewMatrix, D3DXMATRIX _ProjectionMatrix)
{
	HRESULT Result;
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	MatrixBufferType *DataPtr;
	unsigned int BufferNumber;

	D3DXMatrixTranspose(&_WorldMatrix, &_WorldMatrix);
	D3DXMatrixTranspose(&_ViewMatrix, &_ViewMatrix);
	D3DXMatrixTranspose(&_ProjectionMatrix, &_ProjectionMatrix);

	Result = _DeviceContext->Map(m_MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	if(FAILED(Result))
	{
		return false;
	}

	DataPtr = (MatrixBufferType*)MappedResource.pData;
	DataPtr->World = _WorldMatrix;
	DataPtr->View = _ViewMatrix;
	DataPtr->Projection = _ProjectionMatrix;

	_DeviceContext->Unmap(m_MatrixBuffer,0);

	BufferNumber = 0;
	_DeviceContext->VSSetConstantBuffers(BufferNumber, 1, &m_MatrixBuffer);

	return true;
}

void ColorShaderClass::RenderShader(ID3D11DeviceContext *_DeviceContext, int _IndexCount)
{
	_DeviceContext->IASetInputLayout(m_Layout);
	_DeviceContext->VSSetShader(m_VertexShader, NULL, 0);
	_DeviceContext->PSSetShader(m_PixelShader, NULL, 0);
	_DeviceContext->DrawIndexed(_IndexCount,0,0);
	return;
}

