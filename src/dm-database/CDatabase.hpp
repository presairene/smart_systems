// --------------------------------------------------------------------------
// ---------------------- CABECERA DE LA CLASE CDatabase --------------------
// --------------------------------------------------------------------------
// 
// Archivo: CDatabase.hpp
//
// Descripción: Clase encargada de comunicar con la base de datos Domotica.
//
// Versión:
//
// Fecha: 15/01/2014
//
// Autor: Álvaro Sánchez Miralles
//
// --------------------------------------------------------------------------

#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <map>
#include <mutex>

#include <jdbc/mysql_connection.h>
#include <jdbc/mysql_driver.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/exception.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/prepared_statement.h>


#include "CLog.hpp"
#include <istream>

#define INDEX_OFFSET_QUERY_ARGS 1
#define INDEX_OFFSET_IN_RESULT_SETS 1 //Value correspondign to the first column index of a resultset
#define DAYS_ST 1
#define DAYS_MT 31
#define RESOLUTION_MT 300
#define RESOLUTION_LT 300

// --------------------------------------------------------------------------
#include <algorithm>
#include <exception>

class CDatabase
{
	static std::mutex _driverMutex;
	static uint32_t _connectionCounter;
protected:
	sql::Connection *m_p_con;
	sql::Savepoint *m_p_savept;
	sql::Driver *m_p_driver;
	CLog _log;

public:

	// Constructor y destructor
	CDatabase();
	virtual ~CDatabase();

	// Funciones
	bool Conectar(const std::string& data_base, const std::string& server, const std::string& user, const std::string& pass);
	void Desconectar();
	int GetConnections();
	bool EjecutaQuery(const std::string& sql);
	int GetID(const std::string& query);
	bool ComienzaTransaccion();
	bool DeshacerTransaccion();
	bool ConfirmarTransaccion();

	bool EjecutaQueryBlob( const std::string& sql,  std::istream& blob);
};
