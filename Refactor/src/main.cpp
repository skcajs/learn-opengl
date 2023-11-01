#include "shader/Shader.h"
#include "camera/Camera.h"
#include "texture/Texture.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <iostream>


struct Offset
{
	float x;
	float y;
};

void processInput(GLFWwindow* window);
Offset updatePosition(double xposIn, double yposIn);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// settings
const unsigned int SCR_WIDTH = 2560;
const unsigned int SCR_HEIGHT = 1440;

float deltaTime = 0.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
float lastFrame = 0.0f;

float fov = 45.0f;
bool resetMouse = true;

bool keyMPressed = false;
bool lookMode = false;
bool panMode = false;

// Camera
Camera camera{ glm::vec3(0.0f, 0.0f, 3.0f) };

// Light
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 lightColour(1.0f, 1.0f, 1.0f);
float ambientStrength = 0.1f;
float specularStrength = 0.5f;
int shininess = 32;

// Shader
const char* shaders[] = { "phong", "boulaung" };
static const char* current_shader = "phong";
bool usePhong = true;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glEnable(GL_DEPTH_TEST);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// ImGui Setup
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	ImGui::StyleColorsDark();
	io.FontGlobalScale=2.0f;

	Shader phong("shaders/phong.vert", "shaders/phong.frag");
	Shader gouraud("shaders/gouraud.vert", "shaders/gouraud.frag");
	Shader lightSourceShader("shaders/lightSource.vert", "shaders/lightSource.frag");

	// Textures
	Texture texture1{ "assets/container.jpg", GL_TEXTURE0, GL_RGB, GL_RGB };
	Texture texture2{ "assets/awesomeface.png", GL_TEXTURE1, GL_RGBA, GL_RGBA };

	// Create vertices
	float vertices[] = {
		 // positions			 // normals				 // texture coords
		-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,	 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,	 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,	 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,	 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,	 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,	 0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	 1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,	 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,	 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,	 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,	 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,	 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,	 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,	 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,	 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,	 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,	 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,	 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,	 0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,	 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,	 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,	 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,	 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,	 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,	 0.0f, 1.0f
	};

	// Create buffer objects
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	// Light
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Create MVPs
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	while (!glfwWindowShouldClose(window))
	{
		// Input
		processInput(window);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Rendering commands here
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		model = glm::mat4(1.0f);

		view = camera.GetViewMatrix();

		projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(camera.FOV()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		if (current_shader == "phong")
		{
			phong.use();
			phong.setFloat("uAmbientStrength", ambientStrength);
			phong.setFloat("uSpecularStrength", specularStrength);
			phong.setInt("uShininess", shininess);
			phong.setVec3("uObjectColour", 1.0f, 0.5f, 0.31f);
			phong.setVec3("uLightColour", lightColour);
			phong.setVec3("uLightPos", lightPos);
			phong.setMat4("uModel", glm::value_ptr(model));
			phong.setMat4("uView", glm::value_ptr(view));
			phong.setMat4("uProjection", glm::value_ptr(projection));
		}

		else
		{
			gouraud.use();
			gouraud.setFloat("uAmbientStrength", ambientStrength);
			gouraud.setFloat("uSpecularStrength", specularStrength);
			gouraud.setInt("uShininess", shininess);
			gouraud.setVec3("uViewPos", camera.Position());
			gouraud.setVec3("uObjectColour", 1.0f, 0.5f, 0.31f);
			gouraud.setVec3("uLightColour", lightColour);
			gouraud.setVec3("uLightPos", lightPos);
			gouraud.setMat4("uModel", glm::value_ptr(model));
			gouraud.setMat4("uView", glm::value_ptr(view));
			gouraud.setMat4("uProjection", glm::value_ptr(projection));
		}


		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));

		lightSourceShader.use();
		lightSourceShader.setVec3("uLightColour", lightColour);
		lightSourceShader.setMat4("uModel", glm::value_ptr(model));
		lightSourceShader.setMat4("uView", glm::value_ptr(view));
		lightSourceShader.setMat4("uProjection", glm::value_ptr(projection));

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		{
			ImGui::Begin("LearnOpenGL");                          
			ImGui::Text("Helper menu for learning OpenGL.");    
			ImGui::NewLine();
			if (ImGui::BeginCombo("lighting model", current_shader))
			{
				for (int n = 0; n < IM_ARRAYSIZE(shaders); n++)
				{
					bool is_selected = (current_shader == shaders[n]);
					if (ImGui::Selectable(shaders[n], is_selected))
						current_shader = shaders[n];
						if (is_selected)
							ImGui::SetItemDefaultFocus(); 
				}
				ImGui::EndCombo();
			}
			ImGui::SliderFloat("ambient strength", &ambientStrength, 0.0f, 1.0f);
			ImGui::SliderFloat("specular strength", &specularStrength, 0.0f, 1.0f);
			ImGui::SliderInt("shininess", &shininess, 0, 128);
			ImGui::NewLine();
			ImGui::ColorEdit3("clear color", (float*)&clear_color);
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Check and call events and swap the buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
		lookMode = true;
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_3) == GLFW_PRESS)
		panMode = true;
	else if (!keyMPressed)
	{
		panMode = false;
		lookMode = false;
		resetMouse = true;
	}
	
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		keyMPressed = !keyMPressed;
		if (lookMode)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		lookMode = !lookMode;
		resetMouse = true;
	}
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	if (lookMode)
	{
		Offset offset = updatePosition(xposIn, yposIn);
		camera.ProcessMouseMovement(offset.x, offset.y);
	}
	else if (panMode)
	{
		Offset offset = updatePosition(xposIn, yposIn);
		camera.ProcessMousePan(offset.x, offset.y);
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

Offset updatePosition(double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (resetMouse)
	{
		lastX = xpos;
		lastY = ypos;
		resetMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	return Offset{ xoffset, yoffset };
}
