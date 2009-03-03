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
				enum Type{
					Normal = 0, // =
					Addition, // +=
					Substraction, // -=
					Multiplication, // *=
					Division, // /=
					Modulus // %=
				};

				Assignment(const Variable* var, const Expression* value, Type type)
					: Statement(), _var(var), _value(value), _type(type){
				}
				virtual ~Assignment(){
					if(_var)delete _var;
					if(_value)delete _value;
				}
				
				const Variable* getVariable() const{ return _var; }
				const Expression* getValue() const{ return _value; }

				void translatePhp(std::ostream& output, TranslateSettings& settings) const throw(NodeException){
					_var->translatePhp(output, settings);
					if(_type == Normal){
						output << " = ";
					}else if(_type == Addition){
						output << " += ";
					}else if(_type == Substraction){
						output << " -= ";
					}else if(_type == Multiplication){
						output << " *= ";
					}else if(_type == Division){
						output << " /= ";
					}else if(_type == Modulus){
						output << " %= ";
					}else
						throw NodeException("Memory corruption.");

					if(_value)
						_value->translatePhp(output, settings);
					else
						output << "null";
					if(!settings.isIgnoreSemicolon())
						output << ";" << std::endl;
				}
			private:
				const Variable* _var;
				const Expression* _value;
				const Type _type;
		};
	};
};

#endif
