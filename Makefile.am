bin_PROGRAMS = truffle

truffle_SOURCES = \
	src/main.c \
	src/glad.c \
	include/glad/glad.h \
	include/KHR/khrplatform.h

truffle_CFLAGS = @gllibs_CFLAGS@
truffle_LDADD = @gllibs_LIBS@ -lm
