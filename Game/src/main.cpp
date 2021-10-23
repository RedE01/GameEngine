#include <GameEngine.h>
#include "Components/TestScript.h"

namespace GameEngine {

	class Game : public Application {
	public:
		Game(const std::string& name) : Application(name) {

		}

		virtual void registerScriptComponents(ScriptComponentManager* scm) override {
			scm->registerScriptComponent<TestScript>();
		}
	};

}

int main() {
	GameEngine::Game game("Test Game");

	game.getScene()->createEntity().addComponent<TestScript>();

	game.run();
}