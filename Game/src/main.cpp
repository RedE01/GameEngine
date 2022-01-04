#include <GameEngine.h>
#include <Assets.h>
#include <Components.h>
#include "Components/TestScript.h"
#include <Window.h>
#include <SceneManager.h>

using namespace GameEngine;

class Game : public Application {
public:
    Game(const std::string& name) : Application(name) {
        ScriptComponentManager::registerScriptComponent<TestScript>();
	}

	virtual void onUpdate() override {

	}

	virtual void onRender() override {

	}
};

int main() {
	Game game("Test Game");

    game.getSceneManager()->loadScene("testScene.scene");

	game.run();
}
