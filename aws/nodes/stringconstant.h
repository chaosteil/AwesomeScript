#ifndef AWS_NODES_STRINGCONSTANT_H
#define AWS_NODES_STRINGCONSTANT_H

/*
 * aws/nodes/stringconstant.h
 * AwesomeScript String Constant
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include <string>

#include "expression.h"

namespace AwS{
	namespace Nodes{
		class StringConstant : public Expression{
			public:
				StringConstant(const std::string& content)
					: Expression(), _content(content){
						
					std::cout << "String " << _content << std::endl;
				}
				virtual ~StringConstant(){}

				const std::string& getContent() const{return _content; }

				void translatePhp(std::ostream& output, TranslateSettings& settings) const throw(NodeException){
					output << "'" << _content << "'";
				}
			private:
				const std::string _content;
		};
	};
};

#endif
