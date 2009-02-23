#ifndef AWS_NODES_MULTIPLICATION_H
#define AWS_NODES_MULTIPLICATION_H

/*
 * aws/nodes/multiplication.h
 * AwesomeScript Multiplication
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include "binaryexpression.h"

namespace AwS{
	namespace Nodes{
		class Multiplication : public BinaryExpression{
			public:
				Multiplication(Expression* left, Expression* right)
					: BinaryExpression(left, right){
						
					std::cout << "Multiplication" << std::endl;
				}
				virtual ~Multiplication(){}
		};
	};
};

#endif
