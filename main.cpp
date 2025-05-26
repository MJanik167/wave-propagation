#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "GL\glew.h"
#include "GL\freeglut.h"

#include "shaderLoader.h" //narzŕdzie do │adowania i kompilowania shaderˇw z pliku


//funkcje algebry liniowej
#include "glm/vec3.hpp" // glm::vec3
#include "glm/vec4.hpp" // glm::vec4
#include "glm/mat4x4.hpp" // glm::mat4
#include "glm/gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <iostream>


const int nx=600, nz=600; //wymiary tablicy

float tablica[nx][nz] = {};

void init() {
	for (int i = 0; i < nx; i++)
	{
		for (int j = 0; j < nz; j++)
		{
			int a = nz - 1 - j; //odwracamy oś Y, żeby było zgodne z OpenGL
			if (((i >= 100 && a >=300) && (i < 350 && a < 500) )|| ((i >= 250 && a >=100) && (i < 500 && a < 300))) {
				tablica[i][a] = -1.f; 
			}
			else
			{
				tablica[i][a] = 0.f;
			}
		}
	}
	tablica[500][nz-500] = 1.f; //przykładowa wartość, żeby coś było widać
}



//Wymiary okna
int screen_width = 640;
int screen_height = 480;


int pozycjaMyszyX; // na ekranie
int pozycjaMyszyY;
int mbutton; // wcisiety klawisz myszy

double kameraX= 60.0;
double kameraZ = 30.0;
double kameraD = -3.0;
double kameraPredkosc;
double kameraKat = 20;
double kameraPredkoscObrotu;
double poprzednie_kameraX;
double poprzednie_kameraZ;
double poprzednie_kameraD;

double rotation = 0;

//macierze
glm::mat4 MV; //modelview - macierz modelu i świata
glm::mat4 P;  //projection - macierz projekcji, czyli naszej perspektywy



float vertices[] = {
	-1.,-1.,0.,
	1.,-1.,0.,
	1.,1.,0.,
	-1.,1.,0.,
};

float vertices2[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.5f,  0.5f, 0.0f,
   -0.5f, 0.5f, 0.0f,


	//-0.5f, -0.5f, 0.0f,
	// 0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, -0.5f,
	 -0.5f, -0.5f, -0.5f,
};

GLuint elements[] = { 0, 1, 2,3 };
GLuint elements2[] = { 0, 1, 2,3, 0, 1, 4, 5};
//shaders
GLuint programID = 0;

unsigned int VBO,VBO2;
unsigned int ebo, ebo2;
unsigned int VAO[2];


/*###############################################################*/
void mysz(int button, int state, int x, int y)
{
	mbutton = button;
	switch (state)
	{
	case GLUT_UP:
		break;
	case GLUT_DOWN:
		pozycjaMyszyX = x;
		pozycjaMyszyY = y;
		poprzednie_kameraX = kameraX;
		poprzednie_kameraZ = kameraZ;
		poprzednie_kameraD = kameraD;
		break;

	}
}
/*******************************************/
void mysz_ruch(int x, int y)
{
	if (mbutton == GLUT_LEFT_BUTTON)
	{
		kameraX = poprzednie_kameraX - (pozycjaMyszyX - x) * 0.1;
		kameraZ = poprzednie_kameraZ - (pozycjaMyszyY - y) * 0.1;
	}
	if (mbutton == GLUT_RIGHT_BUTTON)
	{
		kameraD = poprzednie_kameraD + (pozycjaMyszyY - y) * 0.1;
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

	std::cout << "screenwidth: " << screen_width << " screenheight: " << screen_height << std::endl;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Kasowanie ekranu

	glColor3f(0.0f, 0.0f, 1.0f); // Ustawienie koloru rysowania
	/*
	glBegin(GL_QUADS);
	glColor3f(1., 1., 1.);
	glVertex2f(-1. , -1. );
	glVertex2f(-1.+sizex, -1.);
	glVertex2f(-1.+sizex ,-1.+ sizez);
	glVertex2f(-1.,-1.+sizez);
	glEnd();
	*/

	
	for (int i = 0; i < nx; i++)
	{
		for (int j = 0; j < nz; j++)
		{
			float xo = float(float(i) * sizex)-1.;
			float yo = float(float(j) * sizez)-1.;
			glBegin(GL_QUADS);
			glColor3f(tablica[i][j],1.,1.);
			glVertex3f(xo , yo , tablica[i][j]);
			glVertex3f(xo , yo + sizez , tablica[i][j]);
			glVertex3f(xo + sizex , yo + sizez , tablica[i][j]);
			glVertex3f(xo + sizex , yo, tablica[i][j]);
			glEnd();
		}
	}
	


	glutSwapBuffers();
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
	
	glFlush();
	*/	
}
/*###############################################################*/
void rozmiar(int width, int height)
{
	screen_width = width;
	screen_height = height;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, screen_width, screen_height);

	P = glm::perspective(glm::radians(60.0f), (GLfloat)screen_width / (GLfloat)screen_height, 1.0f, 1000.0f);

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

void timer(int value) {

	//ad+= k;
	
	//if(ad>1 || ad<0)
	//k=-k;

	//GLfloat attrib[] = { ad, 0.0f,0.0f };
	// Aktualizacja wartości atrybutu wejściowego 1.
	//glVertexAttrib3fv(1, attrib);

	/*
	
	//W vertex_shader np:
	//layout (location = 1) in vec3 incolor;
	
	*/
	glutTimerFunc(20, timer, 0);
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

	glGenVertexArrays(2, VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
	glGenBuffers(1, &ebo2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements2), elements2, GL_STATIC_DRAW);


	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo2);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


	programID = loadShaders("vertex_shader.glsl", "fragment_shader.glsl");

	glUseProgram(programID); //u┐yj programu, czyli naszego shadera

	glutMainLoop();					
	
	glDeleteBuffers(1,&VBO);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &ebo2);
	glDeleteBuffers(2, VAO);
	return(0);
}

