#ifndef AWS_NODES_FUNCTIONCALL_H
#define AWS_NODES_FUNCTIONCALL_H

/*
 * aws/nodes/functioncall.h
 * AwesomeScript Function Call Expression
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include <list>
#include <string>

#include "expression.h"

namespace AwS{
	namespace Nodes{
		class FunctionCall : public Expression{
			public:
				FunctionCall(const std::string& name, std::list<Expression*>* content)
					: Expression(), _name(name), _content(content){
						
					std::cout << "FunctionCall " << name <<  std::endl;
				}
				virtual ~FunctionCall(){
					if(_content){
						for(std::list<Expression*>::iterator i = _content->begin(); i != _content->end(); ++i){
							if(*i)delete *i;
						}
						delete _content;
					}
				}

				const std::string& getName() const{ return _name; }
				const std::list<Expression*>& getContent() const{ return *_content; }

				void translatePhp(std::ostream& output, TranslateSettings& settings) const throw(NodeException){
					output << settings.getFunctionPrefix() << _name << "(";

					bool begin = true;
					for(std::list<Expression*>::iterator i = _content->begin(); i != _content->end(); ++i){
						if(begin == false)
							output << ", ";

						(*i)->translatePhp(output, settings);
					}
					output << ")";
				}
			private:
				const std::string _name;
				std::list<Expression*>* _content;
		};
	};
};

#endif
