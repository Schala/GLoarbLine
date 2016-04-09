HOMEDIR=`dirname "$0"`
cd $HOMEDIR

echo "Make sure to run AppIcon.png through GraphicConverter and save with icon resources"
echo "Use Iconographer to save the .icns file"

TYPES="-only 'ICN#' -only 'ics#' -only ics4 -only ics8 -only icl4 -only icl8 -only icns"
/Developer/Tools/DeRez AppIcon.png $TYPES >AppIcon.r
# /Developer/Tools/Rez AppIcon.r -o AppIcon.rsrc -t rsrc -c RSED 
rm AppIcon.r

#/Developer/Tools/DeRez AppIcon.png -only icns >AppIcon_icns.r
#/Developer/Tools/Rez AppIcon_icns.r -o AppIcon.icns -useDF  

/Developer/Tools/SplitForks AppIcon.icns
rm ._AppIcon.icns

