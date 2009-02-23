#include "awslang.h"
#include <iostream>

using namespace AwS;

// ==============================================================================
// R E F E R E N C E   CLASS
// ==============================================================================

// ==============================================================================
// P U B L I C   M E T H O D S
// ==============================================================================

template<class T>
Reference<T>::Reference(){
}

template<class T>
Reference<T>::~Reference(){

}

template<class T>
void Reference<T>::add(T& element, bool declaration){
	// Check if it's a declaration
	if(declaration == true){
		// throw aexception if already declared
		_alreadyAvailable(element);
		// Remove from used list
		if(_checkUnused(element))
			_unusedElements.remove(element);

		// Add to declared list
		_declaredElements.push_back(element);
	}else{
		// If it's declared or already marked unused, don't add it
		if(_checkAvailable(element) || _checkUnused(element))
			return;
		_unusedElements.push_back(element);
	}
}

template<class T>
bool Reference<T>::isClean() const{
	if(_unusedElements.empty())return true;

	return false;
}

template<class T>
bool Reference<T>::_checkDeclared(T& element) const{
	for(typename std::list<T>::iterator i = _declaredElements.begin(); i != _declaredElements.end(); ++i)
		if(element == *i)return true;
	
	return false;
}

template<class T>
bool Reference<T>::_checkUnused(T& element) const{
	for(typename std::list<T>::iterator i = _unusedElements.begin(); i != _unusedElements.end(); ++i)
		if(element == *i)return true;

	return false;
}

template<class T>
void Reference<T>::_alreadyDeclared(T& element) const throw(Exception){
	// Throw exception if it's declared
	if(_checkDeclared(element)){
		throw Exception(Exception::SyntaxError, "Already declared");
	}
}

