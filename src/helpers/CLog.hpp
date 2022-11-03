#ifndef _LOG_HPP
#define _LOG_HPP

#include <string>
#include <map>

#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/date_time/date_defs.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/core.hpp>
#include <boost/log/utility/setup/file.hpp>

#include "CTimeUtils.hpp" //TODO review this dependency, it is just for the enum CTimeUtils::weekday

class CLog
{
  protected:
	  boost::log::sources::severity_channel_logger<boost::log::trivial::severity_level, std::string> _logger;
	  std::string _logName;

	  /**
	  *		Parse uint16_t into boost::date_time_weekdays enum.
	  *
	  *		[in] weekday
	  *
	  *		@return  boost::date_time::weekdays
	  */
	  boost::date_time::weekdays parseWeekday(const helpers::CTimeUtils::weekday& weekday);
	  
  public:
	  CLog(std::string s);
	  ~CLog(){};
	  std::string getLogName() { return  _logName; };
	  static const std::map<std::string, boost::log::trivial::severity_level> SEVERITY_LEVEL_DESCRIPTIONS;

	  /**
	  *		Initialize log parameters.
	  *
	  *		[in] minimumSecurityLevel		Print messages below this severity level.
	  *		[in] directory
	  *		[in] rotationFileSizeMb			Generate new file after reaching file max size MB.
	  *		[in] directoryLimitSizeMb
	  *		[in] directoryMaxFileNumber
	  *		[in] useConsole					
	  *		[in] instantWriting		
	  *		[in] rotationHour				Rotate new file at this hour.
	  *		[in] rotationMinute
	  *		[in] rotationSecond
	  *		[in] rotationWeekday			Rotate new file at this weekday (default rotates every day). 0 - Monday, 6- Sunday
	  *
	  *		@return
	  */
	  void initializeLogParameters(const boost::log::trivial::severity_level& minimumSecurityLevel, const std::string& directory, const uint16_t& rotationFileSizeMb, 
		  const uint16_t& directoryLimitSizeMb, const uint8_t& directoryMaxFileNumber, 
		  boost::shared_ptr<boost::log::sinks::synchronous_sink< boost::log::sinks::text_file_backend >>& sink,
		  boost::shared_ptr<boost::log::sinks::synchronous_sink< boost::log::sinks::basic_text_ostream_backend<char>>> & sink_console,
		  const bool& useConsole = false, const bool& instantWriting = false,
		  const helpers::CTimeUtils::weekday& rotationWeekday = helpers::CTimeUtils::weekday::default_weekday,
		  const uint8_t & rotationHour = 0, const uint8_t & rotationMinute = 0, const uint8_t & rotationSecond = 0);

	  bool initializeParametersFromIniFile(const std::string& filePath, const std::string& filename, boost::shared_ptr<boost::log::sinks::synchronous_sink< boost::log::sinks::text_file_backend >>& sink,
		  boost::shared_ptr < boost::log::sinks::synchronous_sink < boost::log::sinks::basic_text_ostream_backend<char>>> & sink_console);
	  bool initializeParametersFromIniFile(const std::string& filePath, const std::string& filename);

	  void println(const boost::log::trivial::severity_level& severityLevel, const std::string& message);
	  void removeSink(const boost::shared_ptr<boost::log::sinks::synchronous_sink< boost::log::sinks::text_file_backend >>& sink);
	  static void removeAllSinks(void);
};


#endif