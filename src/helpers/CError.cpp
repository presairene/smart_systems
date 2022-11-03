#include <stdio.h>
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>


#include "CError.h"


//----------------------------------------
//-------   CLASE ERROR FUNCION   --------
//----------------------------------------


void CErrorEnFuncion::AddVar(std::string var_name, std::string s)
{
	m_lp_variables.push_back(std::make_pair(var_name, s));
}



void CErrorEnFuncion::AddVar(std::string var_name, std::size_t i)
{
	std::ostringstream os;
	os << i;
	m_lp_variables.push_back(std::make_pair(var_name, os.str()));
}



void CErrorEnFuncion::AddVar(std::string var_name, int i)
{
	std::ostringstream os;
	os << i;
	m_lp_variables.push_back(std::make_pair(var_name, os.str()));
}



void CErrorEnFuncion::AddVar(std::string var_name, double d)
{
	std::ostringstream os;
	os << d;
	m_lp_variables.push_back(std::make_pair(var_name, os.str()));
}



inline std::ostream& operator << (std::ostream &fav, const CErrorEnFuncion &p)
{
	std::list< std::pair<std::string,std::string> >::const_iterator iter;

	fav << "<TD ALIGN=\"left\">" << "<B>" << p.m_s_nombre_funcion << "</B></TD>" << std::endl;
	fav << "<TD ALIGN=\"left\">" << std::endl;
        for (iter=p.m_lp_variables.begin(); iter != p.m_lp_variables.end(); iter++) {
                fav << "<P><FONT COLOR=\"#0000FF\">'" << (*iter).first << "'</FONT>  =  " << (*iter).second << "</P>" << std::endl;
        }
	fav << "</TD>" << std::endl;
	return fav;
}




//---------------------------------
//-------   CLASE ERROR    --------
//---------------------------------


char * CError::m_p_pool=NULL; 

std::string CError::m_s_directory="";		


CError::CError(std::string s_ca)
{
	if (m_p_pool) {
		delete[]m_p_pool;
		m_p_pool=NULL;
	}
	//else FORCE_EXIT("Es necesario reservar memoria para el POOL al comienzo del programa!");	

	m_s_causa=s_ca;
}



CError::~CError() throw()
{ 
	m_le_errores.clear();
  	std::cout << "Destructor de CError!" << std::endl;
}




inline std::ostream& operator << (std::ostream &fav, const CError &p)
{
	int i;
	std::list<CErrorEnFuncion>::const_iterator iter;

	fav << "<DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2//EN\">" << std::endl;
	fav << "<HTML>" << std::endl;
	fav << "<HEAD>" << std::endl;
        fav << "<TITLE>Fichero de Error del Modelo</TITLE>" << std::endl;
	fav << "</HEAD>" << std::endl;
	fav << "<BODY>" << std::endl;

	fav << "<H2>" << " ERROR: " << p.m_s_causa << "</H2>" << std::endl;

	fav << "<HR WIDTH=200>" << std::endl;

	fav << "<P>" << "STACK DE ERRORES:" << "</P>" << std::endl;

	fav << "<TABLE BORDER ALIGN=\"center\" VALIGN=\"center\" CELLPADDING=2>" << std::endl;

	for (i=0, iter=p.m_le_errores.begin(); iter != p.m_le_errores.end(); iter++, i++) {
		fav << "<TR ALIGN=\"center\">" << std::endl;
		fav << "<TD ALIGN=\"left\">Nivel: " << i << "</TD>" << std::endl;
		fav << (*iter) << std::endl;
		fav << "</TR>" << std::endl;
	}

	fav << "</TABLE>" << std::endl;
	fav << "</BODY>" << std::endl;
	fav << "</HTML>" << std::endl;
	return fav;
}


void CError::SaveExp(std::string nombre_fichero)
{
	std::ofstream fav;
	fav.open(nombre_fichero.c_str());

	if (fav.is_open()==false) {
		std::cout << "FALLO_PRG" << std::endl << *this;
		std::cout.flush();
	}
	else {
		fav << *this;
		fav.close();
	}
}


void CError::ReservaPool()
{
	try {

		if (m_p_pool!=NULL) FORCE_EXIT("POOL YA INICIALIZADO!");
		m_p_pool= new char[1000000];
		if (!m_p_pool) throw CError("No he podido reservar memoria para el POOL!");  // deberia sobrar
#ifdef FORMATO_FORMULARIO
		std::cout << "Pool de memoria reservado!" << std::endl;
#endif

	} catch(std::exception &e) {
		CErrorEnFuncion ef("CError:ReservaPool");
		ef.AddVar("What", e.what());
		THROW_ERROR(e, ef);
	}
}


void CError::LiberaPool()
{
	if (m_p_pool==NULL)  {
		std::cout << "Pool no inicializado o ya liberado" << std::endl;
		return;		//FORCE_EXIT("POOL NO INICIALIZADO!");		
	}
	delete[] m_p_pool;
	m_p_pool=NULL;
#ifdef FORMATO_FORMULARIO
	std::cout << "Pool de memoria liberado!" << std::endl;
#endif
}


//Generar informe html de error
void CError::GenerarInformeError(std::string s_path,std::string s) {
	std::ofstream fav((CError::GetDirectory()+"error.html").c_str());	
	fav << "<DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2//EN\">" << std::endl;
	fav << "<HTML>" << std::endl;
	fav << "<HEAD>" << std::endl;
	fav << "<TITLE>Fichero de Error del Modelo</TITLE>" << std::endl;
	fav << "</HEAD>" << std::endl;
	fav << "<BODY>" << std::endl;
	fav << "<H2>" << " ERROR: " << s << "</H2>" << std::endl;
	fav << "</BODY>" << std::endl;
	fav << "</HTML>" << std::endl;
	fav.close();
}


//-------------------------------------------------------------
//-------------------------------------------------------------

/**
 * La función FORCE_EXIT es el punto por el se finaliza la ejecución y se devuelve el control al sistema 
 * operativo en caso de haberse producido un error. 
 * Esta función es útil para depurar el modelo, colocando un <i>breakpoint</i>.
 * @param texto cadena que describe el error producido.
 */
void FORCE_EXIT(std::string texto)
{
	if (texto!="") std::cout << texto << std::endl;
	exit(1);
}
