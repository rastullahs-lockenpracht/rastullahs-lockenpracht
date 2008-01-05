/****************************************************************************
** Meta object code from reading C++ file 'helper.h'
**
** Created: Sat Jan 5 17:53:26 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "helper.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'helper.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_optionMode[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_optionMode[] = {
    "optionMode\0\0openIfDialog()\0"
};

const QMetaObject optionMode::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_optionMode,
      qt_meta_data_optionMode, 0 }
};

const QMetaObject *optionMode::metaObject() const
{
    return &staticMetaObject;
}

void *optionMode::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_optionMode))
	return static_cast<void*>(const_cast< optionMode*>(this));
    return QWidget::qt_metacast(_clname);
}

int optionMode::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: openIfDialog(); break;
        }
        _id -= 1;
    }
    return _id;
}
