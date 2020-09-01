#!/bin/bash

# perl -ne 'BEGIN{$/="\n\n"}m/^([^\s]+)/;$f=$1;open($fh,">", $f);print $fh $_; close $fh;'

for code in `ls -1 tests/tests_in`
do
    expected=$(grep -oE '^[0-9a-f]{4} [0-9a-f]{4} [0-9a-f]{4} [0-9a-f]{4} [0-9a-f]{4} [0-9a-f]{4} [0-9a-f]{4} [0-9a-f]{4} [0-9a-f]{4} [0-9a-f]{4} [0-9a-f]{4} [0-9a-f]{4}' tests/tests_expected/$code)
    received=$(./bin/test $code | grep -oE '^[0-9a-f]{4} [0-9a-f]{4} [0-9a-f]{4} [0-9a-f]{4} [0-9a-f]{4} [0-9a-f]{4} [0-9a-f]{4} [0-9a-f]{4} [0-9a-f]{4} [0-9a-f]{4} [0-9a-f]{4} [0-9a-f]{4}')
    status=""
    if [ "x$expected" = "x" ]
    then
        status="EXPECTED NOT FOUND"
    fi
    if [ "x$received" = "x" ]
    then
        status="(not implemented)"
    fi

    if [ "x$expected" = "x$received" ]
    then
        A=1
        #echo "$code OK $status"
    else
        echo "$code FAILED $status $received | $expected"
    fi
done
