/*
 * Copyright (c) 2014 Patrick P. Frey
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "normalizer_trim.hpp"
#include "strus/errorBufferInterface.hpp"
#include "strus/base/utf8.hpp"
#include "private/errorUtils.hpp"
#include "private/internationalization.hpp"
#include "private/tokenizeHelpers.hpp"
#include "strus/analyzer/functionView.hpp"
#include <cstring>

using namespace strus;

#define NORMALIZER_NAME "trim"

static std::pair<const char*,const char*> trim( char const* si, const char* se)
{
	for (; si < se && whiteSpaceDelimiter( si, se); si = skipChar( si)){}
	while (si < se)
	{
		const char* ec = utf8prev( se);
		if (!whiteSpaceDelimiter( ec, se)) break;
		se = ec;
	}
	return std::pair<const char*,const char*>( si, se);
}

class TrimNormalizerInstance
	:public NormalizerFunctionInstanceInterface
{
public:
	TrimNormalizerInstance( ErrorBufferInterface* errorhnd)
		:m_errorhnd(errorhnd){}

	virtual std::string normalize(
			const char* src,
			std::size_t srcsize) const
	{
		try
		{
			std::pair<const char*,const char*> boundaries = trim( src, src + srcsize);
			return std::string( boundaries.first, boundaries.second - boundaries.first);
		}
		CATCH_ERROR_ARG1_MAP_RETURN( _TXT("error in %s normalize: %s"), NORMALIZER_NAME, *m_errorhnd, std::string());
	}

	virtual analyzer::FunctionView view() const
	{
		try
		{
			return analyzer::FunctionView( NORMALIZER_NAME)
			;
		}
		CATCH_ERROR_MAP_RETURN( _TXT("error in introspection: %s"), *m_errorhnd, analyzer::FunctionView());
	}

private:
	ErrorBufferInterface* m_errorhnd;
};

NormalizerFunctionInstanceInterface* TrimNormalizerFunction::createInstance( const std::vector<std::string>& args, const TextProcessorInterface*) const
{
	try
	{
		if (!args.empty()) throw std::runtime_error( _TXT("too many arguments"));
		return new TrimNormalizerInstance( m_errorhnd);
	}
	CATCH_ERROR_ARG1_MAP_RETURN( _TXT("error in create \"%s\" normalizer instance: %s"), NORMALIZER_NAME, *m_errorhnd, 0);
}

