
NOTE ON BATCHISP TESTS
======================

The path of the directory of "batchisp.exe" must be included
in the user variable "Path" of the "environment variables".

So, the instruction: INCLUDE "canxx_tst-n_uuuuuuuuuu.txt"
and similar can work without include the path, the path used
will be the path of where the "batchisp" command will be executed.

Customize your own "canxx.bat" to be able to call "batchisp.exe"
from the required folder (i.e. ..\canxx_test).

All results are automatically logged in "canxx_can_logfile.txt" file.