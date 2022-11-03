#ifndef _LOGLISTENER_HPP
#define _LOGLISTENER_HPP

#include <string>

// Clase base para poder implementar distintos listeners de log
class CLogListener
{
  public:
	  CLogListener(){};
	  virtual ~CLogListener(){};

	  // Envía un log a donde sea preciso
	  virtual void Log(std::string log) = 0;
};

#endif