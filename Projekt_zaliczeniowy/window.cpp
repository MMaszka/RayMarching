#include "window.h"

void Window::framebuffer_size() // update shader and window on resize
{
	glfwGetFramebufferSize(window, &scr_width, &scr_height);
	glUniform2f(glGetUniformLocation(ID, "resolution"), static_cast<GLfloat>(scr_width), static_cast<GLfloat>(scr_height));
	glViewport(0, 0, scr_width, scr_height);
}
void Window::add_shader_ID(int id) { // set window main shader - FIX
	ID = id;
}
void Window::Init() { // do everything

	CreateWindow();
	LoadGlad();

}


void Window::CreateWindow() {

	glfwInit(); // configure window settings
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create window
	window = glfwCreateWindow(static_cast<int>(scr_width), static_cast<int>(scr_height), "Shader", glfwGetPrimaryMonitor(), NULL);
	if (window == NULL) {
		std::cout << "Failed to create window";
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	if(captureMouse==true)glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
void Window::LoadGlad() {

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		std::cout << "Failed to load GLFW";
		glfwTerminate();
	}

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// terminate window on ESC press
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

}
