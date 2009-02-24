#ifndef AWS_NODES_SUBSTRACTIONEQUAL_H
#define AWS_NODES_SUBSTRACTIONEQUAL_H

/*
 * aws/nodes/substractionequal.h
 * AwesomeScript Substraction Equal
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include <string>

#include "expression.h"
#include "statement.h"
#include "variable.h"

namespace AwS{
	namespace Nodes{
		class SubstractionEqual : public Statement{
			public:
				SubstractionEqual(const Variable* var, const Expression* value)
					: Statement(), _var(var), _value(value){
					
					std::cout << "SubstractionEqual" << std::endl;
				}
				virtual ~SubstractionEqual(){
					if(_var)delete _var;
					if(_value)delete _value;
				}
				
				const Variable* getVariable() const{ return _var; }
				const Expression* getValue() const{ return _value; }
			private:
				const Variable* _var;
				const Expression* _value;
		};
	};
};

#endif
