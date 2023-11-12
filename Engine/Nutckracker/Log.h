#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace NK 
{
    class NK_API Log
    {
    public:
    
        static void Init();
        
        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;

    };
}

// Core log macros
#define NK_CORE_TRACE(...) ::NK::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define NK_CORE_INFO(...)  ::NK::Log::GetCoreLogger()->info(__VA_ARGS__)
#define NK_CORE_WARN(...)  ::NK::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define NK_CORE_ERROR(...) ::NK::Log::GetCoreLogger()->error(__VA_ARGS__)
#define NK_CORE_FATAL(...) ::NK::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define NK_TRACE(...) ::NK::Log::GetClientLogger()->trace(__VA_ARGS__)
#define NK_INFO(...)  ::NK::Log::GetClientLogger()->info(__VA_ARGS__)
#define NK_WARN(...)  ::NK::Log::GetClientLogger()->warn(__VA_ARGS__)
#define NK_ERROR(...) ::NK::Log::GetClientLogger()->error(__VA_ARGS__)
#define NK_FATAL(...) ::NK::Log::GetClientLogger()->fatal(__VA_ARGS__)