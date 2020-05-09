#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui.h"
#include "DominoInput.h"
#include "DominoSolver.h"

static bool show_demo_window = false;
static bool show_another_window = true;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void my_display_code();

int main(int argc, char** argv){
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Test Imgui", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create a GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}	

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 150");
	
	DominoInput input;
	string solvedTrains = "";
	char dominos[100] = "";
	char startDomino[5] = "";
	char tileRange[5] = "";
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);
		if (show_another_window)
		{
			ImGui::Begin("Another window", &show_another_window);	
			ImGui::Text("Mexican Train Solver");
			ImGui::InputText("Enter starting tile", startDomino, IM_ARRAYSIZE(startDomino), ImGuiInputTextFlags_CharsDecimal);
			ImGui::InputText("Enter tile range", tileRange, IM_ARRAYSIZE(tileRange), ImGuiInputTextFlags_CharsDecimal);
			ImGui::InputTextMultiline("Enter your dominos below", dominos, IM_ARRAYSIZE(dominos));
			if (ImGui::Button("Solve"))
			{
				if (input.input_dominos_char(dominos, 0) && input.input_domino_range_char(tileRange, 0) && input.input_starting_domino_char(startDomino, 0))
				{
					input.print_dominos();
					DominoSolver solver(input.starting_domino, input.domino_range, input.dominos);
					solvedTrains = solver.print_solved_train_string();
					//std::cout << solvedTrains << std::endl;
				}
				else
				{
					solvedTrains = "Invalid Input";
				}
			}
			ImGui::Text(static_cast<const char*>(solvedTrains.c_str()));
			ImGui::End();

		}
		ImGui::Render();
		glClearColor(clear_color.w, clear_color.x, clear_color.y, clear_color.z);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void my_display_code()
{	
	ImGui::ShowDemoWindow(&show_demo_window);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}


