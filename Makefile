CC = g++
LIBS = -lmariadb -lgtest -lgtest_main -lpthread
FLAGS =  -g -Wall -Wextra -pedantic 

TARGET = src/core/main

main.out: ${TARGET}.cpp src/data/SQL.o src/data/database.o
	${CC} ${FLAGS} ${LIBS} ${TARGET}.o src/data/SQL.o src/data/database.o  -o main.out

main.o: ${TARGET}.cpp
	${CC} ${FLAGS} ${LIBS} -c ${TARGET}.cpp -o ${TARGET}.o

SQL.o: src/data/SQL.cpp
	${CC} ${FLAGS} ${LIBS} -c src/data/SQL.cpp -o src/data/SQL.o

database.o: src/data/database.cpp
	${CC} ${FLAGS} ${LIBS} -c src/data/database.cpp -o src/data/database.o

