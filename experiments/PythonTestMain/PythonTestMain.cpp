// PythonTestMain.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include <Python.h>

int main(int argc, char** argv)
{
	Py_Initialize();
	FILE* fp = fopen("test.py", "r");
//	PyRun_SimpleFile(fp, "test.py");
	PyRun_SimpleString("from pythontest import TestClass\nif __name__ == \"__main__\":\n\tprint \"blah\"\n\ta = TestClass()\n\tTestClass.test2(a)\n");
	Py_Finalize();

	return 0;
}
