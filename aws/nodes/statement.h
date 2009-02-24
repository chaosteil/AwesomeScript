#ifndef AWS_NODES_STATEMENT_H
#define AWS_NODES_STATEMENT_H

/*
 * aws/nodes/statement.h
 * AwesomeScript Statement
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include "parsernode.h"

namespace AwS{
	namespace Nodes{
		class Statement : public ParserNode{
			public:
				Statement() : ParserNode(){}
				virtual ~Statement(){}
		};
	};
};

#endif
