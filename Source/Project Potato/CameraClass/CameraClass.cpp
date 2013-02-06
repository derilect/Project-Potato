#include "CameraClass.h"

CameraClass::CameraClass()
{
	m_PosX = 0.0f;
	m_PosY = 0.0f;
	m_PosZ = 0.0f;

	m_RotX = 0.0f;
	m_RotY = 0.0f;
	m_RotZ = 0.0f;
}

CameraClass::CameraClass(const CameraClass &_Other)
{
}

CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(float _X, float _Y, float _Z)
{
	m_PosX = _X;
	m_PosY = _Y;
	m_PosZ = _Z;

	return;
}

void CameraClass::SetRotation(float _X, float _Y, float _Z)
{
	m_RotX = _X;
	m_RotY = _Y;
	m_RotZ = _Z;

	return;
}

D3DXVECTOR3 CameraClass::GetPosition()
{
	return D3DXVECTOR3(m_PosX, m_PosY, m_PosZ);
}

D3DXVECTOR3 CameraClass::GetRotation()
{
	return D3DXVECTOR3(m_RotX, m_RotY, m_RotZ);
}

void CameraClass::Render()
{
	D3DXVECTOR3 Up, Position, LookAt;
	float Yaw, Pitch, Roll;
	D3DXMATRIX RotationMatrix;

	Up.x = 0.0f;
	Up.y = 1.0f;
	Up.z = 0.0f;

	Position.x = m_PosX;
	Position.y = m_PosY;
	Position.z = m_PosZ;
	
	LookAt.x = 0.0f;
	LookAt.y = 0.0f;
	LookAt.z = 1.0f;

	Pitch = m_RotX *  0.0174532925f;
	Yaw = m_RotY *  0.0174532925f;
	Roll = m_RotZ *  0.0174532925f;

	D3DXMatrixRotationYawPitchRoll(&RotationMatrix, Yaw, Pitch, Roll);
	D3DXVec3TransformCoord(&LookAt, &LookAt, &RotationMatrix);
	D3DXVec3TransformCoord(&Up, &Up, &RotationMatrix);

	LookAt = Position + LookAt;

	D3DXMatrixLookAtLH(&m_ViewMatrix, &Position, &LookAt, &Up);
	return;
}

void CameraClass::GetViewMatrix(D3DXMATRIX &_ViewMatrix)
{
	_ViewMatrix = m_ViewMatrix;
	return;
}