from pythontest import TestClass

class TestClassKeinFehler(TestClass):
	def getInteger(self):
		return -99999
		
if __name__ == "__main__":
	print "Test1"
	a = TestClassKeinFehler()
	print a.getInteger()
	TestClass.test2(a)
	print "Ende Test1"
	print ""