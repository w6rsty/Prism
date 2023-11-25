#pragma once

#include <GLFW/glfw3.h>

static GLFWwindow* GAME_WINDOW;
static const int WIDTH = 1800;
static const int HEIGHT = 1200;
static const char* GAME_NAME = "Prism Engine";

static bool FIRST_MOUSE = true;
static float LAST_X = static_cast<float>(WIDTH) / 2;
static float LAST_Y = static_cast<float>(HEIGHT) / 2;

#define PRISM_KEY_LAST GLFW_KEY_LAST

static bool PRISM_KEYS[PRISM_KEY_LAST] = { false };
