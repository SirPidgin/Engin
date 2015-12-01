#include "Engin\Game\GameObject.h"
#include "Engin\Game\Transform.h"
#include "Engin\Game\Sprite.h"
#include "Engin\Game\RigidBody.h"

namespace Engin
{
	namespace Game
	{
		GameObject::GameObject() : alive(true), nameTag("default"), textureBatch(nullptr)
		{
		}
		GameObject::GameObject(Renderer::TextureBatch* refTextureBatch) : nameTag("default"), textureBatch(refTextureBatch)
		{
		}
		GameObject::GameObject(std::string name, Renderer::TextureBatch* refTextureBatch) : nameTag(name), textureBatch(refTextureBatch)
		{
		}
		GameObject::~GameObject()
		{
			for (unsigned int i = 0; i < components.size(); i++)
			{
				delete components[i];
			}
		}

		void GameObject::update()
		{
			for (unsigned int i = 0; i < components.size(); i++)
			{
				components[i]->update();
			}
		}
		void GameObject::draw()
		{
			for (unsigned int i = 0; i < components.size(); i++)
			{
				components[i]->draw();
			}
		}


		bool GameObject::operator!=(const GameObject& other)
		{
			if (strcmp(this->nameTag.c_str(), other.nameTag.c_str()))
			{
				return true;
			}
			return false;
		}


		bool GameObject::operator==(const GameObject& other)
		{
			if (strcmp(this->nameTag.c_str(), other.nameTag.c_str()))
			{
				return false;
			}
			return true;
		}


		void GameObject::setNameTag(std::string newNameTag)
		{
			nameTag = newNameTag;
		}


		Renderer::TextureBatch* GameObject::getTextureBatch()
		{
			return textureBatch;
		}
	}
}
