from pythontest import TestClass

class TestClassFehler(TestClass):
	def getInteger(self):
		blubb()

if __name__ == "__main__":
	print "* TestFehler - Beinhaltet einen Fehler"
	abc = TestClassFehler()
	print abc.getInteger()
	TestClass.test2(abc)
	print "Ende TestFehler"
	print ""