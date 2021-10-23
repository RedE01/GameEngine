#pragma once
#include <memory>
#include <string>

namespace GameEngine {

	class Window;
	class Renderer;
	class Event;
	class Scene;
	class ScriptComponentManager;

	class Application {
	public:
		Application(const std::string& name);
		~Application();

		void run();

		Scene* getScene() const;

	protected:
		virtual void registerScriptComponents(ScriptComponentManager* scm) = 0;
		
	private:
		void eventHandler(Event* event);

	private:
		std::unique_ptr<Window> m_window;
		std::unique_ptr<Renderer> m_renderer;
		std::unique_ptr<Scene> m_scene;
		std::unique_ptr<ScriptComponentManager> m_scriptComponentManager;
		bool m_running = true;
	};

}