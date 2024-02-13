CC = gcc -Wall -Werror -Wextra

TARGET = s21_grep


all: clean s21_grep


s21_grep: clean
	$(CC) -c  s21_grep.c 
	$(CC)  s21_grep.o  -o s21_grep
	


clean :
	rm -rf $(TARGET)
	rm -rf ../*/*.o

rebuild :
	rm $(TARGET)
	rm ../*/*.o
	clang-format -style=Google -i *.c *.h
	$(CC) -c s21_grep.c 
	$(CC)  s21_grep.o  -o s21_grep
	clang-format -style=Google -n *.c *.h
	cppcheck --enable=all --suppress=missingIncludeSystem *.c