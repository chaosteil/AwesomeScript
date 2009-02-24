#ifndef AWS_NODES_RETURNSTATEMENT_H
#define AWS_NODES_RETURNSTATEMENT_H

/*
 * aws/nodes/returnstatement.h
 * AwesomeScript Return Statement
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include "statement.h"

namespace AwS{
	namespace Nodes{
		class ReturnStatement : public Statement{
			public:
				ReturnStatement(const Expression* content)
					: Statement(), _content(content){
						
					std::cout << "ReturnStatement" << std::endl;
				}
				virtual ~ReturnStatement(){
					if(_content)delete _content;
				}

				const Expression* getContent() const{ return _content; }
			private:
				const Expression* _content;
		};
	};
};

#endif