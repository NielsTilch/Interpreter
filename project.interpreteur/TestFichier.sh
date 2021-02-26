#!/bin/bash

make clean
make main

echo "sup"

`./testaro ./tests/fileTest/file1.txt`
status1 = $?

[ $status1 -eq 0 ] && "$command1 a répondu 0 comme prévu :D" || echo "$command1 a raté :(, on a $status1"

`./testaro ./tests/fileTest/file2.txt`
status2 = $?

[ $status2 -eq 1 ] && "$command1 a répondu 1 comme prévu :D" || echo "$command1 a raté :(, on a $status2"