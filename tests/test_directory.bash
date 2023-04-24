#!/bin/bash
echo "Directory Test"

result1="$(./extfat -i test.image -d)"
 
if [[ -n $result1 ]]; then
    result=0
else
    result=1
fi
exit $result