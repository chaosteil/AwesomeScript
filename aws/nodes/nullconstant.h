#ifndef AWS_NODES_NULLCONSTANT_H
#define AWS_NODES_NULLCONSTANT_H

/*
 * aws/nodes/nullconstant.h
 * AwesomeScript Null Constant
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include "expression.h"

namespace AwS{
	namespace Nodes{
		class NullConstant : public Expression{
			public:
				NullConstant()
					: Expression(){
				}
				virtual ~NullConstant(){}
				
				void translatePhp(std::ostream& output, TranslateSettings& settings) const throw(NodeException){
					output << "null";
				}
		};
	};
};

#endif
