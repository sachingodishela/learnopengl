#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../Shader/Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image/stb_image.h"

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	std::cout << "key=" << key << ", action=" << action << std::endl;
	if (action == GLFW_RELEASE and key == GLFW_KEY_W) {
		GLint mode[2], newMode;
		glGetIntegerv(GL_POLYGON_MODE, mode);
		if (mode[0] == GL_LINE) {
			newMode = GL_FILL;
		}
		else {
			newMode = GL_LINE;
		}
		glPolygonMode(GL_FRONT_AND_BACK, newMode);
	}
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Window", nullptr, nullptr);

	if (!window) {
		std::cout << "GLFW window creation failed..." << std::endl;
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "OpenGL Library Loading Failed..." << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	float vertices[] = {
		-0.5f, 0,    0, 1.0f, 0,       0, 0,    0,
		0.5f,  0,    0, 1.0f, 1.0f,    0, 1.0f, 0,
		-0.5f, 1.0f, 0, 0,    1.0f, 1.0f, 0,    1.0f,
		0.5f,  1.0f, 0, 0,    0,    1.0f, 1.0f, 1.0f
	};

	// First create 2 VAOs, which contain everything vertex related. (VBO, EBO, data etc.)
	GLuint* VAO = new GLuint[1];
	glGenVertexArrays(1, VAO);

	// VBO: To bring data from CPU to GPU, create 2 VBOs
	GLuint* VBO = new GLuint[1];
	glGenBuffers(1, VBO);

	// Configure 1st VAO and VBO
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices) / 4, (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertices) / 4, (void*)12);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertices) / 4, (void*)24);

	// EBO
	GLuint indices[] = { 0, 1, 2, 1, 2, 3 };
	GLuint elementBuffer;
	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Textures
	GLuint* tex = new GLuint[2];
	glGenTextures(2, tex);
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	int width, height, nrChannels;
	//stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("wall.jpg", &width, &height, &nrChannels, 0);
	if (!data) {
		std::cout << "failed to read image" << std::endl;
		return -1;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	// Texture 2
	glBindTexture(GL_TEXTURE_2D, tex[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	unsigned char* data2 = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (!data) {
		std::cout << "failed to read image" << std::endl;
		return -1;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data2);

	// Shaders 
	Shader shader("vertexShader.glsl", "fragmentShader.glsl");

	// Pre Draw Commands
	glBindVertexArray(VAO[0]);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glfwSetKeyCallback(window, key_callback);
	shader.use();
	for (int i = 0; i < 3; i++) glEnableVertexAttribArray(i);
	shader.setFloat("yOffset", -0.5);
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex[1]);


	// The render loop
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// de-allocate resources
	glDeleteVertexArrays(1, VAO);
	glDeleteBuffers(1, VBO);
	glDeleteProgram(shader.id);
	glfwTerminate();

	return 0;
}
