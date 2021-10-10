#pragma once
#include "Event.h"

namespace GameEngine {

	class WindowCloseEvent : public Event {
	public:
		virtual EventType getEventType() const override { return EventType::WindowClose; }
		virtual std::string getName() const override { return "WindowClose"; }
		virtual int getCategoryFlags() const override { return (int)EventCategory::Application; }
	};

	class WindowResizeEvent : public Event {
	public:
		virtual EventType getEventType() const override { return EventType::WindowResize; }
		virtual std::string getName() const override { return "WindowResize"; }
		virtual int getCategoryFlags() const override { return (int)EventCategory::Application; }

	public:
		int width;
		int height;
	};

	class WindowMovedEvent : public Event {
	public:
		virtual EventType getEventType() const override { return EventType::WindowMoved; }
		virtual std::string getName() const override { return "WindowMoved"; }
		virtual int getCategoryFlags() const override { return (int)EventCategory::Application; }

	public:
		int xpos;
		int ypos;
	};
}