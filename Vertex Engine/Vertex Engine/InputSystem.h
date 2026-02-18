#pragma once
#include "InputAPI.h"
#include "Window.h"
class InputSystem : public VertexEngine::InputAPI
{
public:
	InputSystem(VertexEngine::Window& _GameWindow);
	virtual bool GetKeyDown(int _key) {};
	virtual bool GetKeyUp(int _key) {};
	virtual bool GetKey(int _key) {};

	// Engine access functions
private:
	void Init();
	VertexEngine::Window& m_GameWindow;

};

