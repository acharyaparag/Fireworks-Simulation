#----------------------------------------------
#    Example Makefile for compiling a program that uses
#    the Matrix and Vector classes
#----------------------------------------------
#
#  Programmer: Donald House
#  Date: March 8, 1999
#
#  Copyright (C) - Donald H. House. 2005
#

CC      = g++
C	= cpp
H	= h
CFLAGS 	= -g

ifeq ("$(shell uname)", "Darwin")
  LDFLAGS     = -framework Foundation -framework GLUT -framework OpenGL -lm
else
  ifeq ("$(shell uname)", "Linux")
    LDFLAGS     = -lglut -lGL -lm -L/usr/local/lib -lGLU
  endif
endif

HFILES 	= Matrix.${H} Vector.${H} Utility.${H} Camera.${H} particle.${H}  rocket.${H} stateVectorRocket.${H}
OFILES 	= Matrix.o Vector.o Utility.o Camera.o particle.o  rocket.o stateVectorRocket.o
PROJECT = fireworks

${PROJECT}:	${PROJECT}.o $(OFILES)
	${CC} $(CFLAGS) -o ${PROJECT} ${PROJECT}.o $(OFILES) $(LDFLAGS)

${PROJECT}.o: ${PROJECT}.${C} $(HFILES)
	${CC} $(CFLAGS) -c ${PROJECT}.${C}

Camera.o: Camera.${C} Camera.${H} Matrix.${H} Vector.${H} Utility.${H} 
	${CC} $(CFLAGS) -c Camera.${C}

Matrix.o: Matrix.${C} Matrix.${H} Vector.${H} Utility.${H} 
	${CC} $(CFLAGS) -c Matrix.${C}

Vector.o: Vector.${C} Vector.${H} Utility.${H} 
	${CC} $(CFLAGS) -c Vector.${C}

Utility.o: Utility.${C} Utility.${H}
	${CC} $(CFLAGS) -c Utility.${C}

particle.o: particle.${C} particle.${H} Vector.${H} Utility.${H} 
	${CC} $(CFLAGS) -c particle.${C}

rocket.o: rocket.${C} rocket.${H} particle.${H} Vector.${H} Utility.${H} 
	${CC} $(CFLAGS) -c rocket.${C}

stateVectorRocket.o: stateVectorRocket.${C} stateVectorRocket.${H} Vector.${H} Utility.${H} 
	${CC} $(CFLAGS) -c stateVectorRocket.${C}

debug:
	make 'DFLAGS = /usr/lib/debug/malloc.o'

clean:
	rm *.o *~ ${PROJECT}