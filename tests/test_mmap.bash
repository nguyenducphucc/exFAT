#!/bin/bash
echo "Mmap Test"

result1=($(/usr/bin/md5sum test.image))

$(./extfat -m -i test.image -o test2.image)

 

result2=($(/usr/bin/md5sum test2.image))

# clean up
$(rm test2.image)
 
if [ "${result1[0]}" != "${result2[0]}" ]; then
    result=0
else
    result=1
fi
exit $result