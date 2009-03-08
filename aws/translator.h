#ifndef AWS_TRANSLATOR_H
#define AWS_TRANSLATOR_H

/*
 * aws/translator.h
 * AwesomeScript Translator
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include <iostream>
#include <string>
#include "exception.h"
#include "nodes/translatesettings.h"

namespace AwS{
	class Translator{
		public:
			Translator(std::istream& input, std::ostream& output, const std::string& varprefix = "", const std::string& funcprefix = "");
			~Translator();

			void translate();
		private:
			void _writeException(const Exception& exception);

			std::istream& _input;
			std::ostream& _output;
			Nodes::TranslateSettings _settings;
	};
};

#endif
