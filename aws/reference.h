#ifndef AWS_REFERENCE_H
#define AWS_REFERENCE_H

/*
 * aws/parser.h
 * AwesomeScript Reference
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include <list>

namespace AwS{
	template<class T>
	class Reference{
		public:
			Reference();
			~Reference();

			enum ReferenceStatus{
				IsDeclared = 0,
				NotDeclared,
				AlreadyDeclared,
				IsReferenced,
				NotReferenced,
				Error
			};

			ReferenceStatus addDeclaration(const T item);
			ReferenceStatus addReference(const T item);
			ReferenceStatus isDeclared(const T& item) const;
		private:
			ReferenceStatus _isReferenced(const T& item) const;

			std::list<T>* _declared;
			std::list<T>* _referenced;
	};
};

#endif
