NOTE ON FLIP (>= 3.1.1) DOWNLOAD EXAMPLES
=========================================

FLIP can be used to download any example "canxx_ringtones_n.hex".
Please, refer to its help on_line.


NOTE ON BATCHISP (FLIP >= 3.1.1) DOWNLOAD EXAMPLES
==================================================

BATCHISP can also be used to download any example "canxx_ringtones_n.hex".

The path of the directory of "batchisp.exe" must be included
in the user variable "Path" of the "environment variables".

So, the instruction: LOADBUFFER "canxx_ringtones_n.hex" can
work without to include the path, the path used will be the path
of where the "batchisp" command will be executed.

Customize your own "canxx_ringtones_n.bat" to be able to call
"batchisp.exe" from the required folder (i.e. ..\download_examples).