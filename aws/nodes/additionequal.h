#ifndef AWS_NODES_ADDITIONEQUAL_H
#define AWS_NODES_ADDITIONEQUAL_H

/*
 * aws/nodes/additionequal.h
 * AwesomeScript Addition Equal
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include <string>

#include "expression.h"
#include "statement.h"
#include "variable.h"

namespace AwS{
	namespace Nodes{
		class AdditionEqual : public Statement{
			public:
				AdditionEqual(const Variable* var, const Expression* value)
					: Statement(), _var(var), _value(value){
					
					std::cout << "AdditionEqual" << std::endl;
				}
				virtual ~AdditionEqual(){
					if(_var)delete _var;
					if(_value)delete _value;
				}
				
				const Variable* getVariable() const{ return _var; }
				const Expression* getValue() const{ return _value; }

				void translatePhp(std::ostream& output, TranslateSettings& settings) const throw(NodeException){
				}
			private:
				const Variable* _var;
				const Expression* _value;
		};
	};
};

#endif
