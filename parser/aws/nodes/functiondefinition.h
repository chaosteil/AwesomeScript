#ifndef AWS_NODES_FUNCTIONDEFINITION_H
#define AWS_NODES_FUNCTIONDEFINITION_H

/*
 * aws/nodes/functiondefinition.h
 * AwesomeScript Function Definition
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include <list>
#include <string>

#include "statement.h"
#include "variable.h"

namespace AwS{
	namespace Nodes{
		class FunctionDefinition : public Statement{
			public:
				FunctionDefinition(const std::string& name, std::list<Variable*>* variables,
					Statement* content)
					: Statement(), _name(name), _variables(variables), _content(content){
						
					std::cout << "Function " << name << std::endl;
				}
				virtual ~FunctionDefinition(){
					if(_variables){
						for(std::list<Variable*>::iterator i = _variables->begin(); i != _variables->end(); ++i){
							if(*i)delete *i;
						}
						delete _variables;
					}
				}

				const std::string& getName() const { return _name; } 
				const Statement* getContent() const{ return _content; }
			private:
				const std::string _name;
				std::list<Variable*>* _variables;
				const Statement* _content;
		};
	};
};

#endif
