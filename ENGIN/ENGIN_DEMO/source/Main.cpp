#include <Engin\Engin.h>
#include "CameraTestScene.h"

int main(int argc, char** argv)
{
	Engin::Engin engine;

	engine.init("resources/engine.ini");
	engine.run(new Engin::Game::CameraTestScene(&engine));
	engine.quit();

	return 0;
}