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

template<class T> Reference<T>::Reference(){
}

template<class T> Reference<T>::~Reference(){
	// First we completely clean the declared list
	_declared.clear();
	// Then we completely clean the reference list
	_referenced.clear();
}

template<class T> typename Reference<T>::ReferenceStatus Reference<T>::addDeclaration(const T item){
	// Add to declared items. If already declared, return. Else, add declaration.
	if(isDeclared(item) == IsDeclared)
		return AlreadyDeclared;
		
	_declared.push_back(item);

	// Not returned, check if is referenced. Remove all references.
	_referenced.remove(item);

	return IsDeclared;
}

template<class T> typename Reference<T>::ReferenceStatus Reference<T>::addReference(const T item){
	// Check if declared. If already declared, return.
	if(isDeclared(item) == IsDeclared)
		return AlreadyDeclared;
	
	// Not returned, check if referenced. If not, add to list.
	if(_isReferenced(item) == NotReferenced){
		_referenced.push_back(item);
	}

	return IsReferenced;
}

template<class T> typename Reference<T>::ReferenceStatus Reference<T>::isDeclared(const T& item) const{
	// We check each item if it's in the declaration list
	for(typename std::list<T>::const_iterator i = _declared.begin(); i != _declared.end(); ++i){
		if(*i == item)
			return IsDeclared;
	}
	return NotDeclared;
}

template<class T> const std::list<T>* Reference<T>::getDeclarations() const{
	return &_declared;
}
template<class T> const std::list<T>* Reference<T>::getReferences() const{
	return &_referenced;
}


/* ==============================================================================
 * P R I V A T E   M E T H O D S
 * ============================================================================*/

template<class T> typename Reference<T>::ReferenceStatus Reference<T>::_isReferenced(const T& item) const{
	// We check if the variable is referenced
	for(typename std::list<T>::const_iterator i = _referenced.begin(); i != _referenced.end(); ++i){
		if(*i == item)
			return IsReferenced;
	}
	// If the variable is not referenced, it can still be declared
	if(isDeclared(item) != IsDeclared)
		return NotReferenced;
	else
		return IsDeclared;
}

#endif
