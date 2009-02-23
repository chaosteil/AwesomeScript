#ifndef AWS_NODES_EXPRESSION_H
#define AWS_NODES_EXPRESSION_H

/*
 * aws/nodes/expression.h
 * AwesomeScript Expression
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include "parsernode.h"

namespace AwS{
	namespace Nodes{
		class Expression : public ParserNode{
			public:
				Expression() : ParserNode(){}
				virtual ~Expression(){}
		};
	};
};

#endif
