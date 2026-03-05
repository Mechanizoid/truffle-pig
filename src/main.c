#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "config.h"

/* GLAD must be placed before GLFW */
#include "glad/glad.h"
#include <GLFW/glfw3.h>


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

/* strings holding shader source */

/* vertex shader source */
const char *vertex_shader_src =
	"#version 460 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

/* orange fragment shader source */
const char *orange_frag_shader_src =
	"#version 460 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";

/* yellow fragment shader source */
const char *yellow_frag_shader_src =
	"#version 460 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"  FragColor = vec4(1.0f, 0.9f, 0.1f, 1.0f);"
	"}\0";


/* function declarations */
void process_input(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
GLuint compile_shader(GLenum type, const char *src);
void check_shader_compilation(GLenum type, GLuint shader);
GLuint link_shader_prog(GLuint vertex_shader, GLuint frag_shader);


int main(void)
{
	unsigned int vao_array[2];
	unsigned int vbo_array[2];
	GLuint vertex_shader, frag_shader;
	GLuint orange_shader, yellow_shader;

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

	/* compile and link shaders */
	vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_shader_src);
	frag_shader = compile_shader(GL_FRAGMENT_SHADER, orange_frag_shader_src);
	orange_shader = link_shader_prog(vertex_shader, frag_shader);

	vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_shader_src);
	frag_shader = compile_shader(GL_FRAGMENT_SHADER, yellow_frag_shader_src);
	yellow_shader = link_shader_prog(vertex_shader, frag_shader);

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

GLuint compile_shader(GLenum type, const char *src)
{
	GLuint shader;

	shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);

	/* check that shader compiled successfully */
	check_shader_compilation(type, shader);

	return shader;
}


void check_shader_compilation(GLenum type, GLuint shader)
{
	int success;
	char info_log[512], type_str[512] = "";

	switch (type) {
	case GL_VERTEX_SHADER:
		strcat(type_str, "VERTEX");
		break;
	case GL_FRAGMENT_SHADER:
		strcat(type_str, "FRAGMENT");
		break;
	}

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if(!success) {
		glGetShaderInfoLog(shader, 512, NULL, info_log);
		fprintf(stderr,	"ERROR::%s::COMPILATION_FAILED\n%s",
			type_str, info_log);
	}
}

GLuint link_shader_prog(GLuint vertex_shader, GLuint frag_shader)
{
	GLuint prog;
	int success;
	char info_log[512];

	/* create shader program */
	prog = glCreateProgram();

	/* attach frag and vertex shaders to program */
	glAttachShader(prog, vertex_shader);
	glAttachShader(prog, frag_shader);

	/* link program */
	glLinkProgram(prog);

	/* check for linking errors */
	glGetProgramiv(prog, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(prog, 512, NULL, info_log);
		fprintf(stderr, "ERROR::PROGRAM::LINKING_FAILURE\n%s\n", info_log);
	}

	/* delete shader objects */
	glDeleteShader(vertex_shader);
	glDeleteShader(frag_shader);

	return prog;
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
