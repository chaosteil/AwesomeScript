#ifndef AWS_NODES_MODULUSEQUAL_H
#define AWS_NODES_MODULUSEQUAL_H

/*
 * aws/nodes/modulusequal.h
 * AwesomeScript Modulus Equal
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include <string>

#include "expression.h"
#include "statement.h"
#include "variable.h"

namespace AwS{
	namespace Nodes{
		class ModulusEqual : public Statement{
			public:
				ModulusEqual(const Variable* var, const Expression* value)
					: Statement(), _var(var), _value(value){
				}
				virtual ~ModulusEqual(){
					if(_var)delete _var;
					if(_value)delete _value;
				}
				
				const Variable* getVariable() const{ return _var; }
				const Expression* getValue() const{ return _value; }

				void translatePhp(std::ostream& output, TranslateSettings& settings) const throw(NodeException){
					_var->translatePhp(output, settings);
					output << " %= ";
					_value->translatePhp(output, settings);
					if(!settings.isIgnoreSemicolon())
						output << ";" << std::endl;
				}
			private:
				const Variable* _var;
				const Expression* _value;
		};
	};
};

#endif
