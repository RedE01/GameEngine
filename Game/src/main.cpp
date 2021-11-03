#include <GameEngine.h>
#include "Assets.h"
#include "Components.h"
#include "Components/TestScript.h"

using namespace GameEngine;

class Game : public Application {
public:
	Game(const std::string& name) : Application(name) {

	}

	virtual void onUpdate() {

	}

	virtual void onRender() {

	}

	virtual void registerScriptComponents(ScriptComponentManager* scm) override {
		scm->registerScriptComponent<TestScript>();
	}
};

int main() {
	Game game("Test Game");

	Entity entity1 = game.getScene()->createEntity();
	entity1.addComponent<TestScript>();
	MeshRendererComponent& component1 = entity1.addComponent<MeshRendererComponent>();
	component1.model = game.getAssetManager()->load<Model>("Game/assets/models/test.fbx");
	

	game.run();
}