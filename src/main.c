#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "config.h"
/* GLAD must be placed before GLFW */
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "shaders.h"


/* Vertex data defining corners of a rectangle */
float triangle_1[] = {
	/* triangle one */
	-0.9f, -0.4f, 0.0f,
	-0.5f,  0.4f, 0.0f,
	-0.1f, -0.4f, 0.0f
};

float triangle_2[] = {
	/* triangle two */
	 0.1f, -0.4f, 0.0f,
	 0.5f,  0.4f, 0.0f,
	 0.9f, -0.4f, 0.0f
};


/* function declarations */
void process_input(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);


int main(void)
{
	unsigned int vao_array[2];
	unsigned int vbo_array[2];
	GLuint orange_shader, yellow_shader;
	const char *gl_version;

	printf("Hello OpenGL world!\n");
	printf("Package %s\n", PACKAGE_NAME);

	/* open GLFW window */
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

	if (window == NULL) {
		fprintf(stderr, "Failed to create GLFW window\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	/* initialize OpenGL */
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		fprintf(stderr, "Failed to initialize GLAD\n");
		exit(EXIT_FAILURE);
	}

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	gl_version = glGetString(GL_VERSION);
	if (gl_version == NULL) {
		fprintf(stderr, "OpenGL context not initialized\n");
	}

	printf("GL version string: %s\n", gl_version);

	/* compile and link shaders */
	orange_shader = build_shader("../assets/shaders/vertex_shader.glsl",
				     "../assets/shaders/orange_frag_shader.glsl");
	yellow_shader = build_shader("../assets/shaders/vertex_shader.glsl",
				     "../assets/shaders/yellow_frag_shader.glsl");

	/* create VAO and VBO arrays */
	glGenVertexArrays(2, vao_array);
	glGenBuffers(2, vbo_array);

	/* set up the first VAO */
	glBindVertexArray(vao_array[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_array[0]);

	glBufferData(GL_ARRAY_BUFFER,
		     sizeof(triangle_1),
		     triangle_1,
		     GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
			      (void*) 0);

	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	/* set up the second VAO */
	glBindVertexArray(vao_array[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_array[1]);

	glBufferData(GL_ARRAY_BUFFER,
		     sizeof(triangle_2),
		     triangle_2,
		     GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
			      (void*) 0);

	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	/* Render loop */
	while (!glfwWindowShouldClose(window))
	{
		/* check whether user pressed ESC key */
		process_input(window);

		/* rendering commands */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		/* draw a pair of happy triangles */
		glUseProgram(orange_shader);

		glBindVertexArray(vao_array[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glUseProgram(yellow_shader);

		glBindVertexArray(vao_array[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		/* set polygon mode back to fill */
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		/* poll events and swap the buffers */
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	/* properly close the resources used by GLFW window */
	glfwTerminate();

	return 0;
}


void process_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	/* draw in wireframe as long as W key is pressed */
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
}


void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}
