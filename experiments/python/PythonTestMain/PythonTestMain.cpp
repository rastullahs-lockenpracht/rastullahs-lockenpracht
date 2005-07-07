// PythonTestMain.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include <Python.h>
#include "TestRegistry.h"

int main(int argc, char** argv)
{
	Py_Initialize();
	PyRun_SimpleString("execfile(\"test.py\")");
	PyRun_SimpleString("execfile(\"test2.py\")");
	PyRun_SimpleString("execfile(\"test3.py\")");
	PyRun_SimpleString("execfile(\"testfehler.py\")");
	PyRun_SimpleString("execfile(\"test4.py\")");
	Py_Finalize();

	return 0;
}
