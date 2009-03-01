#ifndef AWS_NODES_DECREASE_H
#define AWS_NODES_DECREASE_H

/*
 * aws/nodes/decrease.h
 * AwesomeScript Decrease
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include <string>

#include "statement.h"
#include "variable.h"

namespace AwS{
	namespace Nodes{
		class Decrease : public Statement{
			public:
				Decrease(const Variable* var)
					: Statement(), _var(var){
					
					std::cout << "Decrease" << std::endl;
				}
				virtual ~Decrease(){
					if(_var)delete _var;
				}
				
				const Variable* getVariable() const{ return _var; }

				void translatePhp(std::ostream& output, TranslateSettings& settings) const throw(NodeException){
					_var->translatePhp(output, settings);
					output << "--";
					output << ";" << std::endl;
				}
			private:
				const Variable* _var;
		};
	};
};

#endif
