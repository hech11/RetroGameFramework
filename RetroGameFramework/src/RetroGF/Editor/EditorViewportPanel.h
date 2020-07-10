#pragma once

#include "Glm/glm.hpp"
#include "RetroGF/Rendering/API/FrameBuffer.h"
#include "RetroGF/Editor/EditorCamera.h"
#include "RetroGF/Events/Event.h"


namespace RGF {

	class EditorViewportPanel {


		public :
			EditorViewportPanel();

			void OnEvent(Event& e);

			void OnUpdate(float ts);
			void OnImguiRender();


			// This is just temp for now.
			void DrawToViewport();
			void FinishDrawing();

			bool IsHovered() const { return m_IsViewportHovered; }
			bool IsFocused() const { return m_IsViewportFocused; }

			const glm::vec2& GetViewportSize() const { return m_ViewportSize; }
			const glm::vec2& GetPanelPosition() const { return m_ViewportPanelPosition; }



			// Not ideal to have for now..
			const Scoped<EditorCamera>& GetCamera() const { return m_EditorCamera; }
			Scoped<EditorCamera>& GetCamera() { return m_EditorCamera; }

		private :

			Ref<FrameBuffer> m_ViewportFramebuffer;
			Scoped<EditorCamera> m_EditorCamera;

			glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
			glm::vec2 m_ViewportPanelSize = { 0.0f, 0.0f };
			glm::vec2 m_ViewportPanelPosition = { 0.0f, 0.0f };

			bool m_IsViewportHovered, m_IsViewportFocused;

	};
}