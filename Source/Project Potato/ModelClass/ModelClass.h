#ifndef _MODELCLASS_H
#define _MODELCLASS_H

#include <D3D11.h>
#include <D3DX10math.h>

#include "../TextureClass/TextureClass.h"

class ModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 Position;
		D3DXVECTOR2 Texture;
	};

public:
	ModelClass();
	ModelClass(const ModelClass &);
	~ModelClass();

	bool Initialize(ID3D11Device *, WCHAR *);
	void Shutdown();
	void Render(ID3D11DeviceContext *);

	int GetIndexCount();

	ID3D11ShaderResourceView *GetTexture();

private:
	bool InitializeBuffers(ID3D11Device *);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext *);

	bool LoadTexture(ID3D11Device *, WCHAR *);
	void ReleaseTexture();

private:
	ID3D11Buffer *m_VertexBuffer, *m_IndexBuffer;
	int m_VertexCount, m_IndexCount;
	TextureClass *m_Texture;
};

#endif