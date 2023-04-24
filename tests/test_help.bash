#!/bin/bash
echo "Help Option Test"

result1="$(./extfat -h)"
 
if [[ -n $result1 ]]; then
    result=0
else
    result=1
fi
exit $result