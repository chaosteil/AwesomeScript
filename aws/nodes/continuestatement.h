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
				}
				virtual ~ContinueStatement(){}

				void translatePhp(std::ostream& output, TranslateSettings& settings) const throw(NodeException){
					output << "continue;";
				}
		};
	};
};

#endif
