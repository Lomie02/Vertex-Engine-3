#include "Application.h"
#include <iostream>
class SandboxApp : public VertexEngine::Application
{
public:
	SandboxApp() {}
	~SandboxApp() {}

protected:
	void OnUpdate() override {

	}

};

VertexEngine::Application* CreateApp() {
	
	std::cout << "App Create called in sandbox" << std::endl;
	return new SandboxApp();
}
