#ifndef AWS_NODES_TRANSLATESETTINGS
#define AWS_NODES_TRANSLATESETTINGS

/*
 * aws/nodes/nodeexception.h
 * AwesomeScript Translate Settings
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include <ostream>
#include <sstream>

namespace AwS{
	namespace Nodes{
		class TranslateSettings{
			public:
				TranslateSettings(const std::string& varPrefix = "", const std::string& funcPrefix = "")
					: _varPrefix(varPrefix), _funcPrefix(funcPrefix), _ignore(false), _indent(0){
					}
				~TranslateSettings(){}

				const std::string& getVarPrefix() const{ return _varPrefix; }
				const std::string& getFunctionPrefix() const{
					if(_funcPrefix == "" && _varPrefix != "")
						return _varPrefix;
					else
						return _funcPrefix;
				}

				void setIgnoreSemicolon(bool ignore){ _ignore = ignore; }
				bool isIgnoreSemicolon() const{ return _ignore; }

			private:
				const std::string _varPrefix, _funcPrefix;
				bool _ignore;
				int _indent;
		};
	};
};

#endif
