#!/bin/bash
echo "Input Files Test"

result1=($(/usr/bin/md5sum test.image))

./extfat -i test.image

# clean up
$(rm test.image)
 
if [ "${result1[0]}" ]; then
    result=0
else
    result=1
fi
exit $result