
#include "EntryPoint.h"

int main() {

	auto program = CreateApp();
	if (!program) {
		std::cout << "VertexError: Application was nullptr! Exiting Engine." << std::endl;
		return EXIT_FAILURE;
	}
	else {
		std::cout << "VertexLog: Application Init, starting setup." << std::endl;
	}

	program->Execute();
	delete program;
}
