#ifndef NO_UI
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui.h"
#endif

#include <iostream>
#include "DominoInput.h"
#include "DominoSolver.h"

#ifndef NO_UI
static bool show_demo_window = false;
static bool show_another_window = true;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
#endif

int main(int argc, char** argv){
#ifndef NO_UI
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Mexican Train Solver", NULL, NULL);
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

		if (show_another_window)
		{
			ImGui::SetNextWindowPos(ImVec2(0,0));
			ImGui::SetNextWindowSize(ImVec2(800,600));
			ImGui::Begin("Mexican Train Solver", &show_another_window);
			ImGui::Text("Press ESC to exit");	
			ImGui::InputText("Enter starting tile", startDomino, IM_ARRAYSIZE(startDomino), ImGuiInputTextFlags_CharsDecimal);
			ImGui::InputText("Enter tile range", tileRange, IM_ARRAYSIZE(tileRange), ImGuiInputTextFlags_CharsDecimal);
			ImGui::InputTextMultiline("Enter your dominos\none domino per line\nnumbers space separated, ex:\n10 1\n2 3\n...", dominos, IM_ARRAYSIZE(dominos));
			if (ImGui::Button("Solve"))
			{
				if (input.input_dominos_char(dominos, 0) && input.input_domino_range_char(tileRange, 0) && input.input_starting_domino_char(startDomino, 0))
				{
					input.print_dominos();
					DominoSolver solver(input.starting_domino, input.domino_range, input.dominos);
					solvedTrains = solver.print_solved_train_string();
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

#else
	DominoInput in;
	// when not using UI need try catch due to use of fstream functions to take input
	try {
		in.input_starting_domino();
		in.input_domino_range();
		in.input_dominos();
		DominoSolver solve(in.starting_domino, in.domino_range, in.dominos);

		solve.print_solved_train();
	} catch (const char* e) {
		cerr << e << endl;
	}
	return 0;
#endif
}

#ifndef NO_UI
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
#endif

