#pragma once

#include <cstdint>
#include <string>
#include "boost/date_time/posix_time/posix_time.hpp"

namespace helpers {
	class CTimeUtils
	{
	public:

		enum weekday : int8_t
		{
			default_weekday = 0,
			monday = 1,
			tuesday = 2,
			wednesday = 3,
			thursday = 4,
			friday = 5,
			saturday = 6,
			sunday = 7
		};

		CTimeUtils();
		~CTimeUtils();

		static uint64_t seconds_from_epoch(const boost::posix_time::ptime& date, const bool& isAlreadyConvertedToUtc = false);
		static boost::posix_time::ptime from_time_t(const time_t& date);
		static boost::gregorian::date time_t_to_gregorian(const time_t& date);
		static boost::posix_time::ptime parseDate(const std::string& s);
		static boost::posix_time::ptime localize_ptime(const boost::posix_time::ptime& utc_ptime);
		static boost::posix_time::time_duration getLocalAndUniversalTimeDifferential(const boost::posix_time::ptime& uni_time); // UNIVERSAL TIME AS INPUT
		static boost::posix_time::time_duration getLocalAndUniversalTimeDifferential();
		static uint8_t getLocalAndUniversalHourDifferential();
		static std::string getLocalTimeStringFromUnix(const uint64_t& ts);

		/**
		*	Return seconds since same day at 00:00.
		*
		*	[in]	Timestamp ts.
		*
		*	@return seconds in uint32_t.
		*/
		static uint64_t getSecondsSinceMidnight(const uint64_t & ts);

		/**
		* Returns UTC time in ISO 8601 with separating hyphens and colons.
		*
		* Used, among others by OCPP.
		*
		* @return String in the format 2020-09-30T12:14:05Z
		*/
		static std::string getExtendedIsoUtcTime(void);

		/**
		* Returns in local time the format selected. Format example:
		*	%Y/%m/%d -> 2021/01/01
		*
		* @return String in the selected format
		*/
		static std::string getCustomStringFormatFromTimestamp(const uint64_t & ts, const std::string & format);
	};

}