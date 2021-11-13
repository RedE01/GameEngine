#pragma once
#include "Event.h"
#include "../InputDefines.h"

namespace GameEngine {

	class MouseMoved : public Event {
	public:
		MouseMoved(double xpos, double ypos) : xpos(xpos), ypos(ypos) {}

		virtual EventType getEventType() const override { return EventType::MouseMoved; }
		virtual std::string getName() const override { return "MouseMoved"; }
		virtual int getCategoryFlags() const override { return (int)EventCategory::Input + (int)EventCategory::Mouse; }

	public:
		double xpos, ypos;
	};

	class MouseButtonEvent : public Event {
	public:
		MouseButtonEvent(MouseButton button, int mod) : button(button), mod(mod) {}
	public:
		MouseButton button;
		int mod;
	};

	class MouseButtonPressed : public MouseButtonEvent {
	public:
		MouseButtonPressed(MouseButton button, int mod) : MouseButtonEvent(button, mod) {}
		virtual EventType getEventType() const override { return EventType::MouseButtonPressed; }
		virtual std::string getName() const override { return "MouseButtonPressed"; }
		virtual int getCategoryFlags() const override { return (int)EventCategory::Input + (int)EventCategory::Mouse + (int)EventCategory::MouseButton; }
	};

	class MouseButtonReleased : public MouseButtonEvent {
	public:
		MouseButtonReleased(MouseButton button, int mod) : MouseButtonEvent(button, mod) {}
		virtual EventType getEventType() const override { return EventType::MouseButtonReleased; }
		virtual std::string getName() const override { return "MouseButtonReleased"; }
		virtual int getCategoryFlags() const override { return (int)EventCategory::Input + (int)EventCategory::Mouse + (int)EventCategory::MouseButton; }
	};
}