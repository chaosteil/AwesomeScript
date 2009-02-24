/*
 * aws/token.cpp
 * AwesomeScript Token
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

/* ==============================================================================
 * T O K E N   CLASS
 * ============================================================================*/

#include <iostream>

#include "token.h"

using namespace AwS;

/* ==============================================================================
 * P U B L I C   M E T H O D S
 * ============================================================================*/

Token::Token(TokenType type, const std::string& value, const long& line)
	: _type(type), _value(value), _line(line)
{
}

Token::~Token(){
}

const Token::TokenType& Token::getType() const{
	return _type;
}
const std::string& Token::getValue() const{
	return _value;
}

const long& Token::getLine() const{
	return _line;
}

bool Token::is(TokenType type, const std::string& value) const{
	if(type == _type && (_value.compare(value) == 0))
		return true;
	else
		return false;
}

bool Token::operator==(const Token& token) const{
	// Type and value have to be equal
	if(_type == token.getType() && _value.compare(token.getValue()) == 0)
		return true;
	else
		return false;
}

bool Token::operator!=(const Token& token) const{
	return !operator==(token);
}
