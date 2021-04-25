#include "openglwindow.hpp"

#include <fmt/core.h>
#include <imgui.h>
#include <tiny_obj_loader.h>

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/hash.hpp>
#include <unordered_map>
#include <glm/gtc/matrix_inverse.hpp>

// Custom specialization of std::hash injected in namespace std
namespace std {
template <>
struct hash<Vertex> {
  size_t operator()(Vertex const& vertex) const noexcept {
    std::size_t h1{std::hash<glm::vec3>()(vertex.position)};
    return h1;
  }
};
}  // namespace std

void OpenGLWindow::handleEvent(SDL_Event& ev) {
  glm::ivec2 mousePosition;
  SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
  float width = m_viewportWidth/2;
  float height = m_viewportHeight/2;
  //Set x and y between -1 and 1
  x = ((float)mousePosition.x - width)/width;
  y = -((float)mousePosition.y - height)/height;  
  if (ev.type == SDL_KEYDOWN) {
    if (ev.key.keysym.sym == SDLK_UP || ev.key.keysym.sym == SDLK_w)
      m_dollySpeed = 1.0f;
    if (ev.key.keysym.sym == SDLK_DOWN || ev.key.keysym.sym == SDLK_s)
      m_dollySpeed = -1.0f;
    if (ev.key.keysym.sym == SDLK_LEFT || ev.key.keysym.sym == SDLK_a)
      m_truckSpeed = -1.0f;
    if (ev.key.keysym.sym == SDLK_RIGHT || ev.key.keysym.sym == SDLK_d)
      m_truckSpeed = 1.0f;
    if (ev.key.keysym.sym == SDLK_z) m_heightSpeed = 1.0f;
  }
  if (ev.type == SDL_KEYUP) {
    if ((ev.key.keysym.sym == SDLK_UP || ev.key.keysym.sym == SDLK_w) &&
        m_dollySpeed > 0)
      m_dollySpeed = 0.0f;
    if ((ev.key.keysym.sym == SDLK_DOWN || ev.key.keysym.sym == SDLK_s) &&
        m_dollySpeed < 0)
      m_dollySpeed = 0.0f;
    if ((ev.key.keysym.sym == SDLK_LEFT || ev.key.keysym.sym == SDLK_a) &&
        m_truckSpeed < 0)
      m_truckSpeed = 0.0f;
    if ((ev.key.keysym.sym == SDLK_RIGHT || ev.key.keysym.sym == SDLK_d) &&
        m_truckSpeed > 0)
      m_truckSpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_z) m_heightSpeed = 1.0f;
  }
  if (ev.type == SDL_MOUSEMOTION) {
    if(x < -0.3){
      m_panSpeed = -1.0f;
    }
    else if(x > 0.3){
      m_panSpeed = 1.0f;
    }
    else{
      m_panSpeed = 0.0f;
    }
    if(y < -0.3){
      m_panySpeed = -1.0f;
    }
    else if(y > 0.3){
      m_panySpeed = 1.0f;
    }
    else{
      m_panySpeed = 0.0f;
    }
  }
}

void OpenGLWindow::initializeGL() {
  glClearColor(0, 0, 0, 1);

  // Enable depth buffering
  glEnable(GL_DEPTH_TEST);

  // Create program
  m_program = createProgramFromFile(getAssetsPath() + "texture.vert",
                                    getAssetsPath() + "texture.frag");

  // Load model
  loadModel(getAssetsPath() + "Grass_Block.obj");
  m_mappingMode = 3;  // "From mesh" option
  resizeGL(getWindowSettings().width, getWindowSettings().height);
}

void OpenGLWindow::loadModel(std::string_view path) {
  m_model.loadDiffuseTexture(getAssetsPath() + "/Grass_Block_TEX.png");
  m_model.loadFromFile(path);
  m_model.setupVAO(m_program);
  m_trianglesToDraw = m_model.getNumTriangles();

  // Use material properties from the loaded model
  m_Ka = m_model.getKa();
  m_Kd = m_model.getKd();
  m_Ks = m_model.getKs();
  m_shininess = m_model.getShininess();
}

void OpenGLWindow::paintGL() {
  float cubeScales = 0.2f;
  float floor = -0.5f;
  float divider = 4.3f;
  update();

  // Clear color buffer and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  glUseProgram(m_program);

  // Get location of uniform variables
  GLint viewMatrixLoc{glGetUniformLocation(m_program, "viewMatrix")};
  GLint projMatrixLoc{glGetUniformLocation(m_program, "projMatrix")};
  GLint modelMatrixLoc{glGetUniformLocation(m_program, "modelMatrix")};
  GLint normalMatrixLoc{glGetUniformLocation(m_program, "normalMatrix")};
  GLint lightDirLoc{glGetUniformLocation(m_program, "lightDirWorldSpace")};
  GLint shininessLoc{glGetUniformLocation(m_program, "shininess")};
  GLint IaLoc{glGetUniformLocation(m_program, "Ia")};
  GLint IdLoc{glGetUniformLocation(m_program, "Id")};
  GLint IsLoc{glGetUniformLocation(m_program, "Is")};
  GLint KaLoc{glGetUniformLocation(m_program, "Ka")};
  GLint KdLoc{glGetUniformLocation(m_program, "Kd")};
  GLint KsLoc{glGetUniformLocation(m_program, "Ks")};
  GLint diffuseTexLoc{glGetUniformLocation(m_program, "diffuseTex")};
  GLint mappingModeLoc{glGetUniformLocation(m_program, "mappingMode")};

  // Set uniform variables for viewMatrix and projMatrix
  // These matrices are used for every scene object
  glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_camera.m_viewMatrix[0][0]);
  glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_camera.m_projMatrix[0][0]);
  glUniform1i(diffuseTexLoc, 0);
  glUniform1i(mappingModeLoc, m_mappingMode);

  auto lightDirRotated{m_rotation * m_lightDir};
  glUniform4fv(lightDirLoc, 1, &lightDirRotated.x);
  glUniform4fv(IaLoc, 1, &m_Ia.x);
  glUniform4fv(IdLoc, 1, &m_Id.x);
  glUniform4fv(IsLoc, 1, &m_Is.x);

  // Draw Cube   
  glm::mat4 model{1.0f};

  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(10/divider, floor + 1/divider, 10/divider));
  model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1, 0, 0));
  model = glm::scale(model, glm::vec3(cubeScales));

  glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  glUniform1f(shininessLoc, m_shininess);
  glUniform4fv(KaLoc, 1, &m_Ka.x);
  glUniform4fv(KdLoc, 1, &m_Kd.x);
  glUniform4fv(KsLoc, 1, &m_Ks.x);
  m_model.render(m_trianglesToDraw);
  // Draw floor
  for(int i = 0; i < 20; i++){
    for(int j = 0; j < 20; j++){
      model = glm::mat4(1.0);
      model = glm::translate(model, glm::vec3(0.0f + i/divider, floor, 0.0f + j/divider));
      model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1, 0, 0));
      model = glm::scale(model, glm::vec3(cubeScales));

      glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
      auto modelViewMatrix{glm::mat3(m_camera.m_viewMatrix * model)};
      glm::mat3 normalMatrix{glm::inverseTranspose(modelViewMatrix)};
      glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);
      glUniform1f(shininessLoc, m_shininess);
      glUniform4fv(KaLoc, 1, &m_Ka.x);
      glUniform4fv(KdLoc, 1, &m_Kd.x);
      glUniform4fv(KsLoc, 1, &m_Ks.x);
      m_model.render(m_trianglesToDraw);
    }
  }
  glUseProgram(0);
}

void OpenGLWindow::paintUI() { 
  abcg::OpenGLWindow::paintUI(); 
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  m_camera.computeProjectionMatrix(width, height);
}

void OpenGLWindow::terminateGL() {
  glDeleteProgram(m_program);
}

void OpenGLWindow::update() {
  float deltaTime{static_cast<float>(getDeltaTime())};

  // Update LookAt camera
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);
  m_camera.pany(m_panySpeed * deltaTime);
  m_heightSpeed = m_camera.height(m_heightSpeed * deltaTime);
}