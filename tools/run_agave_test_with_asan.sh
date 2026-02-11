#!/bin/bash

export LD_PRELOAD="/lib/x86_64-linux-gnu/libnss_dns.so.2 /lib/x86_64-linux-gnu/libresolv.so.2"
export ASAN_OPTIONS="verify_asan_link_order=0:detect_leaks=1:intercept_tls_get_addr=0:alloc_dealloc_mismatch=0"

./agave_test "$@"

