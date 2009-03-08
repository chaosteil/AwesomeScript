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

/* ==============================================================================
 * F U N C T I O N S
 * ============================================================================*/

void help(int argc, const char** argv){
	std::cerr << "AwsesomeScript translator" << std::endl;
	std::cerr << "Converts awesomescript into php code" << std::endl <<
				 std::endl;
	std::cerr << "Please specify a string to parse." << std::endl;
	std::cerr << "Usage: " << argv[0] << " [-h] [-v prefix] [-f prefix] [-i string|filename] [output]" << std::endl <<
				 std::endl;
	std::cerr << "\tIf filename or output are \"-\" or nothing, then the" << std::endl <<
				 "\tstandard input/output is used for the specified stream (for piping)." << std::endl;
}

void noMemory(){
	std::cerr << "Not enough memory on initialization. Abort." << std::endl;
}

void invalidParam(){
	std::cerr << "Invalid parameters. Abort." << std::endl;
}

/* ==============================================================================
 * M A I N
 * ============================================================================*/
int main(int argc, const char** argv){
	// Get where the input and output does come from
	std::istream* input = NULL;
	std::ostream* output = NULL;
	// Prefixes
	std::string varprefix("");
	std::string funcprefix("");

	// Are there any additional arguments specified?
	if(argc > 1){
		int currentarg = 1;
		// PREFIXES
		// Variable prefixes
		if((strncmp(argv[currentarg], "-v", 3) == 0) || (strncmp(argv[currentarg], "--var", 6) == 0)){
			if(argc > currentarg+1){
				currentarg++;
				varprefix = argv[currentarg];
				currentarg++;
			}else{
				noMemory();
				return 2;
			}
		}
		// Function prefixes
		if((strncmp(argv[currentarg], "-f", 3) == 0) || (strncmp(argv[currentarg], "--func", 7) == 0)){
			if(argc > currentarg+1){
				currentarg++;
				funcprefix = argv[currentarg];
				currentarg++;
			}else{
				invalidParam();
				return 2;
			}
		}

		// INPUT
		// We print help and immediately exit
		if((strncmp(argv[currentarg], "-h", 3) == 0) || (strncmp(argv[currentarg], "--help", 7) == 0)){
			help(argc, argv);
			return 0;

		// We use standard input (piping)
		}else if(strncmp(argv[currentarg], "-", 2) == 0){
			currentarg++;

		// Use a simple input string
		}else if((strncmp(argv[currentarg], "--input", 8) == 0) || (strncmp(argv[currentarg], "-i", 3) == 0)){
			if(argc > currentarg+1){
				currentarg++;
				input = new std::stringstream();
				if(!input){
					noMemory();
					return 1;
				}
				*static_cast<std::stringstream*>(input) << argv[currentarg];
				currentarg++;
			}else{
				invalidParam();
				return 2;
			}

		// Nothing left, we use a file
		}else{
			input = new std::ifstream(argv[currentarg]);
			if(!input){
				noMemory();
				return 1;
			}
			if(!static_cast<std::ifstream*>(input)->is_open() ||
				static_cast<std::ifstream*>(input)->fail()){
				
				std::cerr << "File \"" << currentarg << "\" does not exist or could not be read." << std::endl;
				return 3;
			}
			currentarg++;
		}

		// OUTPUT
		if(argc >= currentarg){
			if(strncmp(argv[currentarg], "-", 2) == 0){
				// Nothing
				currentarg++;

			// We use a file
			}else{
				output = new std::ofstream(argv[currentarg]);
				if(!output){
					noMemory();
					return 1;
				}
				currentarg++;
			}
		}
	}

	// Use standard input/output if not specified
	if(input == NULL){
		input = &std::cin;
	}
	if(output == NULL){
		output = &std::cout;
	}

	// Test output
	AwS::Translator translator(*input, *output, varprefix, funcprefix);
	translator.translate();

	// Clear up memory if standard input/output wasn't used
	if(input != &std::cin){
		delete input; input = NULL;
	}
	if(output != &std::cout){
		delete output; output = NULL;
	}

	// Done
	return 0;
}
