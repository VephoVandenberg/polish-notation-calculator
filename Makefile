
output: main.o functionality.o
  gcc main.o functinoality.o -o output
  
main.o:
  gcc -c main.cpp


fuinctionality.o: functionality.c functionality.h
  gcc -c functionality.c
 
 clean:
  rm *.o output
  
