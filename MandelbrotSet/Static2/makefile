CC = /usr/lib64/openmpi/bin/mpicc
CFLAGS = -Wall -g -std=c99 
LIBS = 
EXEC = PA02
HDRS = ${wildcard *.h}
SRCS = ${wildcard *.c}
OBJS = ${SRCS:.c=.o}

${EXEC}: ${OBJS}
	${CC} ${LIBS} -o ${EXEC} ${OBJS} -lm

%.o: %.c
	${CC} ${LIBS} ${CFLAGS} -c $*.c -lm

${OBJS}: ${HDRS}

clean:
	rm -f ${OBJS} ${EXEC}
