#ifndef _LIGHTCLASS_H
#define _LIGHTCLASS_H

#include <D3DX10math.h>

class LightClass
{
public:
	LightClass();
	LightClass(const LightClass &);
	~LightClass();

	void SetDiffuseColor(float, float, float, float);
	void SetAmbientColor(float, float, float, float);
	void SetLightDirection(float, float, float);
	void SetSpecularColor(float, float, float, float);
	void SetSpecularPower(float);

	D3DXVECTOR4 GetAmbientColor();
	D3DXVECTOR4 GetDiffuseColor();
	D3DXVECTOR3 GetDirection();
	float GetSpecularPower();
	D3DXVECTOR4 GetSpecularColor();

private:
	D3DXVECTOR4 m_DiffuseColor;
	D3DXVECTOR3 m_LightDirection;
	D3DXVECTOR4 m_AmbientColor;
	D3DXVECTOR4 m_SpecularColor;
	float m_SpecularPower;
};

#endif