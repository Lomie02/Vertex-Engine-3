#pragma once

#include "Application.h"
#include <iostream>
#include "Scene.h"
#include "GameObject.h"
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
	std::shared_ptr<VertexEngine::Scene> m_Scene;
	std::weak_ptr<VertexEngine::GameObject> m_MyObject;
};