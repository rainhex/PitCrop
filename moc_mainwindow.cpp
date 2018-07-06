/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[18];
    char stringdata0[412];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 22), // "on_btnIncrease_clicked"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 22), // "on_btnDecrease_clicked"
QT_MOC_LITERAL(4, 58, 18), // "on_btnNext_clicked"
QT_MOC_LITERAL(5, 77, 18), // "on_btnPrev_clicked"
QT_MOC_LITERAL(6, 96, 21), // "on_btnNewCrop_clicked"
QT_MOC_LITERAL(7, 118, 24), // "on_btnDeleteCrop_clicked"
QT_MOC_LITERAL(8, 143, 20), // "on_btnAccept_clicked"
QT_MOC_LITERAL(9, 164, 20), // "on_qgvMain_destroyed"
QT_MOC_LITERAL(10, 185, 28), // "on_amiSelectFolder_triggered"
QT_MOC_LITERAL(11, 214, 23), // "on_actionExit_triggered"
QT_MOC_LITERAL(12, 238, 32), // "on_lstBoxes_itemSelectionChanged"
QT_MOC_LITERAL(13, 271, 31), // "on_lstTags_itemSelectionChanged"
QT_MOC_LITERAL(14, 303, 34), // "on_lstQuality_itemSelectionCh..."
QT_MOC_LITERAL(15, 338, 22), // "on_actionOpt_triggered"
QT_MOC_LITERAL(16, 361, 25), // "on_btnRotateRight_clicked"
QT_MOC_LITERAL(17, 387, 24) // "on_btnRotateLeft_clicked"

    },
    "MainWindow\0on_btnIncrease_clicked\0\0"
    "on_btnDecrease_clicked\0on_btnNext_clicked\0"
    "on_btnPrev_clicked\0on_btnNewCrop_clicked\0"
    "on_btnDeleteCrop_clicked\0on_btnAccept_clicked\0"
    "on_qgvMain_destroyed\0on_amiSelectFolder_triggered\0"
    "on_actionExit_triggered\0"
    "on_lstBoxes_itemSelectionChanged\0"
    "on_lstTags_itemSelectionChanged\0"
    "on_lstQuality_itemSelectionChanged\0"
    "on_actionOpt_triggered\0on_btnRotateRight_clicked\0"
    "on_btnRotateLeft_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   94,    2, 0x0a /* Public */,
       3,    0,   95,    2, 0x0a /* Public */,
       4,    0,   96,    2, 0x0a /* Public */,
       5,    0,   97,    2, 0x0a /* Public */,
       6,    0,   98,    2, 0x0a /* Public */,
       7,    0,   99,    2, 0x0a /* Public */,
       8,    0,  100,    2, 0x0a /* Public */,
       9,    0,  101,    2, 0x08 /* Private */,
      10,    0,  102,    2, 0x08 /* Private */,
      11,    0,  103,    2, 0x08 /* Private */,
      12,    0,  104,    2, 0x08 /* Private */,
      13,    0,  105,    2, 0x08 /* Private */,
      14,    0,  106,    2, 0x08 /* Private */,
      15,    0,  107,    2, 0x08 /* Private */,
      16,    0,  108,    2, 0x08 /* Private */,
      17,    0,  109,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_btnIncrease_clicked(); break;
        case 1: _t->on_btnDecrease_clicked(); break;
        case 2: _t->on_btnNext_clicked(); break;
        case 3: _t->on_btnPrev_clicked(); break;
        case 4: _t->on_btnNewCrop_clicked(); break;
        case 5: _t->on_btnDeleteCrop_clicked(); break;
        case 6: _t->on_btnAccept_clicked(); break;
        case 7: _t->on_qgvMain_destroyed(); break;
        case 8: _t->on_amiSelectFolder_triggered(); break;
        case 9: _t->on_actionExit_triggered(); break;
        case 10: _t->on_lstBoxes_itemSelectionChanged(); break;
        case 11: _t->on_lstTags_itemSelectionChanged(); break;
        case 12: _t->on_lstQuality_itemSelectionChanged(); break;
        case 13: _t->on_actionOpt_triggered(); break;
        case 14: _t->on_btnRotateRight_clicked(); break;
        case 15: _t->on_btnRotateLeft_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
