from pythontest import TestClass, TestRegistry

if __name__ == "__main__":
	print "* Test2"
	print " - Ein Objekt erzeugen"
	print " - C++- und Python-Element anzeigen"
	print
	a = TestClass()
	a.setInteger(123)
	a.setString("Ich bin aus Python")
	TestRegistry.getInstance().add(a)
	TestClass.test2(TestRegistry.getInstance().get(0))
	TestClass.test2(TestRegistry.getInstance().get(1))
	print "Ende Test2"
	print