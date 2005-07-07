from pythontest import TestClass, TestRegistry
if __name__ == "__main__":
	print "Test2"
	a = TestClass()
	a.setInteger(123)
	a.setString("abc")
	TestRegistry.getInstance().add(a)
	TestClass.test2(TestRegistry.getInstance().get(0))
	print "Ende Test2"
	print