#pragma once

#include "engine/log.hpp"
#include "glm/geometric.hpp"
#include "pecs.hpp"
#include "config.hpp"
#include "player.hpp"
#include "ticker.hpp"

using namespace pecs;

inline void chaseSystem(Commands& command, Queryer queryer, Resources resources, Events& events) {
    float length = glm::length(ModelPosition - EnemyPosition);
    auto& timer = resources.Get<Timer>();

    auto entities = queryer.Query<Enemy>();

    if (length < 5.0f) {
        PRISM_INFO("Enemy is chasing you!");
        timer.tick++;

        float deltaX = 40.0f * (1.0f / timer.max);
        HPModelPosition = glm::vec3(HPModelPosition.x - deltaX, HPModelPosition.y, HPModelPosition.z);
    }

    if (timer.tick > timer.max) {
        PRISM_WARN("Game over");
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}