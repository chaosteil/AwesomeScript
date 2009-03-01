#ifndef AWS_NODES_MODULUS_H
#define AWS_NODES_MODULUS_H

/*
 * aws/nodes/modulus.h
 * AwesomeScript Modulus
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include "binaryexpression.h"

namespace AwS{
	namespace Nodes{
		class Modulus : public BinaryExpression{
			public:
				Modulus(Expression* left, Expression* right)
					: BinaryExpression(left, right){
				}
				virtual ~Modulus(){}

				void translatePhp(std::ostream& output, TranslateSettings& settings) const throw(NodeException){
					getLeft()->translatePhp(output, settings);
					output << " % ";
					getRight()->translatePhp(output, settings);
				}
		};
	};
};

#endif
