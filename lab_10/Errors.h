#ifndef ERRORS_H
#define ERRORS_H

#include <exception>
#include <QString>


class Error : public std::exception
{
private:
    QString error_message;
public:
    Error(QString message);
    QString get_message();
};

class UIError : public Error
{
public:
    UIError(QString message);
};

#endif 
