#include <Engin\Engin.h>
//#include "CameraTestScene.h"
#include "Pseudo3D.h"

int main(int argc, char** argv)
{
	Engin::Engin engine;

	engine.init("resources/engine.ini");
	engine.run(new Engin::Game::Pseudo3D(&engine));
	engine.quit();

	return 0;
}