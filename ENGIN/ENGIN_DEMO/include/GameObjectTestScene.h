#pragma once

#include <map>
#include <string>

#include <Engin\Engin.h>
#include "Engin\Game\Scene.h"
#include <Engin\Renderer\Camera.h>
#include <Engin\Renderer\Batch.h>
#include <Engin\Renderer\TextureBatch.h>
#include "Engin\Resources\ResourceManager.h"
#include "Engin\Resources\Texture.h"
#include "Engin\Resources\ShaderProgram.h"
#include "Engin\Game\GameObject.h"
#include "Engin\Game\Component.h"


class GameObjectTestScene : public Engin::Game::Scene
{
public:
	GameObjectTestScene(Engin::Engin* engine);
	~GameObjectTestScene();

	void update(GLfloat step);
	void draw();
	void interpolate(GLfloat alpha){}
public:
	GLfloat alpha;

	Engin::Engin* engine;

	Engin::Resources::Texture* doge;

	Engin::Renderer::Camera camera;

	Engin::Game::GameObject gameObject;

	Engin::Resources::ShaderProgram* shader;
	Engin::Resources::ShaderProgram* textureShader;
	Engin::Resources::ShaderProgram* alphaShader;
	Engin::Renderer::Batch batch;
	Engin::Renderer::TextureBatch textureBatch;
	Engin::Renderer::TextureBatch alphaTextureBatch;
};


class HP_Component : public Engin::Game::Component
{
public:
	HP_Component(Engin::Game::GameObject* o){}
	~HP_Component(){}

	void setIntVariable(std::string variableName, int newHp)
	{
		intVariables.insert(std::pair<int, std::string>(newHp, variableName));
	}
	int getIntVariable(std::string variableName)
	{
		for (auto& it : intVariables)
		{
			if (it.second == variableName)
			{
				return it.first;
			}
		}
		std::cout << "variable not found!";
		return NULL;
	}

private:
	std::map<int, std::string> intVariables;
};