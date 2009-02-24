#ifndef AWS_NODES_COMPARISON_H
#define AWS_NODES_COMPARISON_H

/*
 * aws/nodes/comparison.h
 * AwesomeScript Comparison
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include "binaryexpression.h"

namespace AwS{
	namespace Nodes{
		class Comparison : public BinaryExpression{
			public:
				enum Operator{
					None = 0,
					Equal,
					NotEqual,
					Less,
					Greater,
					LessEqual,
					GreaterEqual
				};
				Comparison(Operator op, Expression* left, Expression* right)
					: BinaryExpression(left, right), _op(op){
					
					std::cout << "Comparison" << std::endl;
				}
				virtual ~Comparison(){}

				Operator getOperator() const{ return _op; }
			private:
				Operator _op;
		};
	};
};

#endif
