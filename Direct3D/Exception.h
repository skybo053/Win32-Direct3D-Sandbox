#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>


class Exception : public std::exception
{
public:
  Exception(int pLineNumber, std::string pFileName, std::string pMessage);
  Exception(std::string pMessage);

  const char* what() const override;

private:
  int         lineNumber;
  std::string fileName;
  mutable     std::string message;
};


#endif

