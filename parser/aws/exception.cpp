/*
 * aws/exception.cpp
 * AwesomeScript Exception
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

/* ==============================================================================
 * E X C E P T I O N   CLASS
 * ============================================================================*/

#include "exception.h"

using namespace AwS;

/* ==============================================================================
 * P U B L I C   M E T H O D S
 * ============================================================================*/

Exception::Exception(ExceptionType type, std::string message)
	: _type(type), _message(message)
{
}

Exception::~Exception(){
}

const std::string& Exception::getMessage() const{
	return _message;
}
Exception::ExceptionType Exception::getType() const{
	return _type;
}
