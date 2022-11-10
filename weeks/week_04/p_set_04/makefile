p_set_04 : filter_less filter_more recover

filter_less : filter_less.c
	clang -g -ggdb3 -gdwarf-4 -O0 -Qunused-arguments -std=c11 -Wall -Werror -Wextra -Wno-gnu-folding-constant -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow -lm -o filter_less filter_less.c helpers.c

filter_more : filter_more.c
	clang -g -ggdb3 -gdwarf-4 -O0 -Qunused-arguments -std=c11 -Wall -Werror -Wextra -Wno-gnu-folding-constant -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow -lm -o filter_more filter_more.c helpers.c

recover : recover.c
	cc -g -lcs50 -o recover recover.c
