#pragma once
#include "../Math/Vector.h"
#include "../Assets/AssetHandle.h"
#include <entt/entity/registry.hpp>
#include <memory>

namespace GameEngine {

	class Camera;
	class RendererData;
	class Model;
	class TransformComponent;

	class Renderer {
	public:
		Renderer(Vector2i viewportSize);
		~Renderer();

		void beginFrame();
        void endFrame(entt::registry& entityRegistry, Camera* camera);
        void renderFrameToDefaultFramebuffer();
		void renderEntities(entt::registry& entityRegistry, Camera* camera);

		void setViewportSize(Vector2i viewportSize);
		void setDefaultShader(ShaderAsset shaderAsset);

        std::shared_ptr<Texture> getFrameTexture();

	private:
		void renderModel(Model* model, TransformComponent* transform, Camera* camera);
        void renderLights(entt::registry& entityRegistry, Camera* camera);

	private:
		std::unique_ptr<RendererData> m_rendererData;
        Vector2i m_viewportSize;
	};

}
