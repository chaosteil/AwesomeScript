#ifndef AWS_NODES_NODEEXCEPTION
#define AWS_NODES_NODEEXCEPTION

/*
 * aws/nodes/nodeexception.h
 * AwesomeScript Node Exception
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include "../exception.h"

namespace AwS{
	class NodeException : public Exception{
		public:
			enum NodeError{
				Undefined = 0,
				NoMemory
			};
			NodeException(const std::string& message, NodeError error = Undefined)
				: Exception(Exception::ConvertError, static_cast<int>(error), message), _error(error){
			}

		private:
			NodeError _error;
	};
};

#endif
