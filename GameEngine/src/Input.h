#pragma once
#include <iostream>
#include "Math/Vector.h"
#include "InputDefines.h"

namespace GameEngine {
	class Event;
	class Window;

	namespace Input {
		bool GetKeyDown(const Key& key);
		bool GetKey(const Key& key);
		bool GetKeyUp(const Key& key);

		bool GetMouseButtonDown(const MouseButton& button);
		bool GetMouseButton(const MouseButton& button);
		Vector2 GetMousePosition();
		Vector2 GetMouseViewportPositon();
		Vector2 GetMouseDelta();
		Vector2 GetMouseViewportDelta();
		float GetMouseX();
		float GetMouseY();
		float GetMouseDeltaX();
		float GetMouseDeltaY();

		void SetCursorMode(CursorMode cursorMode);

		void HandleEvent(Event* e);
		void Update(Window* window);
	}
}