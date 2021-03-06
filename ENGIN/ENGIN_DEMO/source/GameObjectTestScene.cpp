#include "GameObjectTestScene.h"

#include <iostream>
#include <Engin\Renderer\Window.h>
#include <Engin\Renderer\Color.h>
#include <glm\gtc\type_ptr.hpp>
#include <Engin\Game\Component.h>
#include <Engin\Game\RigidBody.h>
#include <Engin\Game\Sprite.h>
#include <Engin\Game\Transform.h>
#include <Engin\Game\ParticleEffect.h>

GameObjectTestScene::GameObjectTestScene(Engin::Engin* engine) : gameObject("main_object", &alphaBatch), camera(createWorldCamera()), useGamePad(false)
{
	this->engine = engine;
	alpha = 0.0f;

	if (engine->gamepadInput->getNumGamepads() > 0)
	{
		useGamePad = true;
	}

	camera->initCamera(0.0f, 0.0f, static_cast<GLfloat>(engine->getWindow().getWindowWidth()), static_cast<GLfloat>(engine->getWindow().getWindowHeight()), 0.0f, 0.0f, engine->getWindow().getWindowWidth() / 2.0f, engine->getWindow().getWindowHeight() / 2.0f);

	camera->setZoomLevel(0.5f);


	gameObject.addComponent<Engin::Game::Sprite>();
	gameObject.addComponent<Engin::Game::Transform>();
	gameObject.addComponent<Engin::Game::RigidBody>();
	gameObject.addComponent<Engin::Game::ParticleEffect>();


	camera->setPositionRotationOrigin(3.0f*32.0f, 9.0f*32.0f);
	shader = Engin::Resources::ResourceManager::getInstance().load<Engin::Resources::ShaderProgram>("resources/shaders/shader");
	textureShader = Engin::Resources::ResourceManager::getInstance().load<Engin::Resources::ShaderProgram>("resources/shaders/texture_shader");
	alphaShader = Engin::Resources::ResourceManager::getInstance().load<Engin::Resources::ShaderProgram>("resources/shaders/alpha_shader");
	batch.init(shader, 4096);
	opaqueBatch.setShader(textureShader);
	alphaBatch.setShader(alphaShader);
	guiBatch.setShader(alphaShader);

	doge = Engin::Resources::ResourceManager::getInstance().load<Engin::Resources::Texture>("resources/doge.png");

	gameObject.accessComponent<Engin::Game::Sprite>()->setCurrentSprite(doge);
	gameObject.accessComponent<Engin::Game::ParticleEffect>()->init(doge);
}

GameObjectTestScene::~GameObjectTestScene()
{
	std::cout << "GaneObject scene shutdown" << std::endl;

	Engin::Resources::ResourceManager::getInstance().unload(doge->getResourcePath());
	Engin::Resources::ResourceManager::getInstance().unload(shader->getResourcePath());
	Engin::Resources::ResourceManager::getInstance().unload(textureShader->getResourcePath());
	Engin::Resources::ResourceManager::getInstance().unload(alphaShader->getResourcePath());
}

void GameObjectTestScene::update(GLfloat step)
{
	if (engine->keyboardInput->keyWasPressed(Engin::HID::KEYBOARD_ESCAPE) ||
		(useGamePad && engine->gamepadInput->buttonWasPressed(Engin::HID::GAMEPAD_BUTTON_B, 0)))
	{
		engine->getSceneManager().pop();
	}

	static float zoomByInput = 1.0f;
	if (engine->mouseInput->mouseWheelWasMoved(Engin::HID::MOUSEWHEEL_UP) ||
		(useGamePad && engine->gamepadInput->buttonIsPressed(Engin::HID::GAMEPAD_BUTTON_RIGHTSHOULDER, 0)))
	{
		if (zoomByInput > 0.0f)
			zoomByInput -= glm::radians(2.0f);
	}
	if (engine->mouseInput->mouseWheelWasMoved(Engin::HID::MOUSEWHEEL_DOWN) ||
		(useGamePad && engine->gamepadInput->buttonIsPressed(Engin::HID::GAMEPAD_BUTTON_LEFTSHOULDER, 0)))
	{
		zoomByInput += glm::radians(2.0f);
	}
	camera->setZoomLevel(zoomByInput);

	static float moveByInputX = 0.0f;
	static float moveByInputY = 0.0f;
	static float moveSpeed = 16.0f;
	if (engine->keyboardInput->keyIsPressed(Engin::HID::KEYBOARD_W) ||
		(useGamePad && engine->gamepadInput->buttonIsPressed(Engin::HID::GAMEPAD_BUTTON_DPAD_UP, 0)))
	{
		moveByInputY += moveSpeed;
	}
	if (engine->keyboardInput->keyIsPressed(Engin::HID::KEYBOARD_A) ||
		(useGamePad && engine->gamepadInput->buttonIsPressed(Engin::HID::GAMEPAD_BUTTON_DPAD_LEFT, 0)))
	{
		moveByInputX -= moveSpeed;
	}
	if (engine->keyboardInput->keyIsPressed(Engin::HID::KEYBOARD_S) ||
		(useGamePad && engine->gamepadInput->buttonIsPressed(Engin::HID::GAMEPAD_BUTTON_DPAD_DOWN, 0)))
	{
		moveByInputY -= moveSpeed;
	}
	if (engine->keyboardInput->keyIsPressed(Engin::HID::KEYBOARD_D) ||
		(useGamePad && engine->gamepadInput->buttonIsPressed(Engin::HID::GAMEPAD_BUTTON_DPAD_RIGHT, 0)))
	{
		moveByInputX += moveSpeed;
	}
	gameObject.accessComponent<Engin::Game::Transform>()->setXPosition(moveByInputX);
	gameObject.accessComponent<Engin::Game::Transform>()->setYPosition(moveByInputY);
			
	static float rotateByInput = 0.0f;
	static float rotateSpeed = 0.05f;
	if (engine->keyboardInput->keyIsPressed(Engin::HID::KEYBOARD_LEFT) ||
		(useGamePad && engine->gamepadInput->buttonIsPressed(Engin::HID::GAMEPAD_BUTTON_X, 0)))
	{
		rotateByInput += rotateSpeed;
	}
	if (engine->keyboardInput->keyIsPressed(Engin::HID::KEYBOARD_RIGHT) ||
		(useGamePad && engine->gamepadInput->buttonIsPressed(Engin::HID::GAMEPAD_BUTTON_Y, 0)))
	{
		rotateByInput -= rotateSpeed;
	}
	gameObject.accessComponent<Engin::Game::Transform>()->setRotation(rotateByInput);

	gameObject.update();
}

void GameObjectTestScene::draw()
{
	gameObject.accessComponent<Engin::Game::Sprite>()->draw();
	gameObject.accessComponent<Engin::Game::ParticleEffect>()->draw();
}
