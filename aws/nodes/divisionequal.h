#ifndef AWS_NODES_DIVISIONEQUAL_H
#define AWS_NODES_DIVISIONEQUAL_H

/*
 * aws/nodes/divisionequal.h
 * AwesomeScript Division Equal
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include <string>

#include "expression.h"
#include "statement.h"
#include "variable.h"

namespace AwS{
	namespace Nodes{
		class DivisionEqual : public Statement{
			public:
				DivisionEqual(const Variable* var, const Expression* value)
					: Statement(), _var(var), _value(value){
					
					std::cout << "DivisionEqual" << std::endl;
				}
				virtual ~DivisionEqual(){
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