main: src/main.c src/algorithms.c src/resources.c
	gcc src/main.c src/algorithms.c src/resources.c -Wall -o BANKERS.exe
debug: src/main.c src/algorithms.c src/resources.c
	gcc src/main.c src/algorithms.c src/resources.c -g -Wall -Wextra -o BANKERS.exe
test.exe: test/testscript.c
	gcc test/testscript.c -Wall -g -o test.exe
