#include <ostream>
#include <istream>
#include <fstream>
#include <iostream>
#include <queue>
#include <time.h>

#include "aws/aws.h"

// Implementation
// * Convert source to tokens via tokenizer (V)
// * Parse all tokens (V)
// * Return valid php code

void help(int argc, const char** argv){
	std::cout << "AwsesomeScript translator" << std::endl;
	std::cout << "Converts awesomescript into php code" << std::endl << std::endl;
	std::cout << "Please specify a string to tokenize." << std::endl;
	std::cout << "Usage: " << argv[0] << " -i string" << std::endl;
	std::cout << "       " << argv[0] << " filename" << std::endl;
}

int main(int argc, const char** argv){
	// Get stream to output
	// Can be file, or currently std output
	std::ostream& output = std::cout;
	
	// Check arguments
	if(argc <= 1 || argc >= 4){
		help(argc, argv);
		return 1;
	}

	// Get where the input does come from
	std::istream* parsing = NULL;
	if(argc == 3 && ((strncmp(argv[1], "-i", 2) == 0) || (strncmp(argv[1], "--input", 8) == 0))){
		// Get everything from the arguments
		parsing = new std::stringstream(argv[2]);
	}else if(argc == 2){
		// Get from the file
		parsing = new std::ifstream(argv[1]);
		if(!static_cast<std::ifstream*>(parsing)->is_open() ||
			static_cast<std::ifstream*>(parsing)->fail()){

			std::cout << "File \"" << argv[1] << "\" does not exist or could not be read." << std::endl;
			return 2; 
		}
	}else{
		help(argc, argv);
		return 1;
	}

	// Get starting time for calculation.
	// We do it this way because the compiler mucks 
	long start = 0; start = time(NULL);
	output << "[!] Start: " << start << std::endl;

	try{
		// Parse everything
		AwS::Parser* parser = new AwS::Parser(*parsing, output);
		for(;;){
			AwS::Nodes::Statement* statement = parser->readStatement();
			if(statement)
				delete statement;
			else
				break;
		}
		// TODO build translator
		delete parser; parser = NULL;
	}catch(AwS::Exception e){
		// Based on exception error code
		output << "[!] Exception caught: " << e.getMessage() << std::endl;
	}

	// close and remove the stream
	delete parsing;

	// Calculate finishing time
	long end = time(NULL);
	output << "[!] End  : " << end << std::endl;
	output << "[!] Total: " << end-start << std::endl;

	// Done
	return 0;
}
