#ifndef _LightShaderClass_H
#define _LightShaderClassH_H

#include <D3D11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <fstream>

using namespace std;

class LightShaderClass
{
private:
	struct MatrixBufferType
	{
		D3DXMATRIX World;
		D3DXMATRIX View;
		D3DXMATRIX Projection;
	};

	struct CameraBufferType
	{
		D3DXVECTOR3 CameraPosition;
		float Padding;
	};

	struct LightBufferType
	{
		D3DXVECTOR4 AmbientColor;
		D3DXVECTOR4 DiffuseColor;
		D3DXVECTOR3 LightDirection;
		float SpecularPower;
		D3DXVECTOR4 SpecularColor;
	};

public:
	LightShaderClass();
	LightShaderClass(const LightShaderClass &);
	~LightShaderClass();

	bool Initialize(ID3D11Device *, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext *, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView *, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3,	D3DXVECTOR4 , float);

private:
	bool InitializeShader(ID3D11Device *, HWND, WCHAR *, WCHAR *);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob *, HWND, WCHAR *);

	bool SetShaderParameters(ID3D11DeviceContext *, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView *, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3 , D3DXVECTOR4, float);
	void RenderShader(ID3D11DeviceContext *, int);

private:
	ID3D11VertexShader *m_VertexShader;
	ID3D11PixelShader *m_PixelShader;
	ID3D11InputLayout *m_Layout;
	ID3D11Buffer *m_MatrixBuffer;
	ID3D11Buffer *m_LightBuffer;
	ID3D11Buffer *m_CameraBuffer;

	ID3D11SamplerState *m_SampleState;
};



#endif