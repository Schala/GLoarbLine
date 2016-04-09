
export CVSROOT=":pserver:ggpp@192.168.23.1:/home/.cvsroot"
cvs login
cd HL_Modules
(
    cd Makefiles
    cvs -q add Makefile.gcc
)
cvs -q commit -m "msg"
cvs logout