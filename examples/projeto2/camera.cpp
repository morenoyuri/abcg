#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

void Camera::computeProjectionMatrix(int width, int height) {
  m_projMatrix = glm::mat4(1.0f);
  auto aspect{static_cast<float>(width) / static_cast<float>(height)};
  m_projMatrix = glm::perspective(glm::radians(70.0f), aspect, 0.1f, 5.0f);
}

void Camera::computeViewMatrix() {
  m_viewMatrix = glm::lookAt(m_eye, m_at, m_up);
}

void Camera::dolly(float speed) {
  // Calcula os vetores para depois arrumar o ângulo que foi modificado com o pany
  glm::vec3 forward_1 = glm::normalize(m_at - m_eye);
  glm::vec3 left_1 = glm::cross(m_up, forward_1);
  glm::mat4 transform{glm::mat4(1.0f)};

  // Rotate camera around its local x axis
  transform = glm::translate(transform, m_eye);
  transform = glm::rotate(transform, m_pany, left_1);
  transform = glm::translate(transform, -m_eye);

  //Transforma o vetor para 0 graus no eixo x
  glm::vec3 m_at_diff = transform * glm::vec4(m_at, 1.0f);

  glm::vec3 forward = glm::normalize(m_at_diff - m_eye);
  // Move eye and center forward (speed > 0) or backward (speed < 0)
  m_eye += forward * speed;
  m_at += forward * speed;

  computeViewMatrix();
}

void Camera::truck(float speed) {
  // Calcula os vetores para depois arrumar o ângulo que foi modificado com o pany
  glm::vec3 forward_1 = glm::normalize(m_at - m_eye);
  glm::vec3 left_1 = glm::cross(m_up, forward_1);
  glm::mat4 transform{glm::mat4(1.0f)};

  // Rotate camera around its local x axis
  transform = glm::translate(transform, m_eye);
  transform = glm::rotate(transform, m_pany, left_1);
  transform = glm::translate(transform, -m_eye);

  //Transforma o vetor para 0 graus no eixo x
  glm::vec3 m_at_diff = transform * glm::vec4(m_at, 1.0f);

  glm::vec3 forward = glm::normalize(m_at_diff - m_eye);
  // Compute vector to the left
  glm::vec3 left = glm::cross(m_up, forward);

  // Move eye and center to the left (speed < 0) or to the right (speed > 0)
  m_at -= left * speed;
  m_eye -= left * speed;

  computeViewMatrix();
}

void Camera::pan(float speed) {
  glm::mat4 transform{glm::mat4(1.0f)};

  // Rotate camera around its local y axis
  transform = glm::translate(transform, m_eye);
  transform = glm::rotate(transform, -speed, m_up);
  transform = glm::translate(transform, -m_eye);

  m_at = transform * glm::vec4(m_at, 1.0f);

  computeViewMatrix();
}

void Camera::pany(float speed) {
  //Verifica o angulo para limitar o movimento
  if(!((m_pany + speed >= 1.5) || (m_pany + speed <= -1.5))){
    // Compute forward vector (view direction)
    glm::vec3 forward = glm::normalize(m_at - m_eye);
    // Compute vector to the left
    glm::vec3 left = glm::cross(m_up, forward);
    // Matriz identidade
    glm::mat4 transform{glm::mat4(1.0f)};

    // Rotate camera around its local x axis
    transform = glm::translate(transform, m_eye);
    transform = glm::rotate(transform, -speed, left);
    transform = glm::translate(transform, -m_eye);
    m_at = transform * glm::vec4(m_at, 1.0f);
    m_pany += speed;
    computeViewMatrix();
  }
}

