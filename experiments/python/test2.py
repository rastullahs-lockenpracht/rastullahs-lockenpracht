from pythontest import TestClass, TestRegistry
if __name__ == "__main__":
	print "Test2"
	a = TestClass()
	TestRegistry.getInstance().add(a)
	TestClass.test2(TestRegistry.getInstance().get(0))
