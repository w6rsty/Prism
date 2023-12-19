#pragma once

#include "geo/geometry.hpp"
#include "pecs.hpp"
#include "glm/glm.hpp"
#include "config.hpp"
#include "systems.hpp"
#include "render_info.hpp"

#include <random>

using namespace pecs;

struct Particle {
    glm::vec3 offset;
    glm::vec3 scale = glm::vec3(1.0f);
};
