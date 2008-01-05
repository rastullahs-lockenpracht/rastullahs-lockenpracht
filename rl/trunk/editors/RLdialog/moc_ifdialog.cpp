/****************************************************************************
** Meta object code from reading C++ file 'ifdialog.h'
**
** Created: Sat Jan 5 16:55:39 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ifdialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ifdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_ifDialog[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ifDialog[] = {
    "ifDialog\0\0changeType(int)\0"
};

const QMetaObject ifDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ifDialog,
      qt_meta_data_ifDialog, 0 }
};

const QMetaObject *ifDialog::metaObject() const
{
    return &staticMetaObject;
}

void *ifDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ifDialog))
	return static_cast<void*>(const_cast< ifDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int ifDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: changeType((*reinterpret_cast< int(*)>(_a[1]))); break;
        }
        _id -= 1;
    }
    return _id;
}
static const uint qt_meta_data_charakter[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x08,
      28,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_charakter[] = {
    "charakter\0\0setCategory(int)\0setGroup(int)\0"
};

const QMetaObject charakter::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_charakter,
      qt_meta_data_charakter, 0 }
};

const QMetaObject *charakter::metaObject() const
{
    return &staticMetaObject;
}

void *charakter::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_charakter))
	return static_cast<void*>(const_cast< charakter*>(this));
    return QWidget::qt_metacast(_clname);
}

int charakter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setCategory((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: setGroup((*reinterpret_cast< int(*)>(_a[1]))); break;
        }
        _id -= 2;
    }
    return _id;
}
