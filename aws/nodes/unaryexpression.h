#ifndef AWS_NODES_UNARYEXPRESSION_H
#define AWS_NODES_UNARYEXPRESSION_H

/*
 * aws/nodes/unaryexpression.h
 * AwesomeScript Unary Expression
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include "expression.h"

namespace AwS{
	namespace Nodes{
		class UnaryExpression : public Expression{
			public:
				UnaryExpression(Expression* value)
					: Expression(), _value(value){
				}
				virtual ~UnaryExpression(){
					if(_value)delete _value;
				}

				const Expression* getValue() const{ return _value; }
			private:
				const Expression* _value;
		};
	};
};

#endif
