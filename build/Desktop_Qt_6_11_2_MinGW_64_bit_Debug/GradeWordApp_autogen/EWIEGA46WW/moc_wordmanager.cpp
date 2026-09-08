/****************************************************************************
** Meta object code from reading C++ file 'wordmanager.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../wordmanager.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wordmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN11WordManagerE_t {};
} // unnamed namespace

template <> constexpr inline auto WordManager::qt_create_metaobjectdata<qt_meta_tag_ZN11WordManagerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "WordManager",
        "gradeChanged",
        "",
        "grade",
        "gradesChanged",
        "grades",
        "currentGrade",
        "setCurrentGrade",
        "addGrade",
        "removeGrade",
        "addWordToDict",
        "word",
        "phonetic",
        "definition",
        "example",
        "removeWordFromDict",
        "findWordInDict",
        "QVariantMap",
        "key",
        "allWordsOfDict",
        "QVariantList",
        "addWordToGrade",
        "removeWordFromGrade",
        "wordsOfGrade",
        "QList<QVariantMap>",
        "wordInGrade",
        "getSpellList",
        "limit",
        "getReadList",
        "onSpellResult",
        "correct",
        "onReadResult"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'gradeChanged'
        QtMocHelpers::SignalData<void(const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Signal 'gradesChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'grades'
        QtMocHelpers::MethodData<QStringList() const>(5, 2, QMC::AccessPublic, QMetaType::QStringList),
        // Method 'currentGrade'
        QtMocHelpers::MethodData<QString() const>(6, 2, QMC::AccessPublic, QMetaType::QString),
        // Method 'setCurrentGrade'
        QtMocHelpers::MethodData<void(const QString &)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Method 'addGrade'
        QtMocHelpers::MethodData<bool(const QString &)>(8, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 3 },
        }}),
        // Method 'removeGrade'
        QtMocHelpers::MethodData<bool(const QString &)>(9, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 3 },
        }}),
        // Method 'addWordToDict'
        QtMocHelpers::MethodData<bool(const QString &, const QString &, const QString &, const QString &)>(10, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 11 }, { QMetaType::QString, 12 }, { QMetaType::QString, 13 }, { QMetaType::QString, 14 },
        }}),
        // Method 'addWordToDict'
        QtMocHelpers::MethodData<bool(const QString &, const QString &, const QString &)>(10, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Bool, {{
            { QMetaType::QString, 11 }, { QMetaType::QString, 12 }, { QMetaType::QString, 13 },
        }}),
        // Method 'removeWordFromDict'
        QtMocHelpers::MethodData<bool(const QString &)>(15, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 11 },
        }}),
        // Method 'findWordInDict'
        QtMocHelpers::MethodData<QVariantMap(const QString &)>(16, 2, QMC::AccessPublic, 0x80000000 | 17, {{
            { QMetaType::QString, 18 },
        }}),
        // Method 'allWordsOfDict'
        QtMocHelpers::MethodData<QVariantList() const>(19, 2, QMC::AccessPublic, 0x80000000 | 20),
        // Method 'addWordToGrade'
        QtMocHelpers::MethodData<bool(const QString &, const QString &)>(21, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 3 }, { QMetaType::QString, 11 },
        }}),
        // Method 'removeWordFromGrade'
        QtMocHelpers::MethodData<bool(const QString &, const QString &)>(22, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 3 }, { QMetaType::QString, 11 },
        }}),
        // Method 'wordsOfGrade'
        QtMocHelpers::MethodData<QVector<QVariantMap>(const QString &) const>(23, 2, QMC::AccessPublic, 0x80000000 | 24, {{
            { QMetaType::QString, 3 },
        }}),
        // Method 'wordInGrade'
        QtMocHelpers::MethodData<bool(const QString &, const QString &) const>(25, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 3 }, { QMetaType::QString, 11 },
        }}),
        // Method 'getSpellList'
        QtMocHelpers::MethodData<QVector<QVariantMap>(const QString &, int)>(26, 2, QMC::AccessPublic, 0x80000000 | 24, {{
            { QMetaType::QString, 3 }, { QMetaType::Int, 27 },
        }}),
        // Method 'getSpellList'
        QtMocHelpers::MethodData<QVector<QVariantMap>(const QString &)>(26, 2, QMC::AccessPublic | QMC::MethodCloned, 0x80000000 | 24, {{
            { QMetaType::QString, 3 },
        }}),
        // Method 'getReadList'
        QtMocHelpers::MethodData<QVector<QVariantMap>(const QString &, int)>(28, 2, QMC::AccessPublic, 0x80000000 | 24, {{
            { QMetaType::QString, 3 }, { QMetaType::Int, 27 },
        }}),
        // Method 'getReadList'
        QtMocHelpers::MethodData<QVector<QVariantMap>(const QString &)>(28, 2, QMC::AccessPublic | QMC::MethodCloned, 0x80000000 | 24, {{
            { QMetaType::QString, 3 },
        }}),
        // Method 'onSpellResult'
        QtMocHelpers::MethodData<void(const QString &, const QString &, bool)>(29, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::QString, 11 }, { QMetaType::Bool, 30 },
        }}),
        // Method 'onReadResult'
        QtMocHelpers::MethodData<void(const QString &, const QString &, bool)>(31, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::QString, 11 }, { QMetaType::Bool, 30 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'currentGrade'
        QtMocHelpers::PropertyData<QString>(6, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'grades'
        QtMocHelpers::PropertyData<QStringList>(5, QMetaType::QStringList, QMC::DefaultPropertyFlags, 1),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<WordManager, qt_meta_tag_ZN11WordManagerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject WordManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11WordManagerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11WordManagerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN11WordManagerE_t>.metaTypes,
    nullptr
} };

void WordManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<WordManager *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->gradeChanged((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->gradesChanged(); break;
        case 2: { QStringList _r = _t->grades();
            if (_a[0]) *reinterpret_cast<QStringList*>(_a[0]) = std::move(_r); }  break;
        case 3: { QString _r = _t->currentGrade();
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 4: _t->setCurrentGrade((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: { bool _r = _t->addGrade((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 6: { bool _r = _t->removeGrade((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 7: { bool _r = _t->addWordToDict((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[4])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 8: { bool _r = _t->addWordToDict((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 9: { bool _r = _t->removeWordFromDict((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 10: { QVariantMap _r = _t->findWordInDict((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QVariantMap*>(_a[0]) = std::move(_r); }  break;
        case 11: { QVariantList _r = _t->allWordsOfDict();
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 12: { bool _r = _t->addWordToGrade((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 13: { bool _r = _t->removeWordFromGrade((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 14: { QList<QVariantMap> _r = _t->wordsOfGrade((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QList<QVariantMap>*>(_a[0]) = std::move(_r); }  break;
        case 15: { bool _r = _t->wordInGrade((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 16: { QList<QVariantMap> _r = _t->getSpellList((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast<QList<QVariantMap>*>(_a[0]) = std::move(_r); }  break;
        case 17: { QList<QVariantMap> _r = _t->getSpellList((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QList<QVariantMap>*>(_a[0]) = std::move(_r); }  break;
        case 18: { QList<QVariantMap> _r = _t->getReadList((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast<QList<QVariantMap>*>(_a[0]) = std::move(_r); }  break;
        case 19: { QList<QVariantMap> _r = _t->getReadList((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QList<QVariantMap>*>(_a[0]) = std::move(_r); }  break;
        case 20: _t->onSpellResult((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[3]))); break;
        case 21: _t->onReadResult((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[3]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (WordManager::*)(const QString & )>(_a, &WordManager::gradeChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (WordManager::*)()>(_a, &WordManager::gradesChanged, 1))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->currentGrade(); break;
        case 1: *reinterpret_cast<QStringList*>(_v) = _t->grades(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setCurrentGrade(*reinterpret_cast<QString*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *WordManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WordManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11WordManagerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int WordManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 22)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 22;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void WordManager::gradeChanged(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void WordManager::gradesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
