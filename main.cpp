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

using namespace std;


GLuint programColor;
GLuint programTexture;

GLuint starsTexture;
GLuint sunTexture;
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

GLuint cubemapTexture;

Core::Shader_Loader shaderLoader;

obj::Model shipModel;
obj::Model sphereModel;

float cameraAngle = 0;
glm::vec3 cameraPos = glm::vec3(-5, 0, 0);
glm::vec3 cameraDir;

glm::mat4 cameraMatrix, perspectiveMatrix;

glm::vec3 lightDir = glm::normalize(glm::vec3(800.0f, 1.0f, 1.0f));

struct Planet {
	float size;
	GLuint texture;
};

Planet planets[8];

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

void initializePlanetsArray() {
        planets[0] = { 0.50f, mercuryTexture };
        planets[1] = { 0.75f, venusTexture   };
        planets[2] = { 1.40f, earthTexture   };
        planets[3] = { 0.75f, marsTexture    };
        planets[4] = { 5.00f, jupiterTexture };
        planets[5] = { 4.00f, saturnTexture  };
        planets[6] = { 1.75f, uranusTexture  };
        planets[7] = { 1.50f, neptuneTexture };
}

void renderScene() {
	cameraMatrix = createCameraMatrix();
	perspectiveMatrix = Core::createPerspectiveMatrix();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glm::mat4 shipModelMatrix = glm::translate(cameraPos + cameraDir * 0.5f + glm::vec3(0,-0.25f,0)) * glm::rotate(-cameraAngle + glm::radians(90.0f), glm::vec3(0,1,0)) * glm::scale(glm::vec3(0.1f));
	drawObjectTexture(&shipModel, shipModelMatrix, shipTexture);

    glm::mat4 planetModelMatrix;

	float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

	glm::mat4 rotationArray[8];
	float basicRadius = 20.0f;

	for (int i = 0; i < 8; i++) {
		glm::mat4 rot;
        glm::mat4 scaling   = glm::scale(glm::vec3(planets[i].size));
        float mnoznik       = 0;

        for (int j = 0 ; j < i; j++) {
            mnoznik += 2*planets[j].size + 1;
        }
        mnoznik+= planets[i].size + 1;

        rot[0][0] 			=  cos(time * 1/planets[i].size);
		rot[2][0] 			= -sin(time * 1/planets[i].size);
		rot[0][2] 			=  sin(time * 1/planets[i].size);
		rot[2][2] 			=  cos(time * 1/planets[i].size);
		rot[3][0] 			=  sin(time * 1/mnoznik) * (basicRadius + mnoznik);
		rot[3][2] 			=  cos(time * 1/mnoznik) * (basicRadius + mnoznik);
        rot = rot * scaling;
		rotationArray[i] 	= rot;
	}

	std::string textures = "textures/";
	std::string ext = ".png";

    float sunSize = 20.0f;
    float startingPoint = 20.0f;


    glm::mat4 sunRotation;
    sunRotation[0][0] 			=  cos(time * 1.0f/sunSize);
    sunRotation[2][0] 			= -sin(time * 1.0f/sunSize);
    sunRotation[0][2] 			=  sin(time * 1.0f/sunSize);
    sunRotation[2][2] 			=  cos(time * 1.0f/sunSize);

    drawObjectTexture(
            &sphereModel,
            glm::translate(glm::vec3(startingPoint,-5,2)) * glm::scale(glm::vec3(sunSize)) * sunRotation,
            sunTexture);

    for (int i = 0; i < 8; i++) {
        drawObjectTexture(
                &sphereModel,
                glm::translate(glm::vec3(startingPoint,-5,2)) * rotationArray[i]/* * glm::scale(glm::vec3(planets[i].size)*/,
                planets[i].texture
        );
    }

	glutSwapBuffers();
}

void init() {
	glEnable(GL_DEPTH_TEST);
	programColor    = shaderLoader.CreateProgram("shaders/shader_color.vert", "shaders/shader_color.frag");
	programTexture  = shaderLoader.CreateProgram("shaders/shader_tex.vert", "shaders/shader_tex.frag");

	sphereModel     = obj::loadModelFromFile("models/sphere.obj");
	shipModel       = obj::loadModelFromFile("models/spaceship.obj");

    starsTexture 	= Core::LoadTexture("textures/stars.png");
    sunTexture 	 	= Core::LoadTexture("textures/sun.png");
    mercuryTexture  = Core::LoadTexture("textures/mercury.png");
    venusTexture    = Core::LoadTexture("textures/venus.png");
    earthTexture    = Core::LoadTexture("textures/earth_big.png");
    marsTexture     = Core::LoadTexture("textures/mars.png");
    jupiterTexture  = Core::LoadTexture("textures/jupiter.png");
    saturnTexture   = Core::LoadTexture("textures/saturn.png");
    uranusTexture   = Core::LoadTexture("textures/uranus.png");
    neptuneTexture  = Core::LoadTexture("textures/neptune.png");
	shipTexture     = Core::LoadTexture("textures/spaceship.png");

    initializePlanetsArray();
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
	glutCreateWindow("Uklad Sloneczny");
	glewInit();

	init();
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(renderScene);
	glutIdleFunc(idle);

	glutMainLoop();

	shutdown();

	return 0;
}
