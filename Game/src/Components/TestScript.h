#pragma once
#include <Component.h>
#include <Components.h>
#include <iostream>

#include <gtx/rotate_vector.hpp>

using namespace GameEngine;

class TestScript : public Component {
public:
	void update() {
		TransformComponent& t = getEntity().getComponent<TransformComponent>();

        Vector3 p = t.getPosition();
        p = glm::rotateY(p, 0.01f);
        t.setPosition(p);
	}
};
