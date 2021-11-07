#pragma once

namespace GameEngine {

	class Scene;
	class Camera;

	class Renderer {
	public:
		Renderer();

		void renderFrame(Scene* scene, Camera* camera);
	};

}