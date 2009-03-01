#ifndef AWS_NODES_DOWHILESTATEMENT_H
#define AWS_NODES_DOWHILESTATEMENT_H

/*
 * aws/nodes/dowhilestatement.h
 * AwesomeScript Do While Statement
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include "statement.h"

namespace AwS{
	namespace Nodes{
		class DoWhileStatement : public Statement{
			public:
				DoWhileStatement(Expression* expression, Statement* statement)
					: Statement(), _expression(expression), _statement(statement){
					
					std::cout << "DoWhileStatement" << std::endl;
				}
				virtual ~DoWhileStatement(){
					if(_expression)delete _expression;
					if(_statement)delete _statement;
				}

				const Expression* getExpression() const{ return _expression; }
				const Statement* getStatement() const{ return _statement; }

				void translatePhp(std::ostream& output, TranslateSettings& settings) const throw(NodeException){
				}
			private:
				const Expression* _expression;
				const Statement* _statement;
		};
	};
};

#endif
