
DIR=`dirname $0`
cd $DIR
make -f Makefile.gcc 2>&1 | tee make.log
