#ifndef AWS_NODES_IFSTATEMENT_H
#define AWS_NODES_IFSTATEMENT_H

/*
 * aws/nodes/ifstatement.h
 * AwesomeScript If Statement
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include "statement.h"

namespace AwS{
	namespace Nodes{
		class IfStatement : public Statement{
			public:
				IfStatement(Expression* expression,
					Statement* trueStatement, Statement* falseStatement)
					: Statement(), _expression(expression),
					_trueStatement(trueStatement), _falseStatement(falseStatement){
				}
				virtual ~IfStatement(){
					if(_expression)delete _expression;
					if(_trueStatement)delete _trueStatement;
					if(_falseStatement)delete _falseStatement;
				}
				
				const Expression* getExpression() const{ return _expression; }
				const Statement* getTrue() const{ return _trueStatement; }
				const Statement* getFalse() const{ return _falseStatement; }

				void translatePhp(std::ostream& output, TranslateSettings& settings) const throw(NodeException){
					output << "if";
					_expression->translatePhp(output, settings); // Is definitely a group
					output << "{" << std::endl;
					_trueStatement->translatePhp(output, settings);
					output << "}" << std::endl;
					if(_falseStatement){
						output << "else " << std::endl;
						_falseStatement->translatePhp(output, settings);
					}
				}
			private:
				const Expression* _expression;
				const Statement* _trueStatement;
				const Statement* _falseStatement;
		};
	};
};

#endif
