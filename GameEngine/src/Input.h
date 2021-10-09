#pragma once
#include <iostream>
#include "Math/Vector.h"
#include "Keycodes.h"

namespace GameEngine {
	class Event;
	class Window;

	namespace Input {
		bool GetKeyDown(const int& keycode);
		bool GetKey(const int& keycode);
		bool GetKeyUp(const int& keycode);

		bool GetMouseKeyDown(const int& keycode);
		bool GetMouseKey(const int& keycode);
		Vector2 GetMousePosition();
		Vector2 GetMouseViewportPositon();
		Vector2 GetMouseDelta();
		Vector2 GetMouseViewportDelta();
		float GetMouseX();
		float GetMouseY();
		float GetMouseDeltaX();
		float GetMouseDeltaY();

		void SetCursorMode(const int& cursorMode);

		void HandleEvent(Event* e);
		void Update(Vector2 viewportSize, Vector2 viewportOffset);
	}
}