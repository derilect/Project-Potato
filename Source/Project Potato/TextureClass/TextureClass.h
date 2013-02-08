#ifndef _TEXTURECLASS_H
#define _TEXTURECLASS_H

#include <D3D11.h>
#include <D3DX11tex.h>

class TextureClass
{
public:
	TextureClass();
	TextureClass(const TextureClass &);
	~TextureClass();

	bool Initialize(ID3D11Device *, WCHAR *);
	void Shutdown();

	ID3D11ShaderResourceView *GetTexture();

private:
	ID3D11ShaderResourceView *m_Texture;
};

#endif