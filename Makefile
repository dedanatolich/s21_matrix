CFLAGS = -Wall -Werror -Wextra

all: clean s21_matrix.a gcov_report

s21_matrix.a: *.c
	gcc -c  $(CFLAGS) *.c
	ar rc  s21_matrix.a *.o
	ranlib s21_matrix.a

test: 
	gcc -o test $(CFLAGS) --coverage *.c tests/*.c -lcheck -lpthread -lm  -I /usr/local/include -L/usr/local/lib

test_ubuntu: *.c
	gcc -o test $(CFLAGS) --coverage *.c tests/*.c -lcheck -lm -lpthread -lsubunit

gcov_report: test_ubuntu
	chmod +x *
	./test
	lcov -t "gcov_test" -o gcov_test.info --no-external -c -d .
	genhtml -o report/ gcov_test.info
	open ./report/index.html

clean:
	rm -f *.o *.a tests/*.gcov tests/*.gcda tests/*.o tests/*.info tests/*.gcno *.gcov *.gcda *.o *.info *.gcno *.out *tests/*.out
	rm -r -f tests/coverage_report
	rm -f test
	rm -f test_ubuntu