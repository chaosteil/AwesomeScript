#ifndef AWS_NODES_PARSERNODE_H
#define AWS_NODES_PARSERNODE_H

/*
 * aws/nodes/parsernode.h
 * AwesomeScript Parser Node
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include <ostream>
#include "nodeexception.h"
#include "translatesettings.h"

namespace AwS{
	namespace Nodes{
		class ParserNode{
			public:
				ParserNode(){}
				virtual ~ParserNode(){}

				virtual void translatePhp(std::ostream& output, TranslateSettings& settings) const throw(NodeException) = 0;
		};
	};
};

#endif
