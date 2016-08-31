#!/bin/bash -ex
DYLD_LIBRARY_PATH=vendor/httpd/libmicrohttpd-0.9.51/src/microhttpd/.libs ./testhttpd 4001
