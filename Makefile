CC = g++
LIBS = -lmariadb -lgtest -lgtest_main -lpthread
FLAGS =  -g -Wall -Wextra -pedantic 

TARGET = src/core/main

main.out: ${TARGET}.cpp src/data/SQL.o src/data/database.o src/core/account.o src/core/myfunctions.o
	${CC} ${FLAGS} ${LIBS} ${TARGET}.o src/data/SQL.o src/data/database.o  -o main.out

main.o: ${TARGET}.cpp
	${CC} ${FLAGS} ${LIBS} -c ${TARGET}.cpp -o ${TARGET}.o

SQL.o: src/data/SQL.cpp
	${CC} ${FLAGS} ${LIBS} -c src/data/SQL.cpp -o src/data/SQL.o

database.o: src/data/database.cpp
	${CC} ${FLAGS} ${LIBS} -c src/data/database.cpp -o src/data/database.o

account.o: src/core/account.cpp
	${CC} ${FLAGS} ${LIBS} -c src/core/account.cpp -o src/core/account.o

myfunctions.o: src/core/myfunctions.cpp
	${CC} ${FLAGS} ${LIBS} -c src/core/myfunctions.cpp -o src/core/myfunctions.o
