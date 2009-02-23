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
					
					std::cout << "Boolean " << content << std::endl;
				}
				virtual ~BooleanConstant(){}

				bool getContent() const{return _content; }
			private:
				const bool _content;
		};
	};
};

#endif
