#pragma once
#include "GuiWindow.h"
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
#include <type_traits>
#include <typeinfo>

namespace GameEngine {

    class Editor;
    class Application;

	class Gui {
	public:
		Gui(Application* application, Editor* editor);

		void OnGuiRender();

        template<class T, typename... Targs>
        T* addWindow(Targs... args) {
            static_assert(std::is_base_of<GuiWindow, T>::value, "Can not add window of type that is not derived from GuiWindow");
            std::size_t typeHash = typeid(T).hash_code();

            std::unique_ptr<T> newWindow = std::make_unique<T>(m_application, m_editor, args...);
            newWindow->setEventFunction(m_eventFunction);
            T* newWindowPtr = newWindow.get();
            m_windows[typeHash].push_back(std::move(newWindow));
            return newWindowPtr;
        }

        template<class T>
        std::vector<T*> getWindows() {
            static_assert(std::is_base_of<GuiWindow, T>::value, "Can not get window of type that is not derived from GuiWindow");
            std::size_t typeHash = typeid(T).hash_code();

            std::vector<T*> windows;
            auto search = m_windows.find(typeHash);
            if(search != m_windows.end()) {
                for(auto& window : search->second) {
                    windows.push_back(dynamic_cast<T*>(window.get()));
                }
            }

            return windows;
        }

		void setEventFunction(std::function<void(Event*)> eventFunction);

	private:
		void init();

	private:
        int m_windowFlags;
        std::unordered_map<std::size_t, std::vector<std::unique_ptr<GuiWindow>>> m_windows;
        Application* const m_application;
        Editor* const m_editor;
        std::function<void(Event*)> m_eventFunction;

        std::string m_saveSceneNameStr;
        std::string m_saveSceneFilepathStr;
	};

}
