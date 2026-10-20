#pragma once
#include <string>
namespace VertexEngine {

	class Application;

	class ApplicationAPI {


	public:
		explicit ApplicationAPI() {};
		explicit ApplicationAPI(Application* _app);

		void Quit(); // Quits application
		bool OpenProgram(std::string _name); // Open a system app

	private:

		Application* m_EngineApplication = nullptr; // Main Application
	};

}