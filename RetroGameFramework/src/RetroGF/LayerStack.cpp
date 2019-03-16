#include "RGFpch.h"
#include "LayerStack.h"





namespace RGF {

	LayerStack::LayerStack() : m_OverLayIndex(0){
	}


	LayerStack::~LayerStack() {
		for (Layer* layer : m_LayerStack) {
			layer->ShutDown();
			delete layer;
		}
	} 


	void LayerStack::PushLayer(Layer* layer) {
		layer->Init();
		m_LayerStack.emplace(m_LayerStack.end() - m_OverLayIndex, layer);
	}
	void LayerStack::PushOverlay(Layer* overlay) {
		overlay->Init();
		m_LayerStack.emplace_back(overlay);
	}

	void LayerStack::PopLayer() {
		m_LayerStack.erase(m_LayerStack.end() - m_OverLayIndex);

	}
	void LayerStack::PopOverlay() {
		m_LayerStack.pop_back();
		m_OverLayIndex--;
	}

	void LayerStack::PopLayer(Layer* layer) {
		const auto& index = std::find(m_LayerStack.begin(), m_LayerStack.end(), layer);
		if (index != m_LayerStack.end()) {
			m_LayerStack.erase(index);
		}
	}
	void LayerStack::PopOverlay(Layer* layer) {
		const auto& index = std::find(m_LayerStack.begin(), m_LayerStack.end(), layer);
		if (index != m_LayerStack.end()) {
			m_LayerStack.erase(index);
			m_OverLayIndex--;
		}
	}

}