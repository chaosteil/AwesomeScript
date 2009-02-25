#ifndef AWS_REFERENCE_CPP
#define AWS_REFERENCE_CPP

/*
 * aws/parser.cpp
 * AwesomeScript Reference
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

/* ==============================================================================
 * R E F E R E N C E   CLASS
 * ============================================================================*/

#include "reference.h"

using namespace AwS;

/* ==============================================================================
 * P U B L I C   M E T H O D S
 * ============================================================================*/

template<class T> Reference<T>::Reference()
	: _declared(NULL), _referenced(NULL){

	_declared = new std::list<T>;
	_referenced = new std::list<T>;

}

template<class T> Reference<T>::~Reference(){
	// First we completely clean the declared list
	if(_declared){
		delete _declared;
	}

	// Then we completely clean the reference list
	if(_referenced){
		delete _referenced;
	}
}

template<class T> typename Reference<T>::ReferenceStatus Reference<T>::addDeclaration(const T item){
	// Add to declared items. If already declared, return. Else, add declaration.
	if(isDeclared(item) == IsDeclared)
		return AlreadyDeclared;
	if(_declared)
		_declared->push_back(item);

	// Not returned, check if is referenced. Remove all references.
	if(_referenced)
		_referenced->remove(item);

	return IsDeclared;
}

template<class T> typename Reference<T>::ReferenceStatus Reference<T>::addReference(const T item){
	// Check if declared. If already declared, return.
	if(isDeclared(item) == IsDeclared)
		return AlreadyDeclared;
	
	// Not returned, check if referenced. If not, add to list.
	if(_isReferenced(item) == NotReferenced){
		if(_referenced)
			_referenced->push_back(item);
	}

	return IsReferenced;
}

template<class T> typename Reference<T>::ReferenceStatus Reference<T>::isDeclared(const T& item) const{
	if(_declared != NULL){
		// We check each item if it's in the declaration list
		for(typename std::list<T>::iterator i = _declared->begin(); i != _declared->end(); ++i){
			if(*i == item)
				return IsDeclared;
		}
		return NotDeclared;
	}
	return Error;
}

/* ==============================================================================
 * P R I V A T E   M E T H O D S
 * ============================================================================*/

template<class T> typename Reference<T>::ReferenceStatus Reference<T>::_isReferenced(const T& item) const{
	if(_referenced != NULL){
		// We check if the variable is referenced
		for(typename std::list<T>::iterator i = _referenced->begin(); i != _referenced->end(); ++i){
			if(*i == item)
				return IsReferenced;
		}
		// If the variable is not referenced, it can still be declared
		if(isDeclared(item) != IsDeclared)
			return NotReferenced;
		else
			return IsDeclared;
	}
	return Error;
}

#endif
