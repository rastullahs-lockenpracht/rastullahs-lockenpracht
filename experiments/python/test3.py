from pythontest import TestClass
from pythontest import TestRegistry

if __name__ == "__main__":
	print
	print "* Test3"
	print "  - C++-Element anzeigen"
	print
	TestClass.test2(TestRegistry.getInstance().get(0))
	print "Ende Test 3"
	print