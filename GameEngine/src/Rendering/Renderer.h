#pragma once

namespace GameEngine {

	class Scene;

	class Renderer {
	public:
		Renderer();

		void renderFrame(Scene* scene);
	};

}