#pragma once

#include "Core.h"

namespace NK {
    
    class NK_API GraphicsContext
    {
    public:
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
    };
}