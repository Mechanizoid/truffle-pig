/* Compile and link shaders */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shaders.h"
/* OpenGL libs */
#include "glad/glad.h"
#include <GLFW/glfw3.h>


/* static function declarations */
static GLuint compile_shader(GLenum type, const char *src);
static void check_shader_compilation(GLenum type, GLuint shader);
static GLuint link_shader(GLuint v_shader, GLuint f_shader);
static char *load_shader_src(const char *filename);


GLuint build_shader(const char *v_src_file, const char *f_src_file)
{
	GLuint s_prog, v_shader, f_shader;
	char *v_shader_src, *f_shader_src;

	v_shader_src = load_shader_src(v_src_file);
	f_shader_src = load_shader_src(f_src_file);

	v_shader = compile_shader(GL_VERTEX_SHADER, v_shader_src);
	f_shader = compile_shader(GL_FRAGMENT_SHADER, f_shader_src);

	s_prog = link_shader(v_shader, f_shader);

	return s_prog;
}


static GLuint compile_shader(GLenum type, const char *src)
{
	GLuint shader;

	shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, NULL);

	glCompileShader(shader);

	check_shader_compilation(type, shader);

	return shader;
}


static void check_shader_compilation(GLenum type, GLuint shader)
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
		fprintf(stderr, "ERROR::%s::COMPILATION_FAILED\n%s",
			type_str, info_log);
	}
}


static GLuint link_shader(GLuint v_shader, GLuint f_shader)
{
	GLuint s_prog;
	int success;
	char info_log[512];

	s_prog = glCreateProgram();

	glAttachShader(s_prog, v_shader);
	glAttachShader(s_prog, f_shader);

	glLinkProgram(s_prog);

	glGetProgramiv(s_prog, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(s_prog, 512, NULL, info_log);
		fprintf(stderr, "ERROR::PROGRAM::LINKING_FAILURE\n%s\n", info_log);
	}

	glDeleteShader(v_shader);
	glDeleteShader(f_shader);

	return s_prog;
}


static char *load_shader_src(const char *filename)
{
	size_t size;
	FILE *src_file;
	char *shader_src;
	int i = 0;

	src_file = fopen(filename, "r");

	if (src_file == NULL) {
		fprintf(stderr, "ERROR: failed to open file %s in load_shader_src\n",
			filename);
		exit(EXIT_FAILURE);
	}

	printf("Status: opened source file %s\n", filename);

	fseek(src_file, 0L, SEEK_END);
	size = ftell(src_file);
	rewind(src_file);

	shader_src = calloc(size + 1, sizeof(char));
	if (shader_src == NULL) {
		fprintf(stderr, "ERROR: call of calloc failed in load_shader_src\n");
		exit(EXIT_FAILURE);
	}

	for ( ; i < size; i++) {
		shader_src[i] = fgetc(src_file);
	}
	shader_src[i] = '\0';

	fclose(src_file);

	return shader_src;
}
