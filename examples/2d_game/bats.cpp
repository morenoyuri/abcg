#include "bats.hpp"

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

void Bats::initializeGL(GLuint program, int quantity) {
    terminateGL();

    // Start pseudo-random number generator
    auto seed{std::chrono::steady_clock::now().time_since_epoch().count()};
    m_randomEngine.seed(seed);

    //Take vars from program
    m_program = program;
    m_colorLoc = glGetUniformLocation(m_program, "color");
    m_rotationLoc = glGetUniformLocation(m_program, "rotation");
    m_scaleLoc = glGetUniformLocation(m_program, "scale");
    m_translationLoc = glGetUniformLocation(m_program, "translation");

    // Create bats
    m_bats.clear();
    m_bats.resize(quantity);
    for (auto &bat : m_bats) {
        bat = createBat();

        // Make sure the bat won't collide with the drone
        bat.m_translation = {m_randomDist(m_randomEngine), -1.2};
    }    
}

void Bats::paintGL() {
    glUseProgram(m_program);

    for (auto &bat : m_bats) {
        //Set scale, color and rotation for every bat
        glBindVertexArray(bat.m_vao);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUniform4fv(m_colorLoc, 1, &bat.m_color.r);
        glUniform1f(m_scaleLoc, bat.m_scale);
        glUniform1f(m_rotationLoc, bat.m_rotation);
        for (auto j : {-2, 0, 2}) {
            //Set position of a bat, y is locked and x change for j
            glUniform2f(m_translationLoc, bat.m_translation.x + j, bat.m_translation.y);
            glDrawElements(GL_TRIANGLES, 36 * 3, GL_UNSIGNED_INT, nullptr);
            glDisable(GL_BLEND);
        }
        glBindVertexArray(0);
    }
    glUseProgram(0);
}

void Bats::terminateGL() {
    for (auto bat : m_bats) {
        glDeleteBuffers(1, &bat.m_vbo);
        glDeleteBuffers(1, &bat.m_ebo);
        glDeleteVertexArrays(1, &bat.m_vao);
    }
}

void Bats::update(float deltaTime) {
    for (auto &bat : m_bats) {
        //Moves the bat using random velocity in x and a random positive velocity in y
        bat.m_translation.y += abs(bat.m_velocity.y) * deltaTime;
        bat.m_translation.x += bat.m_velocity.x * deltaTime;
        //If bat is out of range of the screen, delete it and create another
        if(bat.m_translation.y > 1.2){
            glDeleteBuffers(1, &bat.m_vbo);
            glDeleteBuffers(1, &bat.m_ebo);
            glDeleteVertexArrays(1, &bat.m_vao);
            bat = createBat();
            bat.m_translation = {m_randomDist(m_randomEngine), -1.2};
        }
    }
}

Bats::Bat Bats::createBat(glm::vec2 translation, float scale) {
    Bat bat;
    auto &re{m_randomEngine};  // Shortcut
    //Create random scale of gray
    std::uniform_real_distribution<float> randomIntensity(0.5f, 1.0f);
    bat.m_color = glm::vec4(1) * randomIntensity(re);
    bat.m_color.a = 1.0f;
    bat.m_rotation = 0.0f;
    bat.m_scale = scale;
    bat.m_translation = translation;
    // Choose a random direction
    glm::vec2 direction{m_randomDist(re), m_randomDist(re)};
    bat.m_velocity = glm::normalize(direction) / 7.0f;
    bat.m_velocity.y *= 7.0f;
    //Create the coordinates of bat
    std::array<glm::vec2, 50> positions{
		//right half
		glm::vec2{+02.0f, +00.0f}, glm::vec2{+01.0f, +02.0f},
		glm::vec2{+02.0f, +02.0f}, glm::vec2{+00.0f, +02.0f},
		glm::vec2{+00.0f, +04.0f}, glm::vec2{+02.0f, +04.0f},
		glm::vec2{+00.0f, +05.0f}, glm::vec2{+05.0f, +05.0f},
		glm::vec2{+04.0f, +04.0f}, glm::vec2{+05.0f, +02.0f},
		glm::vec2{+07.0f, +03.0f}, glm::vec2{+07.0f, +05.0f},
		glm::vec2{+08.0f, +04.0f}, glm::vec2{+08.0f, +06.0f},
		glm::vec2{+09.0f, +05.0f}, glm::vec2{+09.0f, +07.0f},
		glm::vec2{+11.0f, +06.0f}, glm::vec2{+09.0f, +08.0f},
        glm::vec2{+02.0f, +07.0f}, glm::vec2{+07.0f, +07.0f},
        glm::vec2{+00.0f, +07.0f}, glm::vec2{+00.0f, +09.0f},
        glm::vec2{+01.0f, +09.0f}, glm::vec2{+02.0f, +10.0f},
        glm::vec2{+02.0f, +09.0f},
        //left half
		glm::vec2{-02.0f, +00.0f}, glm::vec2{-01.0f, +02.0f},
		glm::vec2{-02.0f, +02.0f}, glm::vec2{-00.0f, +02.0f},
		glm::vec2{-00.0f, +04.0f}, glm::vec2{-02.0f, +04.0f},
		glm::vec2{-00.0f, +05.0f}, glm::vec2{-05.0f, +05.0f},
		glm::vec2{-04.0f, +04.0f}, glm::vec2{-05.0f, +02.0f},
		glm::vec2{-07.0f, +03.0f}, glm::vec2{-07.0f, +05.0f},
		glm::vec2{-08.0f, +04.0f}, glm::vec2{-08.0f, +06.0f},
		glm::vec2{-09.0f, +05.0f}, glm::vec2{-09.0f, +07.0f},
		glm::vec2{-11.0f, +06.0f}, glm::vec2{-09.0f, +08.0f},
        glm::vec2{-02.0f, +07.0f}, glm::vec2{-07.0f, +07.0f},
        glm::vec2{-00.0f, +07.0f}, glm::vec2{-00.0f, +09.0f},
        glm::vec2{-01.0f, +09.0f}, glm::vec2{-02.0f, +10.0f},
        glm::vec2{-02.0f, +09.0f},
		};
	// Normalize
	for (auto &position :positions) {
		position /= glm::vec2{11.0f, 10.0f};
	}
    //Create triangles of bat
	std::array indices{
						//right half
						0, 1, 2,
						2, 3, 4,
						2, 4, 5,
						4, 5, 6,
						5, 6, 7,
						5, 7, 8,
						7, 8, 9,
						7, 10, 11,
						11, 12, 13,
						13, 14, 15,
						15, 16, 17,
						15, 17, 18,
						11, 15, 19,
						11, 19, 20,
                        6, 11, 20,
                        18, 20, 21,
                        18, 21, 24,
                        22, 23, 24,
                        //left half
						25, 21, 27,
						27, 28, 29,
						27, 29, 30,
						29, 30, 31,
						30, 31, 32,
						30, 32, 33,
						32, 33, 34,
						32, 35, 36,
						36, 37, 38,
						38, 39, 40,
						40, 41, 42,
						40, 42, 43,
						36, 40, 44,
						36, 44, 45,
                        31, 36, 45,
                        43, 45, 46,
                        43, 46, 49,
                        47, 48, 49
						};

    // Generate VBO
    glGenBuffers(1, &bat.m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, bat.m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Generate EBO
	glGenBuffers(1, &bat.m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bat.m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Get location of attributes in the program
    GLint positionAttribute{glGetAttribLocation(m_program, "inPosition")};

    // Create VAO
    glGenVertexArrays(1, &bat.m_vao);

    // Bind vertex attributes to current VAO
    glBindVertexArray(bat.m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, bat.m_vbo);
    glEnableVertexAttribArray(positionAttribute);
    glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bat.m_ebo);

    // End of binding to current VAO
    glBindVertexArray(0);

    return bat;
}