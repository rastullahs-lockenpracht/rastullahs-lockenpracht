# This file was created automatically by SWIG.
# Don't modify this file, modify the SWIG interface instead.
# This file is compatible with both classic and new-style classes.

import _pythontest

def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "this"):
        if isinstance(value, class_type):
            self.__dict__[name] = value.this
            if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
            del value.thisown
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static) or hasattr(self,name) or (name == "thisown"):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError,name

import types
try:
    _object = types.ObjectType
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0
del types


try:
    from weakref import proxy as weakref_proxy
except:
    weakref_proxy = lambda x: x


class TestClass(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, TestClass, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, TestClass, name)
    def __repr__(self):
        return "<%s.%s; proxy of C++ TestClass instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def getInteger(*args): return _pythontest.TestClass_getInteger(*args)
    def setInteger(*args): return _pythontest.TestClass_setInteger(*args)
    def getDouble(*args): return _pythontest.TestClass_getDouble(*args)
    def setDouble(*args): return _pythontest.TestClass_setDouble(*args)
    def getPointer(*args): return _pythontest.TestClass_getPointer(*args)
    def setPointer(*args): return _pythontest.TestClass_setPointer(*args)
    def getString(*args): return _pythontest.TestClass_getString(*args)
    def setString(*args): return _pythontest.TestClass_setString(*args)
    def test1(*args): return _pythontest.TestClass_test1(*args)
    __swig_getmethods__["test2"] = lambda x: _pythontest.TestClass_test2
    if _newclass:test2 = staticmethod(_pythontest.TestClass_test2)
    def __init__(self, *args):
        _swig_setattr(self, TestClass, 'this', _pythontest.new_TestClass(*args))
        _swig_setattr(self, TestClass, 'thisown', 1)
    def __del__(self, destroy=_pythontest.delete_TestClass):
        try:
            if self.thisown: destroy(self)
        except: pass


class TestClassPtr(TestClass):
    def __init__(self, this):
        _swig_setattr(self, TestClass, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, TestClass, 'thisown', 0)
        _swig_setattr(self, TestClass,self.__class__,TestClass)
_pythontest.TestClass_swigregister(TestClassPtr)

TestClass_test2 = _pythontest.TestClass_test2


