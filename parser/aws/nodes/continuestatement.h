#ifndef AWS_NODES_CONTINUESTATEMENT_H
#define AWS_NODES_CONTINUESTATEMENT_H

/*
 * aws/nodes/continuestatement.h
 * AwesomeScript Continue Statement
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include "statement.h"

namespace AwS{
	namespace Nodes{
		class ContinueStatement : public Statement{
			public:
				ContinueStatement()
					: Statement(){
						
					std::cout << "ContinueStatement" << std::endl;
				}
				virtual ~ContinueStatement(){}
		};
	};
};

#endif
