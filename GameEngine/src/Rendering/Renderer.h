#pragma once
#include "../Math/Vector.h"
#include "../Assets/AssetHandle.h"
#include <memory>

namespace GameEngine {

	class Camera;
	class RendererData;
	class Model;
	class TransformComponent;
    class Scene;

	class Renderer {
	public:
		Renderer(Vector2i viewportSize);
		~Renderer();

		void beginFrame();
        void endFrame(Scene* scene, Camera* camera);
        void renderFrameToDefaultFramebuffer();
		void renderEntities(Scene* scene, Camera* camera);

		void setViewportSize(Vector2i viewportSize);

        std::shared_ptr<Texture> getFrameTexture();

	private:
		void renderModel(Model* model, TransformComponent* transform, Camera* camera);
        void renderLights(Scene* scene, Camera* camera);

	private:
		std::unique_ptr<RendererData> m_rendererData;
        Vector2i m_viewportSize;
	};

}
