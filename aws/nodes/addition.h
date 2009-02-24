#ifndef AWS_NODES_ADDITION_H
#define AWS_NODES_ADDITION_H

/*
 * aws/nodes/addition.h
 * AwesomeScript Addition
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include "binaryexpression.h"

namespace AwS{
	namespace Nodes{
		class Addition : public BinaryExpression{
			public:
				Addition(Expression* left, Expression* right)
					: BinaryExpression(left, right){

					std::cout << "Addition" << std::endl;
				}
				virtual ~Addition(){}
		};
	};
};

#endif
