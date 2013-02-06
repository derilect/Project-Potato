#ifndef _MODELCLASS_H
#define _MODELCLASS_H

#include <d3d11.h>
#include <D3DX10math.h>

class ModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 Position;
		D3DXVECTOR4 Color;
	};

public:
	ModelClass();
	ModelClass(const ModelClass &);
	~ModelClass();

	bool Initialize(ID3D11Device *);
	void Shutdown();
	void Render(ID3D11DeviceContext *);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device *);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext *);

private:
	ID3D11Buffer *m_VertexBuffer, *m_IndexBuffer;
	int m_VertexCount, m_IndexCount;
};

#endif