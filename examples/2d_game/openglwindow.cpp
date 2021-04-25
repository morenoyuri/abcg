#include "openglwindow.hpp"

#include <imgui.h>

#include "abcg.hpp"

#include <iostream>

#include <fmt/core.h>

float x, y;
float debug;
void OpenGLWindow::handleEvent(SDL_Event &event) {
    // Mouse events
    //Get mouse position
    glm::ivec2 mousePosition;
    SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
    float width = m_viewportWidth/2;
    float height = m_viewportHeight/2;
    //Set x and y between -1 and 1
    x = ((float)mousePosition.x - width)/width;
    y = -((float)mousePosition.y - height)/height;
    //Set direction of drone using the actual position
    float direcx = (mousePosition.x - m_viewportWidth / 2) - (m_drone.m_translation.x * (m_viewportWidth/2));
    float direcy = (mousePosition.y - m_viewportHeight / 2) + (m_drone.m_translation.y * (m_viewportHeight/2));
    glm::vec2 direction{glm::vec2{ direcx, direcy}};
    direction.y = -direction.y;
    //When left button is pressed it set rotation to the final position
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT){
            m_drone.setRotation(std::atan2(direction.y, direction.x) - M_PI_2);
        }
    }
    //When left button is released, it set the final position
    if (event.type == SDL_MOUSEBUTTONUP) {
        if (event.button.button == SDL_BUTTON_LEFT){
            m_drone.setPos({x, y});
        }
    }
    //If the position of the drone is next to the final position, it starts to rotate
    if (event.type == SDL_MOUSEMOTION) {
        if((m_drone.m_pos.x - m_drone.m_translation.x) < 0.08 && (m_drone.m_pos.y - m_drone.m_translation.y) < 0.08){
           m_drone.setRotation(std::atan2(direction.y, direction.x) - M_PI_2);
        }
    }
}

void OpenGLWindow::initializeGL() {
  // Load a new font
	ImGuiIO &io{ImGui::GetIO()};
	auto filename{getAssetsPath() + "EvilEmpire-4BBVK.ttf"};
	m_font = io.Fonts->AddFontFromFileTTF(filename.c_str(), 60.0f);
	if (m_font == nullptr) {
		throw abcg::Exception{abcg::Exception::Runtime("Cannot load font file")};
	}

	// Create program to render the other objects
	m_objectsProgram = createProgramFromFile(getAssetsPath() + "objects.vert",
											getAssetsPath() + "objects.frag");
	glClearColor(0.54, 0.27, 0.07, 1);

	#if !defined(__EMSCRIPTEN__)
	glEnable(GL_PROGRAM_POINT_SIZE);
	#endif

	// Start pseudo-random number generator
	auto seed{std::chrono::steady_clock::now().time_since_epoch().count()};
	m_randomEngine.seed(seed);

	restart();
}

void OpenGLWindow::restart() {
	m_gameData.m_state = State::Playing;
	m_drone.initializeGL(m_objectsProgram);
	m_bats.initializeGL(m_objectsProgram, 5);
}

void OpenGLWindow::update() {
	float deltaTime{static_cast<float>(getDeltaTime())};

	// Wait 5 seconds before restarting
	if (m_gameData.m_state != State::Playing &&
		m_restartWaitTimer.elapsed() > 5) {
		restart();
		return;
	}
	m_drone.update(deltaTime);
	m_bats.update(deltaTime);
	if (m_gameData.m_state == State::Playing) {
		checkCollisions();
		checkWinCondition();
	}
}

void OpenGLWindow::checkCollisions() {
  	// Check collision between drone and bats
 	for (auto &bat : m_bats.m_bats) {
		auto batTranslation{bat.m_translation};
		auto distance{glm::distance(m_drone.m_translation, batTranslation)};

		if (distance < m_drone.m_scale * 0.9f + bat.m_scale * 0.85f) {
			m_gameData.m_state = State::GameOver;
			m_restartWaitTimer.restart();
		}
	}
}

void OpenGLWindow::checkWinCondition() {
	if (m_restartWaitTimer.elapsed() > 30) {
		m_gameData.m_state = State::Win;
		m_restartWaitTimer.restart();
	}
}

void OpenGLWindow::paintGL() {
	update();

	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, m_viewportWidth, m_viewportHeight);

	m_bats.paintGL();
	m_drone.paintGL(m_gameData);
}

void OpenGLWindow::paintUI() {
	abcg::OpenGLWindow::paintUI();
	{
		auto size{ImVec2(300, 85)};
		auto position{ImVec2((m_viewportWidth - size.x) / 2.0f,
							(m_viewportHeight - size.y) / 2.0f)};
		ImGui::SetNextWindowPos(position);
		ImGui::SetNextWindowSize(size);
		ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
							ImGuiWindowFlags_NoTitleBar |
							ImGuiWindowFlags_NoInputs};
		ImGui::Begin(" ", nullptr, flags);
		ImGui::PushFont(m_font);

		if (m_gameData.m_state == State::GameOver) {
		ImGui::Text("Game Over!");
		} else if (m_gameData.m_state == State::Win) {
		ImGui::Text("*You Win!*");
		}

		ImGui::PopFont();
		ImGui::End();
	}
}

void OpenGLWindow::resizeGL(int width, int height) {
	m_viewportWidth = width;
	m_viewportHeight = height;

	glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::terminateGL() {
	glDeleteProgram(m_objectsProgram);

	m_drone.terminateGL();
	m_bats.terminateGL();
}
