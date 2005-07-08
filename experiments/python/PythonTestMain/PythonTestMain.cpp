// PythonTestMain.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include <Python.h>
#include "TestClass.h"
#include "TestRegistry.h"

int main(int argc, char** argv)
{
	Py_Initialize();
	TestClass* obj = new TestClass();
	obj->setDouble(3.1415926);
	TestRegistry::getInstance()->add(obj);
	PyRun_SimpleString("execfile(\"test.py\")");
	//PyRun_SimpleString("execfile(\"testfehler.py\")"); // Hier geht es
	PyRun_SimpleString("execfile(\"test2.py\")");
	PyRun_SimpleString("execfile(\"testfehler.py\")"); // Hier nicht
	PyRun_SimpleString("execfile(\"test3.py\")");
	//PyRun_SimpleString("execfile(\"testfehler.py\")"); // Hier auch nicht
	PyRun_SimpleString("execfile(\"test4.py\")");
	PyRun_SimpleString("execfile(\"testfehler.py\")"); // Hier geht es
	Py_Finalize();

	return 0;
}
