bin_PROGRAMS = truffle

truffle_SOURCES = \
	src/main.c \
	src/glad.c \
	src/glad/glad.h \
	src/KHR/khrplatform.h

truffle_CFLAGS = @gllibs_CFLAGS@
truffle_LDADD = @gllibs_LIBS@ -lm
