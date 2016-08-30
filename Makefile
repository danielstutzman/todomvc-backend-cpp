.PHONY: clean run

default: run

clean:
	rm -f testlibpq

testlibpq: testlibpq.c
	gcc $^ -I/Applications/Postgres.app//Contents/MacOS/include/ -lpq -o $@

run: testlibpq
	./testlibpq "host=localhost dbname=todomvc"
