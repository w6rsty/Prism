#pragma once

#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace prism {

class Log {
public:
    static void Init();
    
    inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return coreLogger_; }
    inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return clientLogger_; }
private:
    static std::shared_ptr<spdlog::logger> coreLogger_;
    static std::shared_ptr<spdlog::logger> clientLogger_;
};

#define PRISM_ENABLE_LOG

#ifdef PRISM_ENABLE_LOG
    #define PRISM_CORE_ERROR(...)   ::prism::Log::GetCoreLogger()->error(__VA_ARGS__)
    #define PRISM_CORE_WARN(...)    ::prism::Log::GetCoreLogger()->warn(__VA_ARGS__)
    #define PRISM_CORE_INFO(...)    ::prism::Log::GetCoreLogger()->info(__VA_ARGS__)
    #define PRISM_CORE_TRACE(...)   ::prism::Log::GetCoreLogger()->trace(__VA_ARGS__)
    #define PRISM_CORE_FATAL(...)   ::prism::Log::GetCoreLogger()->fatal(__VA_ARGS__)

    #define PRISM_ERROR(...)        ::prism::Log::GetClientLogger()->error(__VA_ARGS__)
    #define PRISM_WARN(...)         ::prism::Log::GetClientLogger()->warn(__VA_ARGS__)
    #define PRISM_INFO(...)         ::prism::Log::GetClientLogger()->info(__VA_ARGS__)
    #define PRISM_TRACE(...)        ::prism::Log::GetClientLogger()->trace(__VA_ARGS__)
    #define PRISM_FATAL(...)        ::prism::Log::GetClientLogger()->fatal(__VA_ARGS__)
#else 
    #define PRISM_CORE_ERROR(...)
    #define PRISM_CORE_WARN(...) 
    #define PRISM_CORE_INFO(...) 
    #define PRISM_CORE_TRACE(...)
    #define PRISM_CORE_FATAL(...)

    #define PRISM_ERROR(...)     
    #define PRISM_WARN(...)
    #define PRISM_INFO(...)
    #define PRISM_TRACE(...)
    #define PRISM_FATAL(...)
#endif

} // namespace prism