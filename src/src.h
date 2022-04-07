#ifndef SRC_H
#define SRC_H
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/norm.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_JA_range.h" // original header


#include "misc.h"
#include "input.h"
#include "shader.h"
#include "model.h"
#include "physics.h"
#include "camera.h"

extern int  window_w;
extern int  window_h; 
extern bool firstMouse;
extern float deltaTime;
extern float lastFrame;
extern glm::vec3 cursor;
extern Camera camera;
extern std::vector<glm::vec3> lightPositions;

typedef struct _settings_{
    int shade_resolution;
    int window_width;
    int window_height;
    bool shadow;
}settings_t;
#endif
