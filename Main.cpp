#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>	

#include"Texture.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"


const unsigned int width = 800;
const unsigned int height = 800;

// --- Add after the pyramid vertices/indices ---

// Vertices for a 3D rectangle (cuboid), similar format as pyramid
GLfloat rectangleVertices[] =
{ //     COORDINATES     /        COLORS          /    TexCoord   /        NORMALS       //
    // Bottom face
    -0.5f, 0.0f,  0.5f,   0.5f, 0.8f, 0.2f,   0.0f, 0.0f,   0.0f, -1.0f, 0.0f,
    -0.5f, 0.0f, -0.5f,   0.5f, 0.8f, 0.2f,   0.0f, 5.0f,   0.0f, -1.0f, 0.0f,
     0.5f, 0.0f, -0.5f,   0.5f, 0.8f, 0.2f,   5.0f, 5.0f,   0.0f, -1.0f, 0.0f,
     0.5f, 0.0f,  0.5f,   0.5f, 0.8f, 0.2f,   5.0f, 0.0f,   0.0f, -1.0f, 0.0f,
    // Top face
    -0.5f, 0.5f,  0.5f,   0.5f, 0.8f, 0.2f,   0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f,   0.5f, 0.8f, 0.2f,   0.0f, 5.0f,   0.0f, 1.0f, 0.0f,
     0.5f, 0.5f, -0.5f,   0.5f, 0.8f, 0.2f,   5.0f, 5.0f,   0.0f, 1.0f, 0.0f,
     0.5f, 0.5f,  0.5f,   0.5f, 0.8f, 0.2f,   5.0f, 0.0f,   0.0f, 1.0f, 0.0f,
};

GLuint rectangleIndices[] =
{
    // Bottom
    0, 1, 2,
    0, 2, 3,
    // Top
    4, 5, 6,
    4, 6, 7,
    // Front
    0, 3, 7,
    0, 7, 4,
    // Back
    1, 5, 6,
    1, 6, 2,
    // Left
    0, 4, 5,
    0, 5, 1,
    // Right
    3, 2, 6,
    3, 6, 7
};

// New rectangle (base) vertex data
GLfloat baseRectVertices[] = {
    //  COORDINATES      // COLORS         // TEXCOORD // NORMALS
    -0.5f, 0.0f,  0.5f,   0.7f, 0.7f, 0.7f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
     0.5f, 0.0f,  0.5f,   0.7f, 0.7f, 0.7f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
     0.5f, 0.0f, -0.5f,   0.7f, 0.7f, 0.7f,  1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
    -0.5f, 0.0f, -0.5f,   0.7f, 0.7f, 0.7f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
};
GLuint baseRectIndices[] = { 0, 1, 2, 0, 2, 3 };

// Triangle on top (centered, y=0.5)
GLfloat topTriVertices[] = {
    //  COORDINATES      // COLORS         // TEXCOORD // NORMALS
    -0.5f, 0.0f,  0.0f,   1.0f, 0.2f, 0.2f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
     0.5f, 0.0f,  0.0f,   0.2f, 1.0f, 0.2f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
     0.0f, 0.5f,  0.0f,   0.2f, 0.2f, 1.0f,  0.5f, 1.0f,  0.0f, 1.0f, 0.0f,
};
GLuint topTriIndices[] = { 0, 1, 2 };

// 3D cuboid 
GLfloat cuboidVertices[] = {
    // COORDINATES        // COLORS         // TEXCOORD // NORMALS
    // Bottom face
    -0.5f, 0.0f,  0.5f,   0.7f, 0.7f, 0.7f,  0.0f, 0.0f,   0.0f, -1.0f, 0.0f,
    -0.5f, 0.0f, -0.5f,   0.7f, 0.7f, 0.7f,  0.0f, 5.0f,   0.0f, -1.0f, 0.0f,
     0.5f, 0.0f, -0.5f,   0.7f, 0.7f, 0.7f,  5.0f, 5.0f,   0.0f, -1.0f, 0.0f,
     0.5f, 0.0f,  0.5f,   0.7f, 0.7f, 0.7f,  5.0f, 0.0f,   0.0f, -1.0f, 0.0f,
    // Top face
    -0.5f, 0.5f,  0.5f,   0.7f, 0.7f, 0.7f,  0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f,   0.7f, 0.7f, 0.7f,  0.0f, 5.0f,   0.0f, 1.0f, 0.0f,
     0.5f, 0.5f, -0.5f,   0.7f, 0.7f, 0.7f,  5.0f, 5.0f,   0.0f, 1.0f, 0.0f,
     0.5f, 0.5f,  0.5f,   0.7f, 0.7f, 0.7f,  5.0f, 0.0f,   0.0f, 1.0f, 0.0f,
};
GLuint cuboidIndices[] = {
    0, 1, 2, 0, 2, 3, // Bottom
    4, 5, 6, 4, 6, 7, // Top
    0, 3, 7, 0, 7, 4, // Front
    1, 5, 6, 1, 6, 2, // Back
    0, 4, 5, 0, 5, 1, // Left
    3, 2, 6, 3, 6, 7  // Right
};

// 3D pyramid 
GLfloat pyramid3DVertices[] = {
    // COORDINATES        // COLORS         // TEXCOORD // NORMALS
    // Base
    -0.5f, 0.5f,  0.5f,   1.0f, 0.5f, 0.2f,  0.0f, 0.0f,   0.0f, -1.0f, 0.0f,
     0.5f, 0.5f,  0.5f,   1.0f, 0.5f, 0.2f,  1.0f, 0.0f,   0.0f, -1.0f, 0.0f,
     0.5f, 0.5f, -0.5f,   1.0f, 0.5f, 0.2f,  1.0f, 1.0f,   0.0f, -1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f,   1.0f, 0.5f, 0.2f,  0.0f, 1.0f,   0.0f, -1.0f, 0.0f,
    // Apex
     0.0f, 1.2f,  0.0f,   1.0f, 0.8f, 0.2f,  0.5f, 0.5f,   0.0f, 1.0f, 0.0f,
};
GLuint pyramid3DIndices[] = {
    0, 1, 2, 0, 2, 3, // Base
    0, 1, 4, // Side 1
    1, 2, 4, // Side 2
    2, 3, 4, // Side 3
    3, 0, 4  // Side 4
};

// --- Add after the pyramid VAO/VBO/EBO setup, before the main loop ---

// Rectangle VAO, VBO, EBO

GLfloat vertices[] =
{ //     COORDINATES     /        COLORS          /    TexCoord   /        NORMALS       //
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side

	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,     -0.8f, 0.5f,  0.0f, // Left Side

	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f, -0.8f, // Non-facing side

	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.8f, 0.5f,  0.0f, // Right side

	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f,  0.8f  // Facing side
};


// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2, // Bottom side
	0, 2, 3, // Bottom side
	4, 6, 5, // Left side
	7, 9, 8, // Non-facing side
	10, 12, 11, // Right side
	13, 15, 14 // Facing side
};

GLfloat lightVertices[] =
{ //     COORDINATES     //
	-0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f,  0.1f
};

GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};

int main() {

	glfwInit();
	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



	// Create a GLFWwindow object of 800 by 800 pixels, naming it
	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL Window", NULL, NULL);
	// Error check if the window fails to create
	if(window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);
	//Load GLAD so it configures OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0,0,width,height);


	Shader shaderProgram("default.vert", "default.frag");

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Bind();


	// Shader for light cube
	Shader lightShader("light.vert", "light.frag");
	// Generates Vertex Array Object and binds it
	VAO lightVAO;
	lightVAO.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO lightVBO(lightVertices, sizeof(lightVertices));
	// Generates Element Buffer Object and links it to indices
	EBO lightEBO(lightIndices, sizeof(lightIndices));
	// Links VBO attributes such as coordinates and colors to VAO
	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	// Unbind all to prevent accidentally modifying them
	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();

	glm::vec4 lightAmbient = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);



	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	pyramidModel = glm::translate(pyramidModel, pyramidPos);

	// Second pyramid model matrix (positioned to the left of the original pyramid)
	glm::vec3 pyramid2Pos = glm::vec3(-1.2f, 0.0f, 0.0f);
	glm::mat4 pyramid2Model = glm::mat4(1.0f);
	pyramid2Model = glm::translate(pyramid2Model, pyramid2Pos);
	// Rectangle model matrix (positioned to the right of the pyramid)
	glm::vec3 rectanglePos = glm::vec3(1.2f, 0.0f, 0.0f);
	glm::mat4 rectangleModel = glm::mat4(1.0f);
	rectangleModel = glm::translate(rectangleModel, rectanglePos);

	// Place the light above the rectangle
	glm::vec3 lightPos2 = rectanglePos + glm::vec3(0.0f, 1.5f, 0.0f);
	glm::vec4 lightColor2 = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::mat4 lightModel2 = glm::mat4(1.0f);
	lightModel2 = glm::translate(lightModel2, lightPos2);

	glm::vec3 cuboidPos = glm::vec3(3.0f, 0.0f, 0.0f);
	glm::mat4 cuboidModel = glm::mat4(1.0f);
	cuboidModel = glm::translate(cuboidModel, cuboidPos);

	glm::vec3 pyramid3DPos = glm::vec3(3.0f, 0.0f, 0.0f); // 0.5 above the cuboid
	glm::mat4 pyramid3DModel = glm::mat4(1.0f);
	pyramid3DModel = glm::translate(pyramid3DModel, pyramid3DPos);

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	Texture popCat("pop_cat.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	popCat.texUnit(shaderProgram, "tex0", 0);


	glEnable(GL_DEPTH_TEST); // Enable depth testing for 3D rendering

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f)); // Initialize camera


	VAO rectangleVAO;
	rectangleVAO.Bind();

	VBO rectangleVBO(rectangleVertices, sizeof(rectangleVertices));
	EBO rectangleEBO(rectangleIndices, sizeof(rectangleIndices));
	rectangleVAO.LinkAttrib(rectangleVBO, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	rectangleVAO.LinkAttrib(rectangleVBO, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	rectangleVAO.LinkAttrib(rectangleVBO, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	rectangleVAO.LinkAttrib(rectangleVBO, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	rectangleVAO.Unbind();
	rectangleVBO.Unbind();
	rectangleEBO.Unbind();

	// Cuboid VAO/VBO/EBO
	VAO cuboidVAO;
	cuboidVAO.Bind();
	VBO cuboidVBO(cuboidVertices, sizeof(cuboidVertices));
	EBO cuboidEBO(cuboidIndices, sizeof(cuboidIndices));
	cuboidVAO.LinkAttrib(cuboidVBO, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	cuboidVAO.LinkAttrib(cuboidVBO, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	cuboidVAO.LinkAttrib(cuboidVBO, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	cuboidVAO.LinkAttrib(cuboidVBO, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	cuboidVAO.Unbind();
	cuboidVBO.Unbind();
	cuboidEBO.Unbind();

	// Pyramid VAO/VBO/EBO
	VAO pyramid3DVAO;
	pyramid3DVAO.Bind();
	VBO pyramid3DVBO(pyramid3DVertices, sizeof(pyramid3DVertices));
	EBO pyramid3DEBO(pyramid3DIndices, sizeof(pyramid3DIndices));
	pyramid3DVAO.LinkAttrib(pyramid3DVBO, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	pyramid3DVAO.LinkAttrib(pyramid3DVBO, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	pyramid3DVAO.LinkAttrib(pyramid3DVBO, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	pyramid3DVAO.LinkAttrib(pyramid3DVBO, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	pyramid3DVAO.Unbind();
	pyramid3DVBO.Unbind();
	pyramid3DEBO.Unbind();

	// Set rectangle model matrix uniforms
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "rectangleModel"), 1, GL_FALSE, glm::value_ptr(rectangleModel));

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.Inputs(window); // Handle camera inputs

		camera.updateMatrix(45.0f, 0.1f, 100.0f); // Set the camera matrix with FOV, near and far planes

		shaderProgram.Activate();
		glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

		// Draw the first pyramid
		camera.Matrix(shaderProgram, "camMatrix");
		popCat.Bind();
		VAO1.Bind();
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

		// Draw the second pyramid (to the left)
		camera.Matrix(shaderProgram, "camMatrix");
		popCat.Bind();
		VAO1.Bind();
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramid2Model));
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

		// Draw the rectangle
		camera.Matrix(shaderProgram, "camMatrix");
		popCat.Bind();
		rectangleVAO.Bind();
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(rectangleModel));
		glDrawElements(GL_TRIANGLES, sizeof(rectangleIndices) / sizeof(int), GL_UNSIGNED_INT, 0);

		// Draw the light cube at the new position
		lightShader.Activate();
		camera.Matrix(lightShader, "camMatrix");
		glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
		glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		lightVAO.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);

		// Draw the new cuboid
		shaderProgram.Activate();
		camera.Matrix(shaderProgram, "camMatrix");
		popCat.Bind();
		cuboidVAO.Bind();
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(cuboidModel));
		glDrawElements(GL_TRIANGLES, sizeof(cuboidIndices) / sizeof(int), GL_UNSIGNED_INT, 0);

		// Draw the new pyramid on top
		shaderProgram.Activate();
		camera.Matrix(shaderProgram, "camMatrix");
		popCat.Bind();
		pyramid3DVAO.Bind();
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramid3DModel));
		glDrawElements(GL_TRIANGLES, sizeof(pyramid3DIndices) / sizeof(int), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	popCat.Delete();
	shaderProgram.Delete();
	rectangleVAO.Delete();
	rectangleVBO.Delete();
	rectangleEBO.Delete();
	cuboidVAO.Delete();
	cuboidVBO.Delete();
	cuboidEBO.Delete();
	pyramid3DVAO.Delete();
	pyramid3DVBO.Delete();
	pyramid3DEBO.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}