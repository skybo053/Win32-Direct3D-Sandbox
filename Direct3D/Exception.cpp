#include "Exception.h"

#include <sstream>


Exception::Exception(int pLineNumber, std::string pFileName, std::string pMessage) : 
  lineNumber(pLineNumber), fileName(pFileName), message(pMessage)
{

}


Exception::Exception(std::string pMessage) : 
  lineNumber(-1), fileName(""), message(pMessage)
{

}


const char* Exception::what() const
{
  if(lineNumber > -1)
  {
    std::stringstream ss;

    ss << "[Error line number]: " << lineNumber << std::endl << "[Error file name]: " << fileName << std::endl << "[Error message]: " << message << std::endl;

    message = ss.str();
  }
  
  return message.c_str();
}
