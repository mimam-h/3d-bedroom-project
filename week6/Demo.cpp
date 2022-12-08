#include "Demo.h"



Demo::Demo() {

}

Demo::~Demo() {
}

void Demo::Init() {
	// build and compile our shader program
	// ------------------------------------
	shaderProgram = BuildShader("vertexShader.vert", "fragmentShader.frag", nullptr);

	BuildColoredCube();

	BuildColoredPlane();

	InitCamera();
}

void Demo::DeInit() {
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO2);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Demo::ProcessInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	// zoom camera
	// -----------
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		if (fovy < 90) {
			fovy += 0.0001f;
		}
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (fovy > 0) {
			fovy -= 0.0001f;
		}
	}

	// update camera movement 
	// -------------
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		MoveCamera(CAMERA_SPEED);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		MoveCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		StrafeCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		StrafeCamera(CAMERA_SPEED);
	}

	// update camera movement 
    // -------------
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		if (posCamY < 90) {
			posCamY += 0.001f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		if (posCamY > 0) {
			posCamY -= 0.001f;
		}
	}

	// update camera rotation
	// ----------------------
	double mouseX, mouseY;
	double midX = screenWidth / 2;
	double midY = screenHeight / 2;
	float angleY = 0.0f;
	float angleZ = 0.0f;

	// Get mouse position
	glfwGetCursorPos(window, &mouseX, &mouseY);
	if ((mouseX == midX) && (mouseY == midY)) {
		return;
	}

	// Set mouse position
	glfwSetCursorPos(window, midX, midY);

	// Get the direction from the mouse cursor, set a resonable maneuvering speed
	angleY = (float)((midX - mouseX)) / 1000;
	angleZ = (float)((midY - mouseY)) / 1000;

	// The higher the value is the faster the camera looks around.
	viewCamY += angleZ * 2;

	// limit the rotation around the x-axis
	if ((viewCamY - posCamY) > 8) {
		viewCamY = posCamY + 8;
	}
	if ((viewCamY - posCamY) < -8) {
		viewCamY = posCamY - 8;
	}
	RotateCamera(-angleY);

}

void Demo::Update(double deltaTime) {
}

void Demo::Render() {
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);

	// Pass perspective projection matrix
	glm::mat4 projection = glm::perspective(fovy, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(this->shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// LookAt camera (position, target/direction, up)
	glm::mat4 view = glm::lookAt(glm::vec3(posCamX, posCamY, posCamZ), glm::vec3(viewCamX, viewCamY, viewCamZ), glm::vec3(upCamX, upCamY, upCamZ));
	GLint viewLoc = glGetUniformLocation(this->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	// set lighting attribute
	GLint lightPosLoc = glGetUniformLocation(this->shaderProgram, "lightPos");
	glUniform3f(lightPosLoc, 0, 1, 0);
	GLint viewPosLoc = glGetUniformLocation(this->shaderProgram, "viewPos");
	glUniform3f(viewPosLoc, 0, 2, 3);
	GLint lightColorLoc = glGetUniformLocation(this->shaderProgram, "lightColor");
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
	
	float room_y = 12.0f;
	float room_x = 22.0f;
	float room_z = 12.0f;
	float room_thickness = 0.5f;

	float room_pos_x = -1.5f;
	float room_pos_y = 0;
	float room_pos_z = -1.3f;
	
	/*Dinding*/
	DrawColoredCube(glm::vec3(-1.5, 0, -1.3), glm::vec3(0, 1, 0), glm::vec3(room_x, room_y, room_thickness), 0, false, (float)255 / 255, (float)255 / 255, (float)255 / 255);
	DrawColoredCube(glm::vec3(-1.5 - (room_x / 2) - (room_thickness / 2), 0, -1.3 + (room_z / 2) - (room_thickness / 2)), glm::vec3(0, 1, 0), glm::vec3(room_thickness, room_y, room_z), 0, false, (float)255 / 255, (float)255 / 255, (float)255 / 255);
	DrawColoredCube(glm::vec3(-1.5 + (room_x / 2) + (room_thickness / 2), 0, -1.3 + (room_z / 2) - (room_thickness / 2)), glm::vec3(0, 1, 0), glm::vec3(room_thickness, room_y, room_z), 0, false, (float)255 / 255, (float)255 / 255, (float)255 / 255);
	DrawColoredCube(glm::vec3(-1.5, 0, -1.3 + (room_z)-(room_thickness)), glm::vec3(0, 1, 0), glm::vec3(room_x, room_y, room_thickness), 0, false, (float)255 / 255, (float)255 / 255, (float)255 / 255);
	
	/*Object 1: Meja*/
	// Plate Meja
	DrawColoredCube(glm::vec3(0, 0.65, 0), glm::vec3(0, 1, 0), glm::vec3(3.5, 0.1, 2), 0, false, (float) 255/255, (float) 255/255, (float) 255/255);
	DrawColoredCube(glm::vec3(0, 0.5, 0), glm::vec3(0, 1, 0), glm::vec3(3.1, 0.2, 1.6), 0, false, (float)92 / 255, (float)50 / 255, (float)23 / 255);
	// Kaki Meja
	DrawColoredCube(glm::vec3(-1.5, 0, 0.75), glm::vec3(0, 0, 0), glm::vec3(0.1, 1, 0.1), 0, false, (float)92 / 255, (float)50 / 255, (float)23 / 255);
	DrawColoredCube(glm::vec3(1.5, 0, -0.75), glm::vec3(0, 0, 0), glm::vec3(0.1, 1, 0.1), 0, false, (float)92 / 255, (float)50 / 255, (float)23 / 255);
	DrawColoredCube(glm::vec3(-1.5, 0, -0.75), glm::vec3(0, 0, 0), glm::vec3(0.1, 1, 0.1), 0, false, (float)92 / 255, (float)50 / 255, (float)23 / 255);
	DrawColoredCube(glm::vec3(1.5, 0, 0.75), glm::vec3(0, 0, 0), glm::vec3(0.1, 1, 0.1), 0, false, (float)92 / 255, (float)50 / 255, (float)23 / 255);
	

	/*Object 2: Ranjang*/
	// Rangka Ranjang
	DrawColoredCube(glm::vec3(-9.5, 0.25,-0.92), glm::vec3(0, 0, 0), glm::vec3(4, 1, 0.25), 0, false, (float)191 / 255, (float)135 / 255, (float)55 / 255);
	DrawColoredCube(glm::vec3(-9.5, -0.35, 1.955), glm::vec3(0, 0, 0), glm::vec3(4, 0.25, 6), 0, false, (float)191 / 255, (float)135 / 255, (float)55 / 255);
	// Kasur
	DrawColoredCube(glm::vec3(-9.5, 0.1, 2), glm::vec3(0, 0, 0), glm::vec3(3.75, 0.6, 5.5), 0, false, (float)255 / 255, (float)255 / 255, (float)255 / 255);
	// Bantal
	DrawColoredCube(glm::vec3(-10.5, 0.55, -0.25), glm::vec3(0, 0, 0), glm::vec3(1.75, 0.3, 1), 0, false, (float)146 / 255, (float)150 / 255, (float)147 / 255);
	DrawColoredCube(glm::vec3(-8.5, 0.55, -0.25), glm::vec3(0, 0, 0), glm::vec3(1.75, 0.3, 1), 0, false, (float)146 / 255, (float)150 / 255, (float)147 / 255);
	// Selimut
	DrawColoredCube(glm::vec3(-11.40, 0.1, 3), glm::vec3(0, 0, 0), glm::vec3(0.05, 0.6, 3.5), 0, false, (float)255 / 255, (float)7 / 255, (float)7 / 255);
	DrawColoredCube(glm::vec3(-7.6, 0.1, 3), glm::vec3(0, 0, 0), glm::vec3(0.05, 0.6, 3.5), 0, false, (float)255 / 255, (float)7 / 255, (float)7 / 255);
	DrawColoredCube(glm::vec3(-9.5, 0.425, 3), glm::vec3(0, 0, 0), glm::vec3(3.85, 0.05, 3.5), 0, false, (float)255 / 255, (float)7 / 255, (float)7 / 255);


	/*Object 3: Meja Kecil*/
	// Plate Meja
	DrawColoredCube(glm::vec3(-6.75, 0.38, -0.35), glm::vec3(0, 1, 0), glm::vec3(1.25, 0.2, 1.25), 0, false, (float)92 / 255, (float)50 / 255, (float)23 / 255);
	// Kaki Meja
	DrawColoredCube(glm::vec3(-7.25, -0.1, -0.85), glm::vec3(0, 0, 0), glm::vec3(0.1, 0.75, 0.1), 0, false, (float)92 / 255, (float)50 / 255, (float)23 / 255);
	DrawColoredCube(glm::vec3(-6.25, -0.1, -0.85), glm::vec3(0, 0, 0), glm::vec3(0.1, 0.75, 0.1), 0, false, (float)92 / 255, (float)50 / 255, (float)23 / 255);
	DrawColoredCube(glm::vec3(-7.25, -0.1, 0.15), glm::vec3(0, 0, 0), glm::vec3(0.1, 0.75, 0.1), 0, false, (float)92 / 255, (float)50 / 255, (float)23 / 255);
	DrawColoredCube(glm::vec3(-6.25, -0.1, 0.15), glm::vec3(0, 0, 0), glm::vec3(0.1, 0.75, 0.1), 0, false, (float)92 / 255, (float)50 / 255, (float)23 / 255);
	
	/*Object 4: Rak Buku*/
	// Plate Bawah
	DrawColoredCube(glm::vec3(-4, 2.5, -0.4), glm::vec3(0, 1, 0), glm::vec3(2.75, 0.2, 1.25), 0, false, (float)92 / 255, (float)50 / 255, (float)23 / 255);
	// Dock
	// Plate Atas
	DrawColoredCube(glm::vec3(-4, 3.7, -0.4), glm::vec3(0, 1, 0), glm::vec3(2.75, 0.2, 1.25), 0, false, (float)92 / 255, (float)50 / 255, (float)23 / 255);
	// Dinding
	DrawColoredCube(glm::vec3(-5.275, 3.1, -0.4), glm::vec3(0, 1, 0), glm::vec3(0.2, 1.4, 1.25), 0, false, (float)92 / 255, (float)50 / 255, (float)23 / 255);
	DrawColoredCube(glm::vec3(-2.675, 3.1, -0.4), glm::vec3(0, 1, 0), glm::vec3(0.2, 1.4, 1.25), 0, false, (float)92 / 255, (float)50 / 255, (float)23 / 255);

	/*Object 5: Lemari*/
		DrawColoredCube(glm::vec3(4, 1, 2), glm::vec3(0, 1, 0), glm::vec3(1, 6, 3.5), 0, false, (float)92 / 255, (float)50 / 255, (float)23 / 255);
	// atap
	DrawColoredCube(glm::vec3(4, 1 + 3 + 0.01, 2), glm::vec3(0, 1, 0), glm::vec3(1 + 0.3, 0.1, 3.5 + 0.3), 0, false, (float)92 / 255, (float)50 / 255, (float)23 / 255);
	// bawah
	DrawColoredCube(glm::vec3(4, 1 - 2 + 0.01, 2), glm::vec3(0, 1, 0), glm::vec3(1 + 0.3, 0.1, 3.5 + 0.3), 0, false, (float)92 / 255, (float)50 / 255, (float)23 / 255);
	// pintu
	DrawColoredCube(glm::vec3(4 - 0.5 - 0.05, 1, 2 + 1.75 - 0.85), glm::vec3(0, 1, 0), glm::vec3(0.1, 6, 1.7), 0, false, (float)92 / 255, (float)50 / 255, (float)23 / 255);
	DrawColoredCube(glm::vec3(4 - 0.5 - 0.05, 1, 2 - 1.75 + 0.85), glm::vec3(0, 1, 0), glm::vec3(0.1, 6, 1.7), 0, false, (float)92 / 255, (float)50 / 255, (float)23 / 255);
	// gagang
	DrawColoredCube(glm::vec3(4 - 0.5 - 0.15, 1 + 0.7, 2 + 0.3), glm::vec3(0, 1, 0), glm::vec3(0.1, 0.2, 0.1), 0, false, (float)72 / 255, (float)30 / 255, (float)3 / 255);
	DrawColoredCube(glm::vec3(4 - 0.5 - 0.15, 1 + 0.7, 2 - 0.3), glm::vec3(0, 1, 0), glm::vec3(0.1, 0.2, 0.1), 0, false, (float)72 / 255, (float)30 / 255, (float)3 / 255);

	/*Object 6: Kipas Angin*/
	float kipas_x = 0;
	float kipas_y = 4.5;
	float kipas_z = 2;
	// main
	DrawColoredCube(glm::vec3(kipas_x + 0, kipas_y + 0, kipas_z + 0), glm::vec3(0, 1, 0), glm::vec3(3, 0.05, 0.3), 0, true, (float)92 / 255, (float)50 / 255, (float)23 / 255);
	DrawColoredCube(glm::vec3(kipas_x + 0, kipas_y + 0, kipas_z + 0), glm::vec3(0, 1, 0), glm::vec3(0.3, 0.05, 3), 0, true, (float)92 / 255, (float)50 / 255, (float)23 / 255);
	// holder
	DrawColoredCube(glm::vec3(kipas_x + 0, kipas_y + 0.05, kipas_z + 0), glm::vec3(0, 1, 0), glm::vec3(0.4, 0.1, 0.4), 0, false, (float)92 / 255, (float)50 / 255, (float)23 / 255);
	DrawColoredCube(glm::vec3(kipas_x + 0, kipas_y - 0.05, kipas_z + 0), glm::vec3(0, 1, 0), glm::vec3(0.4, 0.1, 0.4), 0, false, (float)92 / 255, (float)50 / 255, (float)23 / 255);
	// pole
	DrawColoredCube(glm::vec3(kipas_x + 0, kipas_y + 0.75, kipas_z + 0), glm::vec3(0, 1, 0), glm::vec3(0.1, 1.5, 0.1), 0, false, (float)92 / 255, (float)50 / 255, (float)23 / 255);
	
	// DrawColoredCube();

	DrawColoredPlane();

	glDisable(GL_DEPTH_TEST);
}

void Demo::BuildColoredCube() {
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-0.5, -0.5, 0.5,  0.0f,  0.0f,  1.0f, // 0
		0.5, -0.5, 0.5,   0.0f,  0.0f,  1.0f, // 1
		0.5,  0.5, 0.5,   0.0f,  0.0f,  1.0f, // 2
		-0.5,  0.5, 0.5,  0.0f,  0.0f,  1.0f, // 3

		// right
		0.5,  0.5,  0.5, 1.0f,  0.0f,  0.0f, // 4
		0.5,  0.5, -0.5, 1.0f,  0.0f,  0.0f, // 5
		0.5, -0.5, -0.5, 1.0f,  0.0f,  0.0f, // 6
		0.5, -0.5,  0.5, 1.0f,  0.0f,  0.0f, // 7

		// back
		-0.5, -0.5, -0.5, 0.0f,  0.0f,  -1.0f, // 8 
		0.5,  -0.5, -0.5, 0.0f,  0.0f,  -1.0f, // 9
		0.5,   0.5, -0.5, 0.0f,  0.0f,  -1.0f, // 10
		-0.5,  0.5, -0.5, 0.0f,  0.0f,  -1.0f, // 11

		// left
		-0.5, -0.5, -0.5, -1.0f,  0.0f,  0.0f, // 12
		-0.5, -0.5,  0.5, -1.0f,  0.0f,  0.0f, // 13
		-0.5,  0.5,  0.5, -1.0f,  0.0f,  0.0f, // 14
		-0.5,  0.5, -0.5, -1.0f,  0.0f,  0.0f, // 15

		// upper
		0.5, 0.5,  0.5, 0.0f,  1.0f,  0.0f, // 16
		-0.5, 0.5, 0.5, 0.0f,  1.0f,  0.0f, // 17
		-0.5, 0.5, -0.5,0.0f,  1.0f,  0.0f, // 18
		0.5, 0.5, -0.5, 0.0f,  1.0f,  0.0f, // 19

		// bottom
		-0.5, -0.5, -0.5, 0.0f,  -1.0f,  0.0f, // 20
		0.5, -0.5, -0.5,  0.0f,  -1.0f,  0.0f, // 21
		0.5, -0.5,  0.5,  0.0f,  -1.0f,  0.0f, // 22
		-0.5, -0.5,  0.5, 0.0f,  -1.0f,  0.0f, // 23
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define normal pointer layout 2
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawColoredCube()
{
	UseShader(shaderProgram);

	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	GLint objectColorLoc = glGetUniformLocation(this->shaderProgram, "objectColor");
	glUniform3f(objectColorLoc, 1.0f, 0.0f, 1.0f);

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0, 0));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Demo::DrawColoredCube(glm::vec3 translate_vector, glm::vec3 rotate_vector, glm::vec3 scale_vector, float rot_angle, bool rotate, float color_r, float color_g, float color_b)
{
	UseShader(shaderProgram);

	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	GLint objectColorLoc = glGetUniformLocation(this->shaderProgram, "objectColor");
	glUniform3f(objectColorLoc, color_r, color_g, color_b);

	glm::mat4 model;
	model = glm::translate(model, translate_vector);

	if (rotate) {
		model = glm::rotate(model, rot_angle, rotate_vector);
	}

	model = glm::scale(model, scale_vector);

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Demo::BuildColoredPlane()
{
	// Load and create a texture 
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("floor-texture.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		-50.0, -0.5, -50.0,  0.0f,  1.0f,  0.0f,
		 50.0, -0.5, -50.0,  0.0f,  1.0f,  0.0f,
		 50.0, -0.5,  50.0,  0.0f,  1.0f,  0.0f,
		-50.0, -0.5,  50.0,  0.0f,  1.0f,  0.0f,
	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::DrawColoredPlane()
{
	UseShader(shaderProgram);

	// glActiveTexture(GL_TEXTURE1);
	// glBindTexture(GL_TEXTURE_2D, texture2);
	// glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	
	GLint objectColorLoc = glGetUniformLocation(this->shaderProgram, "objectColor");
	glUniform3f(objectColorLoc, (float) 3/255, (float) 186/255, (float) 252/255);

	glm::mat4 model;
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);

}

void Demo::InitCamera()
{
	posCamX = 0.0f;
	posCamY = 1.0f;
	posCamZ = 8.0f;
	viewCamX = 0.0f;
	viewCamY = 1.0f;
	viewCamZ = 0.0f;
	upCamX = 0.0f;
	upCamY = 1.0f;
	upCamZ = 0.0f;
	CAMERA_SPEED = 0.001f;
	fovy = 45.0f;
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Demo::MoveCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	// forward positive cameraspeed and backward negative -cameraspeed.
	posCamX = posCamX + x * speed;
	posCamZ = posCamZ + z * speed;
	viewCamX = viewCamX + x * speed;
	viewCamZ = viewCamZ + z * speed;
}

void Demo::StrafeCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	float orthoX = -z;
	float orthoZ = x;

	// left positive cameraspeed and right negative -cameraspeed.
	posCamX = posCamX + orthoX * speed;
	posCamZ = posCamZ + orthoZ * speed;
	viewCamX = viewCamX + orthoX * speed;
	viewCamZ = viewCamZ + orthoZ * speed;
}

void Demo::RotateCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	viewCamZ = (float)(posCamZ + glm::sin(speed) * x + glm::cos(speed) * z);
	viewCamX = (float)(posCamX + glm::cos(speed) * x - glm::sin(speed) * z);
}

int main(int argc, char** argv) {
	RenderEngine &app = Demo();
	app.Start("Basic Lighting: Phong Lighting Model", 800, 600, false, false);
}