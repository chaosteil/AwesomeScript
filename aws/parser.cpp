/*
 * aws/parser.cpp
 * AwesomeScript Parser
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

/* ==============================================================================
 * P A R S E R   CLASS
 * ============================================================================*/

#include <list>

#include "nodes/nodes.h"
#include "parser.h"

using namespace AwS;
using namespace AwS::Nodes;

/* ==============================================================================
 * P U B L I C   M E T H O D S
 * ============================================================================*/

Parser::Parser(std::istream& input)
	: _variableScope(NULL), _tokenizer(NULL), _currentToken(NULL), _input(input){
	// Prepare all the reserved words so variables and functions don't adapt weird names
	_prepareReserved();
	// Prepare required functions or variables the user has to use.
	_prepareRequired();
	// Prepares all the language functions and available variables
	_prepareLanguage();

	// Start tokenizer and read first token
	_tokenizer = new Tokenizer(_input);
	if(!_tokenizer)
		_generateException(NoMemory);
	_readNextToken();

	// Default state, should finish with only default state in stack
	_states.push(Default);
}

Parser::~Parser() throw(Exception){
	if(_tokenizer)
		delete _tokenizer; _tokenizer = NULL;
	
	// We didn't left our state yet
	if((_states.top() != Default))
		_generateException(Unfinished);
	
	const std::list<_FunctionReference>* functions = _functions.getReferences();
	if(functions && functions->size() > 0){
		_generateException(UndeclaredFunction, functions->front().getName());
	}

	_states.pop();
}

Statement* Parser::readStatement(){
	if(_isFinished())
		return NULL;

	if(_states.top() == Default){ // Expect only functions or global variables
		if(_currentToken->is(Token::Word, "function"))
			return _parseStatementFunction();
		/* Global Variable support
		if(_currentToken->is(Token::Word, "var"))
			return _parseStatementVar();
		*/

		_generateException(InvalidStatement, "not function");

	}else if(_states.top() != Default){ // Everything in a function

		// These statements are not allowed in special statement mode, but otherwise are ok
		if(_states.top() != SpecialStatement){
			
			if(_currentToken->is(Token::Word, "break"))
				if(_states.top() == Loop)
					return _parseStatementBreak();
				else _generateException(InvalidStatement, "break");

			if(_currentToken->is(Token::Word, "continue"))
				if(_states.top() == Loop)
					return _parseStatementContinue();
				else _generateException(InvalidStatement, "continue");

			if(_currentToken->getValue().compare("return") == 0)
				return _parseStatementReturn();

			if(_currentToken->is(Token::Symbol, "{"))
				return _parseStatementBlock();

			if(_currentToken->getType() != Token::Word)
				_generateException(InvalidStatement);

			if(_currentToken->getValue().compare("if") == 0)
				return _parseStatementIf();

			if(_currentToken->getValue().compare("while") == 0)
				return _parseStatementWhile();

			if(_currentToken->getValue().compare("do") == 0)
				return _parseStatementDoWhile();

			if(_currentToken->getValue().compare("for") == 0)
				return _parseStatementFor();
		}
		// These statements are allowed in special statement mode

		if(_currentToken->getValue().compare("var") == 0)
			return _parseStatementVar();


		return _parseStatementFunctionCallOrAssignment();
	}

	return NULL;

}

/* ==============================================================================
 * P R I V A T E   M E T H O D S
 * ============================================================================*/

Statement* Parser::_parseStatementBlock(){
	// This is a normal { } block
	_readNextToken(); // Skip {
	
	std::list<Statement*>* statements = new std::list<Statement*>();
	if(!statements)
		_generateException(NoMemory);
	
	while(!_isFinished() && !_currentToken->is(Token::Symbol, "}")){
		statements->push_back(readStatement());
	}

	_checkUnexpectedEnd();
	_readNextToken(); // Skip }

	return new Block(statements);
}

Statement* Parser::_parseStatementFunction(){
	_readNextToken();
	_checkUnexpectedEnd();
	
	if(_currentToken->getType() != Token::Word)
		_generateException(ExpectedFunction);
	
	if(_reserved.isDeclared(_currentToken->getValue()) == Reference<std::string>::IsDeclared)
		_generateException(InvalidReservedWord, "function");

	std::string name = _currentToken->getValue();
	_readNextToken();
	_skipToken(Token::Symbol, "(");
	_checkUnexpectedEnd();
	
	std::list<Variable*>* variables = new std::list<Variable*>();
	if(!variables)
		_generateException(NoMemory);

	if(_variableScope == NULL)
		_variableScope = new Reference<std::string>();

	if(!_currentToken->is(Token::Symbol, ")")){
		if(_currentToken->getType() != Token::Word)
			_generateException(ExpectedSymbol, ")");

		if(_reserved.isDeclared(_currentToken->getValue()) == Reference<std::string>::IsDeclared)
			_generateException(InvalidReservedWord, "variable");
				
		variables->push_back(new Variable(_currentToken->getValue()));
		_variableScope->addDeclaration(_currentToken->getValue());
		_readNextToken();

		while(!_isFinished() && _currentToken->is(Token::Symbol, ",")){
			_readNextToken();
			if(_currentToken->getType() != Token::Word)
				_generateException(ExpectedVariable);

			if(_reserved.isDeclared(_currentToken->getValue()) == Reference<std::string>::IsDeclared)
				_generateException(InvalidReservedWord, "variable");

			variables->push_back(new Variable(_currentToken->getValue()));
			if(_variableScope->addDeclaration(_currentToken->getValue()) == Reference<std::string>::AlreadyDeclared)
				_generateException(InvalidDeclaration, "variable");
			_readNextToken();
		}
		
		_checkUnexpectedEnd();
		if(!_currentToken->is(Token::Symbol, ")"))
			_generateException(ExpectedSymbol, ")");
	}

	Reference<_FunctionReference>::ReferenceStatus status = _functions.addDeclaration(_FunctionReference(name, variables->size(), _FunctionReference::Declaration));
	if(status == Reference<_FunctionReference>::AlreadyDeclared)
		_generateException(InvalidDeclaration, "function");
	
	_readNextToken();
	_checkUnexpectedEnd();
	
	_states.push(Function);
	Statement* statement = readStatement();
	_states.pop();

	if(_variableScope){
		// TODO check if there are unused variables?
		delete _variableScope; _variableScope = NULL;
	}

	return new FunctionDefinition(name, variables, statement);
}

Statement* Parser::_parseStatementIf(){
	_readNextToken();
	_checkUnexpectedEnd();

	if(!_currentToken->is(Token::Symbol, "("))
		_generateException(ExpectedSymbol, "(");

	Expression* expression = _parseExpressionGroup();
	Statement* trueBlock = NULL;
	Statement* falseBlock = NULL;

	trueBlock = readStatement();

	if(_currentToken != NULL && _currentToken->is(Token::Word, "else")){
		_readNextToken();
		falseBlock = readStatement();
	}
	
	return new IfStatement(expression, trueBlock, falseBlock);
}

Statement* Parser::_parseStatementWhile(){
	_readNextToken();
	_checkUnexpectedEnd();

	if(!_currentToken->is(Token::Symbol, "("))
		_generateException(ExpectedSymbol, "(");

	// First we get the expression at the beginning of the while
	Expression* group = _parseExpressionGroup();
	// Then we enter the block, we define it as loop to enable continue and break
	_states.push(Loop);
	Statement* block = readStatement();
	_states.pop();

	return new WhileStatement(group, block);
}

Statement* Parser::_parseStatementDoWhile(){
	_readNextToken(),
	_checkUnexpectedEnd();

	_states.push(Loop);
	Statement* block = readStatement();
	_states.pop();

	_skipToken(Token::Word, "while");
	_checkUnexpectedEnd();

	if(!_currentToken->is(Token::Symbol, "("))
		_generateException(ExpectedSymbol, "(");

	Expression* group = _parseExpressionGroup();

	_skipToken(Token::Symbol, ";");

	return new DoWhileStatement(group, block);
}

Statement* Parser::_parseStatementFor(){
	_readNextToken();

	_skipToken(Token::Symbol, "(");
	_checkUnexpectedEnd();

	_states.push(SpecialStatement);
	
	Statement* begin = NULL;
	Expression* eval = NULL;
	Statement* end = NULL;

	if(!_currentToken->is(Token::Symbol, ";"))
		begin = readStatement();
	_readNextToken();
	_checkUnexpectedEnd();

	if(!_currentToken->is(Token::Symbol, ";"))
		eval = _parseExpression();
	_readNextToken();
	_checkUnexpectedEnd();

	if(!_currentToken->is(Token::Symbol, ")"))
		end = readStatement();
	_states.pop();

	_skipToken(Token::Symbol, ")");
	_checkUnexpectedEnd();

	_states.push(Loop);
	Statement* block = readStatement();
	_states.pop();

	return new ForStatement(begin, eval, end, block);
}

Statement* Parser::_parseStatementVar(){
	_readNextToken();
	_checkUnexpectedEnd();

	std::list<Assignment*>* variables = new std::list<Assignment*>();
	if(!variables)
		_generateException(NoMemory);

	while(!_isFinished()){
		if(_currentToken->getType() != Token::Word)
			_generateException(ExpectedVariable);
		

		// We get the name and check what follows
		if(_reserved.isDeclared(_currentToken->getValue()) == Reference<std::string>::IsDeclared)
			_generateException(InvalidReservedWord, "variable");
		std::string name = _currentToken->getValue();
		_variableScope->addDeclaration(name);

		_readNextToken();
		_checkUnexpectedEnd();

		// We check if an init expression is assigned to the variable
		Assignment* assignment = NULL;
		if(_currentToken->is(Token::Symbol, "=")){
			_states.push(SpecialStatement);
			assignment = static_cast<Assignment*>(_parseStatementAssignment(new Variable(name)));
			_states.pop();
		}

		if(assignment)
			variables->push_back(assignment);
		else
			variables->push_back(new Assignment(new Variable(name), NULL, Assignment::Normal));

		_checkUnexpectedEnd();

		// We want another variable
		if(_currentToken->is(Token::Symbol, ",")){
			_readNextToken();
			continue;
		}
		
		// We actually expect a ; here, but since we have the special
		// statement, we just skip it
		if(_states.top() == SpecialStatement)
			break;

		// We want to finish the declaration
		if(_currentToken->is(Token::Symbol, ";")){
			_readNextToken();
			break;
		}

		// No valid symbols!
		_generateException(ExpectedSymbol, ";");
	}
	_checkUnexpectedEnd();

	return new VariableDeclaration(variables);
}

Statement* Parser::_parseStatementFunctionCallOrAssignment(){
	if(_reserved.isDeclared(_currentToken->getValue()) == Reference<std::string>::IsDeclared)
		_generateException(InvalidReservedWord, "variable");

	std::string name = _currentToken->getValue();
	_readNextToken();
	_checkUnexpectedEnd();

	if(_currentToken->is(Token::Symbol, "("))
		return _parseStatementFunctionCall(name);
	
	if(_currentToken->is(Token::Symbol, "["))
		return _parseStatementArray(name);

	if(_variableScope->addReference(name) != Reference<std::string>::AlreadyDeclared)
		_generateException(UndeclaredVariable, name);
	return _parseStatementOperations(new Variable(name));

	return NULL;
}

Statement* Parser::_parseStatementOperations(const Variable* name){
	if(_currentToken->is(Token::Symbol, "="))
		return _parseStatementAssignment(name);

	if(_currentToken->is(Token::Symbol, "+="))
		return _parseStatementAdditionEqual(name);

	if(_currentToken->is(Token::Symbol, "-="))
		return _parseStatementSubstractionEqual(name);
	
	if(_currentToken->is(Token::Symbol, "*="))
		return _parseStatementMultiplicationEqual(name);

	if(_currentToken->is(Token::Symbol, "/="))
		return _parseStatementDivisionEqual(name);
	
	if(_currentToken->is(Token::Symbol, "%="))
		return _parseStatementModulusEqual(name);

	if(_currentToken->is(Token::Symbol, "++"))
		return _parseStatementIncrease(name);

	if(_currentToken->is(Token::Symbol, "--"))
		return _parseStatementDecrease(name);
	
	_generateException(ExpectedOperation);

	return NULL;
}

Statement* Parser::_parseStatementFunctionCall(const std::string& name){
	if(_reserved.isDeclared(name) == Reference<std::string>::IsDeclared)
		_generateException(InvalidReservedWord);

	FunctionCall* call = _parseFunctionCall(name);

	// If we are in the special statement state, we don't expect a ;
	if(_states.top() != SpecialStatement){
		if(!_currentToken->is(Token::Symbol, ";"))
			_generateException(ExpectedSymbol, ";");
	
		_readNextToken();
	}
	return new FunctionCallStatement(call);
}

Statement* Parser::_parseStatementArray(const std::string& name){
	if(_reserved.isDeclared(name) == Reference<std::string>::IsDeclared)
		_generateException(InvalidReservedWord, "variable");
	_variableScope->addDeclaration(name);
	
	std::list<Expression*>* expressions = new std::list<Expression*>();
	if(!expressions)
		_generateException(NoMemory);

	// We go through all referenced elements.
	while(42){
		_readNextToken(); // Skip [
		_checkUnexpectedEnd();

		expressions->push_back(_parseExpression());
		
		_skipToken(Token::Symbol, "]");

		if(!_currentToken->is(Token::Symbol, "["))
			break;
	}

	return _parseStatementOperations(new Variable(name, expressions));
}

Statement* Parser::_parseStatementAssignment(const Variable* variable){
	_readNextToken(); // Skip = or :
	_checkUnexpectedEnd();


	Expression* expression = NULL;
	if(_currentToken->is(Token::Symbol, "[")){
		expression = _parseExpressionArray();
	}else if(_currentToken->is(Token::Symbol, "{")){
		expression = _parseExpressionAssociativeArray();
	}else
		expression = _parseExpression();

	// If we are in the special statement state, we don't expect a ;
	if(_states.top() != SpecialStatement){
		_skipToken(Token::Symbol, ";");
	}

	return new Assignment(variable, expression, Assignment::Normal);
}

Statement* Parser::_parseStatementAdditionEqual(const Variable* variable){
	_readNextToken(); // Skip +=
	_checkUnexpectedEnd();

	Expression* expression = _parseExpression();

	// If we are in the special statement state, we don't expect a ;
	if(_states.top() != SpecialStatement){
		_skipToken(Token::Symbol, ";");
	}

	return new Assignment(variable, expression, Assignment::Addition);
}

Statement* Parser::_parseStatementSubstractionEqual(const Variable* variable){
	_readNextToken(); // Skip -=
	_checkUnexpectedEnd();

	Expression* expression = _parseExpression();

	// If we are in the special statement state, we don't expect a ;
	if(_states.top() != SpecialStatement){
		_skipToken(Token::Symbol, ";");
	}

	return new Assignment(variable, expression, Assignment::Substraction);
}

Statement* Parser::_parseStatementMultiplicationEqual(const Variable* variable){
	_readNextToken(); // Skip *=
	_checkUnexpectedEnd();

	Expression* expression = _parseExpression();

	// If we are in the special statement state, we don't expect a ;
	if(_states.top() != SpecialStatement){
		_skipToken(Token::Symbol, ";");
	}

	return new Assignment(variable, expression, Assignment::Multiplication);
}

Statement* Parser::_parseStatementDivisionEqual(const Variable* variable){
	_readNextToken(); // Skip /=
	_checkUnexpectedEnd();

	Expression* expression = _parseExpression();

	// If we are in the special statement state, we don't expect a ;
	if(_states.top() != SpecialStatement){
		_skipToken(Token::Symbol, ";");
	}

	return new Assignment(variable, expression, Assignment::Division);
}

Statement* Parser::_parseStatementModulusEqual(const Variable* variable){
	_readNextToken(); // Skip %=
	_checkUnexpectedEnd();

	Expression* expression = _parseExpression();

	// If we are in the special statement state, we don't expect a ;
	if(_states.top() != SpecialStatement){
		_skipToken(Token::Symbol, ";");
	}

	return new Assignment(variable, expression, Assignment::Modulus);
}

Statement* Parser::_parseStatementIncrease(const Variable* variable){
	_readNextToken(); // Skip ++
	_checkUnexpectedEnd();

	if(_states.top() != SpecialStatement){
		_skipToken(Token::Symbol, ";");
	}

	return new Increase(variable);
}

Statement* Parser::_parseStatementDecrease(const Variable* variable){
	_readNextToken(); // Skip --
	_checkUnexpectedEnd();

	if(_states.top() != SpecialStatement){
		_skipToken(Token::Symbol, ";");
	}

	return new Decrease(variable);
}

Statement* Parser::_parseStatementBreak(){
	_readNextToken();
	_checkUnexpectedEnd();
	
	if(!_currentToken->is(Token::Symbol, ";"))
		_generateException(ExpectedSymbol, ";");

	_readNextToken();
	return new BreakStatement();
}

Statement* Parser::_parseStatementContinue(){
	_readNextToken();
	_checkUnexpectedEnd();
	
	if(!_currentToken->is(Token::Symbol, ";"))
		_generateException(ExpectedSymbol, ";");
	
	_readNextToken();
	return new ContinueStatement();
}

Statement* Parser::_parseStatementReturn(){
	_readNextToken();
	_checkUnexpectedEnd();
	
	Expression* expression = NULL;

	if(!_currentToken->is(Token::Symbol, ";")){
		expression = _parseExpression();
	}

	_checkUnexpectedEnd();
	if(!_currentToken->is(Token::Symbol, ";")){
		_generateException(ExpectedSymbol, ";");
	}

	_readNextToken();
	return new ReturnStatement(expression);
}

Expression* Parser::_parseExpression(){
	return _parseExpressionAnd();
}

Expression* Parser::_parseExpressionAnd(){
	Expression* node = _parseExpressionOr();

	while(!_isFinished() && _currentToken->is(Token::Symbol, "&&")){
		_readNextToken(); // Skip "&&"
		node = new AndExpression(node, _parseExpressionOr());
		if(!node)
			_generateException(NoMemory);
	}

	return node;
}

Expression* Parser::_parseExpressionOr(){
	Expression* node = _parseExpressionComparison();

	while(!_isFinished() && _currentToken->is(Token::Symbol, "||")){
		_readNextToken(); // Skip "||"
		node = new OrExpression(node, _parseExpressionComparison());
		if(!node)
			_generateException(NoMemory);
	}

	return node;
}

Expression* Parser::_parseExpressionComparison(){
	Expression* node = _parseExpressionAddition();

	if(!_isFinished() && _currentToken->getType() == Token::Symbol){
		Comparison::Operator op;
		if(_currentToken->getValue().compare("==") == 0)
			op = Comparison::Equal;
		else if(_currentToken->getValue().compare("!=") == 0)
			op = Comparison::NotEqual;
		else if(_currentToken->getValue().compare("<") == 0)
			op = Comparison::Less;
		else if(_currentToken->getValue().compare(">") == 0)
			op = Comparison::Greater;
		else if(_currentToken->getValue().compare("<=") == 0)
			op = Comparison::LessEqual;
		else if(_currentToken->getValue().compare(">=") == 0)
			op = Comparison::GreaterEqual;
		else
			return node;

		_readNextToken();
		return new Comparison(op, node, _parseExpressionAddition());
	}else
		return node;
}

Expression* Parser::_parseExpressionAddition(){
	Expression* node = _parseExpressionMultiplication();

	while(!_isFinished()){
		if(_currentToken->is(Token::Symbol, "+")){
			_readNextToken();
			node = new Addition(node, _parseExpressionMultiplication());
			if(!node)
				_generateException(NoMemory);
		}else if(_currentToken->is(Token::Symbol, "-")){
			_readNextToken();
			node = new Substraction(node, _parseExpressionMultiplication());
			if(!node)
				_generateException(NoMemory);
		}else
			break;
	}

	return node;
}

Expression* Parser::_parseExpressionMultiplication(){
	Expression* node = _parseExpressionUnary();

	while(!_isFinished()){
		if(_currentToken->is(Token::Symbol, "*")){
			_readNextToken();
			node = new Multiplication(node, _parseExpressionUnary());
			if(!node)
				_generateException(NoMemory);
		}else if(_currentToken->is(Token::Symbol, "/")){
			_readNextToken();
			node = new Division(node, _parseExpressionUnary());
			if(!node)
				_generateException(NoMemory);
		}else if(_currentToken->is(Token::Symbol, "%")){
			_readNextToken();
			node = new Modulus(node, _parseExpressionUnary());
			if(!node)
				_generateException(NoMemory);
		}else
			break;
	}

	return node;
}

Expression* Parser::_parseExpressionUnary(){
	_checkUnexpectedEnd();

	if(_currentToken->is(Token::Symbol, "-")){
		_readNextToken();
		return new Negation(_parseExpressionBase());
	}else if(_currentToken->is(Token::Symbol, "!")){
		_readNextToken();
		return new NotExpression(_parseExpressionBase());
	}else if(_currentToken->is(Token::Symbol, "+")){
		_readNextToken();

	}

	return _parseExpressionBase();
}

Expression* Parser::_parseExpressionBase(){
	_checkUnexpectedEnd();

	if(_currentToken->getType() == Token::Number)
		return _parseExpressionNumberConstant();
	else if(_currentToken->getType() == Token::String)
		return _parseExpressionStringConstant();
	else if(_currentToken->getType() == Token::Boolean)
		return _parseExpressionBooleanConstant();
	else if(_currentToken->getType() == Token::Null)
		return _parseExpressionNullConstant();
	else if(_currentToken->getType() == Token::Word)
		return _parseExpressionVariableOrFunctionCall();
	else{ // Token::Symbol
		if(_currentToken->getValue().compare("(") == 0)
			return _parseExpressionGroup();
		else _generateException(InvalidExpression);
	}

	return NULL;
}

Expression* Parser::_parseExpressionGroup(){
	_readNextToken(); // Skip '('

	Expression* node = _parseExpression();
	
	_skipToken(Token::Symbol, ")");

	return new GroupExpression(node);
}

Expression* Parser::_parseExpressionVariableOrFunctionCall(){
	if(_reserved.isDeclared(_currentToken->getValue()) == Reference<std::string>::IsDeclared)
		_generateException(InvalidReservedWord, "variable");

	std::string name = _currentToken->getValue();

	_readNextToken();

	if(!_isFinished() && _currentToken->is(Token::Symbol, "("))
		return _parseFunctionCall(name);
	
	if(_variableScope->addReference(name) != Reference<std::string>::AlreadyDeclared)
		_generateException(UndeclaredVariable, name);

	if(!_isFinished() && _currentToken->is(Token::Symbol, "["))
		return _parseExpressionArrayAccess(name);
	else
		return new Variable(name);
}

Expression* Parser::_parseExpressionStringConstant(){
	std::string value = _currentToken->getValue();
	_readNextToken();

	return new StringConstant(value);
}

Expression* Parser::_parseExpressionNumberConstant(){
	float value = 0;
	if(_convertString<float>(value, _currentToken->getValue(), std::dec)){
		_readNextToken();
		return new NumberConstant(value);
	}else
		_generateException(InvalidNumber);

	return NULL;
}

Expression* Parser::_parseExpressionBooleanConstant(){
	bool value = false;

	// The value is either true or false
	if(_currentToken->getValue().compare("true") == 0)
		value = true;
	
	_readNextToken();
	return new BooleanConstant(value);
}

Expression* Parser::_parseExpressionNullConstant(){
	_readNextToken();
	return new NullConstant();
}

Expression* Parser::_parseExpressionArray(){
	_readNextToken(); // Skip '['
	_checkUnexpectedEnd();

	std::list<Expression*>* arguments = new std::list<Expression*>();
	if(!arguments)
		_generateException(NoMemory);
	if(!_currentToken->is(Token::Symbol, "]")){
		arguments->push_back(_parseExpression());
		_checkUnexpectedEnd();

		while(_currentToken->is(Token::Symbol, ",")){
			_readNextToken();
			_checkUnexpectedEnd();
			if(_currentToken->is(Token::Symbol, "[")){
				_parseExpressionArray();
			}else if(_currentToken->is(Token::Symbol, "{")){
				_parseExpressionAssociativeArray();
			}else{
				arguments->push_back(_parseExpression());
			}
			_checkUnexpectedEnd();
		}
		
		if(!_currentToken->is(Token::Symbol, "]"))
			_generateException(ExpectedSymbol, "]");
	}

	_readNextToken(); // Skip ']'
	return new Array(arguments);
}

Expression* Parser::_parseExpressionAssociativeArray(){
	_readNextToken(); // Skip '{'
	_checkUnexpectedEnd();

	std::list<Assignment*>* values = new std::list<Assignment*>();
	if(!values)
		_generateException(NoMemory);
	if(!_currentToken->is(Token::Symbol, "}")){
		values->push_back(_parseExpressionAssociativeArrayPair());
		_checkUnexpectedEnd();

		while(_currentToken->is(Token::Symbol, ",")){
			_readNextToken();
			_checkUnexpectedEnd();
			values->push_back(_parseExpressionAssociativeArrayPair());
			_checkUnexpectedEnd();
		}
		
		if(!_currentToken->is(Token::Symbol, "}"))
			_generateException(ExpectedSymbol, "}");
	}

	_readNextToken(); // Skip '}'
	return new AssociativeArray(values);
}

Nodes::Assignment* Parser::_parseExpressionAssociativeArrayPair(){
	if(_currentToken->getType() != Token::String && _currentToken->getType() != Token::Word)
		_generateException(ExpectedKey);

	if(_reserved.isDeclared(_currentToken->getValue()) == Reference<std::string>::IsDeclared)
		_generateException(InvalidReservedWord, "key");

	Variable* var = new Variable(_currentToken->getValue());
	if(!var)
		_generateException(NoMemory);

	_readNextToken();
	_checkUnexpectedEnd();
	if(!_currentToken->is(Token::Symbol, ":"))
		_generateException(ExpectedSymbol, ":");
	_states.push(SpecialStatement);
	Assignment* value = static_cast<Assignment*>(_parseStatementAssignment(var));
	_states.pop();

	return value;
	
}

Expression* Parser::_parseExpressionArrayAccess(const std::string& name){
	if(_reserved.isDeclared(name) == Reference<std::string>::IsDeclared)
		_generateException(InvalidReservedWord, "variable");

	if(_variableScope->addReference(name) != Reference<std::string>::AlreadyDeclared)
		_generateException(UndeclaredVariable, name);

	std::list<Expression*>* expressions = new std::list<Expression*>();
	if(!expressions)
		_generateException(NoMemory);

	// We go through all referenced elements.
	while(42){
		_readNextToken(); // Skip [
		_checkUnexpectedEnd();

		expressions->push_back(_parseExpression());
		
		_skipToken(Token::Symbol, "]");

		if(!_currentToken->is(Token::Symbol, "["))
			break;
	}

	return new Variable(name, expressions);
}

FunctionCall* Parser::_parseFunctionCall(const std::string& name){
	if(_reserved.isDeclared(name) == Reference<std::string>::IsDeclared)
		_generateException(InvalidReservedWord, "function");

	_readNextToken(); // Skip '('
	_checkUnexpectedEnd();

	std::list<Expression*>* arguments = new std::list<Expression*>();
	if(arguments == NULL){
		_generateException(NoMemory);
	}

	if(!_currentToken->is(Token::Symbol, ")")){
		arguments->push_back(_parseExpression());

		while(_currentToken->is(Token::Symbol, ",")){
			_readNextToken();
			arguments->push_back(_parseExpression());
			_checkUnexpectedEnd();
		}
		
		if(!_currentToken->is(Token::Symbol, ")"))
			_generateException(ExpectedSymbol, ")");
	}

	try{
		_functions.addReference(_FunctionReference(name, arguments->size(), _FunctionReference::Reference));
	}catch(Exception e){
		_generateException(InvalidCall, "main");
		return NULL;
	}

	_readNextToken(); // Skip ')'
	return new FunctionCall(name, arguments);
}

void Parser::_generateException(_Error error, const std::string& message, long line){
	std::stringstream build;
	
	if(error == NoMemory){
		build << "Not enough memory. Please try again later." << std::endl;
	}else if(error == Unfinished){
		build << "Code is not finished." << std::endl;
	}else if(error == ExpectedFunction){
		build << "Expected function." << std::endl;
	}else if(error == InvalidStatement){
		build << "Invalid Statement: is " << message << "." << std::endl;
	}else if(error == InvalidReservedWord){
		build << "Used reserved word as " << message << "." << std::endl;
	}else if(error == ExpectedVariable){
		build << "Expected variable." << std::endl;
	}else if(error == InvalidDeclaration){
		build << "Invalid declaration of " << message << "." << std::endl;
	}else if(error == UndeclaredVariable){
		build << "Usage of undeclared variable \"" << message << "\"." << std::endl;
	}else if(error == UndeclaredFunction){
		build << "Usage of undeclared function \"" << message << "\"." << std::endl;
	}else if(error == ExpectedOperation){
		build << "Expected operation." << std::endl;
	}else if(error == InvalidExpression){
		build << "Invalid expression." << std::endl;
	}else if(error == InvalidNumber){
		build << "Invalid Number." << std::endl;
	}else if(error == InvalidCall){
		build << "Invalid Call to \"" << message << "\"" << std::endl;
	}else if(error == ExpectedSymbol){
		build << "Expected \"" << message << "\", got ";
		if(_currentToken != NULL){
			build << "\"" << _currentToken->getValue() << "\"";
		}else{
			build << "nothing";
		}
		build << "." << std::endl;
	}else if(error == ExpectedKey){
		build << "Expected key." << std::endl;
	}else{
		build << "Unkown error." << std::endl;
	}

	if(line == 0){
		if(_currentToken != NULL)
			build << "On line: " << _currentToken->getLine() << "." << std::endl;
	}else{
		build << "On line: " << line << "." << std::endl;
	}

	throw Exception(Exception::ParsingError, static_cast<int>(error), build.str());
}

template <class T>
bool Parser::_convertString(T& t, const std::string& s, std::ios_base& (*f)(std::ios_base&)){
	std::istringstream stream(s);
	return !(stream >> f >> t).fail();
}

void Parser::_readNextToken(){
	// Read next token until next valid token
	do{
		if(_currentToken != NULL)
			delete _currentToken;
		_currentToken = NULL;

		_currentToken = _tokenizer->readNextToken();
	}while(_currentToken != NULL && _currentToken->getType() == Token::Skip);
}

void Parser::_skipToken(Token::TokenType type, const std::string& value) throw(Exception){
	// Skip the expected token
	_checkUnexpectedEnd();

	if(_currentToken->is(type, value)){
		_readNextToken();
		return;
	}else{
		_generateException(ExpectedSymbol, value);
	}
}

void Parser::_checkUnexpectedEnd() throw(Exception){
	if(_isFinished()){
		_generateException(Unfinished);
	}
}

bool Parser::_isFinished(){
	if(_currentToken == NULL)
		return true;
	else
		return false;
}

void Parser::_prepareReserved(){
	_reserved.addDeclaration("function");
	_reserved.addDeclaration("var");

	_reserved.addDeclaration("if");
	_reserved.addDeclaration("else");
	_reserved.addDeclaration("while");
	_reserved.addDeclaration("do");
	_reserved.addDeclaration("for");
	_reserved.addDeclaration("foreach");
	_reserved.addDeclaration("new");

	_reserved.addDeclaration("return");
	_reserved.addDeclaration("continue");
	_reserved.addDeclaration("break");

	_reserved.addDeclaration("true");
	_reserved.addDeclaration("false");
	_reserved.addDeclaration("null");

	// PHP KEYWORDS!
	_reserved.addDeclaration("abstract");
	_reserved.addDeclaration("and");
	_reserved.addDeclaration("array");
	_reserved.addDeclaration("as");
	_reserved.addDeclaration("case");
	_reserved.addDeclaration("catch");
	_reserved.addDeclaration("cfunction");
	_reserved.addDeclaration("class");
	_reserved.addDeclaration("clone");
	_reserved.addDeclaration("const");
	_reserved.addDeclaration("declare");
	_reserved.addDeclaration("default");
	_reserved.addDeclaration("elseif");
	_reserved.addDeclaration("enddeclare");
	_reserved.addDeclaration("endfor");
	_reserved.addDeclaration("endforeach");
	_reserved.addDeclaration("endif");
	_reserved.addDeclaration("endswitch");
	_reserved.addDeclaration("endwhile");
	_reserved.addDeclaration("extends");
	_reserved.addDeclaration("final");
	_reserved.addDeclaration("global");
	_reserved.addDeclaration("goto");
	_reserved.addDeclaration("implements");
	_reserved.addDeclaration("interface");
	_reserved.addDeclaration("instanceof");
	_reserved.addDeclaration("namespace");
	_reserved.addDeclaration("new");
	_reserved.addDeclaration("old_function");
	_reserved.addDeclaration("or");
	_reserved.addDeclaration("private");
	_reserved.addDeclaration("protected");
	_reserved.addDeclaration("try");
	_reserved.addDeclaration("use");
	_reserved.addDeclaration("while");
	_reserved.addDeclaration("xor");

	_reserved.addDeclaration("__CLASS__");
	_reserved.addDeclaration("__DIR__");
	_reserved.addDeclaration("__FILE__");
	_reserved.addDeclaration("__FUNCTION__");
	_reserved.addDeclaration("__METHOD__");
	_reserved.addDeclaration("__NAMESPACE__");

	_reserved.addDeclaration("die");
	_reserved.addDeclaration("empty");
	_reserved.addDeclaration("exit");
	_reserved.addDeclaration("eval");
	_reserved.addDeclaration("include");
	_reserved.addDeclaration("include_once");
	_reserved.addDeclaration("isset");
	_reserved.addDeclaration("list");
	_reserved.addDeclaration("require");
	_reserved.addDeclaration("require_once");
	_reserved.addDeclaration("print");
	_reserved.addDeclaration("unset");
}

void Parser::_prepareRequired(){
	// main function with no parameters
	_functions.addReference(_FunctionReference("main", 0, _FunctionReference::Reference));
}

void Parser::_prepareLanguage(){
	_functions.addDeclaration(_FunctionReference("echo", 1, _FunctionReference::Declaration));
}

/* ==============================================================================
 * _ F U N C T I O N R E F E R E N C E   PRIVATE CLASS
 * ============================================================================*/

Parser::_FunctionReference::_FunctionReference(std::string name, int params, Type type)
	: _name(name), _params(params), _type(type){
}

Parser::_FunctionReference::~_FunctionReference(){
}

bool Parser::_FunctionReference::operator==(const _FunctionReference& reference)const{
	// We do seperate check for "main" function, as it must have zero parameters in all cases.
	// This is because main is the entry point for the program, so it must absolutely
	// conform to the specifications.
	if(reference.getName() == "main"){
		// We do not want any reference of main
		if(reference.getType() == Reference)
			throw Exception(Exception::ParsingError, 0, "Invalid call to main.");

		if(reference.getParams() != 0)
			return false;
	}
	
	// We check all the parameters we need
	if(reference.getName() == _name){
		// Since this happens in the isDeclared method, the left one is the declaration, the right
		// one is the reference. The right one has to have either equal or more references to pass
		// as equal. We of course must make sure the left one isn't a reference first.

		// We don't want no double declaration! Blasphemy!
		if(_type == Declaration && reference.getType() == Declaration) 
			return true;

		// Was the reference declared first?
		if(_type == Reference && reference.getType() == Declaration)
			if(_params < reference.getParams())
				return false;
			else
				return true;

		// Both references? We don't care then
		if(_type == Reference && reference.getType() == Reference)
			return false;

		// Right one has to have more or equal amount of parameters
		if(reference.getParams() >= _params)
			return true;
		else
			return false;

	}else
		return false;
}

const std::string& Parser::_FunctionReference::getName() const{
	return _name;
}

int Parser::_FunctionReference::getParams() const{
	return _params;
}

Parser::_FunctionReference::Type Parser::_FunctionReference::getType() const{
	return _type;
}
