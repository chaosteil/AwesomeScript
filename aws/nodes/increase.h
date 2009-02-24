#ifndef AWS_NODES_INCREASE_H
#define AWS_NODES_INCREASE_H

/*
 * aws/nodes/increase.h
 * AwesomeScript Increase
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include <string>

#include "statement.h"
#include "variable.h"

namespace AwS{
	namespace Nodes{
		class Increase : public Statement{
			public:
				Increase(const Variable* var)
					: Statement(), _var(var){
						
					std::cout << "Increase" << std::endl;
				}
				virtual ~Increase(){
					if(_var)delete _var;
				}
				
				const Variable* getVariable() const{ return _var; }
			private:
				const Variable* _var;
		};
	};
};

#endif
