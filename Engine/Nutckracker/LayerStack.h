#pragma once

#include "Nutckracker/Core.h"
#include "Layer.h"

#include <vector>

namespace NK {
    class NK_API LayerStack
    {
    public:
        LayerStack();
        ~LayerStack();
        
        void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

        std::vector<Layer*>::iterator begin() { return m_Layers_.begin(); }
        std::vector<Layer*>::iterator end()   { return m_Layers_.end(); }

    private:
        std::vector<Layer*> m_Layers_;
        unsigned int m_LayerInsertIndex = 0;
    };
}