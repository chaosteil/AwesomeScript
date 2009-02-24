#ifndef AWS_NODES_ASSIGNMENT_H
#define AWS_NODES_ASSIGNMENT_H

/*
 * aws/nodes/assignment.h
 * AwesomeScript Assignment
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include <string>

#include "expression.h"
#include "statement.h"

namespace AwS{
	namespace Nodes{
		class Assignment : public Statement{
			public:
				Assignment(const Variable* var, const Expression* value)
					: Statement(), _var(var), _value(value){
					
					std::cout << "Assignment" << std::endl;
				}
				virtual ~Assignment(){
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