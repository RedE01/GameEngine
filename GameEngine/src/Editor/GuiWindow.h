#pragma once
#include <memory>
#include <vector>
#include <functional>
#include "../Math/Vector.h"

namespace GameEngine {

    class Editor;
    class Event;
    class Application;

	class GuiWindow {
	public:
		GuiWindow(Application* application, Editor* editor);
        virtual ~GuiWindow() = default;

		virtual const char* getWindowName() = 0;
		virtual void renderWindow() = 0;

        bool isFocused() const;
        bool isOpen() const;
        int getWindowFlags() const;

        void setOpen(bool isOpen);
        void setWindowFlags(int windowFlags);
		void setEventFunction(std::function<void(Event*)> eventFunction);

    protected:
        void dispatchEvent(Event* e) const;
        Application* getApplication() const;
        Editor* getEditor() const;

	private:
        bool m_focused = false;
		bool m_open = true;
		int m_windowFlags = 0;
        std::function<void(Event*)> m_eventFunction;
        Application* const m_application;
        Editor* const m_editor;
	};
}
