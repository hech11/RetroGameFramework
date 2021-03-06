#pragma once

#include "Pixelate/Rendering/RenderingContext.h"


// OpenGL's implementation of 'RenderingContext.h'

// TODO: This uses GLFW. Switch to the OS windows implentation.

namespace Pixelate {


	class GLRenderingContext : public RenderingContext {
		public :
			GLRenderingContext();

			virtual void Init(void* window) override;
			virtual void SwapBuffers() override;


			virtual ContextAPI GetContext() const override { return m_Context; }

		private :
			ContextAPI m_Context;
	};
}