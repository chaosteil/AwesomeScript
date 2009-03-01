#ifndef AWS_NODES_NUMBERCONSTANT_H
#define AWS_NODES_NUMBERCONSTANT_H

/*
 * aws/nodes/numberconstant.h
 * AwesomeScript Number Constant
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include "expression.h"

namespace AwS{
	namespace Nodes{
		class NumberConstant : public Expression{
			public:
				NumberConstant(const float& content)
					: Expression(), _content(content){
						
					std::cout << "Number " << content << std::endl;
				}
				virtual ~NumberConstant(){}

				const float& getContent() const{return _content; }

				void translatePhp(std::ostream& output, TranslateSettings& settings) const throw(NodeException){
				}
			private:
				const float _content;
		};
	};
};

#endif
