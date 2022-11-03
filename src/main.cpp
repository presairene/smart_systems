#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"

#include "CLog.hpp"
#include "CLogListenerConsole.hpp"
#include "CLogListenerFile.hpp"
#include "CSingleton.hpp"
#include "CError.h"
#include "CDatabase.hpp"

#include "CDatabaseLab4.h"

#define CONFIG_PATH "config"
#define LOGS_PROPERTIES_FILE "logs.ini"

#define SCHEMA_NAME "practica2"
#define HOST_NAME "127.0.0.1:3306"
#define USER_NAME "root"
#define PASSWORD_USER "BlancaMYSQL"

#define TIME_SCAN_CYCLE_S 900 //15 min

int main(void){

	CDatabaseLab4 dbObject;
	
	try {

		CError::ReservaPool();

		//  ---------------------------- INIT RESOURCES  ---------------------------- 

		//Configure logs
		CLog log("log");
		if (!log.initializeParametersFromIniFile(CONFIG_PATH, LOGS_PROPERTIES_FILE)) {
			std::cout << "ERROR reading database log in file: " << CONFIG_PATH << "/" << LOGS_PROPERTIES_FILE << std::endl;
			return 0;
		}
		log.println(boost::log::trivial::fatal, "Log initialized");

		CError::LiberaPool();

		uint64_t lastExecution = 0;

		//  ---------------------------- START SCAN CYCLE ---------------------------- 
		while (1) {
			boost::posix_time::ptime execTime = boost::posix_time::second_clock::local_time();

			if ((helpers::CTimeUtils::seconds_from_epoch(execTime) - lastExecution) >= TIME_SCAN_CYCLE_S) {

				//The content of this if should go in a execute function of the object which will contain the intelligence module

				log.println(boost::log::trivial::trace, "Starting intelligence execution cycle");

				// ---------------------------- GET DATA FROM DB  ---------------------------- 

				//DDBB connection
				
				dbObject.Conectar(SCHEMA_NAME, HOST_NAME, USER_NAME, PASSWORD_USER);
				log.println(boost::log::trivial::trace, "Hemos conectado con la DB para hacer getters de info");

				//Do stuff with DB: GET DATA
				//EXAMPLE:
				std::string prosumer1MPAN;
				dbObject.getProsumerMPAN(1, prosumer1MPAN);
				

				dbObject.Desconectar();

				// ---------------------------- PROCESS OF DATA & INTELLIGENCE ---------------------------- 





				// ---------------------------- INSERT RESULTS IN db  ---------------------------- 

				//DDBB connection
				dbObject.Conectar(SCHEMA_NAME, HOST_NAME, USER_NAME, PASSWORD_USER);
				log.println(boost::log::trivial::trace, "Hemos conectado con la DB para hacer inserts de info");

				//Insert stuff in DB
				dbObject.ComienzaTransaccion();

				//Do inserts of data
				bool resultInsert = true;

				//Do insert of data 
				//EXAMPLE:
				resultInsert = resultInsert && dbObject.insertPrediction(helpers::CTimeUtils::seconds_from_epoch(execTime), CDatabaseLab4::prediction_type::consumption_kWh, false);

				if (resultInsert) {
					log.println(boost::log::trivial::trace, "Data insert OK");
					dbObject.ConfirmarTransaccion();
				}
				else {
					log.println(boost::log::trivial::trace, "Data insert ERROR");
					dbObject.DeshacerTransaccion();
				}

				dbObject.Desconectar();



				// ---------------------------- Tests Lab 4  ---------------------------- 

				//DDBB connection

				dbObject.Conectar(SCHEMA_NAME, HOST_NAME, USER_NAME, PASSWORD_USER);
				log.println(boost::log::trivial::trace, "Hemos conectado con la DB para hacer getters de info");

				//Inicializar variables
				time_t fech = time(0); // get the current datetime
				struct tm* timeinfo; // structure to manage dates
				timeinfo = localtime(&fech);
				timeinfo->tm_mon = 0; // month: January
				timeinfo->tm_mday = 9; // day of the month: 9
				timeinfo->tm_year = 2015 - 1900; // year 2015
				timeinfo->tm_hour = 0; // hour 0
				timeinfo->tm_min = 0;
				timeinfo->tm_sec = 0;
				fech = mktime(timeinfo);
				CValue value(15.5, fech);
				int32_t idVariable = 2;

				//insertValue:
				dbObject.insertValue(idVariable, value);

				//getValues:
				CValue* val1 = dbObject.getValues(idVariable);

				//updateValue:
				bool result;
				value = CValue(15.5+1, fech);
				result = dbObject.updateValue(idVariable, value);

				//getValues:
				CValue* val2 = dbObject.getValues(idVariable);

				//deleteValue:
				dbObject.deleteValue(idVariable, fech);

				//getValues:
				CValue* val3 = dbObject.getValues(idVariable);

				dbObject.Desconectar();

				lastExecution = helpers::CTimeUtils::seconds_from_epoch(execTime);
			}
		}

	}
	catch (std::exception &e) {
		//Always close connections in case of error
		dbObject.Desconectar();

		CErrorEnFuncion ef("main(void)");
		std::ostringstream os; 
		os << "ERROR:" << e.what(); 
		std::cout << os.str() << std::endl;
		THROW_ERROR(e, ef);
		return (0);
	}

	return 0;
}

