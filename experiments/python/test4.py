from pythontest import TestClass
from pythontest import TestRegistry

if __name__ == "__main__":
	print "* Test4"
	print "  - C++- und Python-Element anzeigen"
	print
	TestClass.test2(TestRegistry.getInstance().get(0))
	TestClass.test2(TestRegistry.getInstance().get(1))
	print "Ende Test 4"
	print