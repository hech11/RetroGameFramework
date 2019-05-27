#pragma once

#include "RetroGF/Core.h"
#include "RetroGF/WindowImpl.h"



#include "RetroGF/Events/Event.h"
#include "RetroGF/Events/KeyEvents.h"
#include "RetroGF/Events/WindowEvents.h"



#include "RetroGF/LayerStack.h"
#include "RetroGF/Imgui/ImguiLayer.h"
#include "RetroGF/Imgui/ImguiEngineEditor.h"

#include "RetroGF/Timer.h"




// This would be inherited from another class from CLIENT side.
// The application class is the main 'hub' of all RGF compondents.

// This would initialize the window, renderer, physics, ect.


namespace RGF {

	class Renderer;
	class RGF_API Application {

		public :
			Application();
			virtual ~Application();


			void Run();
			void OnEvent(Event& e);

			void PushLayer(Layer* layer);
			void PushOverlay(Layer* overlay);


			WindowImpl& GetWindow() { return *m_Window; }
			Renderer& GetRenderer() { return *m_Renderer; }

			inline static Application& GetApp() { return *s_Instance; };
		private :
			static Application* s_Instance;

			std::unique_ptr<WindowImpl> m_Window;
			std::unique_ptr<Renderer> m_Renderer;

			LayerStack m_LayerStack;
			Timer m_AppTimer;


			bool m_IsRunning = true;
#ifndef RGF_DISTRIBUTE
		private :
			ImguiLayer* m_ImguiLayer;
			ImguiEngineEditor* m_EngineEditorLayer;
#endif
		private :
			// Callbacks. ( Should all return bools )
			bool OnWindowClose(WindowCloseEvent& e);

	};

	// Has to be defined on CLIENT side in order to create a RGF application.
	Application* CreateApplication(); // Should return a HEAP allocated application.
}