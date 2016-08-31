.PHONY: clean run

default: run

clean:
	rm -f testlibpq

testlibpq: testlibpq.c
	gcc $^ -I/Applications/Postgres.app//Contents/MacOS/include/ -lpq -o $@

testhttpd: testhttpd.c
	g++ $^ \
		-Ivendor/httpd/libmicrohttpd-0.9.51/src/include \
		-Lvendor/httpd/libmicrohttpd-0.9.51/src/microhttpd/.libs -lmicrohttpd \
	  -L vendor/json/libjson -ljson \
		-o $@

run: testlibpq
	./testlibpq "host=localhost dbname=todomvc"
