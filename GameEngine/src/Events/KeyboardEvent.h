#pragma once
#include "Event.h"
#include "../InputDefines.h"

namespace GameEngine {

	class KeyboardEvent : public Event {
	public:
		KeyboardEvent(Key key, int scancode, int mod) : key(key), scancode(scancode), mod(mod) {}

	public:
		Key key;
		int scancode, mod;
	};

	class KeyPress : public KeyboardEvent {
	public:
		KeyPress(Key key, int scancode, int mod) : KeyboardEvent(key, scancode, mod) {}

		virtual EventType getEventType() const override { return EventType::KeyPress; }
		virtual std::string getName() const override { return "KeyPress"; }
		virtual int getCategoryFlags() const override { return (int)EventCategory::Input + (int)EventCategory::Keyboard; }
	};

	class KeyReleased : public KeyboardEvent {
	public:
		KeyReleased(Key key, int scancode, int mod) : KeyboardEvent(key, scancode, mod) {}

		virtual EventType getEventType() const override { return EventType::KeyReleased; }
		virtual std::string getName() const override { return "KeyReleased"; }
		virtual int getCategoryFlags() const override { return (int)EventCategory::Input + (int)EventCategory::Keyboard; }
	};

	class KeyTyped : public KeyboardEvent {
	public:
		KeyTyped(Key key, int scancode, int mod) : KeyboardEvent(key, scancode, mod) {}

		virtual EventType getEventType() const override { return EventType::KeyTyped; }
		virtual std::string getName() const override { return "KeyTyped"; }
		virtual int getCategoryFlags() const override { return (int)EventCategory::Input + (int)EventCategory::Keyboard; }
	};
}