#ifndef AWS_NODES_ARRAY_H
#define AWS_NODES_ARRAY_H

/*
 * aws/nodes/array.h
 * AwesomeScript Array
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include <list>
#include <string>

#include "expression.h"

namespace AwS{
	namespace Nodes{
		class Array : public Expression{
			public:
				Array(std::list<Expression*>* content)
					: Expression(), _content(content){

					std::cout << "Array" << std::endl;
				}
				virtual ~Array(){
					if(_content){
						for(std::list<Expression*>::iterator i = _content->begin(); i != _content->end(); ++i){
							if(*i)delete *i;
						}
						delete _content;
					}
				}

				const std::list<Expression*>& getContent() const{ return *_content; }

				void translatePhp(std::ostream& output, TranslateSettings& settings) const throw(NodeException){
				}
			private:
				std::list<Expression*>* _content;
		};
	};
};

#endif
