#pragma once


#include "Renderable.h"
#include "RetroGF/Core.h"



// This contains classes for "Sprite" and "BatchedSprite"
/* 
	TODO: Sprites and BatchedSprites are implemented in this file. 
		  Investigate to see if it is worth seperating into their own files
		  such as "GLSprite" or "DX11Sprite".

*/

/* 
	TODO: "BatchedSprite" may be unnecessary. Investiage on combining "BatchSprite" into the "Sprite" class and 
		  switch between either batched or dynamic via a value or boolean.
*/

namespace RGF {

	class RGF_API Sprite : public Renderable {

		public :
			Sprite(glm::vec3 position, glm::vec3 scale, glm::vec4 color, Shader* shader) 
				: Renderable(position, scale, color), m_Shader(shader)
			{
				float vertex[]{
					position.x, position.y, position.z, color.x, color.y, color.z, color.w, // bottom left 0 
					position.x, position.y + scale.y, position.z, color.x, color.y, color.z, color.w, // bottom right 1
					position.x + scale.x, position.y + scale.y, position.z, color.x, color.y, color.z, color.w, // top right 2
					position.x + scale.x, position.y, position.z, color.x, color.y, color.z, color.w // top left 3
				};
				unsigned short indicies[] = {
					0, 1, 2,
					2, 3, 0
				};

				m_VertexArray = VertexArray::Create();
				VertexBuffer* vbo = nullptr;
				vbo = VertexBuffer::Create();
				VertexBufferLayout layout;

				m_VertexArray->Bind();
				vbo->Bind();

				vbo->SetData(sizeof(vertex), vertex);
				m_VertexArray->PushBuffer(vbo);

				m_IndexBuffer = IndexBuffer::Create(indicies, 6);
				m_IndexBuffer->Bind();


				layout.Push<float>(3);
				layout.Push<float>(4);
				m_VertexArray->Bind();
				m_IndexBuffer->Bind();

				vbo->SetLayout(layout);
				delete vbo;

			}

			~Sprite() {
				delete m_VertexArray;
				delete m_IndexBuffer;
			}


			inline const VertexArray* GetVao() const override { return m_VertexArray; }
			inline const IndexBuffer* GetIbo() const override { return m_IndexBuffer; }
			inline Shader* GetShader() const override { return m_Shader; }

		private :
			VertexArray* m_VertexArray;
			IndexBuffer* m_IndexBuffer;
			Shader* m_Shader;
	};




	class RGF_API BatchedSprite : public Renderable {

		public:
			BatchedSprite(glm::vec3 position, glm::vec3 scale, glm::vec4 color)
				: Renderable(position, scale, color)
			{
			
			}


	};

}