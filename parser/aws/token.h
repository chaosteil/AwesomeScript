#ifndef AWS_TOKEN_H
#define AWS_TOKEN_H

/*
 * aws/token.h
 * AwesomeScript Token
 * Author: Dominykas Djacenka
 * Email: Chaosteil@gmail.com
 */

#include <string>

namespace AwS{
	//! Contains a lexical unit extracted from the Tokenizer.
	/*!
	 * A token will be parsed by the Parser based by its type and content.
	 */
	class Token{
		public:
			//! Defines the type of the token. Allows to easily recognize the token.
			enum TokenType{
				None = 0, //!< This token doesn't exist. Should never occur.
				Skip, //!< If this type is thrown, the parser should skip it and read the next.
				Word, //!< Defines a word. A word can be a variable, a function name or any reserved word.
				Boolean, //!< Is either true or false
				Number, //!< A number can be a regular integer or a float.
				Null, //!< A value of nothing.
				String, //!< A string without the apostrophes around it.
				Symbol //!< A single or multiple length symbol, used for various operations.
			};

			//! Constructor.
			/*!
			 * \param type The type of the token, to identify it easier for the Parser.
			 * \param value The contents of the token. Must be a string, even if it is a number.
			 * \param line The line on which this token was found.
			 */
			Token(TokenType type, const std::string& value, const long& line);
			//! Destructor.
			~Token();
			
			//! Gets the current type of the token.
			/*!
			 * \return The type of the Token, allows to identify the Token more easily by the Parser.
			 */
			const TokenType& getType() const;
			//! Gets the current value of the token.
			/*!
			 * \return The value of the Token, is always in a string form, even if it's a number or a symbol
			 */
			const std::string& getValue() const;

			//! Gets the line where the token was found.
			/*!
			 * \return The line of the token. Is always >= 1.
			 */
			const long& getLine() const;
			
			//! Compares the values of the Token to the provided data.
			/* \param type Checks the provided type with the Tokens type.
			 * \param value Checks the provided value with the Tokens value.
			 * \return true if the values are equal, false if not.
			 */
			bool is(TokenType type, const std::string& value) const;

			//! Compares two tokens with each other.
			/*!
			 * \param token The token to compare with the current token.
			 * \return True if the value and type of the tokens are equal, false if not.
			 */
			bool operator==(const Token& token) const;
			//! Compares two tokens with each other.
			/*!
			 * \param token The token to compare with the current token.
			 * \return True if the values of both tokens are not equal, false if they are equal.
			 */
			bool operator!=(const Token& token) const;
		private:
			const TokenType _type; //!< Contains the current type of the token.
			const std::string _value; //!< Contains the current value of the token.
			const long _line; //!< Contains the line on which this token was found.
	};
};

#endif
