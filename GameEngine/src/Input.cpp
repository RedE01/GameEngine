#include "Input.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"
#include "Window.h"

#include <GLFW/glfw3.h>
#include <algorithm>
#include <cstring>

namespace GameEngine {

	bool g_keys[KEY_LAST + 1] = { false };
	bool g_keysPrevious[KEY_LAST + 1] = { false };
	Vector2 g_mousePos = Vector2(0, 0);
	Vector2 g_prevMousePos = Vector2(0, 0);
	bool g_mouseKeys[MOUSE_BUTTON_LAST + 1] = { false };
	bool g_mouseKeysPrevious[MOUSE_BUTTON_LAST + 1] = { false };
	Vector2i g_viewportSize = Vector2i(0, 0);
	Window* m_window = nullptr;

	bool Input::GetKeyDown(const int& keycode) {
		if(keycode < 0 || keycode > KEY_LAST) {
			return false;
		}

		return g_keys[keycode] && !g_keysPrevious[keycode];
	}

	bool Input::GetKey(const int& keycode) {
		if(keycode < 0 || keycode > KEY_LAST) {
			return false;
		}

		return g_keys[keycode];
	}

	bool Input::GetKeyUp(const int& keycode) {
		if (keycode < 0 || keycode > KEY_LAST) {
			return false;
		}

		return !g_keys[keycode] && g_keysPrevious[keycode];
	}

	bool Input::GetMouseKeyDown(const int& keycode) {
		if(keycode < 0 || keycode > MOUSE_BUTTON_LAST) {
			return false;
		}
		return g_mouseKeys[keycode] && !g_mouseKeysPrevious[keycode];
	}

	bool Input::GetMouseKey(const int& keycode) {
		if(keycode < 0 || keycode > MOUSE_BUTTON_LAST) {
			return false;
		}
		return g_mouseKeys[keycode];
	}

	Vector2 Input::GetMousePosition() {
		return g_mousePos;
	}

	Vector2 Input::GetMouseViewportPositon() {
		return GetMousePosition() / Vector2(g_viewportSize);
	}

	Vector2 Input::GetMouseDelta() {
		return g_mousePos - g_prevMousePos;
	}

	Vector2 Input::GetMouseViewportDelta() {
		return GetMouseDelta() / Vector2(g_viewportSize);
	}

	float Input::GetMouseX() {
		return g_mousePos.x;
	}

	float Input::GetMouseY() {
		return g_mousePos.y;
	}

	float Input::GetMouseDeltaX() {
		return g_mousePos.x - g_prevMousePos.x;
	}

	float Input::GetMouseDeltaY() {
		return g_mousePos.y - g_prevMousePos.y;
	}

	void Input::SetCursorMode(const int& cursorMode) {
		glfwSetInputMode(m_window->getWindow(), GLFW_CURSOR, cursorMode);
	}

	void Input::HandleEvent(Event* e) {
		if(e->isInCategory(EventCategory::Keyboard)) {
			KeyboardEvent* keyEvent = (KeyboardEvent*)e;
			if(keyEvent->keycode < 0 || keyEvent->keycode > KEY_LAST) {
				return;
			}

			if(keyEvent->getEventType() == EventType::KeyPress) {
				g_keys[keyEvent->keycode] = 1;
			}
			else if(keyEvent->getEventType() == EventType::KeyReleased) {
				g_keys[keyEvent->keycode] = 0;
			}
		}
		else if(e->isInCategory(EventCategory::Mouse)) {
			if(e->getEventType() == EventType::MouseMoved) {
				MouseMoved* mouseMovedEvent = (MouseMoved*)e;
				
				g_mousePos.x = mouseMovedEvent->xpos;
				g_mousePos.y = mouseMovedEvent->ypos;
			}
			else if(e->isInCategory(EventCategory::MouseButton)) {
				MouseButton* mouseEvent = (MouseButton*)e;
				if(mouseEvent->getEventType() == EventType::MouseButtonPressed) {
					g_mouseKeys[mouseEvent->button] = 1;
				}
				else if(mouseEvent->getEventType() == EventType::MouseButtonReleased) {
					g_mouseKeys[mouseEvent->button] = 0;
				}
			}
		}
	}

	void Input::Update(Vector2i viewportSize) {
		g_viewportSize = viewportSize;

		std::memcpy(g_keysPrevious, g_keys, KEY_LAST);
		std::memcpy(g_mouseKeysPrevious, g_mouseKeys, MOUSE_BUTTON_LAST);

		g_prevMousePos = g_mousePos;
	}

}