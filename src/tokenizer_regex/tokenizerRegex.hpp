/*
 * Copyright (c) 2014 Patrick P. Frey
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
/// \brief Implementation of the regular expression tokenizer function
/// \file tokenizerRegex.hpp
#ifndef _STRUS_ANALYZER_TOKENIZER_FUNCTION_REGEX_IMPLEMENTATION_HPP_INCLUDED
#define _STRUS_ANALYZER_TOKENIZER_FUNCTION_REGEX_IMPLEMENTATION_HPP_INCLUDED
#include "strus/tokenizerFunctionInterface.hpp"

/// \brief strus toplevel namespace
namespace strus {

/// \brief Forward declaration
class ErrorBufferInterface;

/// \class RegexTokenizerFunction
/// \brief Regular expression tokenizer constructor
class RegexTokenizerFunction
	:public TokenizerFunctionInterface
{
public:
	RegexTokenizerFunction( ErrorBufferInterface* errorhnd_)
		:m_errorhnd(errorhnd_){}
	virtual ~RegexTokenizerFunction(){}

	virtual TokenizerFunctionInstanceInterface* createInstance(
			const std::vector<std::string>& args,
			const TextProcessorInterface* tp) const;

	virtual const char* getDescription() const;

private:
	ErrorBufferInterface* m_errorhnd;
};

}//namespace
#endif


