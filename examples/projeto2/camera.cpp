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
  // Compute forward vector (view direction)
  glm::vec3 forward_1 = glm::normalize(m_at - m_eye);
  // Compute vector to the left
  glm::vec3 left_1 = glm::cross(m_up, forward_1);
  // Matriz identidade
  glm::mat4 transform{glm::mat4(1.0f)};

  // Rotate camera around its local y axis
  transform = glm::translate(transform, m_eye);
  transform = glm::rotate(transform, m_pany, left_1);
  transform = glm::translate(transform, -m_eye);

  glm::vec3 m_at_diff = transform * glm::vec4(m_at, 1.0f);

  glm::vec3 forward = glm::normalize(m_at_diff - m_eye);
  // Move eye and center forward (speed > 0) or backward (speed < 0)
  m_eye += forward * speed;
  m_at += forward * speed;

  computeViewMatrix();
}

void Camera::truck(float speed) {
  // Compute forward vector (view direction)
  glm::vec3 forward_1 = glm::normalize(m_at - m_eye);
  // Compute vector to the left
  glm::vec3 left_1 = glm::cross(m_up, forward_1);
  // Matriz identidade
  glm::mat4 transform{glm::mat4(1.0f)};

  // Rotate camera around its local y axis
  transform = glm::translate(transform, m_eye);
  transform = glm::rotate(transform, m_pany, left_1);
  transform = glm::translate(transform, -m_eye);

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
  if(!((m_pany + speed >= 1.5) || (m_pany + speed <= -1.5))){
    // Compute forward vector (view direction)
    glm::vec3 forward = glm::normalize(m_at - m_eye);
    // Compute vector to the left
    glm::vec3 left = glm::cross(m_up, forward);
    // Matriz identidade
    glm::mat4 transform{glm::mat4(1.0f)};

    // Rotate camera around its local y axis
    transform = glm::translate(transform, m_eye);
    transform = glm::rotate(transform, -speed, left);
    transform = glm::translate(transform, -m_eye);
    m_at = transform * glm::vec4(m_at, 1.0f);
    m_pany += speed;
    computeViewMatrix();
  }
}

float Camera::height(float speed) {
  m_at += m_up * speed * m_velocity;
  m_eye += m_up * speed * m_velocity;
  m_height += m_velocity * speed;
  if(speed <= 0.0f){
    m_velocity = 1.0f;
  }
  else if(m_velocity > 0.2f){
    m_velocity -= 0.1f;
  }
  if(m_height >= 0.6f){
    m_jump_direc = -1.0f;
  }
  if(m_height <= 0.0f && speed < 0.0f){
    m_jump_direc = 0.0f;
  }
  if(m_height < 0.6f && speed > 0.0f){
    m_jump_direc = 1.0f;
  }
  computeViewMatrix();
  return m_jump_direc;
}
