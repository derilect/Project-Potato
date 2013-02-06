#include "ModelClass.h"

ModelClass::ModelClass()
{
	m_VertexBuffer = 0;
	m_VertexCount = 0;
}

ModelClass::ModelClass(const ModelClass &_Other)
{
}

ModelClass::~ModelClass()
{
}

bool ModelClass::Initialize(ID3D11Device *_Device)
{
	bool Result;

	Result = InitializeBuffers(_Device);
	
	if(!Result)
	{
		return false;
	}

	return true;
}

void ModelClass::Shutdown()
{
	ShutdownBuffers();
	return;
}

void ModelClass::Render(ID3D11DeviceContext *_DeviceContext)
{
	RenderBuffers(_DeviceContext);
	return;
}

int ModelClass::GetIndexCount()
{
	return m_IndexCount;
}

bool ModelClass::InitializeBuffers(ID3D11Device *Device)
{
	VertexType *Vertices;
	unsigned long *Indices;
	D3D11_BUFFER_DESC VertexBufferDesc, IndexBufferDesc;
	D3D11_SUBRESOURCE_DATA VertexData, IndexData;
	HRESULT Result;

	m_VertexCount = 3;

	m_IndexCount = 3;

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

	Vertices[0].Position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	Vertices[0].Color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	Vertices[1].Position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	Vertices[1].Color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	Vertices[2].Position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	Vertices[2].Color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	Indices[0] = 0;
	Indices[1] = 1;
	Indices[2] = 2;

	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.ByteWidth = sizeof(VertexType) * m_VertexCount;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = 0;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	VertexData.pSysMem = Vertices;
	VertexData.SysMemPitch = 0;
	VertexData.SysMemSlicePitch = 0;

	Result = Device->CreateBuffer(&VertexBufferDesc, &VertexData, &m_VertexBuffer);
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

	Result = Device->CreateBuffer(&IndexBufferDesc, &IndexData, &m_IndexBuffer);
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

void ModelClass::ShutdownBuffers()
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

	return;
}


void ModelClass::RenderBuffers(ID3D11DeviceContext *_DeviceContext)
{
	unsigned int Stride;
	unsigned int Offset;

	Stride = sizeof(VertexType);
	Offset = 0;

	_DeviceContext->IASetVertexBuffers(0,1,&m_VertexBuffer, &Stride, &Offset);
	_DeviceContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	_DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	return;
}