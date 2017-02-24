.PHONY: test1 test2

normalize1: normalize1.c test.c
	gcc -O3 -o normalize1 normalize1.c test.c

normalize2: normalize2.c test.c
	gcc -O3 -o normalize2 normalize2.c test.c

test1: normalize1
	./normalize1

test2: normalize2
	./normalize2
