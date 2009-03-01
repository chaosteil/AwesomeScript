#ifndef AWS_NODES_SUBSTRACTION_H
#define AWS_NODES_SUBSTRACTION_H

/*
 * aws/nodes/substraction.h
 * AwesomeScript Substraction
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */


#include "binaryexpression.h"

namespace AwS{
	namespace Nodes{
		class Substraction : public BinaryExpression{
			public:
				Substraction(Expression* left, Expression* right)
					: BinaryExpression(left, right){
						
					std::cout << "Substraction" << std::endl;
				}
				virtual ~Substraction(){}

				void translatePhp(std::ostream& output, TranslateSettings& settings) const throw(NodeException){
				}
		};
	};
};

#endif
