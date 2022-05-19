CC_FLAGS = -Wall -Werror -Wextra -std=c11
ASAN = -g -fsanitize=address
TEST_FLAGS = -lcheck

OS := $(shell uname -s)
ifeq ($(OS), Darwin)
    FLAGS += -D OS_MAC
else
    FLAGS += -D OS_LINUX
	TEST_FLAGS += -lpthread -pthread -lrt -lm -lsubunit
endif

C_FILES = s21_functions.c s21_sprintf.c s21_sscanf.c s21_additional.c 
O_FILES = s21_functions.o s21_sprintf.o s21_sscanf.o s21_additional.o 

all: clean s21_string.a

rebuild: clean all

test: $(O_FILES) atest.c
	gcc -c $(CC_FLAGS) atest.c
	gcc $(FLAGS) $(TEST_FLAGS) atest.o $(O_FILES) 
	./a.out

debug_test: $(O_FILES) atest.c
	gcc -c -g $(CC_FLAGS) $(ASAN) atest.c
	gcc -g $(FLAGS) $(TEST_FLAGS) $(ASAN) atest.o $(O_FILES) 

s21_string.a: all_functions
	ar rcs s21_string.a $(O_FILES)
	ranlib s21_string.a

gcov_report: clean atest.c
	gcc --coverage -c $(CC_FLAGS) $(TEST_FLAGS) atest.c $(C_FILES)
	gcc --coverage $(TEST_FLAGS) atest.o $(O_FILES)
	./a.out
	lcov -t "coverage_report" -o coverage.info -c -d .
	genhtml -o report coverage.info

gcov_report_additional: atest.c 
	gcc --coverage -lcheck -c $(CC_FLAGS) atest.c $(C_FILES)
	gcc --coverage -lcheck atest.o $(O_FILES)
	./a.out
	geninfo . -b . -o ./coverage.info
	genhtml coverage.info -o coverage

style: clean
	cppcheck *.h *.c
	cp ../materials/linters/CPPLINT.cfg CPPLINT.cfg
	python3 ../materials/linters/cpplint.py --extension=c *.c *.h

leaks: test
	CK_FORK=no leaks --atExit -- ./a.out

atest.c: atest.check
	checkmk clean_mode=1 atest.check > atest.c 

all_functions: $(C_FILES)
	gcc -c $(CC_FLAGS) $(C_FILES)

clean:
	rm -rf *.o *.gch *.out *.gcno *.gcda *.info *.gcov
	rm -rf report/
	rm -rf temp/
	rm -rf atest.c
