from pythontest import TestClass

class TestClassFehler(TestClass):
	def getInteger(self):
		blubb()

if __name__ == "__main__":
	print "* TestFehler - Beinhaltet einen Fehler"
	a = TestClassFehler()
	print a.getInteger()
	TestClass.test2(a)
	print "Ende TestFehler"
	print ""