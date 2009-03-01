#ifndef AWS_NODES_NOTEXPRESSION_H
#define AWS_NODES_NOTEXPRESSION_H

/*
 * aws/nodes/notexpression.h
 * AwesomeScript Not Expression
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include "unaryexpression.h"

namespace AwS{
	namespace Nodes{
		class NotExpression : public UnaryExpression{
			public:
				NotExpression(Expression* value)
					: UnaryExpression(value){
						
					std::cout << "NotExpression" << std::endl;
				}
				virtual ~NotExpression(){}

				void translatePhp(std::ostream& output, TranslateSettings& settings) const throw(NodeException){
					output << "!";
					getValue()->translatePhp(output, settings);
				}
		};
	};
};

#endif
