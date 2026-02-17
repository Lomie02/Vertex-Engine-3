
#include "EntryPoint.h"

int main() {

	auto program = CreateApp(); // Create the app & inject the sandbox app

	if (!program) {
		std::cout << "VertexError: Application was nullptr! Exiting Engine." << std::endl;
		return EXIT_FAILURE;
	}
	else {
		std::cout << "VertexLog: Application Init, starting setup." << std::endl;
	}

	program->Execute(); // Run the engine

	delete program; // delete the engine & clean up
}
