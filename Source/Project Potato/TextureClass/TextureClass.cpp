#include "TextureClass.h"

TextureClass::TextureClass()
{
	m_Texture = 0;
}

TextureClass::TextureClass(const TextureClass &_Other)
{
}

TextureClass::~TextureClass()
{
}

bool TextureClass::Initialize(ID3D11Device *_Device, WCHAR *_Filename)
{
	HRESULT Result;

	Result = D3DX11CreateShaderResourceViewFromFile(_Device, _Filename, NULL, NULL, &m_Texture, NULL);
	if(FAILED(Result))
	{
		return false;
	}

	return true;
}

void TextureClass::Shutdown()
{
	if(m_Texture)
	{
		m_Texture->Release();
		m_Texture = 0;
	}
	
	return;
}

ID3D11ShaderResourceView * TextureClass::GetTexture()
{
	return m_Texture;
}