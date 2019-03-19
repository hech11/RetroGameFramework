#pragma once


#include "RetroGF/Rendering/API/IndexBuffer.h"

namespace RGF {

	class RGF_API GLIndexBuffer : public IndexBuffer {
		public :

			GLIndexBuffer(unsigned int* data, unsigned int count);
			GLIndexBuffer(unsigned short* data, unsigned int count);
			GLIndexBuffer(unsigned char* data, unsigned int count);

			~GLIndexBuffer();

			void Bind() override;
			void Unbind() override;


			unsigned int GetCount() const override;
		private :
			unsigned int m_RendererID;
			unsigned int m_Count;
	};

}