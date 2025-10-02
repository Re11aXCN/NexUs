#ifndef SINGLETON_H
#define SINGLETON_H

#include <memory>
#define DISABLE_COPY(Class) \
    Class(const Class &) = delete;\
    Class &operator=(const Class &) = delete;

#define DISABLE_COPY_MOVE(Class) \
    DISABLE_COPY(Class) \
    Class(Class &&) = delete; \
    Class &operator=(Class &&) = delete;

template<typename T>
class Singleton {
private:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;

public:
    static T& getInstance() {
        static T instance;
        return instance;
    }

protected:
    Singleton() = default;
    ~Singleton() = default;
};

#define SINGLETON_CREATE(Class)               \
private:                                        \
    friend class Singleton<Class>;              \
                                                \
public:                                         \
    static Class& getInstance()                 \
    {                                           \
        return Singleton<Class>::getInstance(); \
    }

// You must make the destructor private!!!
#define SINGLETON_CREATE_H(Class)                    \
private:                                                    \
    static std::unique_ptr<Class> _instance;                \
    friend std::default_delete<Class>;                      \
    template<typename... Args>                              \
    static std::unique_ptr<Class> _impl_create(Args&&... args) { \
        struct make_unique_helper_##Class : public Class            \
        {                                                   \
            make_unique_helper_##Class(Args&&... a) : Class(std::forward<Args>(a)...){}     \
        };                                                  \
        return std::make_unique<make_unique_helper_##Class>(std::forward<Args>(args)...);   \
    }                                                       \
public:                                                     \
    DISABLE_COPY_MOVE(Class)                                \
    static Class* getInstance();

#define SINGLETON_CREATE_CPP(Class)          \
    std::unique_ptr<Class> Class::_instance = nullptr; \
    Class* Class::getInstance()     \
    {                                               \
        static std::once_flag flag;                 \
        std::call_once(flag, [&]() {                \
            _instance = _impl_create();                  \
        });                                         \
        return _instance.get();                     \
    } 

#define SINGLETON_CREATE_SHARED_H(Class)                  \
private:                                                    \
    static std::shared_ptr<Class> _instance;                \
    friend std::default_delete<Class>;                      \
    template<typename... Args>                              \
    static std::shared_ptr<Class> _impl_create(Args&&... args) { \
        struct make_shared_helper_##Class : public Class            \
        {                                                   \
            make_shared_helper_##Class(Args&&... a) : Class(std::forward<Args>(a)...){}     \
        };                                                  \
        return std::make_shared<make_shared_helper_##Class>(std::forward<Args>(args)...);   \
    }                                                       \
public:                                                     \
    DISABLE_COPY_MOVE(Class)                                \
    static std::shared_ptr<Class> getInstance();

// std::shared_ptr<Class>(new Class(), [](Class* ptr) { delete ptr; });   
#define SINGLETON_CREATE_SHARED_CPP(Class)          \
    std::shared_ptr<Class> Class::_instance = nullptr; \
    std::shared_ptr<Class> Class::getInstance()     \
    {                                               \
        static std::once_flag flag;                 \
        std::call_once(flag, [&]() {                \
            _instance = _impl_create();                  \
        });                                         \
        return _instance;                           \
    } 
#endif // SINGLETON_H
