#ifndef BATS_HPP_
#define BATS_HPP_

#include <list>
#include <random>

#include "abcg.hpp"
#include "gamedata.hpp"
#include "drone.hpp"

class OpenGLWindow;

class Bats {
 public:
  void initializeGL(GLuint program, int quantity);
  void paintGL();
  void terminateGL();

  void update(float deltaTime);

 private:
  friend OpenGLWindow;

  GLuint m_program{};
  GLint m_colorLoc{};
  GLint m_rotationLoc{};
  GLint m_translationLoc{};
  GLint m_scaleLoc{};
  abcg::ElapsedTimer m_restartWaitTimer;

  struct Bat {
    GLuint m_vao{};
    GLuint m_vbo{};
    GLuint m_ebo{};

    float m_angularVelocity{};
    glm::vec4 m_color{0};
    bool m_hit{false};
    float m_rotation{};
    float m_scale{};
    glm::vec2 m_translation{glm::vec2(0)};
    glm::vec2 m_velocity{glm::vec2(0)};
  };

  std::list<Bat> m_bats;

  std::default_random_engine m_randomEngine;
  std::uniform_real_distribution<float> m_randomDist{-1.0f, 1.0f};

  Bats::Bat createBat(glm::vec2 translation = glm::vec2(0),
                                     float scale = 0.18f);
};

#endif