#include "LightClass.h"

LightClass::LightClass()
{
}

LightClass::LightClass(const LightClass &_Other)
{
}

LightClass::~LightClass()
{
}

void LightClass::SetDiffuseColor(float _R, float _G, float _B, float _A)
{
	m_DiffuseColor = D3DXVECTOR4(_R,_G,_B,_A);
	return;
}

void LightClass::SetSpecularColor(float _R, float _G, float _B, float _A)
{
	m_SpecularColor = D3DXVECTOR4(_R,_G,_B,_A);
	return;
}

void LightClass::SetSpecularPower(float _Power)
{
	m_SpecularPower = _Power;
	return;
}

void LightClass::SetLightDirection(float _X, float _Y, float _Z)
{
	m_LightDirection = D3DXVECTOR3(_X,_Y,_Z);
	return;
}

void LightClass::SetAmbientColor(float _R, float _G, float _B, float _A)
{
	m_AmbientColor = D3DXVECTOR4(_R,_G,_B,_A);
}

D3DXVECTOR4 LightClass::GetDiffuseColor()
{
	return m_DiffuseColor;
}

D3DXVECTOR3 LightClass::GetDirection()
{
	return m_LightDirection;
}

D3DXVECTOR4 LightClass::GetAmbientColor()
{
	return m_AmbientColor;
}

D3DXVECTOR4 LightClass::GetSpecularColor()
{
	return m_SpecularColor;
}

float LightClass::GetSpecularPower()
{
	return m_SpecularPower;
}