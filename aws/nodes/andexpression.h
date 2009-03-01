#ifndef AWS_NODES_ANDEXPRESSION_H
#define AWS_NODES_ANDEXPRESSION_H

/*
 * aws/nodes/andexpression.h
 * AwesomeScript And Expression
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include "binaryexpression.h"

namespace AwS{
	namespace Nodes{
		class AndExpression : public BinaryExpression{
			public:
				AndExpression(Expression* left, Expression* right)
					: BinaryExpression(left, right){

					std::cout << "AndExpression" << std::endl;
				}
				virtual ~AndExpression(){}

				void translatePhp(std::ostream& output, TranslateSettings& settings) const throw(NodeException){
					getLeft()->translatePhp(output, settings);
					output << " && ";
					getRight()->translatePhp(output, settings);
				}
		};
	};
};

#endif
