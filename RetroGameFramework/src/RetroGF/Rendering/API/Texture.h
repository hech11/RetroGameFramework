#pragma once

#include "RetroGF/Core/Core.h"

#include "RGFpch.h"
#include <GLM/glm/glm.hpp>


// Texture interface.
// The 'Create' method will decide depending on the API choice. OpenGL, Directx 11 or 12, vulkan etc.

namespace RGF {
	// A abstracted way to format the textures.
	// The abstraction code should be implemented in the API texture's ".cpp" file.

	





	class RGF_API Texture {

		public :
					
			struct TextureProperties {
				enum class Format {
					None = -1,
					RGBA,
					RGB
				};
				enum class Filter {
					None = -1,
					Nearest,
					Linear,
				};
				enum class Wrap {
					None = -1,
					Clamp_To_Edge,
					Repeat
				};

				Format TexFormat = Format::RGBA;
				Filter TexFilter = Filter::Nearest;
				Wrap TexWrap = Wrap::Clamp_To_Edge;
				bool GenerateMipMaps = false;

				int Width = 0, Height = 0, BPP = 0;
				mutable unsigned char Slot = 0;

				mutable bool IsBound = false;
			};


			Texture() {}
			virtual ~Texture(){}

			virtual void Bind(unsigned char slot = 0) const = 0;
			virtual void Unbind() const = 0;

			//TODO: Set data only sets the whole texture.
			virtual void SetData(void* data, unsigned int size) = 0;
			virtual void SetData(const std::string& filepath) = 0;

			virtual unsigned int GetHandleID() const = 0;


			//TODO: Should this be stored in the platform specific classes or in this class?
			inline int GetWidth() const { return m_Props.Width; }
			inline int GetHeight() const { return m_Props.Height; }
			inline int GetBPP() const { return m_Props.BPP; }
			inline unsigned char GetCurrentSlot() const { return m_Props.Slot; }
			inline bool IsBound() const { return m_Props.IsBound; }

			virtual bool operator==(const Texture& other) const = 0;

		public :
			static Ref<Texture> Create(const std::string& filepath, TextureProperties props = TextureProperties());
			static Ref<Texture> Create(unsigned int width, unsigned int height, Texture::TextureProperties::Format format = Texture::TextureProperties::Format::RGBA);

		protected :
			TextureProperties m_Props;
			
	};

	class TextureBounds {
		private :

			void CalculateBounds(const glm::u32vec4& bounds) {
				float xOffset = (float) bounds.x / m_Texture->GetWidth();
				float yOffset = (float) bounds.y / m_Texture->GetHeight();

				float xSize = (float) bounds.z / m_Texture->GetWidth();
				float ySize = (float) bounds.w / m_Texture->GetHeight();



				m_NormilizedBounds[0] = { xOffset, 1.0f - (yOffset + ySize) }; // bottom left
				m_NormilizedBounds[1] = { xOffset + xSize, 1.0f - (yOffset + ySize) }; // bottom right
				m_NormilizedBounds[2] = { xOffset + xSize, 1.0f - yOffset }; // top right
				m_NormilizedBounds[3] = { xOffset, 1.0f - yOffset }; // top left

			}

		public :

			TextureBounds(const Ref<Texture>& texture, const glm::u32vec4& bounds)
				: m_Texture(texture)
			{
				CalculateBounds(bounds);
			}


			inline const std::array<glm::vec2, 4>& GetBoundsNormilized() const { return m_NormilizedBounds; }
			inline const Ref<Texture>& GetTexture() const { return m_Texture; }


			static Ref<TextureBounds> Create(const Ref<Texture>& texture, const glm::u32vec4& bounds) {
				return CreateRef<TextureBounds>(texture, bounds);
			}
		private :
			Ref<Texture> m_Texture;
			std::array<glm::vec2, 4> m_NormilizedBounds;

	};

}