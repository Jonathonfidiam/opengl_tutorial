#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const char* vertexShaderSource = "#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragShaderSource = "#version 460 core\n"
"out vec4 FragColor;\n"
"void main(){\n"
"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

int main() {

	 glfwInit();
	 glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	 glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	 glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	 //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); only for OS-X

	 GLFWwindow* mainWindow = glfwCreateWindow(1920, 1080, "Main Window", NULL, NULL);
	 if (mainWindow == NULL) {
		  std::cout << "Failed to create main window";
		  glfwTerminate();
		  return -1;
	 }

	 glfwMakeContextCurrent(mainWindow);
	 glfwSetFramebufferSizeCallback(mainWindow, framebuffer_size_callback);

	 if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		  std::cout << "Failed to initialize GLAD" << std::endl;
		  return -1;
	 }

	 glViewport(0, 0, 1920, 1080);

	 // vertex data
	 float vertices[] = {
	 -0.5f, -0.5f, 0.0f,
	  0.5f, -0.5f, 0.0f,
	  0.0f,  0.5f, 0.0f
	 };

	 // vertices buffer allocation and setup
	 unsigned int VAO1;
	 glGenVertexArrays(1, &VAO1);
	 unsigned int VBO1;
	 glGenBuffers(1, &VBO1);
	 glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	 glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	 glEnableVertexAttribArray(0);


	 unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	 glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	 glCompileShader(vertexShader);

	 int success;
	 char errorLog[512];
	 glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	 if (success != 1) {
		  glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		  std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << errorLog << std::endl;
	 }

	 unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	 glShaderSource(fragShader, 1, &fragShaderSource, NULL);
	 glCompileShader(fragShader);

	 glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);

	 if (success != 1) {
		  glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		  std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << errorLog << std::endl;
	 }

	 unsigned int shaderProgram = glCreateProgram();
	 glAttachShader(shaderProgram, vertexShader);
	 glAttachShader(shaderProgram, fragShader);
	 glLinkProgram(shaderProgram);

	 glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &success);

	 if (success != 1) {
		  glGetShaderInfoLog(shaderProgram, 512, NULL, errorLog);
		  std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << errorLog << std::endl;
	 }


	
	 glDeleteShader(vertexShader);
	 glDeleteShader(fragShader);

	while (!glfwWindowShouldClose(mainWindow)) {
		 processInput(mainWindow);
		 glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
		 glClear(GL_COLOR_BUFFER_BIT);
		 glUseProgram(shaderProgram);
		 glBindVertexArray(VAO1);
		 glDrawArrays(GL_TRIANGLES, 0, 3);
		 glfwSwapBuffers(mainWindow);
		 glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	 glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	 if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		  glfwSetWindowShouldClose(window, true);
}