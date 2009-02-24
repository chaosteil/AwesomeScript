#ifndef AWS_PARSER_H
#define AWS_PARSER_H

/*
 * aws/parser.h
 * AwesomeScript Parser
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include <iostream>
#include <istream>
#include <ostream>
#include <stack>
#include <string>

#include "nodes/nodes.h"
#include "reference.h"
#include "token.h"
#include "tokenizer.h"

namespace AwS{
	//! Parses a stream from Awesomescript.
	/*!
	 * The parser itself does not evaluate any expressions or interprets any code. It is merely used to structurize the program.
	 */
	class Parser{
		public:
			//! Constructor.
			/*!
			 * \param input The stream which the parser passes to the tokenizer.
			 * \param output The stream where the parser passes error messages.
			 */
			Parser(std::istream& input = std::cin, std::ostream& output = std::cout);
			//! Destructor.
			/*!
			 * Does NOT clean up any statements passed with readStatement().
			 */
			~Parser() throw(Exception);

			//! Reads a statement from the stream.
			/*!
			 * The statement is a valid, translateable language unit.
			 * \return An initialized statement. NULL, if finished reading the stream.
			 */
			Nodes::Statement* readStatement();
		private:
			Nodes::Statement* _parseStatementBlock();
			Nodes::Statement* _parseStatementFunction();
			Nodes::Statement* _parseStatementIf();
			Nodes::Statement* _parseStatementWhile();
			Nodes::Statement* _parseStatementDoWhile();
			Nodes::Statement* _parseStatementFor();
			Nodes::Statement* _parseStatementVar();
			Nodes::Statement* _parseStatementFunctionCallOrAssignment();
			Nodes::Statement* _parseStatementOperations(const Nodes::Variable* variable);
			Nodes::Statement* _parseStatementFunctionCall(const std::string& name);
			Nodes::Statement* _parseStatementArray(const std::string& name);
			Nodes::Statement* _parseStatementAssignment(const Nodes::Variable* variable);
			Nodes::Statement* _parseStatementAdditionEqual(const Nodes::Variable* variable);
			Nodes::Statement* _parseStatementSubstractionEqual(const Nodes::Variable* variable);
			Nodes::Statement* _parseStatementMultiplicationEqual(const Nodes::Variable* variable);
			Nodes::Statement* _parseStatementDivisionEqual(const Nodes::Variable* variable);
			Nodes::Statement* _parseStatementModulusEqual(const Nodes::Variable* variable);
			Nodes::Statement* _parseStatementIncrease(const Nodes::Variable* variable);
			Nodes::Statement* _parseStatementDecrease(const Nodes::Variable* variable);
			Nodes::Statement* _parseStatementBreak();
			Nodes::Statement* _parseStatementContinue();
			Nodes::Statement* _parseStatementReturn();

			Nodes::Expression* _parseExpression();
			Nodes::Expression* _parseExpressionAnd();
			Nodes::Expression* _parseExpressionOr();
			Nodes::Expression* _parseExpressionComparison();
			Nodes::Expression* _parseExpressionAddition();
			Nodes::Expression* _parseExpressionMultiplication();
			Nodes::Expression* _parseExpressionUnary();
			Nodes::Expression* _parseExpressionBase();
			Nodes::Expression* _parseExpressionGroup();
			Nodes::Expression* _parseExpressionVariableOrFunctionCall();
			Nodes::Expression* _parseExpressionStringConstant();
			Nodes::Expression* _parseExpressionNumberConstant();
			Nodes::Expression* _parseExpressionBooleanConstant();
			Nodes::Expression* _parseExpressionNullConstant();
			Nodes::Expression* _parseExpressionArray();
			Nodes::Expression* _parseExpressionAssociativeArray();
			Nodes::Assignment* _parseExpressionAssociativeArrayPair();
			Nodes::Expression* _parseExpressionArrayAccess(const std::string& name);
			Nodes::FunctionCall* _parseFunctionCall(const std::string& name);

			template <class T>
			static bool _convertString(T& t, const std::string& s, std::ios_base& (*f)(std::ios_base&));

			void _readNextToken();
			bool _isFinished();
			void _skipToken(Token::TokenType type, const std::string& value) throw(Exception);
			void _checkUnexpectedEnd() throw(Exception);

			void _prepareReserved();

			enum ParserState{
				Default = 0, //!< We are in the global state
				Function, //!< We are in a function
				Loop, //!< We are in a loop
				SpecialStatement //!< Limit statements to a specific set and skip the ';'
			};
			std::stack<ParserState> _states; //!< The state stack. Depending on the top value, the parser parses differemt statements.

			Reference<std::string> _reserved;
			//Reference<AwS::FunctionDefinition> _functions;
			//Reference<AwS::Variable> _variables;

			Tokenizer* _tokenizer;
			Token* _currentToken;
			std::istream& _input;
			std::ostream& _output;
	};
};

#endif
