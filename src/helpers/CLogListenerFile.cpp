#include "CLogListenerFile.hpp"

using namespace std;

CLogListenerFile::CLogListenerFile(string filename, string path, bool append, bool refresh)
{
	_filename = filename;
	_path     = path;
	_refresh  = refresh;
	_append   = append;

	// Si no queremos añadirlo a lo anterior, se borra el archivo
	if (!_append) {
		_file.open(getFullPathName().c_str(), ofstream::out);
    _file.clear();
		_file.close();
	}
}

CLogListenerFile::~CLogListenerFile()
{
	if (_file.is_open())
		_file.close();
}

void CLogListenerFile::Log(string log)
{
	_mutex.lock();
    if (!_file.is_open())
		  _file.open(getFullPathName().c_str(), ofstream::out | ofstream::app);

	  _file << log;

	  if (_refresh)
		  _file.close();
  _mutex.unlock();
}