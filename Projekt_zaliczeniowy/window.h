#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window {
	int ID{};
	
public:
	GLFWwindow* window{};
	int scr_width = 1920;
	int scr_height = 1200;
	bool captureMouse = false;
	void framebuffer_size();
	void add_shader_ID(int id);

	void Init();
	void CreateWindow();
	void LoadGlad();

};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);