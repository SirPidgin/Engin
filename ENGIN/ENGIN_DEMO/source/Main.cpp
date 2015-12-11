#include <Engin\Engin.h>
#include "TitleScene.h"

int main(int argc, char** argv)
{
	Engin::Engin engine;

	engine.init("resources/engine.ini");
	engine.run(new Engin::Game::TitleScene(&engine));
	engine.quit();

	return 0;
}