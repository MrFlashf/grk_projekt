#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include <cmath>

#include "Shader_Loader.h"
#include "Render_Utils.h"
#include "Camera.h"
#include "Texture.h"


GLuint programColor;
GLuint programTexture;

GLuint mercuryTexture;
GLuint venusTexture;
GLuint earthTexture;
GLuint marsTexture;
GLuint jupiterTexture;
GLuint saturnTexture;
GLuint uranusTexture;
GLuint neptuneTexture;
GLuint gridTexture;
GLuint shipTexture;

Core::Shader_Loader shaderLoader;

obj::Model shipModel;
obj::Model sphereModel;

float cameraAngle = 0;
glm::vec3 cameraPos = glm::vec3(-5, 0, 0);
glm::vec3 cameraDir;

glm::mat4 cameraMatrix, perspectiveMatrix;

glm::vec3 lightDir = glm::normalize(glm::vec3(1.0f, -0.9f, -1.0f));

void keyboard(unsigned char key, int x, int y) {
	float angleSpeed = 0.2f;
	float moveSpeed = 5.0f;
	switch(key) {
        case 'z': cameraAngle -= angleSpeed; break;
        case 'x': cameraAngle += angleSpeed; break;
        case 'w': cameraPos += cameraDir * moveSpeed; break;
        case 's': cameraPos -= cameraDir * moveSpeed; break;
        case 'd': cameraPos += glm::cross(cameraDir, glm::vec3(0,1,0)) * moveSpeed; break;
        case 'a': cameraPos -= glm::cross(cameraDir, glm::vec3(0,1,0)) * moveSpeed; break;
	}
}

glm::mat4 createCameraMatrix() {
	// Obliczanie kierunku patrzenia kamery (w plaszczyznie x-z) przy uzyciu zmiennej cameraAngle kontrolowanej przez klawisze.
	cameraDir = glm::vec3(cosf(cameraAngle), 0.0f, sinf(cameraAngle));
	glm::vec3 up = glm::vec3(0,1,0);

	return Core::createViewMatrix(cameraPos, cameraDir, up);
}


void drawObjectTexture(obj::Model * model, glm::mat4 modelMatrix, GLuint textureID) {
	GLuint program = programTexture;

	glUseProgram(program);
    glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);
	Core::SetActiveTexture(textureID, "objectTex", program, 0);

	Core::DrawModel(model);

	glUseProgram(0);
}

void renderScene() {
	// Aktualizacja macierzy widoku i rzutowania. Macierze sa przechowywane w zmiennych globalnych, bo uzywa ich funkcja drawObject.
	// (Bardziej elegancko byloby przekazac je jako argumenty do funkcji, ale robimy tak dla uproszczenia kodu.
	//  Jest to mozliwe dzieki temu, ze macierze widoku i rzutowania sa takie same dla wszystkich obiektow!)
	cameraMatrix = createCameraMatrix();
	perspectiveMatrix = Core::createPerspectiveMatrix();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.3f, 0.3f, 1.0f);

	// Macierz statku "przyczepia" go do kamery. Warto przeanalizowac te linijke i zrozumiec jak to dziala.
	glm::mat4 shipModelMatrix = glm::translate(cameraPos + cameraDir * 0.5f + glm::vec3(0,-0.25f,0)) * glm::rotate(-cameraAngle + glm::radians(90.0f), glm::vec3(0,1,0)) * glm::scale(glm::vec3(0.25f));
	drawObjectTexture(&shipModel, shipModelMatrix, shipTexture);

    glm::mat4 planetModelMatrix;
	drawObjectTexture(&sphereModel, glm::translate(glm::vec3(0.0f,0,-2)) * glm::scale(glm::vec3(2.0f)) * glm::rotate(2.0f, glm::vec3(1.0f, 1.0f, 1.0f)), mercuryTexture);
	drawObjectTexture(&sphereModel, glm::translate(glm::vec3(20.0f,0,5)) * glm::scale(glm::vec3(4.96f)) * glm::rotate(0.1f, glm::vec3(1.0f, 1.0f, 1.0f)), venusTexture);
    drawObjectTexture(&sphereModel, glm::translate(glm::vec3(35.0f,0,2)) * glm::scale(glm::vec3(5.24f)), earthTexture);
    drawObjectTexture(&sphereModel, glm::translate(glm::vec3(50.06f,0,2)) * glm::scale(glm::vec3(2.78f)), marsTexture);
    drawObjectTexture(&sphereModel, glm::translate(glm::vec3(120.0f,0,2)) * glm::scale(glm::vec3(58.77f)), jupiterTexture);
    drawObjectTexture(&sphereModel, glm::translate(glm::vec3(250,0,-2)) * glm::scale(glm::vec3(47.86f)), saturnTexture);
    drawObjectTexture(&sphereModel, glm::translate(glm::vec3(330,0,2)) * glm::scale(glm::vec3(19.29f)), uranusTexture);
    drawObjectTexture(&sphereModel, glm::translate(glm::vec3(380,0,2)) * glm::scale(glm::vec3(18.67f)), neptuneTexture);
	glutSwapBuffers();
}

void init() {
	glEnable(GL_DEPTH_TEST);
	programColor    = shaderLoader.CreateProgram("shaders/shader_color.vert", "shaders/shader_color.frag");
	programTexture  = shaderLoader.CreateProgram("shaders/shader_tex.vert", "shaders/shader_tex.frag");

	sphereModel = obj::loadModelFromFile("models/sphere.obj");
	shipModel   = obj::loadModelFromFile("models/spaceship.obj");

    mercuryTexture  = Core::LoadTexture("textures/mercury.png");
    venusTexture    = Core::LoadTexture("textures/venus.png");
    earthTexture    = Core::LoadTexture("textures/earth_big.png");
    marsTexture     = Core::LoadTexture("textures/mars.png");
    jupiterTexture  = Core::LoadTexture("textures/jupiter.png");
    saturnTexture   = Core::LoadTexture("textures/saturn.png");
    uranusTexture   = Core::LoadTexture("textures/uranus.png");
    neptuneTexture  = Core::LoadTexture("textures/neptune.png");
	shipTexture     = Core::LoadTexture("textures/spaceship.png");
}

void shutdown() {
	shaderLoader.DeleteProgram(programColor);
	shaderLoader.DeleteProgram(programTexture);
}

void idle() {
	glutPostRedisplay();
}

int main(int argc, char ** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(1920, 1080);
	glutCreateWindow("Układ Słoneczny");
	glewInit();

	init();
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(renderScene);
	glutIdleFunc(idle);

	glutMainLoop();

	shutdown();

	return 0;
}

