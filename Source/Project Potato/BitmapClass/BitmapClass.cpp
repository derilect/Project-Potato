#include "BitmapClass.h"

BitmapClass::BitmapClass()
{
	m_VertexBuffer = 0;
	m_IndexBuffer = 0;
	m_Texture = 0;
}

BitmapClass::BitmapClass(const BitmapClass &_Other)
{
}

BitmapClass::~BitmapClass()
{
}

bool BitmapClass::Initialize(ID3D11Device *_Device, int _ScreenWidth, int _ScreenHeight, WCHAR *_TextureFilename, int _BitmapWidth, int _BitmapHeight)
{
	bool Result;
	m_ScreenWidth = _ScreenWidth;
	m_ScreenHeight = _ScreenHeight;

	m_BitmapWidth = _BitmapWidth;
	m_BitmapHeight = _BitmapHeight;
	m_PreviousX = -1;
	m_PreviousY = -1;

	Result = InitializeBuffers(_Device);
	if(!Result)
	{
		return false;
	}

	Result = LoadTexture(_Device, _TextureFilename);
	if(!Result)
	{
		return false;
	}

	return true;
}

void BitmapClass::Shutdown()
{
	ReleaseTexture();
	ShutdownBuffers();
	return;
}

bool BitmapClass::Render(ID3D11DeviceContext *_DeviceContext, int _PosX, int _PosY)
{
	bool Result;

	Result = UpdateBuffers(_DeviceContext, _PosX, _PosY);
	if(!Result)
	{
		return false;
	}

	RenderBuffers(_DeviceContext);
	return true;
}

int BitmapClass::GetIndexCount()
{
	return m_IndexCount;
}

ID3D11ShaderResourceView * BitmapClass::GetTexture()
{
	return m_Texture->GetTexture();
}

bool BitmapClass::InitializeBuffers(ID3D11Device *_Device)
{
	VertexType *Vertices;
	unsigned long *Indices;
	D3D11_BUFFER_DESC VertexBufferDesc, IndexBufferDesc;
	D3D11_SUBRESOURCE_DATA VertexData, IndexData;
	HRESULT Result;
	int I;

	m_VertexCount = 6;
	m_IndexCount = m_VertexCount;

	Vertices = new VertexType[m_VertexCount];
	if(!Vertices)
	{
		return false;
	}

	Indices = new unsigned long[m_IndexCount];
	if(!Indices)
	{
		return false;
	}

	memset(Vertices, 0, (sizeof(VertexType) * m_VertexCount));

	for(I=0; I < m_IndexCount; I++)
	{
		Indices[I] = I;
	}

	VertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	VertexBufferDesc.ByteWidth = sizeof(VertexType) * m_VertexCount;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	VertexData.pSysMem = Vertices;
	VertexData.SysMemPitch = 0;
	VertexData.SysMemSlicePitch = 0;

	Result = _Device->CreateBuffer(&VertexBufferDesc, &VertexData, &m_VertexBuffer);
	if(FAILED(Result))
	{
		return false;
	}

	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = sizeof(unsigned long) * m_IndexCount;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;

	IndexData.pSysMem = Indices;
	IndexData.SysMemPitch = 0;
	IndexData.SysMemSlicePitch = 0;

	Result = _Device->CreateBuffer(&IndexBufferDesc, &IndexData, &m_IndexBuffer);
	if(FAILED(Result))
	{
		return false;
	}

	delete [] Vertices;
	Vertices = 0;
	delete [] Indices;
	Indices = 0;
	
	return true;
}

void BitmapClass::ShutdownBuffers()
{
	if(m_IndexBuffer)
	{
		m_IndexBuffer->Release();
		m_IndexBuffer = 0;
	}

	if(m_VertexBuffer)
	{
		m_VertexBuffer->Release();
		m_VertexBuffer = 0;
	}
}

bool BitmapClass::UpdateBuffers(ID3D11DeviceContext *_DeviceContext, int _PosX, int _PosY)
{
	float Left, Right, Top, Bottom;
	VertexType *Vertices;
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	VertexType *VerticesPtr;
	HRESULT Result;

	if((_PosX == m_PreviousX) && (_PosY == m_PreviousY))
	{
		return true;
	}

	m_PreviousX = _PosX;
	m_PreviousY = _PosY;

	Left = (float)((m_ScreenWidth / 2) * -1) + (float)_PosX;
	Right = Left + (float)m_BitmapWidth;
	Top = (float)(m_ScreenHeight / 2) - (float)_PosY;
	Bottom = Top - (float)m_BitmapHeight;

	Vertices = new VertexType[m_VertexCount];
	if(!Vertices)
	{
		return false;
	}

	Vertices[0].Position = D3DXVECTOR3(Left, Top, 0.0f);
	Vertices[0].Texture = D3DXVECTOR2(0.0f, 0.0f);

	Vertices[1].Position = D3DXVECTOR3(Right, Bottom, 0.0f);
	Vertices[1].Texture = D3DXVECTOR2(1.0f, 1.0f);

	Vertices[2].Position = D3DXVECTOR3(Left, Bottom, 0.0f);
	Vertices[2].Texture = D3DXVECTOR2(0.0f, 1.0f);

	Vertices[3].Position = D3DXVECTOR3(Left, Top, 0.0f);
	Vertices[3].Texture = D3DXVECTOR2(0.0f, 0.0f);

	Vertices[4].Position = D3DXVECTOR3(Right, Top, 0.0f);
	Vertices[4].Texture = D3DXVECTOR2(1.0f, 0.0f);

	Vertices[5].Position = D3DXVECTOR3(Right, Bottom, 0.0f);
	Vertices[5].Texture = D3DXVECTOR2(1.0f, 1.0f);

	Result = _DeviceContext->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	if(FAILED(Result))
	{
		return false;
	}

	VerticesPtr = (VertexType *)MappedResource.pData;
	memcpy(VerticesPtr, (void *)Vertices, (sizeof(VertexType) * m_VertexCount));
	_DeviceContext->Unmap(m_VertexBuffer, 0);

	delete [] Vertices;
	Vertices = 0;

	return true;
}

void BitmapClass::RenderBuffers(ID3D11DeviceContext *_DeviceContext)
{
	unsigned int Stride;
	unsigned int Offset;

	Stride = sizeof(VertexType);
	Offset = 0;

	_DeviceContext->IASetVertexBuffers(0,1, &m_VertexBuffer, &Stride, &Offset);
	_DeviceContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return;
}

bool BitmapClass::LoadTexture(ID3D11Device *_Device, WCHAR *_Filename)
{
	bool Result;

	m_Texture = new TextureClass;
	if(!m_Texture)
	{
		return false;
	}

	Result = m_Texture->Initialize(_Device, _Filename);
	if(!Result)
	{
		return false;
	}
	return true;
}

void BitmapClass::ReleaseTexture()
{
	if(m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}
	return;
}