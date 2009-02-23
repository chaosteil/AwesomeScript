#ifndef AWS_NODES_VARIABLEDECLARATION_H
#define AWS_NODES_VARIABLEDECLARATION_H

/*
 * aws/nodes/variabledeclaration.h
 * AwesomeScript Variable Declaration
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include <list>

#include "assignment.h"
#include "statement.h"

namespace AwS{
	namespace Nodes{
		class VariableDeclaration : public Statement{
			public:
				VariableDeclaration(std::list<Assignment*>* content)
					: Statement(), _content(content){
						
					std::cout << "VariableDeclaration" << std::endl;
				}
				virtual ~VariableDeclaration(){
					if(_content){
						for(std::list<Assignment*>::iterator i = _content->begin(); i != _content->end(); ++i){
							if(*i)delete *i;
						}
						delete _content;
					}
				}

				const std::list<Assignment*>& getContent() const{ return *_content; }
			private:
				std::list<Assignment*>* _content;
		};
	};
};

#endif
