#ifndef AWSLANG_H
#define AWSLANG_H

	template<class T> class Reference{
		public:
			Reference();
			~Reference();

			void add(T& element, bool declaration);
			bool isClean() const;
		private:
			void _alreadyDeclared(T& element) const throw(Exception);
			bool _checkDeclared(T& element) const;
			bool _checkUnused(T& element) const;

			std::list<T> _declaredElements;
			std::list<T> _unusedElements;
	};

};
#endif
