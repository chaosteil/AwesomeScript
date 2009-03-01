#ifndef AWS_NODES_GROUPEXPRESSION_H
#define AWS_NODES_GROUPEXPRESSION_H

/*
 * aws/nodes/groupexpression.h
 * AwesomeScript Group Expression
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include "unaryexpression.h"

namespace AwS{
	namespace Nodes{
		class GroupExpression : public UnaryExpression{
			public:
				GroupExpression(Expression* content)
					: UnaryExpression(content){
				}
				virtual ~GroupExpression(){
				}

				void translatePhp(std::ostream& output, TranslateSettings& settings) const throw(NodeException){
					output << "(";
					getValue()->translatePhp(output, settings);
					output << ")";
				}
			private:
		};
	};
};

#endif
