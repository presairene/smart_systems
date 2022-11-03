#include "CDatabase.hpp"
#include <CValue.h>


class CDatabaseLab4 : public CDatabase
{
public:

	enum class prediction_type : int32_t {
		default_prediction = 0,
		consumption_kWh = 1,
		temeperature_ref = 2
	};

	CDatabaseLab4();
	~CDatabaseLab4();

	/**
	* Inserts a prediction
	*
	* @param[in] initialDate			Date of prediction
	* @param[in] pred_type				Type of prediction
	* @param[in] useAutoIncrement		Defines wether query uses auto increment or not
	*
	* @return id of prediction
	*/
	int insertPrediction(const uint64_t& initialDate, const prediction_type& pred_type = prediction_type::default_prediction, const bool useAutoIncrement = true);

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

	//Insert an idVariable and CValue into VARIABLE_VALUE table
	bool insertValue(const int32_t& idVariable, const CValue& c);

	//Function that receives an IdVariable and returns a list of CValues
	CValue* getValues(const int32_t& idVariable);

	//Function that receives an IdVariable and a CValue to update the Database
	bool updateValue(const int32_t& idVariable, const CValue& c);

	//Function that receives an IdVariable and a DateTime and it DELETES the row from the database
	void deleteValue(const int32_t& idVariable, time_t dateTime);

private:

};


