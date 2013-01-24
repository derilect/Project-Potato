#include "InputClass.h"

InputClass::InputClass()
{
}

InputClass::InputClass(const InputClass &_Other)
{
}

InputClass::~InputClass()
{
}

void InputClass::Initialize()
{
	for(int Index = 0; Index < 256; Index++)
	{
		m_Keys[Index] = false;
	}

	return;
}

void InputClass::KeyDown(unsigned int _Input)
{
	m_Keys[_Input] = true;
	return;
}

void InputClass::KeyUp(unsigned int _Input)
{
	m_Keys[_Input] = false;
}

bool InputClass::IsKeyDown(unsigned int _Key)
{
	return m_Keys[_Key];
}