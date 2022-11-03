#include "CLog.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"

#include <boost/log/expressions.hpp>

#include <boost/log/utility/setup/console.hpp>

#include <boost/log/utility/setup/common_attributes.hpp>

#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/support/date_time.hpp>

#include <boost/core/null_deleter.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

const std::map<std::string, boost::log::trivial::severity_level> CLog::SEVERITY_LEVEL_DESCRIPTIONS = {
		{"trace", boost::log::trivial::trace},
		{"debug", boost::log::trivial::debug},
		{"info", boost::log::trivial::info},
		{"warning", boost::log::trivial::warning},
		{"error", boost::log::trivial::error},
		{"fatal", boost::log::trivial::fatal}
};

BOOST_LOG_ATTRIBUTE_KEYWORD(channel, "Channel", std::string)

//Constructor necessary to enable singleton definition
CLog::CLog(std::string log) : _logger(boost::log::keywords::channel = log)
{
	_logName = log;
}

bool CLog::initializeParametersFromIniFile(const std::string& filePath, const std::string& filename) {
	boost::shared_ptr<boost::log::sinks::synchronous_sink< boost::log::sinks::text_file_backend >> sink;
	boost::shared_ptr < boost::log::sinks::synchronous_sink < boost::log::sinks::basic_text_ostream_backend<char>>> sink_console;
	return initializeParametersFromIniFile(filePath, filename, sink, sink_console);
}

bool CLog::initializeParametersFromIniFile(const std::string& filePath, const std::string& filename, boost::shared_ptr<boost::log::sinks::synchronous_sink< boost::log::sinks::text_file_backend >>& sink,
	boost::shared_ptr < boost::log::sinks::synchronous_sink < boost::log::sinks::basic_text_ostream_backend<char>>> & sink_console) {
	try {
		std::string pathToFile = filePath + "/" + filename;

		boost::property_tree::ptree pt;
		boost::property_tree::ini_parser::read_ini(pathToFile, pt);

		std::string securityLevel = pt.get<std::string>(_logName + ".security_level");
		if (CLog::SEVERITY_LEVEL_DESCRIPTIONS.find(securityLevel) == CLog::SEVERITY_LEVEL_DESCRIPTIONS.end()) {
			//Unkown severity level found
			return false;
		}
		//Mandatory parameters
		std::string directory = pt.get<std::string>(_logName + ".directory");
		uint16_t rotationFileSize = pt.get<uint16_t>(_logName + ".rotation_file_size_mb");
		uint16_t directoryLimitSizeMb = pt.get<uint16_t>(_logName + ".directory_limit_size_mb");
		uint8_t directoryMaxFileNumber = pt.get<uint8_t>(_logName + ".directory_max_file_number");

		//Optional parameters
		bool instant_writing = false;
		try {
			instant_writing = pt.get<bool>(_logName + ".instant_writing");
		}
		catch (std::exception e) {
			instant_writing = false;
		}
		bool use_console = false;
		try {
			use_console = pt.get<bool>(_logName + ".console");
		}
		catch (std::exception e) {
			use_console = false;
		}

		uint8_t rotationHour = 0; 
		try {
			rotationHour = pt.get<uint8_t>(_logName + ".rotation_hour");
		}
		catch (std::exception e) {
			rotationHour = 0;
		}
		uint8_t rotationMinute = 0;
		try {
			rotationMinute = pt.get<uint8_t>(_logName + ".rotation_minute");
		}
		catch (std::exception e) {
			rotationMinute = 0;
		}
		uint8_t rotationSecond = 0;
		try {
			rotationSecond = pt.get<uint8_t>(_logName + ".rotation_second");
		}
		catch (std::exception e) {
			rotationSecond = 0;
		}
		helpers::CTimeUtils::weekday rotationWeekday = helpers::CTimeUtils::weekday::default_weekday;
		try {
			rotationWeekday = static_cast<helpers::CTimeUtils::weekday>(pt.get<int8_t>(_logName + ".rotation_weekday"));
		}
		catch (std::exception e) {
			rotationWeekday = helpers::CTimeUtils::weekday::default_weekday;
		}

		initializeLogParameters(CLog::SEVERITY_LEVEL_DESCRIPTIONS.at(securityLevel), directory, rotationFileSize, directoryLimitSizeMb, directoryMaxFileNumber, sink, sink_console, use_console, instant_writing,
			rotationWeekday, rotationHour, rotationMinute, rotationSecond);
	}
	catch (std::exception e) {
		return false;
	}
	return true;
}

void CLog::initializeLogParameters(const boost::log::trivial::severity_level& minimumSecurityLevel, const std::string& directory, const uint16_t& rotationFileSizeMb,
	const uint16_t& directoryLimitSizeMb, const uint8_t& directoryMaxFileNumber,
	boost::shared_ptr<boost::log::sinks::synchronous_sink< boost::log::sinks::text_file_backend >>& sink,
	boost::shared_ptr<boost::log::sinks::synchronous_sink< boost::log::sinks::basic_text_ostream_backend<char>>> & sink_console,
	const bool& useConsole /*= false*/, const bool& instantWriting /*= false */ ,
	const helpers::CTimeUtils::weekday& rotationWeekday /*= helpers::CTimeUtils::weekday::default_weekday*/,
	const uint8_t & rotationHour /*= 0*/, const uint8_t & rotationMinute /*= 0 */ , const uint8_t & rotationSecond /*= 0*/)
{
	std::ostringstream os;
	os << directory << "/" << _logName << "_%Y-%m-%dT%H%M%S_%N.log";

	//Protect parameters
	uint16_t protectedRotationFileSizeMb = rotationFileSizeMb;
	if (protectedRotationFileSizeMb == 0) {
		protectedRotationFileSizeMb = 1;
	}

	uint8_t protectedRotationHour = rotationHour;
	if (protectedRotationHour > 23) {
		protectedRotationHour = 23;
	}

	uint8_t protectedRotationMinute = rotationMinute;
	if (protectedRotationMinute > 59) {
		protectedRotationMinute = 59;
	}

	uint8_t protectedRotationSecond = rotationSecond;
	if (protectedRotationSecond > 59) {
		protectedRotationSecond = 59;
	}
	bool rotateWeekly = false;
	if (rotationWeekday != helpers::CTimeUtils::weekday::default_weekday) {
		rotateWeekly = true;
	}

	sink = boost::log::add_file_log
	(
		boost::log::keywords::file_name = os.str(), //File name pattern
		boost::log::keywords::rotation_size = protectedRotationFileSizeMb * 1024 * 1024,  // Rotates file every X MB 
		rotateWeekly ? boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(protectedRotationHour, protectedRotationMinute, protectedRotationSecond)
		: boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(parseWeekday(rotationWeekday), protectedRotationHour, protectedRotationMinute, protectedRotationSecond), // Or at specified day time (default midnight) 
		boost::log::keywords::filter = channel == _logName && boost::log::trivial::severity >= minimumSecurityLevel,  //File filter
		boost::log::keywords::auto_flush = instantWriting,
		boost::log::keywords::format = (boost::log::expressions::stream
			// line id will be written in hex, 8-digits, zero-filled
			<< boost::log::expressions::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
			<< " #" << std::hex << std::setw(8) << std::setfill('0') << boost::log::expressions::attr< unsigned int >("LineID")
			<< " <" << boost::log::trivial::severity
			<< ">: " << boost::log::expressions::smessage)

	);
	boost::posix_time::ptime time;

	if (useConsole) {
		sink_console = boost::log::add_console_log
		(
			std::cout,
			boost::log::keywords::auto_flush = instantWriting,
			boost::log::keywords::filter = channel == _logName && boost::log::trivial::severity >= minimumSecurityLevel,  //File filter
			boost::log::keywords::format = (boost::log::expressions::stream
				// line id will be written in hex, 8-digits, zero-filled
				<< boost::log::expressions::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
				<< " #" << std::hex << std::setw(8) << std::setfill('0') << boost::log::expressions::attr< unsigned int >("LineID")
				<< " <" << boost::log::trivial::severity
				<< ">: " << boost::log::expressions::smessage)

		);
	}

	boost::log::add_common_attributes();

	if (directoryLimitSizeMb > 0 && directoryMaxFileNumber > 0) {
		sink->locked_backend()->set_file_collector(boost::log::sinks::file::make_collector(
			boost::log::keywords::target = directory, //Log collector path
			boost::log::keywords::max_size = directoryLimitSizeMb * 1024 * 1024, //Log collector max directory size
			boost::log::keywords::max_files = directoryMaxFileNumber //Log collector max number of files in directory
		));

		sink->locked_backend()->scan_for_files();
	}

	boost::log::core::get()->add_sink(sink);

}

void CLog::removeSink(const boost::shared_ptr<boost::log::sinks::synchronous_sink< boost::log::sinks::text_file_backend >>& sink) {
	boost::log::core::get()->remove_sink(sink);
}

void CLog::removeAllSinks(void) {
	boost::log::core::get()->remove_all_sinks();
}

boost::date_time::weekdays CLog::parseWeekday(const helpers::CTimeUtils::weekday& weekday) {

	switch (weekday) {
	case helpers::CTimeUtils::weekday::monday:
		return boost::date_time::weekdays::Monday;
	case helpers::CTimeUtils::weekday::tuesday:
		return boost::date_time::weekdays::Tuesday;
	case helpers::CTimeUtils::weekday::wednesday:
		return boost::date_time::weekdays::Wednesday;
	case helpers::CTimeUtils::weekday::thursday:
		return boost::date_time::weekdays::Thursday;
	case helpers::CTimeUtils::weekday::friday:
		return boost::date_time::weekdays::Friday;
	case helpers::CTimeUtils::weekday::saturday:
		return boost::date_time::weekdays::Saturday;
	case helpers::CTimeUtils::weekday::sunday:
		return boost::date_time::weekdays::Sunday;
	default:
		return boost::date_time::weekdays::Monday;
	}
	
}
// -----------------------------------------------------------------------------

void CLog::println(const boost::log::trivial::severity_level& severityLevel, const std::string& message) {
	
	try {
		BOOST_LOG_SEV(_logger, severityLevel) << message;
	}
	catch (std::exception e) {
		std::cout << "ERROR in CLog: " << e.what();
	}
}