#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
{
}

GraphicsClass::GraphicsClass(const GraphicsClass &)
{
}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(int _ScreenWidth, int _ScreenHeight, HWND _Handle)
{
	return true;
}

void GraphicsClass::Shutdown()
{
	return;
}

bool GraphicsClass::Frame()
{
	return true;
}

bool GraphicsClass::Render()
{
	return true;
}