#pragma once

#include "Application.h"
#include <iostream>
class SandboxApp : public VertexEngine::Application
{
public:
	SandboxApp() {}
	~SandboxApp() {}

protected:
	void OnAwake() override;
	void OnStart() override;
	void OnUpdate() override;

private:
	float m_Timer;
};