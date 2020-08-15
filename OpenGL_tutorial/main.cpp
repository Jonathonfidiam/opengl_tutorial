#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

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

	while (!glfwWindowShouldClose(mainWindow)) {
		 glfwSwapBuffers(mainWindow);
		 glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	 glViewport(0, 0, width, height);
}