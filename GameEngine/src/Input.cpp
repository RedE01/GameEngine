#include "Input.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"
#include "Window.h"

#include <GLFW/glfw3.h>
#include <algorithm>
#include <cstring>

namespace GameEngine {

	template <typename T>
	constexpr int toInt(T v) noexcept {
		return static_cast<int>(v);
	}

	bool g_keys[toInt(KEY_LAST) + 1] = { false };
	bool g_keysPrevious[toInt(KEY_LAST) + 1] = { false };
	Vector2 g_mousePos = Vector2(0, 0);
	Vector2 g_prevMousePos = Vector2(0, 0);
	bool g_mouseKeys[toInt(MOUSE_BUTTON_LAST) + 1] = { false };
	bool g_mouseKeysPrevious[toInt(MOUSE_BUTTON_LAST) + 1] = { false };
	Vector2i g_viewportSize = Vector2i(0, 0);
	Window* m_window = nullptr;

	bool Input::GetKeyDown(const Key& key) {
		return g_keys[toInt(key)] && !g_keysPrevious[toInt(key)];
	}

	bool Input::GetKey(const Key& key) {
		return g_keys[toInt(key)];
	}

	bool Input::GetKeyUp(const Key& key) {
		return !g_keys[toInt(key)] && g_keysPrevious[toInt(key)];
	}

	bool Input::GetMouseButtonDown(const MouseButton& button) {
		return g_mouseKeys[toInt(button)] && !g_mouseKeysPrevious[toInt(button)];
	}

	bool Input::GetMouseButton(const MouseButton& button) {
		return g_mouseKeys[toInt(button)];
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

			if(keyEvent->getEventType() == EventType::KeyPress) {
				g_keys[toInt(keyEvent->key)] = 1;
			}
			else if(keyEvent->getEventType() == EventType::KeyReleased) {
				g_keys[toInt(keyEvent->key)] = 0;
			}
		}
		else if(e->isInCategory(EventCategory::Mouse)) {
			if(e->getEventType() == EventType::MouseMoved) {
				MouseMoved* mouseMovedEvent = (MouseMoved*)e;
				
				g_mousePos.x = mouseMovedEvent->xpos;
				g_mousePos.y = mouseMovedEvent->ypos;
			}
			else if(e->isInCategory(EventCategory::MouseButton)) {
				MouseButtonEvent* mouseEvent = (MouseButtonEvent*)e;
				if(mouseEvent->getEventType() == EventType::MouseButtonPressed) {
					g_mouseKeys[toInt(mouseEvent->button)] = 1;
				}
				else if(mouseEvent->getEventType() == EventType::MouseButtonReleased) {
					g_mouseKeys[toInt(mouseEvent->button)] = 0;
				}
			}
		}
	}

	void Input::Update(Vector2i viewportSize) {
		g_viewportSize = viewportSize;

		std::memcpy(g_keysPrevious, g_keys, toInt(KEY_LAST));
		std::memcpy(g_mouseKeysPrevious, g_mouseKeys, toInt(MOUSE_BUTTON_LAST));

		g_prevMousePos = g_mousePos;
	}

}