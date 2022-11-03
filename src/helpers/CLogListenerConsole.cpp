#include "CLogListenerConsole.hpp"

using namespace std;

CLogListenerConsole::CLogListenerConsole(void) {}

CLogListenerConsole::~CLogListenerConsole() {}

void CLogListenerConsole::Log(string log)
{
	_mutex.lock();
    cout << log;
  _mutex.unlock();
}