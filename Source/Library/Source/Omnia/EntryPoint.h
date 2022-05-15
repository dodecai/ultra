#pragma once

#include "Omnia/Omnia.pch"
#include "Omnia/Core.h"

extern Omnia::Application *CreateApplication();

int main(int argc, char **argv) {
	// Preparation
	#ifdef APP_PLATFORM_WINDOWS
		system("chcp 65001 >nul"); // Switch to UTF-8 codepage
	#endif

	// Initialization
	auto app = CreateApplication();

	// Main
	app->Run();
	
	// Termination
	delete app;
	return 0;
}
