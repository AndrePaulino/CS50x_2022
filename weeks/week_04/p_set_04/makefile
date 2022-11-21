p_set_04: filter_less filter_more recover

filter_less: filter_less.c helpers.o
	clang -ggdb3 -gdwarf-4 -O0 -Qunused-arguments -std=c11 -Wall -Werror -Wextra -Wno-gnu-folding-constant -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow -lm -o filter_less filter_less.c helpers.o

filter_more: filter_more.c helpers.o
	clang -ggdb3 -gdwarf-4 -O0 -Qunused-arguments -std=c11 -Wall -Werror -Wextra -Wno-gnu-folding-constant -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow -lm -o filter_more filter_more.c helpers.o

helpers.o: helpers.c helpers.h
	cc -c helpers.c

recover : recover.c
	cc -o recover recover.c

clean:
	rm -rf *.o
