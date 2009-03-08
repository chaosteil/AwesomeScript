/*
 * aws/tokenizer.cpp
 * AwesomeScript Tokenizer
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

/* ==============================================================================
 * T O K E N I Z E R   CLASS
 * ============================================================================*/

#include "tokenizer.h"

using namespace AwS;

/* ==============================================================================
 * P U B L I C   M E T H O D S
 * ============================================================================*/

Tokenizer::Tokenizer(std::istream& input)
	: _input(input), _currentChar('\0'), _currentLine(1), _tokenValueBuffer(""),
	_commentState(Comment_None)
{
	// Start with the first character
	_readNextChar();
}

Tokenizer::~Tokenizer(){
}

Token* Tokenizer::readNextToken(){
	// First we skip every characters which are considered whitespace
	_skipWhitespace();
	
	// Check if it the end of the file
	if(_isFinished())
		return NULL;

	// Check if it's a word, for example a variable, or a reserved word
	if(_isLetter(_currentChar) || (_currentChar == '_'))
		return _readWord();
	
	// Check if it's a number
	if(_isNumber(_currentChar))
		return _readNumber();

	// Check for string start
	if(_currentChar == '\'')
		return _readString();
	
	//Check if it's a symbol
	if(_isSymbol(_currentChar)){
		Token* token = _readSymbol();
		// If comment state is off, return token normally
		if(_commentState == Comment_None)
			return token;
		else{
			// Delete current token
			if(token)
				delete token; token = NULL;
			// Skip the current comment
			_skipComment();
			// Return back empty token, so parser goes through
			// the function again
			return new Token(Token::Skip, "", _currentLine);
		}
	}
	
	// absolute fail
	return NULL;
}

/* ==============================================================================
 * P R I V A T E   M E T H O D S
 * ============================================================================*/

Token* Tokenizer::_readWord(){	
	do{
		// we found the character when we entered the method
		// read the next one until
		_storeCharReadNext();
		// it's not a letter, nor a number or underscore
	}while(_isLetter(_currentChar) || _isNumber(_currentChar) ||
		(_currentChar == '_'));

	// Check if the token is a boolean
	std::string extracted = _extractString();
	if(extracted.compare("true") == 0 || extracted.compare("false") == 0)
		return new Token(Token::Boolean, extracted, _currentLine);
	// Check if the token is null
	if(extracted.compare("null") == 0)
		return new Token(Token::Null, extracted, _currentLine);

	// return the current word
	return new Token(Token::Word, extracted, _currentLine);
}

Token* Tokenizer::_readNumber(){
	bool floatingPoint = false;

	do{
		// store the current number
		_storeCharReadNext();

		// check for floating point
		if(!floatingPoint && _currentChar == '.'){
			floatingPoint = true;
			_storeCharReadNext();
			// next char HAS to be a number
			if(_isNumber(_currentChar)){
				_storeCharReadNext();
			}else{
				_invalidCharacter(); // no number!
			}
		}
		// until the next char isn't a number
	}while(_isNumber(_currentChar));

	// return the current number
	return new Token(Token::Number, _extractString(), _currentLine);
}

Token* Tokenizer::_readString(){
	// skip '
	_readNextChar();

	// We mark only the start of the string
	long line = _currentLine;

	while(!_isFinished() && _currentChar != '\''){
		if(_currentChar == '\n')
			_currentLine++;
		// Check if escaping started
		if(_currentChar == '\\'){
			_storeCharReadNext();
			
			// Check if it's the wanted escape char
			if(_currentChar == '\'')
				_storeCharReadNext();
			else
				continue;
		}else
			// normal letter
			_storeCharReadNext();
	}

	_checkUnexpectedEnd(); // the string wasn't complete
	// Now the char is ', so we skip it
	_readNextChar();

	// return the string (without ')
	return new Token(Token::String, _extractString(), line);
}

Token* Tokenizer::_readSymbol(){
	switch(_currentChar){
		// single characters
		case '(': case ')':
		case '{': case '}':
		case '[': case ']':
		case '.':
		case ',':
		case ';':
		case ':':
			_storeCharReadNext();
			break;

		case '&':
			_storeCharReadNext();
			if(_currentChar == '&'){
				_storeCharReadNext();
				break;
			}else{
				_invalidCharacter();
				return NULL;
			}

		case '|':
			_storeCharReadNext();
			if(_currentChar == '|'){
				_storeCharReadNext();
				break;
			}else{
				_invalidCharacter();
				return NULL;
			}

		// either single characters, ending with = or double 
		case '+':
			_storeCharReadNext();
			if(_currentChar == '=' || _currentChar == '+')
				_storeCharReadNext();
			break;
		case '-':
			_storeCharReadNext();
			if(_currentChar == '=' || _currentChar == '-')
				_storeCharReadNext();
			break;

		// either single characters or ending with =
		case '*':
		case '%':
		case '!':
		case '=':
		case '<': case '>':
			_storeCharReadNext();
			if(_currentChar == '=')
				_storeCharReadNext();
			break;

		// Comment OR division
		case '/': 
			_storeCharReadNext();
			if(_currentChar == '/'){
				_commentState = Comment_Line;
			}else if(_currentChar == '*'){
				_commentState = Comment_Block;
			}else if(_currentChar == '='){
				_storeCharReadNext();
			}
			break;

		// No valid symbol
		default:
			_checkUnexpectedEnd();
			_invalidCharacter();
			return NULL;
	}

	// return current symbol
	return new Token(Token::Symbol, _extractString(), _currentLine);
}

void Tokenizer::_readNextChar(){
	// Read char, if failed, stop reading
	_input.get(_currentChar);
	if(!_input.good())_currentChar = '\0';
}

bool Tokenizer::_isFinished(){
	// Check if we have extracted the last character
	if(_currentChar == '\0')
		return true;
	else
		return false;
}

void Tokenizer::_skipWhitespace(){
	// Skip all chars which shouldn't be detected by the parser
	while(_currentChar <= ' ' && _currentChar > '\0'){
		if(_currentChar == '\n')
			_currentLine++;
		_readNextChar();
	}
}

void Tokenizer::_skipComment(){
	// Check if we are in a comment state
	if(_commentState == Comment_None)return;
		
	// Skip everything until end of line
	if(_commentState == Comment_Line){
		while(!_isFinished() && _currentChar != '\n')
			_readNextChar();
		_commentState = Comment_None;
	
	// Skip everything until */ is found
	}else if(_commentState == Comment_Block){
		while(!_isFinished()){
			_readNextChar();
			if(_currentChar == '\n')
				_currentLine++;
			if(_currentChar == '*'){
				_readNextChar();
				if(_currentChar == '/'){
					_readNextChar();
					break;
				}
			}
		}
		_checkUnexpectedEnd();
		_commentState = Comment_None;
	}
}

void Tokenizer::_storeCharReadNext(){
	// Add char to stream and read the next one
	_tokenValueBuffer << _currentChar;
	_readNextChar();
}

std::string Tokenizer::_extractString(){
	// Retrieves everything in the buffer and clears it
	std::string value;
	value.assign(_tokenValueBuffer.str());
	_tokenValueBuffer.str(""); // Only correct way to clear stringstream?

	return value;
}

void Tokenizer::_checkUnexpectedEnd() throw(Exception){
	if(_isFinished()){
		throw Exception(Exception::ReadingError, static_cast<int>(EndOfSource), "Unexpected end of source.");
	}
}

void Tokenizer::_invalidCharacter() throw(Exception){
	std::stringstream errormessage;

	// If nothing is in the buffer, we just write the character
	if(_tokenValueBuffer.str().length() == 0){
		errormessage << "Invalid character '" << _currentChar << "'" << std::endl;
		throw Exception(Exception::ReadingError, static_cast<int>(InvalidCharacter), errormessage.str());
	}else{
		// We write what's in the buffer and our invalid char
		errormessage << "Invalid character '" << _currentChar << "' after '" << _tokenValueBuffer.str() << "'" << std::endl;
		throw Exception(Exception::ReadingError, static_cast<int>(InvalidCharacter), errormessage.str());
	}
}

/* ==============================================================================
 * S T A T I C   P R I V A T E   M E T H O D S
 * ============================================================================*/

bool Tokenizer::_isNumber(char letter){
	if(letter >= '0' && letter <= '9')
		return true;
	return false;
}

bool Tokenizer::_isSymbol(char letter){
	// ASCII characters
	if((letter >= 33 && letter <= 47) ||
		(letter >= 58 && letter <= 64) || 
		(letter >= 91 && letter <= 96) ||
		(letter >= 123 && letter <= 126))
		return true;

	return false;
}

bool Tokenizer::_isLetter(char letter){
	if(_isLowercaseLetter(letter) || _isUppercaseLetter(letter))
		return true;
	return false;
}

bool Tokenizer::_isLowercaseLetter(char letter){
	if(letter >= 'a' && letter <= 'z')
		return true;
	return false;
}

bool Tokenizer::_isUppercaseLetter(char letter){
	if(letter >= 'A' && letter <= 'Z')
		return true;
	return false;
}
