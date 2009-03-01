#ifndef AWS_NODES_FORSTATEMENT_H
#define AWS_NODES_FORSTATEMENT_H

/*
 * aws/nodes/forstatement.h
 * AwesomeScript For Statement
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include <string>

#include "expression.h"
#include "statement.h"

namespace AwS{
	namespace Nodes{
		class ForStatement : public Statement{
			public:
				ForStatement(const Statement* begin, const Expression* eval,
					const Statement* end, const Statement* block)
					: Statement(), _begin(begin), _eval(eval), _end(end), _block(block){
						
					std::cout << "For Statement" << std::endl;
				}
				virtual ~ForStatement(){
					if(_begin)delete _begin;
					if(_eval)delete _eval;
					if(_end)delete _end;
					if(_block)delete _block;
				}
				
				const Statement* getBegin() const{ return _begin; }
				const Expression* getEval() const { return _eval; }
				const Statement* getEnd() const { return _end; }
				const Statement* getBlock() const { return _block; }

				void translatePhp(std::ostream& output, TranslateSettings& settings) const throw(NodeException){
					output << "for" << "(";
					if(_begin)
						_begin->translatePhp(output, settings);
					output << ";";
					if(_eval)
						_eval->translatePhp(output, settings);
					output << ";";
					if(_end)
						_end->translatePhp(output, settings);
					output << ")";
					_block->translatePhp(output, settings);
				}
			private:
				const Statement* _begin;
				const Expression* _eval;
				const Statement* _end;
				const Statement* _block;
		};
	};
};

#endif
