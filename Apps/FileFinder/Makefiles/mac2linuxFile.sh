
cp $1 $1.tmp
tr '\r' '\n' < $1.tmp > $1
rm $1.tmp
echo "" >>$1