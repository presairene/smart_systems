#ifndef _LOGLISTENERFILE_HPP
#define _LOGLISTENERFILE_HPP

#include "CLogListener.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <boost/thread/mutex.hpp>

// Log listener de tipo escritura en un archivo concreto
class CLogListenerFile : public CLogListener
{
  public:
	  CLogListenerFile(std::string filename, std::string path, bool append, bool refresh);
	  virtual ~CLogListenerFile();

	  void Log(std::string log);

  private:
    std::ofstream _file;		 // Stream al archivo de salida
    std::string   _filename; // Nombre del archivo
    std::string   _path;		 // Path del archivo
    bool          _append;	 // Variable que indica si se suma el log a lo que había antes en el archivo
    bool          _refresh;	 // Variable que indica si se refresca el archivo cada vez que se realiza un Log
    boost::mutex  _mutex;

    // Devuelve el nombre completo del archivo, incluido el path
    std::string getFullPathName() { return _path + _filename; }
};

#endif