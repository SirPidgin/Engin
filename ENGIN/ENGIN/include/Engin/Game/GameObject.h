#pragma once

#include <vector>
#include <string>
#include <iostream>

#include <glm\vec2.hpp>

#include "Component.h"
#include "RigidBody.h"
#include "Engin\Renderer\TextureBatch.h"

/*
Game object class.
*/

namespace Engin
{
	namespace Game
	{
		class GameObject
		{
		public:
			GameObject();
			GameObject(Renderer::TextureBatch* refTextureBatch);
			GameObject(std::string name, Renderer::TextureBatch* refTextureBatch);
			~GameObject();

			void update();
			void draw();

			bool isAlive();
			void kill();

			bool operator!=(const GameObject& other);//compares the nameTag of both objects
			bool operator==(const GameObject& other);//compares the nameTag of both objects

			void setNameTag(std::string newNameTag);
			template <typename type> void addComponent();
			template <typename type> type* accessComponent();

			Renderer::TextureBatch* getTextureBatch();
			
		private:
			void initialize();
			std::string nameTag;
			std::vector<Component*> components;
			Renderer::TextureBatch* textureBatch;
			bool alive;
		};


		//Template Methods:

		template <typename type>
		void GameObject::addComponent()
		{
			type* cast = new type(this);

			//if (std::is_base_of<Component, type>::value == false)
			//{
			//	std::cout << "Given type in not a Component!" << std::endl;
			//	return;
			//}
			components.push_back(cast);
		}

		template <typename type>
		type* GameObject::accessComponent()
		{
			type* temp;
			for (unsigned int i = 0; i < components.size(); i++)
			{
				temp = dynamic_cast<type*>(components[i]);
				if (temp != nullptr)
				{
					return dynamic_cast<type*>(components[i]);
				}
			}
			std::cout << "Component not found!" << std::endl;
			return nullptr;
		}
	}
}