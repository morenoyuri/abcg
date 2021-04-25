#ifndef DRONE_HPP_
#define DRONE_HPP_

#include "abcg.hpp"
#include "gamedata.hpp"

class Game;
class OpenGLWindow;

class Drone {
 public:
  void initializeGL(GLuint program);
  void paintGL(const GameData &gameData);
  void terminateGL();

  void update(float deltaTime);
  void setRotation(float rotation) { m_rotation = rotation; }
  void setTranslation(glm::vec2 translation) { m_translation = translation; }
  void setPos(glm::vec2 pos) { m_pos = pos; }
  float m_rotation{};
  float m_scale{0.125f};
  glm::vec2 m_translation{glm::vec2({0, 1})};
  glm::vec2 m_pos{glm::vec2({0, 1})};
  glm::vec2 m_velocity{glm::vec2(0)};
  bool m_moving = false;

 private:
  friend Game;
  friend OpenGLWindow;

  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_colorLoc{};
  GLint m_scaleLoc{};
  GLint m_rotationLoc{};

  GLuint m_vao{};
  GLuint m_vbo{};
  GLuint m_ebo{};

  glm::vec4 m_color{1};

  abcg::ElapsedTimer m_trailBlinkTimer;
  abcg::ElapsedTimer m_bulletCoolDownTimer;
};

#endif