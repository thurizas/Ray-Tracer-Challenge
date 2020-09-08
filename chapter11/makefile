CC=g++
CCFLAGS=-g -std=c++11 -pedantic -Wall

LK=g++
LFLAGS=

OBJS=camera.o canvas.o color.o light.o material.o matrix2x2.o matrix3x3.o matrix4x4.o object.o \
     pattern.o pch.o plane.o point.o ray.o sphere.o transformations.o util.o vector.o world.o 

PROG=chapter11

PROG: $(OBJS)
	$(CC) -c $(CCFLAGS) chapter11.cpp  -o chapter11.o
	$(LK) chapter11.o $(OBJS) -o chapter11

all : clean PROG

camera.o : camera.cpp camera.h		    	        
	$(CC) -c $(CCFLAGS) camera.cpp -o camera.o

canvas.o : canvas.cpp canvas.h	
	$(CC) -c $(CCFLAGS) canvas.cpp -o canvas.o

color.o : color.cpp color.h
	$(CC) -c $(CCFLAGS) color.cpp -o color.o

light.o : light.cpp light.h
	$(CC) -c $(CCFLAGS) light.cpp -o light.o

material.o : material.cpp material.h
	$(CC) -c $(CCFLAGS) material.cpp -o material.o

matrix2x2.o : matrix2x2.cpp matrix2x2.h
	$(CC) -c $(CCFLAGS) matrix2x2.cpp -o matrix2x2.o

matrix3x3.o : matrix3x3.cpp matrix3x3.h
	$(CC) -c $(CCFLAGS) matrix3x3.cpp -o matrix3x3.o

matrix4x4.o : matrix4x4.cpp matrix4x4.h
	$(CC) -c $(CCFLAGS) matrix4x4.cpp -o matrix4x4.o

object.o : object.cpp object.h
	$(CC) -c $(CCFLAGS) object.cpp -o object.o

pattern.o : pattern.cpp pattern.h
	$(CC) -c $(CCFLAGS) pattern.cpp -o pattern.o

pch.o :	pch.cpp pch.h
	$(CC) -c $(CCFLAGS) pch.cpp -o pch.o

plane.o : plane.cpp plane.h
	$(CC) -c $(CCFLAGS) plane.cpp -o plane.o

point.o : point.cpp point.h
	$(CC) -c $(CCFLAGS) point.cpp -o point.o

ray.o :	ray.cpp ray.h
	$(CC) -c $(CCFLAGS) ray.cpp -o ray.o

sphere.o : sphere.cpp sphere.h
	$(CC) -c $(CCFLAGS) sphere.cpp -o sphere.o

transformations.o : transformations.cpp transformations.h
	$(CC) -c $(CCFLAGS) transformations.cpp -o transformations.o

util.o : util.cpp util.h
	$(CC) -c $(CCFLAGS) util.cpp -o util.o

vector.o : vector.cpp vector.h
	$(CC) -c $(CCFLAGS) vector.cpp -o vector.o

world.o : world.cpp world.h
	$(CC) -c $(CCFLAGS) world.cpp -o world.o

clean :
	rm -rf *.o
	rm -rf *.*~
	rm -rf $(PROG)


