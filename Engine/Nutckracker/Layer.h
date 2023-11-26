#pragma once

#include "Nutckracker/Core.h"
#include "Nutckracker/Events/Event.h"


namespace NK {
    
    class NK_API Layer
    {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer();

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate() {}
        virtual void OnImGuiRender() {}
        virtual void OnEvent(Event& event) {}

        inline const std::string& GetName() const {return m_DebugName_;}

    protected:
        std::string m_DebugName_;
    };

}
