#include <stdlib.h>
#include <stdio.h>
#include "config.h"

/* GLAD must be placed before GLFW */
#include "glad/glad.h"
#include <GLFW/glfw3.h>


/* Vertex data defining three points of a triangle */
float vertices[] = {
	-0.5f,  -0.5f, 0.0f,
	 0.5f,  -0.5f, 0.0f,
	 0.0f,   0.5f, 0.0f
};

/* strings holding shader source */

/* vertex shader source */
const char *vertex_shader_src =
	"#version 460 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

/* fragment shader source */
const char *frag_shader_src =
	"#version 460 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";


/* function declarations */
void process_input(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

int main(void)
{
	unsigned int VBO;

	/* variables for checking if shader compiliation succeeded */
	int v_success, f_success, l_success;
	char info_log[512];

	printf("Hello Opengl world!\n");

	/* implement code to open GLFW window here */
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(800,
					      600,
					      "LearnOpenGL",
					      NULL,
					      NULL);

	if (window == NULL) {
		fprintf(stderr, "Failed to create GLFW window\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		fprintf(stderr, "Failed to initialize GLAD\n");
		exit(EXIT_FAILURE);
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	/* compile vertex shader */
	unsigned int vertex_shader;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader,
		       1,
		       &vertex_shader_src,
		       NULL);
	glCompileShader(vertex_shader);

	/* check for compilation errors */
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &v_success);
	if (!v_success) {
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);

		fprintf(stderr, "ERROR::VERTEX::COMPILATION_FAILED\n");
		fprintf(stderr, "%s\n", info_log);
	}

	/* compile fragment shader */
	unsigned int fragment_shader;
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader,
		       1,
		       &frag_shader_src,
		       NULL);
	glCompileShader(fragment_shader);

	/* check for compilation errors */
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &f_success);
	if (!f_success) {
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);

		fprintf(stderr, "ERROR::FRAGMENT::COMPILATION_FAILED\n");
		fprintf(stderr, "%s\n", info_log);
	}

	/* link shader program */
	unsigned int shader_program;
	shader_program = glCreateProgram();

	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	/* check for linking errors */
	glGetProgramiv(shader_program, GL_LINK_STATUS, &l_success);
	if (!l_success) {
		glGetProgramInfoLog(shader_program, 512, NULL, info_log);

		fprintf(stderr, "ERROR::PROGRAM::LINKING_FAILED\n");
		fprintf(stderr, "%s\n", info_log);
	}

	/* delete shader objects */
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);


	/* set up a VAO to hold our VBO */
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER,
		     sizeof(vertices),
		     vertices,
		     GL_STATIC_DRAW);

	/* tell OpenGL how to interpret our vector data */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
			      (void*) 0);

	glEnableVertexAttribArray(0);


	/* Render loop */
	while(!glfwWindowShouldClose(window))
	{
		/* check whether user pressed ESC key */
		process_input(window);

		/* rendering commands */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		/* draw a happy triangle */
		glUseProgram(shader_program);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

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
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}
