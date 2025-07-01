#ifndef STDAFX_H
#define STDAFX_H
#include <QObject>
#include <QtCore/qglobal.h>
#include <QtCore/qmetaobject.h>

#ifdef NX_BUILD_DLL
#define NX_EXPORT Q_DECL_EXPORT
#else
#define NX_EXPORT Q_DECL_IMPORT
#endif

#define Q_PROPERTY_CREATE(TYPE, M)                          \
    Q_PROPERTY(TYPE p##M MEMBER _p##M NOTIFY p##M##Changed) \
public:                                                     \
    Q_SIGNAL void p##M##Changed();                          \
    void set##M(TYPE M)                                     \
    {                                                       \
        _p##M = M;                                          \
        Q_EMIT p##M##Changed();                             \
    }                                                       \
    TYPE get##M() const                                     \
    {                                                       \
        return _p##M;                                       \
    }                                                       \
                                                            \
private:                                                    \
    TYPE _p##M;


// Q_D Q_Q普通属性快速创建
#define Q_PROPERTY_CREATE_Q_H(TYPE, M)                                  \
    Q_PROPERTY(TYPE p##M READ get##M WRITE set##M NOTIFY p##M##Changed) \
public:                                                                 \
    Q_SIGNAL void p##M##Changed();                                      \
    void set##M(TYPE M);                                                \
    TYPE get##M() const;

// Q_D Q_Q指针变量快速创建
#define Q_PRIVATE_CREATE_Q_H(TYPE, M) \
public:                               \
    void set##M(TYPE M);              \
    TYPE get##M() const;

#define Q_PROPERTY_CREATE_Q_CPP(CLASS, TYPE, M) \
    void CLASS::set##M(TYPE M)                  \
    {                                           \
        Q_D(CLASS);                             \
        d->_p##M = M;                           \
        Q_EMIT p##M##Changed();                 \
    }                                           \
    TYPE CLASS::get##M() const                  \
    {                                           \
        return d_ptr->_p##M;                    \
    }

#define Q_PRIVATE_CREATE_Q_CPP(CLASS, TYPE, M) \
    void CLASS::set##M(TYPE M)                 \
    {                                          \
        Q_D(CLASS);                            \
        d->_p##M = M;                          \
    }                                          \
    TYPE CLASS::get##M() const                 \
    {                                          \
        return d_ptr->_p##M;                   \
    }

#define Q_PUBLIC_CREATE_CHAIN_Q_H(CLASS, TYPE, M) \
public:                                       \
    CLASS& set##M(TYPE M);              \
    TYPE get##M() const;

#define Q_PUBLIC_CREATE_CHAIN_Q_CPP(CLASS, TYPE, M) \
    CLASS& CLASS::set##M(TYPE M)          \
    {                                           \
        d->_p##M = M;                             \
        return *this;                           \
    }                                           \
    TYPE CLASS::get##M() const                  \
    {                                           \
        return d->_p##M;                    \
    }

#define Q_PROPERTY_CREATE_D(TYPE, M) \
private:                             \
    TYPE _p##M;

#define Q_PRIVATE_CREATE_D(TYPE, M) \
private:                            \
    TYPE _p##M;

#define Q_PRIVATE_CREATE(TYPE, M) \
public:                           \
    void set##M(TYPE M)           \
    {                             \
        _p##M = M;                \
    }                             \
    TYPE get##M() const           \
    {                             \
        return _p##M;             \
    }                             \
                                  \
private:                          \
    TYPE _p##M;

#define Q_Q_CREATE(CLASS)                                       \
protected:                                                      \
    explicit CLASS(CLASS##Private& dd, CLASS* parent = nullptr);\
    QScopedPointer<CLASS##Private> d_ptr;                       \
                                                                \
private:                                                        \
    Q_DISABLE_COPY(CLASS)                                       \
    Q_DECLARE_PRIVATE(CLASS);

#define Q_D_CREATE(CLASS) \
protected:                \
    CLASS* q_ptr;         \
                          \
private:                  \
    Q_DECLARE_PUBLIC(CLASS);

//閺嬫矮濡囩猾璇差嚤閸? 閸忕厧顔怮T5娴ｅ海澧楅張?
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
#define Q_BEGIN_ENUM_CREATE(CLASS) \
    namespace CLASS                \
    {                              \
    Q_NAMESPACE_EXPORT(NX_EXPORT)

#define Q_END_ENUM_CREATE(CLASS) }

#define Q_ENUM_CREATE(CLASS) Q_ENUM_NS(CLASS)
#else
#define Q_BEGIN_ENUM_CREATE(CLASS)          \
    class NX_EXPORT CLASS : public QObject \
    {                                       \
        Q_OBJECT                            \
    public:

#define Q_END_ENUM_CREATE(CLASS) \
private:                         \
    Q_DISABLE_COPY(CLASS)        \
    }                            \
    ;

#define Q_ENUM_CREATE(CLASS) Q_ENUM(CLASS)
#endif

#define Q_GET_ENUM_NAME(value) \
    (QMetaEnum::fromType<std::decay_t<decltype(value)>>().valueToKey(static_cast<int>(value)))

template<typename T>
QString q_get_enum_name(T value)
{
    const QMetaEnum metaEnum = QMetaEnum::fromType<T>();
    return QString(metaEnum.valueToKey(static_cast<int>(value)));
}

#endif // STDAFX_H
