#ifndef AWS_NODES_BLOCK_H
#define AWS_NODES_BLOCK_H

/*
 * aws/nodes/block.h
 * AwesomeScript Block
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include <list>

#include "statement.h"

namespace AwS{
	namespace Nodes{
		class Block : public Statement{
			public:
				Block(std::list<Statement*>* content)
					: Statement(), _content(content){
				}
				virtual ~Block(){
					if(_content){
						for(std::list<Statement*>::iterator i = _content->begin(); i != _content->end(); ++i){
							if(*i)delete *i;
						}
						delete _content;
					}
				}

				void translatePhp(std::ostream& output, TranslateSettings& settings) const throw(NodeException){
					// output << "{" << std::endl;
					for(std::list<Statement*>::iterator i = _content->begin(); i != _content->end(); ++i){
						if(*i != NULL)
							(*i)->translatePhp(output, settings);
					}
					// output << "}" << std::endl;
				}
			private:
				std::list<Statement*>* _content;
		};
	};
};

#endif
