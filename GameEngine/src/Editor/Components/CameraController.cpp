#include "CameraController.h"
#include "../../Components/TransformComponent.h"
#include "../../Input.h"

namespace GameEngine {

	void CameraController::update() {
		TransformComponent& transform = getEntity().getComponent<TransformComponent>();

		if(Input::GetMouseButton(MouseButton::Right)) {
			Input::SetCursorMode(CursorMode::Disabled);

			if(Input::GetKey(Key::W)) transform.move(Vector3(0.0, 0.0, -0.1), true);
			if(Input::GetKey(Key::A)) transform.moveX(-0.1, true);
			if(Input::GetKey(Key::S)) transform.moveZ(0.1, true);
			if(Input::GetKey(Key::D)) transform.moveX(0.1, true);

			if(Input::GetKey(Key::Space)) transform.moveY(0.1, true);
			if(Input::GetKey(Key::Left_control)) transform.moveY(-0.1, true);

			if(Input::GetKey(Key::Q)) transform.rotateZ(0.05);
			if(Input::GetKey(Key::E)) transform.rotateZ(-0.05);

			transform.rotateY(Input::GetMouseViewportDelta().x * -2.0);
			transform.rotateX(Input::GetMouseViewportDelta().y * -2.0);
		}
		else {
			Input::SetCursorMode(CursorMode::Normal);
		}
	}

}