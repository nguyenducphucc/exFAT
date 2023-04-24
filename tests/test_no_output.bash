#!/bin/bash
echo "User-Provided Output Files Test"

result1=($(/usr/bin/md5sum test.image))

./extfat -i test.image
 
result2=($(/usr/bin/md5sum test.image))

# clean up
$(rm test.image)
 
if [ "${result1[0]}" = "${result2[0]}" ]; then
    result=0
else
    result=1
fi
exit $result