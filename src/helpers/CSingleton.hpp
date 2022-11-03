/***************************************************************************
 **** IMPLEMENTACION GENERICA DE UN SINGLETON. CUALQUIER CLASE SE PUEDE ****
 **** TRANSFORMAR EN SINGLETON UTILIZANDO ESTE INTERFAZ, SIN NECESIDAD  ****
 ****                  DE CAMBIAR NADA DE LA CLASE ORIGINAL             ****
 ***************************************************************************/

#ifndef _SINGLETON_HPP
#define _SINGLETON_HPP

#include <iostream>
#include <string>

//---------------------------------------------------------
//-----   METODO MEYERS DE CONSTRUCCION DEL SINGLETON -----
//---------------------------------------------------------

/**
 * Esta forma de crear el CSingleton elimina los LEAKS!
 */
template<class T> struct CCreateMeyers
{
	static T* create(std::string s) {
		static T _instance(s);
		return &_instance;
	}
};

//--------------------------------------------
//-----   DEFINICICION DEL SINGLETON<T>  -----
//--------------------------------------------

/**
 * Clase generica de Singletons. Para utilizarla
 * CSingleton<CMyclase>::instance("crear");
 */
template<class T> class CSingleton
{
  public:
	  static T* instance(std::string s); /**< Crea el CSingleton y la clase T, devuelve una T* */
	  static T* instance();              /**< Devuelve una instancia de la clase T una vez inicializado. */

  private:
	  CSingleton();
	  ~CSingleton();

	  CSingleton(const CSingleton &o);
	  CSingleton& operator=(const CSingleton &o);

	  static T* m_p_instance;   /**< Puntero a la clase T de interés. */
};

//----------------------------------------------
//-----   IMPLEMENTACION DEL SINGLETON<T>  -----
//----------------------------------------------

/**
  * Inicializacion a cero del CSingleton
  */
template<class T> T* CSingleton<T>::m_p_instance = 0;



/**
  * Peticion del Handle una vez que el CSingleton ha sido inicializado
  */
template<class T> T * CSingleton<T>::instance()
{
	if (m_p_instance==0)
    return NULL;

	return m_p_instance;
}

/**
  * Iniciar el CSingleton y la clase T, luego devolver el Handle
  */
template<class T> T* CSingleton<T>::instance(std::string s)
{
	if (m_p_instance == 0)
	  m_p_instance = CCreateMeyers<T>::create(s);

	return m_p_instance;
}

#endif
