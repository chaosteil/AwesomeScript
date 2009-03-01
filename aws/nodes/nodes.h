#ifndef AWS_NODES_NODES_H
#define AWS_NODES_NODES_H

/*
 * aws/nodes/nodes.h
 * AwesomeScript Nodes Header
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include "parsernode.h"

// Statements
#include "statement.h"

#include "functiondefinition.h"
#include "functioncallstatement.h"
#include "block.h"
#include "ifstatement.h"
#include "whilestatement.h"
#include "dowhilestatement.h"
#include "forstatement.h"

#include "breakstatement.h"
#include "continuestatement.h"
#include "returnstatement.h"

#include "variable.h"
#include "variabledeclaration.h"
#include "assignment.h"
#include "booleanconstant.h"
#include "numberconstant.h"
#include "nullconstant.h"
#include "stringconstant.h"
#include "increase.h"
#include "decrease.h"
#include "additionequal.h"
#include "substractionequal.h"
#include "multiplicationequal.h"
#include "divisionequal.h"
#include "modulusequal.h"

// Expressions
#include "expression.h"
#include "unaryexpression.h"
#include "binaryexpression.h"
#include "groupexpression.h"

#include "addition.h"
#include "substraction.h"
#include "multiplication.h"
#include "division.h"
#include "modulus.h"

#include "comparison.h"
#include "andexpression.h"
#include "orexpression.h"
#include "negation.h"
#include "notexpression.h"
#include "array.h"
#include "associativearray.h"
#include "functioncall.h"

#endif
