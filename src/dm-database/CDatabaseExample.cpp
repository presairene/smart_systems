#include "CDatabaseExample.hpp"

//Constructor and destructor of the object
CDatabaseExample::CDatabaseExample()
{

}

CDatabaseExample::~CDatabaseExample()
{

}

// ----------------- INSERT EXAMPLE -----------------

int CDatabaseExample::insertPrediction(const uint64_t & initialDate, const prediction_type & pred_type /*=prediction_type::default_prediction*/, const bool useAutoIncrement /*=true*/) {

	try {
		//This condition checks that there is a connection active
		if (m_p_con != NULL) {
			std::string query("INSERT INTO PREDICTION (ID_PREDICTION, PREDICTION_TYPE, PREDICTION_DATE)  VALUES (");
			std::ostringstream os;
			if (useAutoIncrement) {
				int id = GetID("SELECT MAX(ID_PREDICTION) FROM PREDICTION");
				os << std::to_string(id) << ", ";
			}
			else {
				os << "NULL, ";
			}

			os << static_cast<int32_t> (pred_type) << ", FROM_UNIXTIME(" << initialDate << ", '%Y-%m-%d %H:%i:%s' ))";
			query += os.str();
			bool result = EjecutaQuery(query);

			int id = GetID("SELECT MAX(ID_PREDICTION) FROM PREDICTION");
			if (result) return id;
		}
		else {
			return 0;
		}		
	}
	catch (sql::SQLException &e) {
		std::ostringstream os; os << "ERROR:" << e.what(); _log.println(boost::log::trivial::error, os.str());
		return 0;
	}
	return 0;
}


// ----------------- SELECT EXAMPLE ----------------- 

bool CDatabaseExample::getProsumerMPAN(const int32_t& prosumer_id, std::string& mpan) {

	sql::ResultSet *res = NULL; sql::Statement *p_stmt = NULL;
	bool result = false;

	std::ostringstream os;
	os << "SELECT MPAN from prosumer where id_prosumer =  " << prosumer_id;

	try {
		if (m_p_con != NULL) {
			std::string query;
			query = os.str();
			p_stmt = m_p_con->createStatement(); 
			res = p_stmt->executeQuery(query);

			while (res->next()) {
				//This loop goes thorugh the rows of the response one by one getting the value of the column with the label returned in the query
				//Bear in mind you can access elements too by the index of the column (below both options)
				mpan = res->getString("MPAN");
				//mpan = res->getString(0 + INDEX_OFFSET_IN_RESULT_SETS);
				result = true;
			}

			delete res;
			delete p_stmt;
			p_stmt = NULL;
		}
	}
	catch (sql::SQLException &e) {
		if (res != NULL) delete res;
		if (p_stmt != NULL) delete p_stmt;
		std::ostringstream os; os << "ERROR:" << e.what(); _log.println(boost::log::trivial::error, os.str());
		return false;
	}
	return result;
}

// ----------------- UPDATE EXAMPLE ----------------- 

bool CDatabaseExample::updateProsumerMPAN(const int32_t& prosumer_id, const std::string& mpan) {

	sql::ResultSet *res = NULL; sql::Statement *p_stmt = NULL;

	std::ostringstream os;
	os << " UPDATE prosumer SET MPAN = \'" << mpan << "\' WHERE(ID_PROSUMER = " << prosumer_id << ");";
	std::string query;
	query = os.str();
	try {
		if (!EjecutaQuery(query)) return false;
	}
	catch (sql::SQLException &e) {
		std::ostringstream os; os << "ERROR:" << e.what(); _log.println(boost::log::trivial::error, os.str());
		return false;
	}
	return true;
}
