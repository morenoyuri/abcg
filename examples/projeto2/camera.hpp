#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class OpenGLWindow;

class Camera {
 public:
  void computeViewMatrix();
  void computeProjectionMatrix(int width, int height);

  void dolly(float speed);
  void truck(float speed);
  void pan(float speed);
  void pany(float speed);
  float m_height = 0.0f;
  float m_pany = 0.0f;

 private:
  friend OpenGLWindow;
  float m_velocity = 1.0f;
  glm::vec3 m_eye{glm::vec3(0.0f, m_height, 2.5f)};  // Camera position
  glm::vec3 m_at{glm::vec3(0.0f, m_height, 0.0f)};   // Look-at point
  glm::vec3 m_up{glm::vec3(0.0f, m_velocity, 0.0f)};   // "up" direction

  // Matrix to change from world space to camera soace
  glm::mat4 m_viewMatrix;

  // Matrix to change from camera space to clip space
  glm::mat4 m_projMatrix;
};

#endif