#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const GLchar* vertexShaderSource =
"#version 430 core\n"
"layout(location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
"}\0";

const GLchar* fragmentShaderSource =
"#version 430 core\n"
"out vec4 FragColor;"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

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

	// vertex shader
	GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderObject, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShaderObject);
	std::cout << "Compiling Vertex Shader: ";
	char vertexShaderInfoLog[512];
	GLsizei vertexShaderInfoLogLength;
	glGetShaderInfoLog(vertexShaderObject, 512, &vertexShaderInfoLogLength, vertexShaderInfoLog);
	std::cout << (vertexShaderInfoLogLength ? vertexShaderInfoLog : "DONE!") << std::endl;

	// fragment shader
	GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderObject, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShaderObject);
	std::cout << "Compiling Fragment Shader: ";
	char fragmentShaderInfoLog[512];
	GLsizei fragmentShaderInfoLogLength;
	glGetShaderInfoLog(fragmentShaderObject, 512, &fragmentShaderInfoLogLength, fragmentShaderInfoLog);
	std::cout << (fragmentShaderInfoLogLength ? fragmentShaderInfoLog : "DONE!") << std::endl;
	
	// shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShaderObject);
	glAttachShader(shaderProgram, fragmentShaderObject);
	glLinkProgram(shaderProgram);

	float vertices[] = { -0.5f, 0, 0, 0.5f, 0, 0, 0, 0.6f, 0 };
	float vertices2[] = { -0.5f, 0, 0, 0.5f, 0, 0, 0, -0.6f, 0 };

	// First create 2 VAOs, which contain everything vertex related. (VBO, EBO, data etc.)
	GLuint* VAO = new GLuint[2];
	glGenVertexArrays(2, VAO);

	// VBO: To bring data from CPU to GPU, create 2 VBOs
	GLuint* VBO = new GLuint[2];
	glGenBuffers(2, VBO);

	// Configure 1st VAO and VBO
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, (void*)0);

	// Configure 2nd VAO and VBO
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, (void*)0);

	// Pre Draw Commands
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glUseProgram(shaderProgram);
	glfwSetKeyCallback(window, key_callback);

	// The render loop
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClear(GL_COLOR_BUFFER_BIT);
		for (int i = 0; i < 2; i++) {
			glBindVertexArray(VAO[i]);
			glEnableVertexAttribArray(0);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// de-allocate resources
	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);
	glDeleteProgram(shaderProgram);
	glfwTerminate();

	return 0;
}
