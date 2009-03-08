/*
 * aws/translator.cpp
 * AwesomeScript Translator
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

/* ==============================================================================
 * T R A N S L A T O R   CLASS
 * ============================================================================*/

#include <list>
#include <sstream>

#include "translator.h"
#include "aws.h"
#include "nodes/nodes.h"

using namespace AwS;

/* ==============================================================================
 * P U B L I C   M E T H O D S
 * ============================================================================*/

Translator::Translator(std::istream& input, std::ostream& output)
	: _input(input), _output(output){
}

Translator::~Translator(){
}

void Translator::translate(){
	Exception memoryerror(Exception::DefaultError, 0, "Not enough memory. Please try again later.");

	std::list<AwS::Nodes::Statement*>* statements = new std::list<AwS::Nodes::Statement*>();
	if(!statements){ _writeException(memoryerror); return; }

	try{
		AwS::Parser* parser = new AwS::Parser(_input);
		if(!parser){ _writeException(memoryerror); return; }

		// Parsing the main statements
		for(;;){
			AwS::Nodes::Statement* statement = parser->readStatement();
			
			if(statement)
				statements->push_back(statement);
			else
				break;
		}
	}catch(Exception e){
		// Error happened, we quit
		for(std::list<AwS::Nodes::Statement*>::iterator i = statements->begin(); i != statements->end(); ++i){
			delete *i; *i = NULL;
		}
		
		_writeException(e);
		return;
	}

	std::stringstream* code = new std::stringstream();
	if(!code){ _writeException(memoryerror); return; }
	
	*code << "<?" << std::endl;
	for(std::list<AwS::Nodes::Statement*>::iterator i = statements->begin(); i != statements->end(); ++i){
		if(*i){
			(*i)->translatePhp(*code, _settings);
			delete *i; *i = NULL;
		}else{
			_writeException(memoryerror);
			return;
		}
	}
	*code << "?>" << std::endl;
	
	// Everything went fine!
	Exception noerror(Exception::None, 0, "");
	_writeException(noerror);
	_output << code->str();
}

/* ==============================================================================
 * P R I V A T E   M E T H O D S
 * ============================================================================*/

void Translator::_writeException(const Exception& exception){
	_output << exception.getId() << std::endl;
	_output << exception.getMessage() << std::endl;
}
