// --------------------------------------------------------------------------
// ---------------------- CABECERA DE LA CLASE CDatabaseExample --------------------
// --------------------------------------------------------------------------
// 
// Archivo: CDatabase.hpp
//
// Descripción: Clase ejemplo donde iran las queries del proyecto.
//
// Versión:
//
// Fecha: 04/10/2021
//
// Autor: Carlos Becker Robles
//
// --------------------------------------------------------------------------

#include "CDatabase.hpp"


class CDatabaseExample : public CDatabase
{
public:

	enum class prediction_type : int32_t {
		default_prediction = 0,
		consumption_kWh = 1,
		temeperature_ref = 2
	};

	CDatabaseExample();
	~CDatabaseExample();

	/**
	* Inserts a prediction
	*
	* @param[in] initialDate			Date of prediction
	* @param[in] pred_type				Type of prediction
	* @param[in] useAutoIncrement		Defines wether query uses auto increment or not
	*
	* @return id of prediction
	*/
	int insertPrediction(const uint64_t & initialDate, const prediction_type & pred_type = prediction_type::default_prediction, const bool useAutoIncrement = true);

	/**
	* Gets prosumer mpan
	*
	* @param[in] prosumer_id			Prosumer ID
	* @param[in] mpan					MPAN string reference
	*
	* @return Success or failure
	*/
	bool getProsumerMPAN(const int32_t& prosumer_id, std::string& mpan);

	/**
	* Updates prosumer mpan
	*
	* @param[in] prosumer_id			Prosumer ID
	* @param[in] mpan					MPAN
	*
	* @return Success or failure
	*/
	bool updateProsumerMPAN(const int32_t& prosumer_id, const std::string& mpan);

private:

};
