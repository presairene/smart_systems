#include "CTimeUtils.hpp"

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/local_time_adjustor.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>
#include <boost/date_time.hpp>


helpers::CTimeUtils::CTimeUtils()
{
}

helpers::CTimeUtils::~CTimeUtils()
{
}

boost::posix_time::ptime helpers::CTimeUtils::parseDate(const std::string& s){
	std::istringstream is(s);

	std::locale formats[] = {
		std::locale(is.getloc(), new boost::local_time::local_time_input_facet("%Y-%m-%d %H:%M:%S %ZP")),
		std::locale(is.getloc(), new boost::local_time::local_time_input_facet("%d-%m-%Y %H:%M:%S %ZP"))
	};

	boost::posix_time::ptime pt;
	boost::local_time::local_date_time zonetime(boost::local_time::not_a_date_time);
	for (size_t i = 0; i < sizeof(formats); ++i){
		std::istringstream is_aux(s);
		is_aux.imbue(formats[i]);
		if (is_aux >> zonetime) {
			pt = zonetime.utc_time();
			break;
		}
	}

	return pt;
}

uint64_t helpers::CTimeUtils::seconds_from_epoch(const boost::posix_time::ptime& date, const bool& isAlreadyConvertedToUtc /*= false*/)
{
	uint64_t seconds = boost::posix_time::to_time_t(date);
	if (!isAlreadyConvertedToUtc) seconds -= getLocalAndUniversalTimeDifferential(date).total_seconds();
	return seconds;
}


boost::posix_time::ptime helpers::CTimeUtils::from_time_t(const time_t& date){
	return boost::posix_time::from_time_t(date);
}

boost::gregorian::date helpers::CTimeUtils::time_t_to_gregorian(const time_t& date) {
	boost::posix_time::ptime ptime = from_time_t(date);
	return ptime.date();
}

boost::posix_time::ptime helpers::CTimeUtils::localize_ptime(const boost::posix_time::ptime& utc_ptime) {
	
	return utc_ptime + getLocalAndUniversalTimeDifferential();
}

uint8_t helpers::CTimeUtils::getLocalAndUniversalHourDifferential() {
	return static_cast<uint8_t>(int(getLocalAndUniversalTimeDifferential().total_seconds()) / 3600);
}

std::string helpers::CTimeUtils::getLocalTimeStringFromUnix(const uint64_t& unix)
{
	time_t ts = unix;
	char stringLocalDate[20];
	strftime(stringLocalDate, 20, "%Y-%m-%d %H:%M:%S", localtime(&ts));
	return stringLocalDate;
}

boost::posix_time::time_duration helpers::CTimeUtils::getLocalAndUniversalTimeDifferential(const boost::posix_time::ptime& uni_time) {
	boost::posix_time::ptime loc_time = boost::date_time::c_local_adjustor<boost::posix_time::ptime>::utc_to_local(uni_time);
	boost::posix_time::time_duration diff_time = loc_time - uni_time;
	return diff_time;
}

boost::posix_time::time_duration helpers::CTimeUtils::getLocalAndUniversalTimeDifferential() {
	boost::posix_time::ptime uni_time = boost::posix_time::second_clock::universal_time();
	boost::posix_time::ptime loc_time = boost::date_time::c_local_adjustor<boost::posix_time::ptime>::utc_to_local(uni_time);
	boost::posix_time::time_duration diff_time = loc_time - uni_time;
	return diff_time;
}

uint64_t helpers::CTimeUtils::getSecondsSinceMidnight(const uint64_t & ts)
{
	std::time_t time = ts; // In UTC
	boost::posix_time::ptime time_pt = from_time_t(ts);

	return helpers::CTimeUtils::localize_ptime(time_pt).time_of_day().total_seconds();
}

std::string helpers::CTimeUtils::getExtendedIsoUtcTime(void) {
	boost::posix_time::ptime time = boost::posix_time::second_clock::universal_time();
	return boost::posix_time::to_iso_extended_string(time) + "Z";
}

std::string helpers::CTimeUtils::getCustomStringFormatFromTimestamp(const uint64_t & ts, const std::string & format) {
	// Convert from uint64_t to ptime
	std::time_t time = ts; // In UTC
	boost::posix_time::ptime time_pt = from_time_t(ts);
	boost::posix_time::ptime time_pt_local = helpers::CTimeUtils::localize_ptime(time_pt);
	// Create format
	boost::posix_time::time_facet* facet = new boost::posix_time::time_facet();
	facet->format(format.c_str());
	// Convert to string
	std::stringstream stream;
	stream.imbue(std::locale(std::locale::classic(), facet));
	stream << time_pt_local;

	return stream.str();
}
