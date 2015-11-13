#include "GameObjectTestScene.h"

#include <iostream>
#include <Engin\Renderer\Window.h>
#include <Engin\Renderer\Color.h>
#include <glm\gtc\type_ptr.hpp>
#include <Engin\Game\Component.h>
#include <Engin\Game\RigidBody.h>
#include <Engin\Game\Sprite.h>
#include <Engin\Game\Transform.h>

GameObjectTestScene::GameObjectTestScene(Engin::Engin* engine) : gameObject("main_object", &textureBatch)
{
	this->engine = engine;
	alpha = 0.0f;

	camera.initCamera(0.0f, 0.0f, 400.0f, 400.0f, 0.0f, 0.0f, 200, 200);

	camera.setZoomLevel(0.5f);


	gameObject.addComponent<Engin::Game::Transform>();
	gameObject.addComponent<Engin::Game::Sprite>();
	gameObject.addComponent<Engin::Game::RigidBody>();
	gameObject.addComponent<HP_Component>();


	camera.setPositionRotationOrigin(3.0f*32.0f, 9.0f*32.0f);
	shader = Engin::Resources::ResourceManager::getInstance().load<Engin::Resources::ShaderProgram>("resources/shaders/shader");
	textureShader = Engin::Resources::ResourceManager::getInstance().load<Engin::Resources::ShaderProgram>("resources/shaders/texture_shader");
	alphaShader = Engin::Resources::ResourceManager::getInstance().load<Engin::Resources::ShaderProgram>("resources/shaders/alpha_shader");
	batch.init(shader, 4096);
	textureBatch.setShader(textureShader);
	alphaTextureBatch.setShader(alphaShader);
	alphaTextureBatch.setSortMode(Engin::Renderer::TextureSortMode::FrontToBack);

	doge = Engin::Resources::ResourceManager::getInstance().load<Engin::Resources::Texture>("resources/doge.png");

	gameObject.accessComponent<Engin::Game::Sprite>()->setCurrentSprite(doge);
}

GameObjectTestScene::~GameObjectTestScene()
{
	std::cout << "GaneObject scene shutdown" << std::endl;
}

void GameObjectTestScene::update(GLfloat step)
{
	static float zoomByInput = 1.0f;
	if (engine->mouseInput->mouseWheelWasMoved(Engin::HID::MOUSEWHEEL_UP))
	{
		if (zoomByInput > 0.0f)
			zoomByInput -= glm::radians(2.0f);
	}
	if (engine->mouseInput->mouseWheelWasMoved(Engin::HID::MOUSEWHEEL_DOWN))
	{
		zoomByInput += glm::radians(2.0f);
	}

	static float moveByInputX = 0.0f;
	static float moveByInputY = 0.0f;
	static float moveSpeed = 64.0f;
	if (engine->keyboardInput->keyWasPressed(Engin::HID::KEYBOARD_W))
	{
		moveByInputY += moveSpeed;
	}
	if (engine->keyboardInput->keyWasPressed(Engin::HID::KEYBOARD_A))
	{
		moveByInputX -= moveSpeed;
	}
	if (engine->keyboardInput->keyWasPressed(Engin::HID::KEYBOARD_S))
	{
		moveByInputY -= moveSpeed;
	}
	if (engine->keyboardInput->keyWasPressed(Engin::HID::KEYBOARD_D))
	{
		moveByInputX += moveSpeed;
	}
	gameObject.accessComponent<Engin::Game::Transform>()->setXPosition(moveByInputX);
	gameObject.accessComponent<Engin::Game::Transform>()->setYPosition(moveByInputY);
			
	static float rotateByInput = 0.0f;
	static float rotateSpeed = 1.0f;
	if (engine->keyboardInput->keyIsPressed(Engin::HID::KEYBOARD_LEFT))
	{
		rotateByInput += rotateSpeed;
	}
	if (engine->keyboardInput->keyIsPressed(Engin::HID::KEYBOARD_RIGHT))
	{
		rotateByInput -= rotateSpeed;
	}
	gameObject.accessComponent<Engin::Game::Transform>()->setRotation(rotateByInput);
}

void GameObjectTestScene::draw()
{
	textureBatch.begin();
	alphaTextureBatch.begin();

	gameObject.accessComponent<Engin::Game::Sprite>()->draw();

	textureBatch.end();
	alphaTextureBatch.end();

	camera.activateViewPort();
	batch.flush(camera);
	textureBatch.flush(camera);
	alphaTextureBatch.flush(camera);

	textureBatch.clear();
	alphaTextureBatch.clear();
	batch.clear();
}
