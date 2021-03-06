#pragma once


#include "Pixelate\Rendering\RendererAPI.h"



namespace Pixelate {

	class PX_API GLRendererAPI : public RendererAPI {

		public:

			void Init() override;

			void Clear() override;
			void SetClearColor(float r, float g, float b, float a) override;

			void SetViewport(int x, int y, int width, int height)override;


			void SetDepthTesting(bool enable) override;
			void SetStencilTesting(bool enable) override;
			void RenderWireFrame(bool enable) override;

			void SetBlending(bool enable) override;
			void SetBlendFunc(BlendFunc& source, BlendFunc& dest) override;
			void SetLineThickness(float width) override;

			void DrawElements(const Ref<VertexArray>& vao, PimitiveRenderType type, unsigned int count) override;


			virtual const RenderAPIData& GetAPIData() const override;



		private:
			unsigned int ConvertBlendFunctions(const BlendFunc& func);

	};
}

