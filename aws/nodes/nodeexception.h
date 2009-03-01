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
				NoMemory = 0
			};
			NodeException(NodeError error, const std::string& message)
				: Exception(Exception::ConvertError, message), _error(error){
			}

		private:
			NodeError _error;
	};
};

#endif
