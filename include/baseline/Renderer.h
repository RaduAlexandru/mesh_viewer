#pragma once

#include <iostream>
#include <memory>

//OPENGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "linmath.h"

#include "baseline/Scene.h"

#include "shaders/preamble.glsl"


class Renderer {
public:
  Renderer();

  // ImVec4 m_clear_color = ImColor(114, 144, 154);
  glm::vec4 m_clear_color;

  GLuint m_vbo_static;
  GLuint m_ibo_static;

  void draw();
  void init (std::shared_ptr<Scene> scene);
private:
  std::shared_ptr<Scene> m_scene;
};
