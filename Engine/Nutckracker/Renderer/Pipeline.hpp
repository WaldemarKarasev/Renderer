#pragma once

#include "FrameInfo.hpp"
#include "Shader.hpp"
namespace NK {

    class Pipeline 
    {
    public:
        virtual ~Pipeline() {}

        virtual void Init(SystemInfo& systemInfo) = 0;

        virtual void Bind() = 0;

        static Pipeline* Create(Shader* shader);
    };
    
}