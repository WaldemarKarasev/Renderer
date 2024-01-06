#pragma once

#include <stdint.h>

#include "Nutckracker/Core.h"

namespace NK {

    class UniformBuffer
    {
    public:
        virtual ~UniformBuffer() {}
        virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;


        static UniformBuffer* Create(uint32_t size, uint32_t binding);
    };
}