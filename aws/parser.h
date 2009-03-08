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
#include <map>
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
			 */
			Parser(std::istream& input = std::cin);
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


			enum _Error{
				Unknown = 0,
				NoMemory,
				Unfinished,
				ExpectedFunction,
				InvalidStatement,
				InvalidReservedWord,
				ExpectedVariable,
				InvalidDeclaration,
				UndeclaredVariable,
				UndeclaredFunction,
				ExpectedOperation,
				InvalidExpression,
				InvalidNumber,
				InvalidCall,
				ExpectedSymbol,
				ExpectedKey
			};

			void _generateException(_Error error, const std::string& message = "", long line = 0);

			template <class T>
			static bool _convertString(T& t, const std::string& s, std::ios_base& (*f)(std::ios_base&));

			void _readNextToken();
			bool _isFinished();
			void _skipToken(Token::TokenType type, const std::string& value) throw(Exception);
			void _checkUnexpectedEnd() throw(Exception);

			void _prepareReserved();
			void _prepareRequired();
			void _prepareLanguage();

			enum ParserState{
				Default = 0, //!< We are in the global state
				Function, //!< We are in a function
				Loop, //!< We are in a loop
				SpecialStatement //!< Limit statements to a specific set and skip the ';'
			};
			std::stack<ParserState> _states; //!< The state stack. Depending on the top value, the parser parses differemt statements.

			//! FunctionReference class for managing function declarations and references.
			/*!
			 * Designed to work with the Reference<T> class and the AwesomeScript requirements.
			 */
			class _FunctionReference{
				public:
					enum Type{
						Declaration = 0,
						Reference
					};

					//! Constructor
					/*!
					 * \param name The name of the function to declare or reference.
					 * \param params The number of parameters the function has or will pass.
					 * \param type The type of the reference. Is it a declaration or a call?
					 */
					_FunctionReference(std::string name, int params, Type type);
					//! Destructor
					~_FunctionReference();

					//! Overloaded == operator
					/*!
					 * Works with the Reference<T> class to ensure that the referenced classes don't access less
					 * parameters than they should. If more parameters than required are in a reference,
					 * everything should be fine, but returns false if the declaration needs more.
					 * \param reference The reference to compare to.
					 */
					bool operator==(const _FunctionReference& reference)const;

					//! The name of the function.
					/*!
					 * \return The name of the declared or referenced function
					 */
					const std::string& getName() const;
					//! Parameters.
					/*!
					 * \return The amount of parameters in this declaration or reference.
					 */
					int getParams() const;

					//! Type.
					/*!
					 * \return The type of the function on initialization.
					 */
					Type getType() const;
				private:
					std::string _name; //!< Name of function.
					int _params; //!< Amount of parameters in function declaration or reference.
					Type _type; //!< The type of the function declaration or call.
			};

			Reference<std::string> _reserved;
			Reference<_FunctionReference> _functions;
			Reference<std::string>* _variableScope;

			Tokenizer* _tokenizer;
			Token* _currentToken;
			std::istream& _input;
	};
};

#endif
