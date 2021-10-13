#pragma once
#include "Component/Component.h"
#include "Util/Resource.h"
#include "Util/ResourceArray.h"

namespace GameEngine {

	class GameObject {
	public:
		GameObject(unsigned int id);

		void init();
		void update();

		Component* createComponent();
		bool removeComponent(Resource<Component> componentResource);
		std::optional<Component*> getComponent(Resource<Component> componentResource);

		const unsigned int getID() const;

	private:
		unsigned int m_id;
		ResourceArray<Component> m_components;
	};

}
