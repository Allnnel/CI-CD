CC = gcc -Wall -Werror -Wextra

TARGET = clean s21_cat

all: clean s21_cat

s21_cat:
	$(CC) -c  s21_cat.c s21_element.c
	$(CC)  s21_cat.o s21_element.o -o s21_cat

clean :
	rm -rf $(TARGET)
	rm -rf ../*/*.o

rebuild :
	rm $(TARGET)
	rm ../*/*.o
	clang-format -style=Google -i *.c *.h
	$(CC) -c  s21_cat.c s21_element.c
	$(CC)  s21_cat.o s21_element.o -o s21_cat
	clang-format -style=Google -n *.c *.h
	cppcheck --enable=all --suppress=missingIncludeSystem *.c

