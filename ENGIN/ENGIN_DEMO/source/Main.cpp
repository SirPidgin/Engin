#include <Engine.h>
#include <Window.h>
#include <iostream>

#include <GL/glew.h>


int main(int argc, char** argv)
{
	Engin::init();
	Engin::Renderer::Window window;
	window.createWindow("HAHAHAHA", 600, 600, 800, 600, 0);

	Engin::initGlew();
	//std::cin.ignore();

	Engin::helloWorld();
	Engin::quit();
	return 0;
}