#ifndef AWS_NODES_GROUPEXPRESSION_H
#define AWS_NODES_GROUPEXPRESSION_H

/*
 * aws/nodes/groupexpression.h
 * AwesomeScript Group Expression
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include "expression.h"

namespace AwS{
	namespace Nodes{
		class GroupExpression : public Expression{
			public:
				GroupExpression(Expression* content)
					: Expression(), _content(content){

					std::cout << "GroupExpression" << std::endl;
				}
				virtual ~GroupExpression(){
					if(_content)
						delete _content;
				}

				void translatePhp(std::ostream& output, TranslateSettings& settings) const throw(NodeException){
					output << "(";
					_content->translatePhp(output, settings);
					output << ")";
				}
			private:
				Expression* _content;
		};
	};
};

#endif
