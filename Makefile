main: src/main.c src/algorithms.c
	gcc src/main.c src/algorithms.c -Wall -o BANKERS.exe
debug: src/main.c src/algorithms.c
	gcc src/main.c src/algorithms.c -g -Wall -Wextra -o BANKERS.exe
test.exe: test/testscript.c src/data_structures/queue.c
	gcc test/testscript.c src/data_structures/queue.c -Wall -g -o test.exe
