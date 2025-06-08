#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "GL\glew.h"
#include "GL\freeglut.h"
#include <algorithm>

#include "shaderLoader.h" //narzŕdzie do │adowania i kompilowania shaderˇw z pliku
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

//funkcje algebry liniowej
#include "glm/vec3.hpp" // glm::vec3
#include "glm/vec4.hpp" // glm::vec4
#include "glm/mat4x4.hpp" // glm::mat4
#include "glm/gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <iostream>


const float pi = 3.14159265358979323846f;

const int nx=600, nz=600; //wymiary tablicy
float fpeak = 30.f;
float dt = 0.002f;
float ds = 0.02f;
// et nie jest potrzebne

int xcenter = 450;
int zcenter = nz-350;
float vmax = 1000;
float dtr = ds / (2. * vmax);

float tablica[nx * nz] = {};

void init() {
	for (int i = 0; i < nx; i++)
	{
		for (int j = 0; j < nz; j++)
		{
			int a = nz - 1 - j; //odwracamy oś Y, żeby było zgodne z OpenGL
			if (((i >= 90 && a >= 290) && (i < 360 && a < 410)) || ((i >= 240 && a >= 190) && (i < 510 && a < 310))) {
				tablica[i * nz + a] = 343.f;
			}
			else
			{
				tablica[i * nz + a] = 1000.f;
			}
		}
	}
}


//Wymiary okna
int screen_width = 640;
int screen_height = 480;


int pozycjaMyszyX; // na ekranie
int pozycjaMyszyY;
int mbutton; // wcisiety klawisz myszy

//shaders
GLuint programID = 0;

float p[nx * nz] = {};
float p_future[nx * nz] = {};
float p_past[nx * nz] = {};

unsigned int ebo, ebo2;


/*###############################################################*/ 
void mysz(int button, int state, int x, int y)
{

}
/*******************************************/
void mysz_ruch(int x, int y)
{


}
/******************************************/



void klawisz(GLubyte key, int x, int y)
{
	switch (key) {

	case 27:    /* Esc - koniec */
		exit(1);
		break;

	case 'x':
		
		break;
	case '1':

		break;
	case '2':

		break;
	}
	
	
}
/*###############################################################*/
void rysuj()
{

	//GLfloat color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	//glClearBufferfv(GL_COLOR, 0, color);
	float sizex = 2./nx;
	float sizez = 2./nz;
	float dx = sizex*0.1;
	float dz = sizez*0.1;
	
	GLuint res_id = glGetUniformLocation(programID, "iResolution");
	glUniform2f(res_id, screen_width, screen_height);

	//std::cout << "screenwidth: " << screen_width << " screenheight: " << screen_height << std::endl;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Kasowanie ekranu
	
	for (int i = 0; i < nx; i++)
	{
		for (int j = 0; j < nz; j++)
		{
			float xo = float(float(i) * sizex) - 1.;
			float yo = float(float(j) * sizez) - 1.;
			float z = p[i * nz + j];
			glBegin(GL_QUADS);
			glVertex3f(xo, yo, z);
			glVertex3f(xo, yo + sizez, z);
			glVertex3f(xo + sizex, yo + sizez, z);
			glVertex3f(xo + sizex, yo, z);
			glEnd();
		}
	}

	glFlush();
}
/*###############################################################*/
void window_size_callback(GLFWwindow* window, int width, int height)
{
	screen_width = width;
	screen_height = height;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, screen_width, screen_height);

}

/*###############################################################*/
void idle()
{

}

/*###############################################################*/
GLfloat k = 0.05;
GLfloat ad = 0.0;

long int t = 0; //licznik czasu

float max = 0.;
float min = 0.;

bool show_demo_window = true;
bool show_another_window = false;


double currentTime = 0;
double lastTime = 0;
int i = 0;


void timer() {

	for (int i = 1; i < (nz - 1); i++) {
		for (int j = 1; j < (nx - 1); j++)
		{
			int a = nz - 1 - j;
			p_past[i * nz + j] = 2.0 * p[i * nz + j] - p_future[i * nz + j] +
				((dtr * dtr) / (ds * ds)) * tablica[i * nz + j] * tablica[i * nz + j] *
				(p[i * nz + (j + 1)] + p[i * nz + (j - 1)] + p[(i + 1) * nz + j] + p[(i - 1) * nz + j] - 4.0 * p[i * nz + j]);
		}
	}

	float tf = t * dtr;
	float exp_result = expf(-(((pi * fpeak * (tf - (1.0f / fpeak))) * (pi * fpeak * (tf - (1.0f / fpeak))))));
	p_past[xcenter * nz + zcenter] = p_past[xcenter * nz + zcenter] + exp_result * (1.0 - 2.0 * ((pi * fpeak * (tf - (1.0 / fpeak))) * (pi * fpeak * (tf - (1.0 / fpeak)))));

	GLuint MVP_id = glGetUniformLocation(programID, "iTime");
	glUniform1f(MVP_id, float(t));

	for (int i = 0; i < nz; i++)
	{
		p_past[i * nz + 0] = p[i * nz + 0] + p[i * nz + 1] - p_future[i * nz + 1] + tablica[i * nz + 0] * (dtr / ds) * (p[i * nz + 1] - p[i * nz + 0] - (p_future[i * nz + 2] - p_future[i * nz + 1]));
		p_past[i * nz + (nz - 1)] = p[i * nz + (nz - 1)] + p[i * nz + (nz - 2)] - p_future[i * nz + (nz - 2)] + tablica[i * nz + (nz - 1)] * (dtr / ds) * (p[i * nz + (nz - 2)] - p[i * nz + (nz - 1)] - (p_future[i * nz + (nz - 3)] - p_future[i * nz + (nz - 2)]));
		p_past[0 * nz + i] = p[0 * nz + i] + p[1 * nz + i] - p_future[1 * nz + i] + tablica[0 * nz + i] * (dtr / ds) * (p[1 * nz + i] - p[0 * nz + i] - (p_future[2 * nz + i] - p_future[1 * nz + i]));
		p_past[(nx - 1) * nz + i] = p[(nx - 1) * nz + i] + p[(nx - 2) * nz + i] - p_future[(nx - 2) * nz + i] + tablica[(nx - 1) * nz + i] * (dtr / ds) * (p[(nx - 2) * nz + i] - p[(nx - 1) * nz + i] - (p_future[(nx - 3) * nz + i] - p_future[(nx - 2) * nz + i]));
	}

	t += 1;
	std::copy(&p[0], &p[0] + nz * nx, &p_future[0]);
	std::copy(&p_past[0], &p_past[0] + nz * nx, &p[0]);

}

/*###############################################################*/
static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main(int argc, char **argv)
{
	init();


	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	const char* glsl_version = "#version 330";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	//glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);
	GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
	if (window == nullptr)
		return 1;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0); // Enable vsync
	glfwSetWindowSizeCallback(window, window_size_callback);


	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	//ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	glewInit(); //init rozszerzeszeń OpenGL z biblioteki GLEW

	glEnable(GL_DEPTH_TEST);

	programID = loadShaders("vertex_shader.glsl", "fragment_shader.glsl");

	glUseProgram(programID); //u┐yj programu, czyli naszego shadera



	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


	while (!glfwWindowShouldClose(window))
	{



		timer(); // Update the simulation state
		rysuj(); // Render the scene
		
		
		
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		glfwPollEvents();
		if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
		{
			ImGui_ImplGlfw_Sleep(10);
			continue;
		}

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}

		// 3. Show another simple window.
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();



	//glutMainLoop();					
	

	return(0);
}

