#include "Engin\Game\GameObject.h"

namespace Engin
{
	namespace Game
	{
		GameObject::GameObject()
		{
		}
		GameObject::~GameObject()
		{
			for (int i = 0; i < components.size(); i++)
			{
				delete components[i];
			}
		}

		void GameObject::update()
		{
			for (int i = 0; i < components.size(); i++)
			{
				components[i]->update();
			}
		}
		void GameObject::draw()
		{
			for (int i = 0; i < components.size(); i++)
			{
				components[i]->draw();
			}
		}

		void GameObject::addComponent(Component* newComponent)
		{
			components.push_back(newComponent);
		}
	}
}

