#pragma once
#include <memory>
#include <vector>
#include <functional>
#include "../Math/Vector.h"

namespace GameEngine {

    class Editor;

	class GuiWindow {
	public:
		GuiWindow();

		virtual const char* getWindowName() = 0;
		virtual void renderWindow() = 0;

        bool isFocused() const;
        bool isOpen() const;
        int getWindowFlags() const;

        void setOpen(bool isOpen);
        void setWindowFlags(int windowFlags);

	private:
        bool m_focused = false;
		bool m_open = true;
		int m_windowFlags = 0;
	};
}
