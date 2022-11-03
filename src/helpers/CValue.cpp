#include "CValue.h"
#include <iostream>

using namespace std;

//Constructors
CValue::CValue() {
	value = 0;
	date = time(0);
	cout << "Default creation" << endl;
}

CValue::CValue(float v, time_t d) {
	value = v;
	date = d;
	cout << "I have been created with value = " << value
		<< " and date = " << date << endl;
}

CValue::CValue(const CValue& c) {
	*this = c;
	cout << "I have been created with value = " << c.value
		<< " and date = " << c.date << endl;
}

//Destructor
CValue::~CValue() {
	cout << "I have been deleted value = " << value
		<< " and date = " << date << endl;
}

//Functions for member access
float CValue::getValue() const {
	return value;
}

time_t CValue::getDate() const{
	return date;
}

//Operator= por referencia
void CValue::operator=(const CValue& c) {
	value = c.value;
	date = c.date;
	cout << "Operator= por referencia" << endl;
}

//Operator<<
ostream& operator<<(ostream& o, const CValue& c) {
	o << "Value: " << c.value << ",the day: " << c.date << ".";
	return o;
}

//Operator>>
istream& operator>>(istream& is, CValue& c) {
	is >> c.value;
	is >> c.date;
	return is;
}

//Operator+=
void CValue::operator+=(const CValue& c) {
	value = value + c.value;
	cout << "Operator+= por referencia" << endl;
}

//Operator+ un solo valor enviado
CValue CValue::operator+(const CValue& c) {
	CValue t = *this;
	t += c; //Guardamos la fecha del de la izq
	cout << "Operator+ por referencia" << endl;
	return t;
}

CValue operator+(const CValue& c1, const CValue& c2) {
	CValue t;
	t.value = c1.value + c2.value;
	t.date = c1.date + c2.date;
	cout << "Operator+ friend" << endl;
	return t;
}