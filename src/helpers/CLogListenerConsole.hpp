#ifndef _LOGLISTENERCONSOLE_HPP
#define _LOGLISTENERCONSOLE_HPP

#include "CLogListener.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <boost/thread/mutex.hpp>

// Log listener de tipo escritura en un archivo concreto
class CLogListenerConsole : public CLogListener
{
  public:
	  CLogListenerConsole();
	  virtual ~CLogListenerConsole();

	  void Log(std::string log);

  private:
    boost::mutex _mutex;
};

#endif