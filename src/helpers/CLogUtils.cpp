#include "CLogUtils.hpp"

#include "CError.h"
#include <sstream>
#include <thread>
#include <iomanip>
stemy::helpers::CLogUtils::CLogUtils()
{
}


stemy::helpers::CLogUtils::~CLogUtils()
{
}

std::string stemy::helpers::CLogUtils::buildCommunicationMessage(const std::string& message, const std::string& ipAddress, const bool& displayThreadId, const uint64_t& connectionId)
{
	std::ostringstream os;

	if (!ipAddress.empty())
		os << " @ " << ipAddress;

	if (connectionId > 0) {
		std::ostringstream osstr;
		osstr << std::setw(4) << std::setfill('0') << connectionId;
		os << " # " << osstr.str();
	}

   if (displayThreadId)
	   os << " (" << std::this_thread::get_id() << ")";
     //*this << "(" <<  std::this_thread::get_id().hash() << ") ";

   os << ": " << message;

   return os.str();
}
