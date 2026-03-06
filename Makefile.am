bin_PROGRAMS = truffle

truffle_SOURCES = \
	src/main.c \
	src/glad.c \
	src/shaders.c \
	src/shaders.h \
	src/glad/glad.h \
	src/KHR/khrplatform.h

# assetsdir = $(srcdir)/assets

# assets_DATA = shaders/vertex_shader.glsl \
# 	shaders/orange_frag_shader.glsl \
# 	shaders/yellow_frag_shader.glsl


truffle_CFLAGS = @gllibs_CFLAGS@
truffle_LDADD = @gllibs_LIBS@ -lm
