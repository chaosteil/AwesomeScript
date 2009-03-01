#ifndef AWS_NODES_WHILESTATEMENT_H
#define AWS_NODES_WHILESTATEMENT_H

/*
 * aws/nodes/whilestatement.h
 * AwesomeScript While Statement
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include "statement.h"

namespace AwS{
	namespace Nodes{
		class WhileStatement : public Statement{
			public:
				WhileStatement(Expression* expression, Statement* statement)
					: Statement(), _expression(expression), _statement(statement){
					
					std::cout << "WhileStatement" << std::endl;
				}
				virtual ~WhileStatement(){
					if(_expression)delete _expression;
					if(_statement)delete _statement;
				}

				const Expression* getExpression() const{ return _expression; }
				const Statement* getStatement() const{ return _statement; }

				void translatePhp(std::ostream& output, TranslateSettings& settings) const throw(NodeException){
					output << "while";
					_expression->translatePhp(output, settings); // This is a group statement ()
					_statement->translatePhp(output, settings);
				}
			private:
				const Expression* _expression;
				const Statement* _statement;
		};
	};
};

#endif
