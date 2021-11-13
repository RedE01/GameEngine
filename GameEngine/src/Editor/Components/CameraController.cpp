#include "CameraController.h"
#include "../../Components/TransformComponent.h"
#include "../../Input.h"

namespace GameEngine {

	void CameraController::update() {
		TransformComponent& transform = getEntity().getComponent<TransformComponent>();

		if(Input::GetMouseButton(MouseButton::Right)) {
			if(Input::GetKey(Key::W)) transform.move(Vector3(0.0, 0.0, -0.1), true);
			if(Input::GetKey(Key::A)) transform.moveX(-0.1, true);
			if(Input::GetKey(Key::S)) transform.moveZ(0.1, true);
			if(Input::GetKey(Key::S)) transform.moveX(0.1, true);

			transform.rotateY(Input::GetMouseViewportDelta().x * -2.0);
			transform.rotateX(Input::GetMouseViewportDelta().y * -2.0);
		}
	}

}