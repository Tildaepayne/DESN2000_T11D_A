@ECHO OFF
ECHO Downloading demo.hex... Please wait.
"C:\Program Files (x86)\Keil\UV4\UV4.exe" -f demo.uvproj -t"QVGA Base Board"
DEL demo.hex.plg
DEL "demo_QVGA Base Board.dep"
