#!/bin/sh

for v in e?
do
echo $v 
cat $v > $v.tree
./parser_4op < $v | ~/git/progs/tree/tree/tree >> $v.tree
/usr/local/bin/h.sh $v.tree
done

