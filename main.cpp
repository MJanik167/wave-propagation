#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "GL\glew.h"
#include "GL\freeglut.h"
#include <algorithm>

#include "shaderLoader.h" //narzŕdzie do │adowania i kompilowania shaderˇw z pliku


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


float tablica[nx][nz] = {};

void init() {
	for (int i = 0; i < nx; i++)
	{
		for (int j = 0; j < nz; j++)
		{
			int a = nz - 1 - j; //odwracamy oś Y, żeby było zgodne z OpenGL
			if (((i >= 90 && a >=290) && (i < 360 && a < 410) )|| ((i >= 240 && a >=190) && (i < 510 && a < 310))) {
				tablica[i][a] = 343.f; 
			}
			else
			{
				tablica[i][a] = 1000.f;
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

float p[nx][nz] = {};
float p_future[nx][nz] = {};
float p_past[nx][nz] = {};
unsigned int ebo, ebo2;


/*###############################################################*/ 
void mysz(int button, int state, int x, int y)
{
	mbutton = button;
	switch (state)
	{
	case GLUT_UP:
		break;
	case GLUT_DOWN:
		break;

	}
}
/*******************************************/
void mysz_ruch(int x, int y)
{
	if (mbutton == GLUT_LEFT_BUTTON)
	{

	}
	if (mbutton == GLUT_RIGHT_BUTTON)
	{
	
	}

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
void rysuj(void)
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
			float xo = float(float(i) * sizex)-1.;
			float yo = float(float(j) * sizez)-1.;
			float z = p[i][j];
			glBegin(GL_QUADS);
			glVertex3f(xo, yo, z );
			glVertex3f(xo , yo + sizez , z);
			glVertex3f(xo + sizex , yo + sizez , z);
			glVertex3f(xo + sizex , yo, z);
			glEnd();
		}
	}
	


	glutSwapBuffers();

	glFlush();
	//glUseProgram(programID); //u┐yj programu, czyli naszego shadera	


	//GLuint MVP_id = glGetUniformLocation(programID, "MVP"); // pobierz lokalizację zmiennej 'uniform' "MV" w programie
	//glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &(MVP[0][0]));	   // wyślij tablicę mv do lokalizacji "MV", która jest typu mat4	

	/*
	glVertexAttrib2f(2, screen_width,screen_height);
	glVertexAttrib3f(1, 0.5f, 0.5f, 0.5f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(VAO[0]);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glVertexAttrib3f(1, 0.0f, 0.0f, 1.0f);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
	*/	
}
/*###############################################################*/
void rozmiar(int width, int height)
{
	screen_width = width;
	screen_height = height;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, screen_width, screen_height);

	glutPostRedisplay(); // Przerysowanie sceny
}

/*###############################################################*/
void idle()
{

	glutPostRedisplay();
}

/*###############################################################*/
GLfloat k = 0.05;
GLfloat ad = 0.0;

long int t = 0; //licznik czasu

float max = 0.;
float min = 0.;

void timer(int value) {
	for (int i = 1;i <(nz - 1);i++) {
		for (int j =1;j<(nx - 1);j++)
		{	
			int a = nz - 1 - j;
			p_past[i][j] = 2.0 * p[i][j] - p_future[i][j] + ((dtr * dtr) / (ds * ds)) * tablica[i][j] * tablica[i][j] * (p[i][j+1] + p[i][j-1] + p[i+1][j] + p[i - 1][ j] - 4.0 * p[i][j]);
		}
	}


	float tf = t*dtr;
	float exp_result = expf(-(((pi * fpeak * (tf - (1.0f / fpeak))) * (pi * fpeak * (tf - (1.0f / fpeak))))));
	p_past[xcenter][zcenter] = p_past[xcenter][zcenter] + exp_result * (1.0 - 2.0 * ((pi * fpeak * (tf - (1.0 / fpeak))) * (pi * fpeak * (tf - (1.0 / fpeak)))));


	GLuint MVP_id = glGetUniformLocation(programID, "iTime"); // pobierz lokalizację zmiennej 'uniform' "MV" w programie

	glUniform1f(MVP_id,float(t));

	for (int i = 0; i < nz;i++)
	{
		p_past[i][0] = p[i][0] + p[i][1] - p_future[i][1] + tablica[i][0] * (dtr / ds) * (p[i][1] - p[i][0] - (p_future[i][2] - p_future[i][1]));
		p_past[i][nz-1] = p[i][nz-1] + p[i][nz - 2] - p_future[i][nz - 2] + tablica[i][nz-1] * (dtr / ds) * (p[i][nz - 2] - p[i][nz-1] - (p_future[i][nz - 3] - p_future[i][ nz - 2]));
		p_past[0][i] = p[0][i] + p[1][i] - p_future[1][i] + tablica[0][i] * (dtr / ds) * (p[1][i] - p[0][i] - (p_future[2][i] - p_future[1][i]));
		p_past[nz - 1][i] = p[nx - 1][i] + p[nx - 2][i] - p_future[nx - 2][i] + tablica[nx - 1][i] * (dtr / ds) * (p[nx - 2][i] - p[nx - 1][i] - (p_future[nx - 3][i] - p_future[nx - 2][i]));
	}
	

	t += 1;
	std::copy(&p[0][0], &p[0][0]+nz*nx, &p_future[0][0]);
	std::copy(&p_past[0][0], &p_past[0][0] + nz * nx, &p[0][0]);
	//std::copy(&myint[0][0], &myint[0][0] + rows * columns, &favint[0][0]);
	glutTimerFunc(.12, timer, 0);
    
}
/*###############################################################*/
int main(int argc, char **argv)
{
	init();


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screen_width, screen_height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Przyklad 5");

	glewInit(); //init rozszerzeszeń OpenGL z biblioteki GLEW

	glutDisplayFunc(rysuj);			// def. funkcji rysuj¦cej
    glutIdleFunc(idle);			// def. funkcji rysuj¦cej w czasie wolnym procesoora (w efekcie: ci¦gle wykonywanej)
	glutTimerFunc(20, timer, 0);
	glutReshapeFunc(rozmiar); // def. obs-ugi zdarzenia resize (GLUT)
									
	glutKeyboardFunc(klawisz);		// def. obsługi klawiatury
	glutMouseFunc(mysz); 		// def. obsługi zdarzenia przycisku myszy (GLUT)
	glutMotionFunc(mysz_ruch); // def. obsługi zdarzenia ruchu myszy (GLUT)


	glEnable(GL_DEPTH_TEST);


	
	/*
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo2);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	*/

	programID = loadShaders("vertex_shader.glsl", "fragment_shader.glsl");

	glUseProgram(programID); //u┐yj programu, czyli naszego shadera

	glutMainLoop();					
	

	return(0);
}

