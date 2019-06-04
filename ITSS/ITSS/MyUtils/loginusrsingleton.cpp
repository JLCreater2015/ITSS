#include "loginusrsingleton.h"

LogInUsrSingleton::LogInUsrSingleton(){

}

LogInUsrSingleton::LogInUsrSingleton(QString n, int a){
    userName = n;
    authority = a;
}


LogInUsrSingleton::LogInUsrSingleton(const LogInUsrSingleton& l){
    userName = l.userName;
    authority = l.authority;
}


LogInUsrSingleton& LogInUsrSingleton::operator=(const LogInUsrSingleton& l){
    LogInUsrSingleton lo(l.userName,l.authority);
    return lo;
}


//在此处初始化
LogInUsrSingleton* LogInUsrSingleton::getInstance(){
    if(m_pInstance == NULL)
        m_pInstance = new LogInUsrSingleton();
    return m_pInstance;
}

LogInUsrSingleton* LogInUsrSingleton::getInstance(QString str,int a)
{
    if(m_pInstance == NULL)
        m_pInstance = new LogInUsrSingleton(str,a);
    return m_pInstance;
}

LogInUsrSingleton* LogInUsrSingleton::m_pInstance = new LogInUsrSingleton("admin",0);
LogInUsrSingleton::CGarbo garbo;   //这个很重要
