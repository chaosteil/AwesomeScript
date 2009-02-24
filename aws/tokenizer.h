#ifndef AWS_TOKENIZER_H
#define AWS_TOKENIZER_H

/*
 * aws/tokenizer.h
 * AwesomeScript Tokenizer
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include <istream>
#include <sstream>
#include <string>

#include "exception.h"
#include "token.h"

namespace AwS{
	//! Converts a stream to Tokens.
	/*!
	 * Uses any input stream to return valid to parse Tokens.
	 * The tokenizer skips comments and retrns tokens one by one.
	 */
	class Tokenizer{
		public:
			//! Constructor.
			/*!
			 * \param input The input stream which provides text to convert the text to Tokens. The stream has to be open as long as the Tokenizer works.
			 */
			Tokenizer(std::istream& input);
			//! Destructor.
			~Tokenizer();

			//! Gets the next Token from the input stream.
			/*!
			 * \return A valid Token. If reading from the stream is finished, NULL is returned.
			 */
			Token* readNextToken();
		private:
			//! The state of the comment reading. 
			enum CommentState{
				Comment_None = 0, //!< Not in a Coment, normal reading.
				Comment_Line, //!< In a line, reads till EOL.
				Comment_Block //!< In a comment block, reads till end of block.
			};
			
			//! Reads the current word, as identified by readNextToken().
			/*!
			 * The word won't contain any special symbols except underscores. Can also contain numbers.
			 * \return A valid Token or NULL if invalid.
			 */
			Token* _readWord();
			//! Reads the current number, as identified by readNextToken().
			/*!
			 * The Token will be a normal integer or a float.
			 * \return A valid Token or NULL if invalid.
			 */
			Token* _readNumber();
			//! Reads the current string, as identified by readNextToken().
			/*!
			 * The string Token will contain a string without quotes.
			 * \return A valid Token or NULL if invalid.
			 */
			Token* _readString();
			//! Reads the current symbol, as identified by readNextToken().
			/*!
			 * A symbol consists of either one or two characters.
			 * \return A valid Token or NULL if invalid.
			 */
			Token* _readSymbol();

			void _readNextChar();
			bool _isFinished();
			void _skipWhitespace();
			void _skipComment();
			void _storeCharReadNext();
			std::string _extractString();
			void _checkUnexpectedEnd() throw(Exception);
			void _invalidCharacter() throw(Exception);

			static bool _isNumber(const char letter);
			static bool _isSymbol(const char letter);
			static bool _isLetter(const char letter);
			static bool _isLowercaseLetter(const char letter);
			static bool _isUppercaseLetter(const char letter);

			std::istream& _input;
			char _currentChar;
			long _currentLine;
			std::stringstream _tokenValueBuffer;
			CommentState _commentState;

	};
};

#endif
