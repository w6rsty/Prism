#pragma once

#include "log.hpp"

#define PRISM_CORE_ASSERT(x, ...) { if(!(x)) { PRISM_CORE_ERROR("Core Assertion failed {0}", __VA_ARGS__); assert(false); } }

#define PRISM_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)