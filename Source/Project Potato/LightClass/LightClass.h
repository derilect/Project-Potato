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
	void SetLightDirection(float, float, float);

	D3DXVECTOR4 GetDiffuseColor();
	D3DXVECTOR3 GetDirection();

private:
	D3DXVECTOR4 m_DiffuseColor;
	D3DXVECTOR3 m_LightDirection;
};

#endif