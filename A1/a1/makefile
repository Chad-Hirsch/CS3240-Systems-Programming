all:
	gcc -Wall -Wpedantic -std=gnu99 -g BuildDataBase.c -o BuildDataBase
	gcc -Wall -Wpedantic -std=gnu99 -g UseDataBase.c -o UseDataBase
build:
	./BuildDataBase
use:
	./UseDataBase < test-input.txt
