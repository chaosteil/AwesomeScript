#ifndef AWS_NODES_VARIABLE_H
#define AWS_NODES_VARIABLE_H

/*
 * aws/nodes/variable.h
 * AwesomeScript Variable
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include <string>

#include "expression.h"

namespace AwS{
	namespace Nodes{
		class Variable : public Expression{
			public:
				Variable(const std::string& name, Expression* index = NULL)
					: Expression(), _name(name), _index(index){
					
					std::cout << "Variable " << name << std::endl;
				}
				virtual ~Variable(){
					if(_index)delete _index;
				}
				
				const std::string& getName() const{ return _name; }
				const Expression* getIndex() const{ return _index; }
			private:
				const std::string _name;
				const Expression* _index;
		};
	};
};

#endif
