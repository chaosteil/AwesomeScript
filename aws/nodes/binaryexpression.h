#ifndef AWS_NODES_BINARYEXPRESSION_H
#define AWS_NODES_BINARYEXPRESSION_H

/*
 * aws/nodes/binaryexpression.h
 * AwesomeScript Binary Expression
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include "expression.h"

namespace AwS{
	namespace Nodes{
		class BinaryExpression : public Expression{
			public:
				BinaryExpression(Expression* left = NULL, Expression* right = NULL)
					: Expression(), _left(left), _right(right){
						
					std::cout << "Binary Expression" << std::endl;
				}
				virtual ~BinaryExpression(){
					if(_left)delete _left;
					if(_right)delete _right;
				}

				const Expression* getLeft() const{ return _left; }
				const Expression* getRight() const{ return _right; }
			private:
				const Expression* _left;
				const Expression* _right;
		};
	};
};

#endif
