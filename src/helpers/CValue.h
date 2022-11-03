#pragma once
#include <time.h>
#include <iostream>


class CValue {
private:
	float value;
	time_t date;

public:
	//Defaut Constructor
	CValue();
	//Constructor
	CValue(float value, time_t date);
	//Constructor por copia
	CValue(const CValue& c);
	//Destructor
	~CValue();

	//Access functions
	float getValue() const;
	time_t getDate() const;

	//Operator= por referencia
	void operator=(const CValue& c);

	//Operator<<
	friend std::ostream& operator<<(std::ostream& o, const CValue& c);

	//Operator>>
	friend std::istream& operator>>(std::istream& is, CValue& c);

	//Operator+=
	void operator+=(const CValue& c);

	//Operator+ 1 solo objeto enviado
	CValue operator+(const CValue& c);

	//Operator+ friend
	friend CValue operator+(const CValue& c1, const CValue& c2);
};