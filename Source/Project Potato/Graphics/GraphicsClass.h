#ifndef _GRAPHICSCLASS_H
#define _GRAPHICSCLASS_H

#include "../D3DClass/D3DClass.h"
#include "../CameraClass/CameraClass.h"
#include "../ModelClass/ModelClass.h"
#include "../ColorClass/ColorClass.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass &);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:
	D3DClass *m_D3D;
	CameraClass *m_Camera;
	ModelClass *m_Model;
	ColorShaderClass *m_ColorShader;
};

#endif