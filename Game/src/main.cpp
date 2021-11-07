#include <GameEngine.h>
#include <Assets.h>
#include <Components.h>
#include "Components/TestScript.h"
#include <Window.h>

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

	Entity cameraEntity = game.getScene()->createEntity();
	CameraComponent& camera = cameraEntity.addComponent<CameraComponent>(ProjectionType::Perspective, glm::radians(45.0), game.getWindow()->getWindowSize());
	cameraEntity.getComponent<TransformComponent>().setPosition(Vector3(0.0, 0.0, 10.0));
	cameraEntity.addComponent<TestScript>();

	Entity entity1 = game.getScene()->createEntity();
	MeshRendererComponent& component1 = entity1.addComponent<MeshRendererComponent>();
	component1.model = game.getAssetManager()->load<Model>("Game/assets/models/test.fbx");
	

	game.run();
}