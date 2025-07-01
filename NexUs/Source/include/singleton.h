#ifndef SINGLETON_H
#define SINGLETON_H

#include <QMutex>
#include <memory>

template <typename T>
class Singleton
{
public:
    static T* getInstance();

private:
    Q_DISABLE_COPY(Singleton)
};

template <typename T>
T* Singleton<T>::getInstance()
{
    static QMutex mutex;
    QMutexLocker locker(&mutex);
    static T* instance = nullptr;
    if (instance == nullptr)
    {
        instance = new T();
    }
    return instance;
}

#define Q_SINGLETON_CREATE(Class)               \
private:                                        \
    friend class Singleton<Class>;              \
                                                \
public:                                         \
    static Class* getInstance()                 \
    {                                           \
        return Singleton<Class>::getInstance(); \
    }

#define Q_SINGLETON_CREATE_H(Class)                 \
private:                                            \
    static std::unique_ptr<Class> _instance;        \
    friend std::default_delete<Class>;              \
                                                    \
public:                                             \
    static Class* getInstance();

#define Q_SINGLETON_CREATE_CPP(Class)  \
    std::unique_ptr<Class> Class::_instance = nullptr; \
    Class* Class::getInstance()        \
    {                                  \
        static QMutex mutex;           \
        QMutexLocker locker(&mutex);   \
        if (_instance == nullptr)      \
        {                              \
            _instance.reset(new Class());   \
        }                              \
        return _instance.get();              \
    }

#define Q_SINGLETON_CREATE_SHARED_H(Class)                    \
private:                                                    \
    static std::shared_ptr<Class> _instance;                \
    friend std::default_delete<Class>;                      \
    template<typename... Args>                              \
    static std::shared_ptr<Class> _create(Args&&... args) { \
        struct make_shared_helper : public Class            \
        {                                                   \
            make_shared_helper(Args&&... a) : Class(std::forward<Args>(a)...){}     \
        };                                                  \
        return <make_shared_helper>(std::forward<Args>(args)...);   \
    }                                                       \
public:                                                     \
    static std::shared_ptr<Class> getInstance();

// std::shared_ptr<Class>(new Class(), [](Class* ptr) { delete ptr; });   
#define Q_SINGLETON_CREATE_SHARED_CPP(Class)          \
    std::shared_ptr<Class> Class::_instance = nullptr; \
    std::shared_ptr<Class> Class::getInstance()     \
    {                                               \
        static std::once_flag flag;                 \
        std::call_once(flag, [&]() {                \
            _instance = _create();                  \
        });                                         \
        return _instance;                           \
    } 
#endif // SINGLETON_H
