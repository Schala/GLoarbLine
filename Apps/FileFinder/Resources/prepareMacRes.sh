DIR=`dirname "$0"`
NAME=`basename "$0" .sh | sed -e "s/ /_/g"`
REZDIR="/Applications/Metrowerks CodeWarrior 8.0/Metrowerks CodeWarrior/MacOS Support/Universal/Interfaces/RIncludes"
DEREZ="/Developer/Tools/DeRez"
REZ="/Developer/Tools/Rez"
CPMAC="/Developer/Tools/CpMac"
SPLITFORK="/Developer/Tools/SplitForks"
cd "$DIR"

echo "#include <Types.r>" >AppIcon.r
$DEREZ AppIcon.rsrc Types.r -i "$REZDIR" >>AppIcon.r

echo "#include <Types.r>" >SimpleAlert.r
$DEREZ SimpleAlert.rsrc Types.r -i "$REZDIR" >>SimpleAlert.r

echo "#include <Types.r>" >Version.r
$DEREZ Version.rsrc Types.r -i "$REZDIR" >>Version.r
