/*
 * main.cpp
 * AwesomeScript Translator
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

/* ==============================================================================
 * I N C L U D E S
 * ============================================================================*/

#include <ostream>
#include <istream>
#include <fstream>
#include <iostream>
#include <queue>
#include <time.h>

#include "aws/aws.h"
#include "aws/nodes/translatesettings.h"

/* ==============================================================================
 * F U N C T I O N S
 * ============================================================================*/

void help(int argc, const char** argv){
	std::cout << "AwsesomeScript translator" << std::endl;
	std::cout << "Converts awesomescript into php code" << std::endl << std::endl;
	std::cout << "Please specify a string to parse." << std::endl;
	std::cout << "Usage: " << argv[0] << "[-i] string" << std::endl;
}

/* ==============================================================================
 * M A I N
 * ============================================================================*/
int main(int argc, const char** argv){
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
		if(!parsing){
			std::cerr << "Memory Error" << std::endl;
			return 3;
		}
	}else if(argc == 2){
		// Get from the file
		parsing = new std::ifstream(argv[1]);
		if(!parsing){
			std::cerr << "Memory Error" << std::endl;
			return 3;
		}

		if(!static_cast<std::ifstream*>(parsing)->is_open() ||
			static_cast<std::ifstream*>(parsing)->fail()){

			std::cerr << "File \"" << argv[1] << "\" does not exist or could not be read." << std::endl;
			return 2; 
		}
	}else{
		help(argc, argv);
		return 1;
	}

	// Test output
	std::ofstream outfile("test.php");
	AwS::Translator translator(*parsing, outfile);
	translator.translate();
	outfile.close();

	// close and remove the stream
	delete parsing;

	// Done
	return 0;
}
