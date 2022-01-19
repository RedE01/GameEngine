#pragma once
#include "Event.h"
#include "../Math/Vector.h"

namespace GameEngine {

	class SceneSaveEvent : public Event {
	public:
        SceneSaveEvent(const std::string& name = "", const std::string& filepath = "") 
            : name(name), filepath(filepath) {}
		virtual EventType getEventType() const override { return EventType::SaveScene; }
		virtual std::string getName() const override { return "SaveSceneEvent"; }
		virtual int getCategoryFlags() const override { return (int)EventCategory::Editor; }
        const std::string name;
        const std::string filepath;
	};

	class EditorViewportResizeEvent : public Event {
	public:
        EditorViewportResizeEvent(Vector2 viewportSize) : viewportSize(viewportSize) {}
		virtual EventType getEventType() const override { return EventType::EditorViewportResize; }
		virtual std::string getName() const override { return "EditorViewportResize"; }
		virtual int getCategoryFlags() const override { return (int)EventCategory::Editor; }

        Vector2i viewportSize;
	};

    class SaveSettingsEvent : public Event {
    public:
        SaveSettingsEvent() {}
		virtual EventType getEventType() const override { return EventType::SaveSettings; }
		virtual std::string getName() const override { return "SaveSettingsEvent"; }
		virtual int getCategoryFlags() const override { return (int)EventCategory::Editor; }
    };

}

