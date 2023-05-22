#include "Errors.h"

Error::Error(QString message) : error_message(message)
{

}

QString Error::get_message()
{
    return error_message;
}

UIError::UIError(QString message) : Error(message)
{

}
