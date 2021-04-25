#include "openglwindow.hpp"
#include <fmt/core.h>
#include <imgui.h>
#include <vector>

static std::vector<std::string> buttonTexts;
static std::string turno = "X";
static std::string turnoCompleto = turno + " turn";
static std::string result;

std::string verifyWinner(std::vector<std::string> positions){
  if(positions[0] == positions[1] && positions[1] == positions[2]){
    return positions[0];
  }
  if(positions[3] == positions[4] && positions[4] == positions[5]){
    return positions[3];
  }
  if(positions[6] == positions[7] && positions[7] == positions[8]){
    return positions[6];
  }
  if(positions[0] == positions[3] && positions[3] == positions[6]){
    return positions[0];
  }
  if(positions[1] == positions[4] && positions[4] == positions[7]){
    return positions[1];
  }
  if(positions[2] == positions[5] && positions[5] == positions[8]){
    return positions[2];
  }
  if(positions[0] == positions[4] && positions[4] == positions[8]){
    return positions[0];
  }
  if(positions[2] == positions[4] && positions[4] == positions[6]){
    return positions[2];
  }
  bool test = true;
  for(int i = 0; i < 9; i++){
    if(positions[i] == ""){
      test = false;
    }
  }
  if(test){
    return "Tie";
  }
  return "";
}

void TextCenter(std::string text, ImVec2 tamanho) {
  float font_size = ImGui::GetFontSize() * text.size() / 2;
  ImGui::SameLine(
  ImGui::GetWindowSize().x / 2 -
  font_size + (font_size / 2)
  );
  ImGui::Text(text.c_str(), tamanho);
}

void OpenGLWindow::initializeGL() {
  auto windowSettings{getWindowSettings()};
  fmt::print("Initial window size: {}x{}\n", windowSettings.width,
             windowSettings.height);
}

void OpenGLWindow::paintGL() {
  // Set the clear color
  glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2],
               m_clearColor[3]);
  // Clear the color buffer
  glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::paintUI() {
  // Parent class will show fullscreen button and FPS meter
  abcg::OpenGLWindow::paintUI();
  buttonTexts.resize(9);

  // Our own ImGui widgets go below
  {
    // Window begin
    ImGui::Begin("Tic-Tac-Toe!");

    // Static text
    auto settings = abcg::OpenGLWindow::getWindowSettings();
    ImGui::SetWindowSize(ImVec2(settings.width, settings.height));
    ImGui::SetWindowPos(ImVec2(0, 0));
    settings.showFPS = false;
    settings.showFullscreenButton = false;
    abcg::OpenGLWindow::setWindowSettings(settings);
    // More static text
    TextCenter(turnoCompleto, ImVec2(-1, 20));
    ImGui::Columns(3);
    for(int i = 0; i < buttonTexts.size(); i++){
      ImGui::Button(buttonTexts[i].c_str(), ImVec2(settings.width/3, (settings.height-110)/3));
      if (ImGui::IsItemClicked() && buttonTexts[i] == "" && (turnoCompleto != "X is the winner!" && turnoCompleto != "O is the winner!")){
        buttonTexts[i] = turno;
        if(turno == "O"){
          turno = "X";
        }
        else{
          turno = "O";
        }
        turnoCompleto = turno + " turn";
      }
      if(i%3 == 2){
        ImGui::NextColumn();
      }
    }
    ImGui::Columns(1);
    ImGui::Button("Restart Game", ImVec2(-1, 50));
    if (ImGui::IsItemClicked()){
      for(int i = 0; i < buttonTexts.size(); i++){
        buttonTexts[i] = "";
        turno = "X";
        turnoCompleto = turno + " turn";
      }
    }
    result = verifyWinner(buttonTexts);
    if(result == "Tie"){
      turnoCompleto = result;
    }
    else if(result == "O" || result == "X"){
      turnoCompleto = result + " is the winner!";
    }

    // Window end
    ImGui::End();
  }
}