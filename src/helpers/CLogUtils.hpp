#pragma once

#include <cstdint>
#include <string>

namespace stemy {
	namespace helpers {
		class CLogUtils
		{
		public:
			CLogUtils();
			~CLogUtils();

			static std::string buildCommunicationMessage(const std::string& message, const std::string& ipAddress = std::string(), const bool& displayThreadId = false, const uint64_t& connectionId = 0);
		};
	}
}

