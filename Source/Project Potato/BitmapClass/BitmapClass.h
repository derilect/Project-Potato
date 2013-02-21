#ifndef _BITMAPCLASS_H
#define _BITMAPCLASS_H

#include <D3D11.h>
#include <D3DX10math.h>

#include "../TextureClass/TextureClass.h"

class BitmapClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 Position;
		D3DXVECTOR2 Texture;
	};

public:
	BitmapClass();
	BitmapClass(const BitmapClass &);
	~BitmapClass();

	bool Initialize(ID3D11Device *, int, int, WCHAR *, int, int);
	void Shutdown();
	bool Render(ID3D11DeviceContext *, int, int);

	int GetIndexCount();
	ID3D11ShaderResourceView *GetTexture();

private:
	bool InitializeBuffers(ID3D11Device *);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext *, int, int);
	void RenderBuffers(ID3D11DeviceContext *);

	bool LoadTexture(ID3D11Device *, WCHAR *);
	void ReleaseTexture();
	
private:
	ID3D11Buffer *m_VertexBuffer, *m_IndexBuffer;
	int m_VertexCount, m_IndexCount;
	TextureClass *m_Texture;
	int m_ScreenWidth, m_ScreenHeight;
	int m_BitmapWidth, m_BitmapHeight;
	int m_PreviousX, m_PreviousY;
};

#endif