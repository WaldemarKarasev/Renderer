#pragma once

#include "Core.h"
#include "RendererAPI.hpp"

namespace NK {
    
    class NK_API GraphicsContext
    {
    public:
        virtual void Init() = 0;
        static GraphicsContext* Create();
    };
}