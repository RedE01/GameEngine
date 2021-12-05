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

	virtual void onUpdate() override {

	}

	virtual void onRender() override {

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

	Entity entity1 = game.getScene()->createEntity();
	MeshRendererComponent& component1 = entity1.addComponent<MeshRendererComponent>();
    component1.model = game.getAssetManager()->load<Model>("Game/assets/models/test.fbx");

    Entity ent2 = game.getScene()->createEntity();
    ent2.getComponent<TransformComponent>().setScale(Vector3(0.1, 0.1, 0.1));
    ent2.getComponent<TransformComponent>().setPosition(Vector3(2.0, 0.0, 0.0));
    ent2.addComponent<LightComponent>();
    MeshRendererComponent& comp2 = ent2.addComponent<MeshRendererComponent>();
    comp2.model = game.getAssetManager()->load<Model>("Game/assets/models/sphere.fbx");
    ent2.addComponent<TestScript>();

	game.run();
}
