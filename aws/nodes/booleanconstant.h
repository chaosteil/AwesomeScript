#ifndef AWS_NODES_BOOLEANCONSTANT_H
#define AWS_NODES_BOOLEANCONSTANT_H

/*
 * aws/nodes/booleanconstant.h
 * AwesomeScript Boolean Constant
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include "expression.h"

namespace AwS{
	namespace Nodes{
		class BooleanConstant : public Expression{
			public:
				BooleanConstant(bool content)
					: Expression(), _content(content){
				}
				virtual ~BooleanConstant(){}

				bool getContent() const{return _content; }

				void translatePhp(std::ostream& output, TranslateSettings& settings) const throw(NodeException){
					if(_content == true){
						output << "true";
					}else{
						output << "false";
					}
				}
			private:
				const bool _content;
		};
	};
};

#endif
