#pragma once

#include "Nutckracker/Layer.h"

#include "Nutckracker/Events/ApplicationEvent.h"
#include "Nutckracker/Events/KeyEvent.h"
#include "Nutckracker/Events/MouseEvent.h"

namespace NK {
    class NK_API ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnImGuiRender() override;

        void Begin();
        void End();
    private:
        float m_Time = 0.0f;
    };
}