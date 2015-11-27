#include <string>

#include "Engin\Game\Component.h"
#include "Engin\Game\RigidBody.h"


namespace Engin
{
	namespace Game
	{
		Component::Component() : ownerObject(nullptr)
		{}
		Component::Component(GameObject* o) : ownerObject(o)
		{}
		Component::~Component()
		{}

		void Component::draw()
		{}
		void Component::update()
		{}
	}
}