#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <bp/shader.h>
string vertexShaderSource = bp::loadShaderSourceFromFile("assets/vertexShader.vert");
string fragmentShaderSource = bp::loadShaderSourceFromFile("assets/fragmentShader.frag");
unsigned int shader = createShaderProgram(vertexShaderSource.c_str(), fragmentShaderSource.c_str());

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

float vertices[18] = {
	//x   //y  //z   
	 1.0, -1.0, 1.0, 
	 1.0, 1.0, 1.0,
	 1.0, 1.0, 1.0,

	-1.0, 1.0, 1.0,
	 0.0, -1.0, 1.0,
	 0.5, -1.0, 1.0,
};

float triangleColor[3] = { 1.0f, 0.5f, 0.0f };
float triangleBrightness = 1.0f;
bool showImGUIDemoWindow = true;

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	bp::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	unsigned int vao = createVAO(vertices, 6);

	shader.use();
	glBindVertexArray(vao);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Set uniforms
		shader.setFloat("_MyFloat", floatValue);
		shader.setVec2("_MyVec2", vec2[0], vec2[1]);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::Checkbox("Show Demo Window", &showImGUIDemoWindow);
			ImGui::ColorEdit3("Color", triangleColor);
			ImGui::SliderFloat("Brightness", &triangleBrightness, 0.0f, 1.0f);
			ImGui::End();
			if (showImGUIDemoWindow) {
				ImGui::ShowDemoWindow(&showImGUIDemoWindow);
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

