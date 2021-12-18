#pragma once
#include "Event.h"
#include "../Math/Vector.h"

namespace GameEngine {

	class EditorViewportResizeEvent : public Event {
	public:
        EditorViewportResizeEvent(Vector2 viewportSize) : viewportSize(viewportSize) {}
		virtual EventType getEventType() const override { return EventType::EditorViewportResize; }
		virtual std::string getName() const override { return "EditorViewportResize"; }
		virtual int getCategoryFlags() const override { return (int)EventCategory::Editor; }

        Vector2i viewportSize;
	};

}

