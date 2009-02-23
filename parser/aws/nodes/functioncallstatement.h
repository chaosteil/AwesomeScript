#ifndef AWS_NODES_FUNCTIONCALLSTATEMENT_H
#define AWS_NODES_FUNCTIONCALLSTATEMENT_H

/*
 * aws/nodes/functioncall.h
 * AwesomeScript Function Call Statement
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include <list>
#include <string>

#include "statement.h"
#include "functioncall.h"

namespace AwS{
	namespace Nodes{
		class FunctionCallStatement : public Statement{
			public:
				FunctionCallStatement(FunctionCall* content)
					: Statement(), _content(content){
					
					std::cout << "FunctionCallStatement" <<  std::endl;
				}
				virtual ~FunctionCallStatement(){
					if(_content)delete _content;
				}

				const FunctionCall& getFunctionCall() const{ return *_content; }
			private:
				const FunctionCall* _content;
		};
	};
};

#endif
