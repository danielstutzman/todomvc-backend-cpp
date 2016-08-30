.PHONY: clean run

default: run

clean:
	rm -f testlibpq

testlibpq: testlibpq.c
	gcc $^ -I/Applications/Postgres.app//Contents/MacOS/include/ -lpq -o $@

testjson: testjson.c
	#gcc $^ -Wl,-rpath,./vendor/json/libjson -ljson -o $@ -v
	g++ $^ -L vendor/json/libjson -ljson -o $@ -v

run: testlibpq
	./testlibpq "host=localhost dbname=todomvc"
