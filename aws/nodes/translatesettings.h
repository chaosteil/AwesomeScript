#ifndef AWS_NODES_TRANSLATESETTINGS
#define AWS_NODES_TRANSLATESETTINGS

/*
 * aws/nodes/nodeexception.h
 * AwesomeScript Translate Settings
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

namespace AwS{
	class TranslateSettings{
		public:
			TranslateSettings(const std::string& varPrefix = "", const std::string& funcPrefix = "")
				: _varPrefix(varPrefix), _funcPrefix(funcPrefix){
				}
			~TranslateSettings();

			const std::string& getVarPrefix() const{ return _varPrefix; }
			const std::string& getFunctionPrefix() const{
				if(_funcPrefix == "" && _varPrefix != "")
					return _varPrefix;
				else
					return _funcPrefix;
			}

		private:
			const std::string _varPrefix, _funcPrefix;
	};
};

#endif
