#include <iomanip>
#include <string>

#include "Shader.h"
#include "movement.h"
#include "window.h"

#include <iostream>

int main() {
	// MENU
	std::string tmp;
	std::string shaderName{};
	int shaderNumber{};
	bool captureMouse = false;
	char choice{};
	std::cout << "Which shader do you want to run?:\n";
	std::cout << "1 - spiral.fs - (epilepsy warning!)\n";
	std::cout << "2 - mandelbrot.fs\n";
	std::cout << "3 - juliaSet.fs\n";
	std::cout << "4 - darkMatter.fs\n";
	std::cout << "5 - Custom shader\n";
	std::cout << "\nOther options stop the program\n\n";
	std::cin >> shaderNumber;

	switch (shaderNumber) {
	case 1:
		shaderName = "spiral.fs";
		break;
	case 2:

		shaderName = "mandelbrot.fs";
		captureMouse = true;
		break;
	case 3:

		shaderName = "juliaSet.fs";
		break;
	case 4:

		shaderName = "darkMatter.fs";
		captureMouse = true;
		break;
	case 5:

		std::cin >> shaderName;
		std::cout << "Capture the mouse? y/n";
		std::cin >> choice;
		if (choice == 'y' || choice == 'Y') captureMouse = true;
		break;
	default:
		return 1;
	}
	// converting string to char
	char* selectedShader = new char[shaderName.length()+1];
	int i{};
	while (shaderName[i]) {
		selectedShader[i] = shaderName[i];
		i++;
	}
	selectedShader[i] = '\0';

	// creating window
	Window Win;
	Win.captureMouse = captureMouse;
	Win.Init();
	
	const GLubyte *version = glGetString(GL_VERSION); // get version
	std::cout<<*version<<"\n";
	if (*version > static_cast<GLubyte>(300)) shaderName = "shader330.vs\0";
	else shaderName = "shader120.vs\0";
	// convert string to char again
	char* vertexShader= new char[shaderName.length()+1];
	i = 0;
	while (shaderName[i]) {
		vertexShader[i] = shaderName[i];
		i++;
	}
	vertexShader[i] = '\0';
	std::cout << vertexShader<<"\n";


	Camera cam;
	Shader shader(vertexShader, selectedShader);
	delete[] vertexShader, selectedShader;
	shader.use();
	shader.setVec2("resolution", Win.scr_width, Win.scr_height); // pass screen width and height to selected shader

	// set plane position
	float vertices[]{
		1.0f,1.0f,0.0f,
		-1.0f,1.0f,0.0f,
		1.0f,-1.0f,0.0f,
		-1.0f,-1.0f,0.0f
	};
	unsigned int indices[]{
		0,1,2,
		3,2,1

	};

	// fill buffer and vertex data with plane data
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glBindAttribLocation(shader.ID, 0, "aPos"); // used in version 120

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glfwSwapInterval(1); // fps limit

	// camera settings
	glm::vec3 CamPos = glm::vec3(0.0,1.0,0.0);
	glm::vec3 CamDir = glm::vec3(0.0, 0.0, 1.0);
	glm::vec3 CamUp = glm::vec3(0.0, 1.0, 0.0);
	glm::vec3 CamRight = glm::vec3(1.0, 0.0, 0.0);
	glm::vec4 quaternion=glm::vec4(0.0,0.0,0.0,-1.0);
	glm::vec4 NewQuaternion{};
	float deltaTime{};
	float lastFrame{};
	
	// Camera class does the operations
	cam.CamPos = &CamPos;
	cam.CamDir = &CamDir;
	cam.CamUp = &CamUp;
	cam.CamRight = &CamRight;
	while (!glfwWindowShouldClose(Win.window)) {
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// clear screen
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		cam.processMouseMovement(Win.window); // get mouse position change
		cam.processInput(Win.window);
		Win.framebuffer_size();

		// rotate quaternion on mouse movement
		if (cam.DeltaPos.x || cam.DeltaPos.y) {

			// convert camera movement into vector pointed in "z" direction
			glm::vec3 CamMove = normalize(glm::vec3(cam.DeltaPos.x, cam.DeltaPos.y, std::max(2.0*sqrt(abs(cam.DeltaPos.x)), 2.0 * sqrt(abs(cam.DeltaPos.y)))));

			// get cross product of "pure z" axis and vector from camera movement. It's direction of rotation axis
			glm::vec3 Direction = normalize(glm::cross(CamMove, glm::vec3(0.0,0.0,1.0))); 

			// Creates quaternion x + yi + zj + wk where "x" determines an angle of rotation and y, z, w are vector part (Direction).
			NewQuaternion = normalize(glm::vec4(cos(CamMove.z * 3.1415f), float(sin(CamMove.z * 3.1415f))*Direction)); 

			// rotate old quaternion using new one
			quaternion = cam.quatMultip(NewQuaternion,quaternion);
			CamDir = cam.quaternionRotationMatrix(quaternion)*glm::vec3(0.0,0.0,1.0);
			CamUp = cam.quaternionRotationMatrix(quaternion) * glm::vec3(0.0, 1.0, 0.0);
			CamRight = glm::cross(CamDir, CamUp);
		}

		// update shader data
		shader.setVec3("CamPos", CamPos);
		shader.setVec3("CamDir", normalize(CamDir));
		shader.setVec4("quaternion", quaternion);
		shader.setFloat("iTime", currentFrame);
		shader.setVec2("mPos", float(cam.xpos),float(cam.ypos));

		
		//draws plane
		glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
		
		glfwSwapBuffers(Win.window);
		glfwPollEvents();
	}
	glfwTerminate();
}
