dispatcher: src/main.c src/loader/loader.c src/data_structures/queue.c src/data_structures/heap.c src/schedulers/LTscheduler.c src/schedulers/MTscheduler.c src/dispatcher/dispatcher.c src/schedulers/STscheduler.c
	gcc src/main.c src/loader/loader.c src/data_structures/queue.c src/data_structures/heap.c src/schedulers/LTscheduler.c src/schedulers/MTscheduler.c src/schedulers/STscheduler.c -Wall -o HOST.exe
	gcc -Wall -std=gnu99 src/dispatcher/dispatcher.c -o dispatcher.exe
debug: src/main.c src/loader/loader.c src/data_structures/queue.c src/data_structures/heap.c src/schedulers/LTscheduler.c src/schedulers/MTscheduler.c src/dispatcher/dispatcher.c src/schedulers/STscheduler.c
	gcc src/main.c src/loader/loader.c src/data_structures/queue.c src/data_structures/heap.c src/schedulers/LTscheduler.c src/schedulers/MTscheduler.c src/schedulers/STscheduler.c -Wall -Wextra -g -o HOST.exe
	gcc -Wall -Wextra -std=gnu99 src/dispatcher/dispatcher.c -g -o dispatcher.exe
test.exe: test/testscript.c src/data_structures/queue.c
	gcc test/testscript.c src/data_structures/queue.c -Wall -g -o test.exe
