#include "drone.hpp"

#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

void Drone::initializeGL(GLuint program) {
	terminateGL();

	m_program = program;
	m_colorLoc = glGetUniformLocation(m_program, "color");
	m_rotationLoc = glGetUniformLocation(m_program, "rotation");
	m_scaleLoc = glGetUniformLocation(m_program, "scale");
	m_translationLoc = glGetUniformLocation(m_program, "translation");

	m_rotation = 0.0f;
	// Create coordinates of drone
	std::array<glm::vec2, 116> positions{
		//1st guard
		glm::vec2{+00.0f, +08.0f}, glm::vec2{+00.0f, +09.0f},
		glm::vec2{+01.0f, +11.0f}, glm::vec2{+03.0f, +13.0f},
		glm::vec2{+05.0f, +14.0f}, glm::vec2{+09.0f, +14.0f},
		glm::vec2{+11.0f, +13.0f}, glm::vec2{+13.0f, +11.0f},
		glm::vec2{+14.0f, +09.0f}, glm::vec2{+14.0f, +05.0f},
		glm::vec2{+13.0f, +03.0f}, glm::vec2{+11.0f, +01.0f},
		glm::vec2{+09.0f, +00.0f}, glm::vec2{+08.0f, +00.0f},
		glm::vec2{+05.0f, +13.0f}, glm::vec2{+09.0f, +13.0f},
		glm::vec2{+13.0f, +09.0f}, glm::vec2{+13.0f, +05.0f},
		//2nd guard
		glm::vec2{-00.0f, +08.0f}, glm::vec2{-00.0f, +09.0f},
		glm::vec2{-01.0f, +11.0f}, glm::vec2{-03.0f, +13.0f},
		glm::vec2{-05.0f, +14.0f}, glm::vec2{-09.0f, +14.0f},
		glm::vec2{-11.0f, +13.0f}, glm::vec2{-13.0f, +11.0f},
		glm::vec2{-14.0f, +09.0f}, glm::vec2{-14.0f, +05.0f},
		glm::vec2{-13.0f, +03.0f}, glm::vec2{-11.0f, +01.0f},
		glm::vec2{-09.0f, +00.0f}, glm::vec2{-08.0f, +00.0f},
		glm::vec2{-05.0f, +13.0f}, glm::vec2{-09.0f, +13.0f},
		glm::vec2{-13.0f, +09.0f}, glm::vec2{-13.0f, +05.0f},
		//3rd guard
		glm::vec2{+00.0f, -08.0f}, glm::vec2{+00.0f, -09.0f},
		glm::vec2{+01.0f, -11.0f}, glm::vec2{+03.0f, -13.0f},
		glm::vec2{+05.0f, -14.0f}, glm::vec2{+09.0f, -14.0f},
		glm::vec2{+11.0f, -13.0f}, glm::vec2{+13.0f, -11.0f},
		glm::vec2{+14.0f, -09.0f}, glm::vec2{+14.0f, -05.0f},
		glm::vec2{+13.0f, -03.0f}, glm::vec2{+11.0f, -01.0f},
		glm::vec2{+09.0f, -00.0f}, glm::vec2{+08.0f, -00.0f},
		glm::vec2{+05.0f, -13.0f}, glm::vec2{+09.0f, -13.0f},
		glm::vec2{+13.0f, -09.0f}, glm::vec2{+13.0f, -05.0f},
		//4th guard
		glm::vec2{-00.0f, -08.0f}, glm::vec2{-00.0f, -09.0f},
		glm::vec2{-01.0f, -11.0f}, glm::vec2{-03.0f, -13.0f},
		glm::vec2{-05.0f, -14.0f}, glm::vec2{-09.0f, -14.0f},
		glm::vec2{-11.0f, -13.0f}, glm::vec2{-13.0f, -11.0f},
		glm::vec2{-14.0f, -09.0f}, glm::vec2{-14.0f, -05.0f},
		glm::vec2{-13.0f, -03.0f}, glm::vec2{-11.0f, -01.0f},
		glm::vec2{-09.0f, -00.0f}, glm::vec2{-08.0f, -00.0f},
		glm::vec2{-05.0f, -13.0f}, glm::vec2{-09.0f, -13.0f},
		glm::vec2{-13.0f, -09.0f}, glm::vec2{-13.0f, -05.0f},
		//Main body
		glm::vec2{-02.0f, +07.0f}, glm::vec2{-04.0f, +05.0f},
		glm::vec2{-05.0f, +03.0f}, glm::vec2{-08.0f, -00.0f},
		glm::vec2{-03.0f, -02.0f}, glm::vec2{-00.0f, -08.0f},
		glm::vec2{+03.0f, -02.0f}, glm::vec2{+08.0f, -00.0f},
		glm::vec2{+05.0f, +03.0f}, glm::vec2{+04.0f, +05.0f},
		glm::vec2{+02.0f, +07.0f}, glm::vec2{+00.0f, +05.0f},
		//Propellers
		//1  
		glm::vec2{+06.0f, +06.0f}, glm::vec2{+08.0f, +06.0f},
		glm::vec2{+06.0f, +08.0f}, glm::vec2{+08.0f, +08.0f},
		glm::vec2{+07.0f, +12.0f}, glm::vec2{+12.0f, +07.0f},
		glm::vec2{+07.0f, +02.0f}, glm::vec2{+02.0f, +07.0f},
		//2 
		glm::vec2{-06.0f, +06.0f}, glm::vec2{-08.0f, +06.0f},
		glm::vec2{-06.0f, +08.0f}, glm::vec2{-08.0f, +08.0f},
		glm::vec2{-07.0f, +12.0f}, glm::vec2{-12.0f, +07.0f},
		glm::vec2{-07.0f, +02.0f}, glm::vec2{-02.0f, +07.0f},
		//3  
		glm::vec2{+06.0f, -06.0f}, glm::vec2{+08.0f, -06.0f},
		glm::vec2{+06.0f, -08.0f}, glm::vec2{+08.0f, -08.0f},
		glm::vec2{+07.0f, -12.0f}, glm::vec2{+12.0f, -07.0f},
		glm::vec2{+07.0f, -02.0f}, glm::vec2{+02.0f, -07.0f},
		//4  
		glm::vec2{-06.0f, -06.0f}, glm::vec2{-08.0f, -06.0f},
		glm::vec2{-06.0f, -08.0f}, glm::vec2{-08.0f, -08.0f},
		glm::vec2{-07.0f, -12.0f}, glm::vec2{-12.0f, -07.0f},
		glm::vec2{-07.0f, -02.0f}, glm::vec2{-02.0f, -07.0f},
		};

	// Normalize
	for (auto &position :positions) {
		position /= glm::vec2{14.0f, 14.0f};
	}
	// Create triangles of drone
	std::array<int, 276> indices;
	indices = {
						//1st guard
						0, 1, 2,
						0, 2, 14,
						2, 3, 14,
						3, 4, 15,
						4, 5, 15,
						5, 6, 15,
						6, 15, 16,
						6, 7, 16,
						7, 8, 17,
						8, 9, 17,
						9, 10, 17,
						10, 11, 17,
						11, 12, 13,
						11, 13, 17,
						//2nd guard
						18, 19, 20,
						18, 20, 32,
						20, 21, 32,
						21, 22, 33,
						22, 23, 33,
						23, 24, 33,
						24, 33, 34,
						24, 25, 34,
						25, 26, 35,
						26, 27, 35,
						27, 28, 35,
						28, 29, 35,
						29, 30, 31,
						29, 31, 35,
						//3rd guard
						36, 37, 38,
						36, 38, 50,
						38, 39, 50,
						39, 40, 51,
						40, 41, 51,
						41, 42, 51,
						42, 51, 52,
						42, 43, 52,
						43, 44, 53,
						44, 45, 53,
						45, 46, 53,
						46, 47, 53,
						47, 48, 49,
						47, 49, 53,
						//4th guard
						54, 55, 56,
						54, 56, 68,
						56, 57, 68,
						57, 58, 69,
						58, 59, 69,
						59, 60, 69,
						60, 69, 70,
						60, 61, 70,
						61, 62, 71,
						62, 63, 71,
						63, 64, 71,
						64, 65, 71,
						65, 66, 67,
						65, 67, 71,
						//Main body
						0, 72, 82,
						72, 82, 83,
						72, 73, 83,
						81, 82, 83,
						73, 74, 83,
						80, 81, 83,
						74, 80, 83,
						74, 75, 80,
						75, 79, 80,
						75, 76, 79,
						76, 77, 78,
						76, 78, 79,
						//Propellers
						//1
						84, 85, 87,
						84, 86, 87,
						86, 87, 88,
						85, 87, 89,
						84, 85, 90,
						84, 86, 91,
						//2
						92, 93, 95,
						92, 94, 95,
						94, 95, 96,
						93, 95, 97,
						92, 93, 98,
						92, 94, 99,
						//3
						100, 101, 103,
						100, 102, 103,
						102, 103, 104,
						101, 103, 105,
						100, 101, 106,
						100, 102, 107,
						//4
						108, 109, 111,
						108, 110, 111,
						110, 111, 112,
						109, 111, 113,
						108, 109, 114,
						108, 110, 115
						};

	// Generate VBO
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
				GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Generate EBO
	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
				GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Get location of attributes in the program
	GLint positionAttribute{glGetAttribLocation(m_program, "inPosition")};

	// Create VAO
	glGenVertexArrays(1, &m_vao);

	// Bind vertex attributes to current VAO
	glBindVertexArray(m_vao);

	glEnableVertexAttribArray(positionAttribute);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

	// End of binding to current VAO
	glBindVertexArray(0);
}

void Drone::paintGL(const GameData &gameData) {
    if (gameData.m_state != State::Playing) return;
	
    glUseProgram(m_program);

    glBindVertexArray(m_vao);

    glUniform1f(m_scaleLoc, m_scale);
    glUniform1f(m_rotationLoc, m_rotation);

    // Restart propellers blink timer every 100 ms
    if (m_trailBlinkTimer.elapsed() > 100.0 / 1000.0) m_trailBlinkTimer.restart();
	
    // Show propellers during 50 ms
    glUniform4fv(m_colorLoc, 1, &m_color.r);
    if (m_trailBlinkTimer.elapsed() < 50.0 / 1000.0) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Draw all drone with propellers 50% transparent
        glUniform4f(m_colorLoc, 1, 1, 1, 0.5f);
        glUniform2f(m_translationLoc, m_translation.x, m_translation.y);
        glDrawElements(GL_TRIANGLES, 92 * 3, GL_UNSIGNED_INT, nullptr);

        glDisable(GL_BLEND);
    }
	// Draw drone without propellers
    glUniform2f(m_translationLoc, m_translation.x, m_translation.y);
    glDrawElements(GL_TRIANGLES, 68 * 3, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);

    glUseProgram(0);
}

void Drone::terminateGL() {
  glDeleteBuffers(1, &m_vbo);
  glDeleteBuffers(1, &m_ebo);
  glDeleteVertexArrays(1, &m_vao);
}

void Drone::update(float deltaTime) {
	//Get direction
	m_velocity = m_pos - m_translation;
	//Then changes the position
	m_translation += m_velocity * deltaTime;
}