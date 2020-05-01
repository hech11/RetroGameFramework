#pragma once

#ifndef RGF_DISTRIBUTE

#include "RetroGF/Core/Core.h"
#include "RetroGF/Core/Layer.h"

#include "RetroGF/Events/WindowEvents.h"

namespace RGF {

	class RGF_API ImguiLayer : public Layer {

	public:
		ImguiLayer();

		~ImguiLayer() {}


		virtual void OnUpdate(float dt) override {}
		virtual void OnEvent(Event& e) override {}

		virtual void Init()override;
		virtual void ShutDown()override;
		virtual void OnImguiRender() override;

		void Start();
		void End();



	};

}

#endif