// --------------------------------------------------------------------------
// ----------------------------- CLASE CDatabase ----------------------------
// --------------------------------------------------------------------------
//
// Archivo: CDatabase.cpp
//
// Descripción: Clase encargada de comunicar con la base de datos.
//
// Versión:
//
// Fecha: 15/01/2014
//
// Autor: Álvaro Sánchez Miralles
//
// --------------------------------------------------------------------------

#include "CDatabase.hpp"
#include <sstream>
#include <stdlib.h>
#include "CError.h"
#include <time.h> 
#include <jdbc/cppconn/statement.h>
//#include "mariadbpp/statement.hpp"

using namespace std;

// -----------------------------------------------------------------------------
std::mutex CDatabase::_driverMutex;
uint32_t CDatabase::_connectionCounter = 0;
// Constructor por defecto
CDatabase::CDatabase() : _log("log") {
	m_p_con = NULL;
	m_p_savept = NULL;
	m_p_driver = NULL;
}

// Destructor
CDatabase::~CDatabase() {

	if (m_p_con != NULL) Desconectar();
	if (m_p_driver != NULL) {
		m_p_driver->threadEnd();
		m_p_driver = NULL;
	}

	//if (_log != NULL) {
	//	delete _log;
	//	_log = NULL;
	//}

}

// -----------------------------------------------------------------------------

// Función que prepara la conexión con la base de datos para acceder a la información
bool CDatabase::Conectar(const string& s_data_base, const string& s_server, const string& s_user, const string& s_pass) {
	try {

		_driverMutex.lock();
		m_p_driver = get_driver_instance();
		_driverMutex.unlock();

		if (m_p_driver != NULL) {
			m_p_con = m_p_driver->connect(s_server.c_str(), s_user.c_str(), s_pass.c_str());
			m_p_con->setSchema(s_data_base);
			m_p_con->setAutoCommit(false);
			m_p_driver->threadInit();
			_connectionCounter++;
		}
		//	_log.println("Database: Creating connection " + std::to_string(_connectionCounter));

	}
	catch (sql::SQLException &e) {
		std::ostringstream os;
		os << "ERROR:" << e.what();
		_log.println(boost::log::trivial::error, os.str());
		return false;
	}
	return true;

}

// Función que libera la memoria utilizada en la conexión
void CDatabase::Desconectar(void) {
	try {
		if (m_p_con != NULL) {
			m_p_con->close();
			delete m_p_con;
			m_p_con = NULL;

			//	_log.println("Database: Deleting connection " + std::to_string(_connectionCounter));
			_connectionCounter--;

		}
		if (m_p_driver != NULL && m_p_driver != nullptr) {
			m_p_driver->threadEnd();
			m_p_driver = NULL;
		}


	}
	catch (sql::SQLException &e) {
		std::ostringstream os;
		os << "ERROR:" << e.what();
		_log.println(boost::log::trivial::error, os.str());
	}

}

// -----------------------------------------------------------------------------

bool CDatabase::ComienzaTransaccion(void) {
	sql::Statement *p_stmt = NULL;
	try {
		if (m_p_con != NULL) {
			//p_stmt = m_p_con->createStatement();
			//p_stmt->execute("START TRANSACTION");
			//p_stmt->execute("SET unique_checks=0");
			//p_stmt->execute("SET foreign_key_checks=0");
			//delete p_stmt;
			//p_stmt = NULL;
		}
	}
	catch (sql::SQLException &e) {
		if (p_stmt != NULL) delete p_stmt;
		std::ostringstream os;
		os << "ERROR:" << e.what();
		_log.println(boost::log::trivial::error, os.str());
		return false;
	}
	return true;
}


// -----------------------------------------------------------------------------

// Función que libera la memoria utilizada en la conexión
bool CDatabase::ConfirmarTransaccion(void) {
	sql::Statement *p_stmt = NULL;
	try {
		if (m_p_con != NULL) {
			m_p_con->commit();
			p_stmt = m_p_con->createStatement();
			p_stmt->execute("SET unique_checks=1");
			p_stmt->execute("SET foreign_key_checks=1");
			delete p_stmt;
			p_stmt = NULL;
		}
	}
	catch (sql::SQLException &e) {
		if (p_stmt != NULL) delete p_stmt;
		std::ostringstream os;
		os << "ERROR:" << e.what();
		_log.println(boost::log::trivial::error, os.str());
		return false;
	}
	return true;

}

// -----------------------------------------------------------------------------

// Función que libera la memoria utilizada en la conexión
bool CDatabase::DeshacerTransaccion(void) {
	try {
		if (m_p_con != NULL) {
			m_p_con->rollback();
			//		m_p_con->rollback(m_p_savept);
			//		m_p_con->releaseSavepoint (m_p_savept);
		}

	}
	catch (sql::SQLException &e) {
		std::ostringstream os;
		os << "ERROR:" << e.what();
		_log.println(boost::log::trivial::error, os.str());
		return false;
	}
	return true;
}


// -----------------------------------------------------------------------------
int CDatabase::GetID(const std::string& query)
{
	int id = 0;
	sql::ResultSet *res = NULL; sql::Statement *p_stmt = NULL;
	try {
		if (m_p_con != NULL) {
			p_stmt = m_p_con->createStatement();
			res = p_stmt->executeQuery(query);
			if (res->next()) {
				id = res->getInt(0 +INDEX_OFFSET_IN_RESULT_SETS);
			}
			delete res;
			delete p_stmt;
			p_stmt = NULL;
		}
	}
	catch (sql::SQLException &e) {
		if (res != NULL) delete res;
		if (p_stmt != NULL) delete p_stmt;
		std::ostringstream os;
		os << "ERROR:" << e.what();
		_log.println(boost::log::trivial::error, os.str());
		return -1;
	}
	return id;
}


bool CDatabase::EjecutaQuery(const string& sql)
{
	
	sql::Statement *p_stmt = NULL;
	try {

		if (m_p_con != NULL) {
			p_stmt = m_p_con->createStatement();
			p_stmt->execute(sql);
			p_stmt->close();

			delete p_stmt;
			p_stmt = NULL;
		}
	}
	catch (sql::SQLException &e) {
		if (p_stmt != NULL) delete p_stmt;
		std::ostringstream os;
		os << "ERROR:" << e.what();
		_log.println(boost::log::trivial::error, os.str());
		return false;
	}
	return true;
}

bool CDatabase::EjecutaQueryBlob(const string& sql,  std::istream& blob){
	
	sql::PreparedStatement *p_stmt = NULL;
	bool result = true;
	try {

		if (m_p_con != NULL) {
			p_stmt = m_p_con->prepareStatement(sql);
			p_stmt->setBlob(1, &blob);
			result = p_stmt->executeUpdate();
			p_stmt->close();

			delete p_stmt;
			p_stmt = NULL;
		}
	}
	catch (sql::SQLException &e) {
		if (p_stmt != NULL) delete p_stmt;
		std::ostringstream os;
		os << "ERROR:" << e.what();
		_log.println(boost::log::trivial::error, os.str());
		return false;
	}
	return result;
}

int CDatabase::GetConnections() {
	
	int id = 0;
	sql::ResultSet *res = NULL; sql::Statement *p_stmt = NULL;
	try {
		if (m_p_con != NULL) {
			p_stmt = m_p_con->createStatement();
			res = p_stmt->executeQuery("show PROCESSLIST");
			id = res->rowsCount();
			delete res;
			delete p_stmt;
			p_stmt = NULL;
		}
	}
	catch (sql::SQLException &e) {
		if (res != NULL) delete res;
		if (p_stmt != NULL) delete p_stmt;
		std::ostringstream os;
		os << "ERROR:" << e.what();
		_log.println(boost::log::trivial::error, os.str());
		return -1;
	}
	return id;
}

