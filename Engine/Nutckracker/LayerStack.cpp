#include "nkpch.h"
#include "LayerStack.h"

namespace NK {
    LayerStack::LayerStack()
	{
		m_LayerInsert_ = m_Layers_.begin();
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers_)
			delete layer;
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_LayerInsert_ = m_Layers_.emplace(m_LayerInsert_, layer);
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers_.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_Layers_.begin(), m_Layers_.end(), layer);
		if (it != m_Layers_.end())
		{
			m_Layers_.erase(it);
			m_LayerInsert_--;
		}   
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(m_Layers_.begin(), m_Layers_.end(), overlay);
		if (it != m_Layers_.end())
			m_Layers_.erase(it);
	}
}