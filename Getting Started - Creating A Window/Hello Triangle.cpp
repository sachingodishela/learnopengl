#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const GLchar* vertexShaderSource = ""
"#version 430 core\n"
"layout(location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x-0.25f, aPos.y, aPos.z, 1.0f);\n"
"}\0";

const GLchar* fragmentShaderSource = ""
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

	/** NEW SINCE LAST LESSON
		Read vertex data from CPU to GPU
		What are openGL objects, types of openGL objects
		Generating objects and object names
		Binding those objects to targets
		copying data to those target objects
		create a vertex shader object
		attach source to the vertex shader object
		compile the shader
	*/
	float vertices[] = {-0.5f, -0.4f, 0,   0.5f, -0.4f, 0,      0, 0.6f, 0,
						0,      0.6f, 0,   0.5f, -0.4f, 0,   1.0f, 0.6f, 0};

	// VAO
	GLuint array;
	glGenVertexArrays(1, &array);
	glBindVertexArray(array);

	// VBO
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, (void*)0);
	// attribute index as defined in vertex shader, i.e. layout (location = 0)
	// no. of components in current attribute
	// type of data
	// do we require normalization?
	// stride between consecutive same attributes
	// offset of current attribute from the beginning of data
	glEnableVertexAttribArray(0);
	glUseProgram(shaderProgram);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}	