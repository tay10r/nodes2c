#pragma once

#include <GLFW/glfw3.h>

class app;

app* app_setup(GLFWwindow*);

void app_loop(app* self);

void app_teardown(app* self);
