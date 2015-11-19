#include <Engin\Engin.h>
#include "CameraTestScene.h"
#include "GameObjectTestScene.h"

int main(int argc, char** argv)
{
	Engin::Engin engine;

	engine.init("resources/engine.ini");
	engine.run(new GameObjectTestScene(&engine));
	//engine.run(new Engin::Game::CameraTestScene(&engine));
	engine.quit();

	return 0;
}