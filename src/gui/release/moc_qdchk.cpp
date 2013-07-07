/****************************************************************************
** Meta object code from reading C++ file 'qdchk.h'
**
** Created: Thu Mar 19 19:40:30 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../qdchk.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qdchk.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_qdchk[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
       7,    6,    6,    6, 0x08,
      26,    6,    6,    6, 0x08,
      48,   46,    6,    6, 0x08,
      85,   46,    6,    6, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_qdchk[] = {
    "qdchk\0\0on_start_clicked()\0on_useDNS_toggled()\0"
    "v\0on_authority_automatic_toggled(bool)\0"
    "on_authority_manual_toggled(bool)\0"
};

const QMetaObject qdchk::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_qdchk,
      qt_meta_data_qdchk, 0 }
};

const QMetaObject *qdchk::metaObject() const
{
    return &staticMetaObject;
}

void *qdchk::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_qdchk))
        return static_cast<void*>(const_cast< qdchk*>(this));
    return QDialog::qt_metacast(_clname);
}

int qdchk::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_start_clicked(); break;
        case 1: on_useDNS_toggled(); break;
        case 2: on_authority_automatic_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: on_authority_manual_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
