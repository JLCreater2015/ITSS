#ifndef LOGINUSRSINGLETON_H
#define LOGINUSRSINGLETON_H
#include <QString>
// PropertyBuilderByName 用于生成类的成员变量
// 并生成set和get方法
// type 为变量类型
// access_permission 为变量的访问权限(public, priavte, protected)

//1.非指针类型的成员变量的生成
#define PropertyBuilderByName(type, name, access_permission)\
    access_permission:\
        type m_##name;\
    public:\
    inline void set##name(type v) {\
        m_##name = v;\
    }\
    inline type get##name() {\
        return m_##name;\
    }\

//指针类型的成员变量的生成
#define PointerPropertyBuilderByName(type, name, access_permission)\
    access_permission:\
        type* m_##name;\
    public:\
        inline void set##name(type* v){\
            m_##name = v;\
        }\
        inline type* get##name(){\
            return m_##name;\
        }\

class LogInUsrSingleton
{
public:
    static LogInUsrSingleton* getInstance();
    static LogInUsrSingleton* getInstance(QString str,int a);

    PropertyBuilderByName(QString, userName, private);
    PropertyBuilderByName(int, authority, private);
    //PointerPropertyBuilderByName(double, DBV, private);

private:
    LogInUsrSingleton();
    //把复制构造函数和=操作符也设为私有,防止被复制
    LogInUsrSingleton(QString n,int a);
    //把复制构造函数和=操作符也设为私有,防止被复制
    LogInUsrSingleton(const LogInUsrSingleton&);
    LogInUsrSingleton& operator=(const LogInUsrSingleton&);

public:
    class CGarbo // 它的唯一工作就是在析构函数中删除CSingleton的实例
    {
    public:
        ~CGarbo()
        {
            if (LogInUsrSingleton::m_pInstance)
                delete LogInUsrSingleton::m_pInstance;
        }
    };

private:
    static LogInUsrSingleton* m_pInstance;

private:
    QString userName;
    int authority;
};

#endif // LOGINUSRSINGLETON_H
