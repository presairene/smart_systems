/************************************************************************
Autores: Jesús Peco, Álvaro Sánchez
************************************************************************/

#ifndef CERROR_H
#define CERROR_H


#include <list>
#include <string>
#include <fstream>		



class CErrorEnFuncion
{
	std::string  m_s_nombre_funcion;

	std::list< std::pair<std::string,std::string> > m_lp_variables;

 public:
	CErrorEnFuncion(std::string f) { m_s_nombre_funcion=f; }
	~CErrorEnFuncion() { m_lp_variables.clear(); }

	void AddVar(std::string var_name, std::string s);
	void AddVar(std::string var_name, std::size_t i);
	void AddVar(std::string var_name, int i);
	void AddVar(std::string var_name, double d);

	friend std::ostream& operator << (std::ostream &fav, const CErrorEnFuncion &p);
};



class CError: public std::exception
{
private:
	static char *m_p_pool;
	static std::string m_s_directory;		

	std::list<CErrorEnFuncion> m_le_errores;
	std::string    m_s_causa;    // almecena la causa del error

	friend std::ostream& operator << (std::ostream& os, const CError &e);


public:
	CError(std::string s_ca);
	~CError() throw();

	static void SetDirectory(std::string s) {m_s_directory=s;};    // Fija el directorio de salida
	static std::string GetDirectory() {return m_s_directory;};    // Obtiene el directorio de salida

	std::string GetCausa() {return m_s_causa;};    // devuelve la causa del error	

	void AddErrorEnFuncion(CErrorEnFuncion ef){m_le_errores.push_back(ef);}
	void SaveExp(std::string path);

	//Generar informe de error para excepciones estandar
	static void GenerarInformeError(std::string s_path,std::string s);



	/**
	 * Esta funcion reserva al comienzo del modelo una cantidad de
	 * memoria para liberarla y que haya espacio suficiente para almacenar la
	 * pila de errores
	 */
	static void ReservaPool(); 


	/**
	 * Esta funcion libera el pool para dejar hueco
	 * a la pila de errores
	 */
	static void LiberaPool();  
};



/**
 * La función FORCE_EXIT es el punto por el se finaliza la ejecución y se devuelve el control al sistema 
 * operativo en caso de haberse producido un error. 
 * Esta función es útil para depurar el modelo, colocando un <i>breakpoint</i>.
 * @param texto cadena que describe el error producido.
 */
void FORCE_EXIT(std::string texto="");



//Ahora informa de excepciones no controladas, con el stack
//Salvo que con este formato no se recupera el nombre de la ultima funcion en excepciones no controladas
  /**
   * Esta funcion realiza unas tareas rutinarias para relanzar el error
   * al resto de las funciones
   * @param e  excepcion standard o CError
   * @param ef datos de los errores de una funcion
   */
#define THROW_ERROR(e, ef);   \
        CError *deb; \
        if (!(deb=dynamic_cast<CError*>(&e))) deb=new CError(e.what()); \
        deb->AddErrorEnFuncion(ef); \
        throw *deb;  \
		} catch(...) {   \
			CError *deb; \
			CErrorEnFuncion ef2("");  \
			deb=new CError("Excepcion void no controlada"); \
	        deb->AddErrorEnFuncion(ef2); \
		    throw *deb;

#endif


