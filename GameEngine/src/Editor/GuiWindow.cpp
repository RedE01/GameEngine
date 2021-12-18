#include "GuiWindow.h"

namespace GameEngine {
    
    GuiWindow::GuiWindow() {

    }

    bool GuiWindow::isFocused() const {
        return m_focused;
    }
    bool GuiWindow::isOpen() const {
        return m_open;
    }

    int GuiWindow::getWindowFlags() const {
        return m_windowFlags;
    }

    void GuiWindow::setOpen(bool isOpen) {
        m_open = isOpen;
    }

    void GuiWindow::setWindowFlags(int windowFlags) {
        m_windowFlags = windowFlags;
    }

    void GuiWindow::setEventFunction(std::function<void(Event*)> eventFunction) {
        m_eventFunction = eventFunction;
    }

    void GuiWindow::dispatchEvent(Event* e) const {
        if(m_eventFunction) {
            m_eventFunction(e);
        }
    }

}
