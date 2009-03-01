#ifndef AWS_NODES_NEGATION_H
#define AWS_NODES_NEGATION_H

/*
 * aws/nodes/negation.h
 * AwesomeScript Negation
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include "unaryexpression.h"

namespace AwS{
	namespace Nodes{
		class Negation : public UnaryExpression{
			public:
				Negation(Expression* value)
					: UnaryExpression(value){
						
					std::cout << "Negation" << std::endl;
				}
				virtual ~Negation(){}

				void translatePhp(std::ostream& output, TranslateSettings& settings) const throw(NodeException){
				}
		};
	};
};

#endif
