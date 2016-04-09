DIR=`dirname $0`
cd $DIR
cd ..
export CVSROOT=":pserver:ggpp@192.168.23.1:/home/.cvsroot"
cvs login
if [ -d CVS ]
then 
    echo commiting changes
    cvs -q commit -m "" | tee cvs.log
else
    cvs import -d -m "" HL_Modules 'Hotsprings' 'core+files' | tee cvs.log
fi
cvs logout