DIR=`dirname $0`
cd $DIR
cd ../..
export CVSROOT=":pserver:ggpp@192.168.23.1:/home/.cvsroot"
cvs login
if [ -d HL_Modules ]
then
    cd HL_Modules
    cvs -q update -d
else
    cvs checkout HL_Modules
    cd HL_Modules
fi
cvs logout