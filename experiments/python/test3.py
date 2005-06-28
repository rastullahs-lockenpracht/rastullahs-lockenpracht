from pythontest import TestClass
from pythontest import TestRegistry
if __name__ == "__main__":
	print "Test3"
	TestClass.test2(TestRegistry.getInstance().get(0))
