.PHONY: clean run

default: run

clean:
	rm -f testlibpq

testhttpd: testhttpd.c
	g++ $^ \
		-Ivendor/httpd/libmicrohttpd-0.9.51/src/include \
	  -I/Applications/Postgres.app//Contents/MacOS/include/ \
		-Lvendor/httpd/libmicrohttpd-0.9.51/src/microhttpd/.libs -lmicrohttpd \
	  -Lvendor/json/libjson -ljson \
		-lpq \
		-o $@

run: testlibpq
	./testlibpq "host=localhost dbname=todomvc"
