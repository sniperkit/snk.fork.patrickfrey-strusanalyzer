/*
---------------------------------------------------------------------
    The C++ library strus implements basic operations to build
    a search engine for structured search on unstructured data.

    Copyright (C) 2013,2014 Patrick Frey

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

--------------------------------------------------------------------

	The latest version of strus can be found at 'http://github.com/patrickfrey/strus'
	For documentation see 'http://patrickfrey.github.com/strus'

--------------------------------------------------------------------
*/
#include "textProcessor.hpp"
#include "strus/tokenizerFunctionInterface.hpp"
#include "strus/tokenizerFunctionInstanceInterface.hpp"
#include "strus/tokenizerExecutionContextInterface.hpp"
#include "strus/normalizerFunctionInterface.hpp"
#include "strus/normalizerFunctionInstanceInterface.hpp"
#include "strus/normalizerExecutionContextInterface.hpp"
#include "strus/analyzer/token.hpp"
#include "private/utils.hpp"
#include "resourceDirectory.hpp"
#include <stdexcept>
#include <cstring>

using namespace strus;
using namespace strus::analyzer;


class EmptyNormalizerExecutionContext
	:public NormalizerExecutionContextInterface
{
public:
	EmptyNormalizerExecutionContext(){}

	virtual std::string normalize( const char* src, std::size_t srcsize)
	{
		return std::string();
	}
};

class EmptyNormalizerInstance
	:public NormalizerFunctionInstanceInterface
{
public:
	EmptyNormalizerInstance(){}

	virtual NormalizerExecutionContextInterface* createExecutionContext() const
	{
		return new EmptyNormalizerExecutionContext();
	}
};

class EmptyNormalizerFunction
	:public NormalizerFunctionInterface
{
public:
	virtual NormalizerFunctionInstanceInterface* createInstance( const std::vector<std::string>& args, const TextProcessorInterface*) const
	{
		if (args.size()) throw std::runtime_error("no arguments expected for 'empty' normalizer");
		return new EmptyNormalizerInstance();
	}
};

class OrigNormalizerExecutionContext
	:public NormalizerExecutionContextInterface
{
public:
	OrigNormalizerExecutionContext(){}

	virtual std::string normalize( const char* src, std::size_t srcsize)
	{
		std::string rt;
		std::size_t ii=0;
		for (;ii<srcsize; ++ii)
		{
			if ((unsigned char)src[ii] <= 32)
			{
				for (;ii+1 < srcsize && (unsigned char)src[ii+1] <= 32; ++ii){}
				rt.push_back( ' ');
			}
			else
			{
				rt.push_back( src[ii]);
			}
		}
		return rt;
	}
};

class OrigNormalizerInstance
	:public NormalizerFunctionInstanceInterface
{
public:
	OrigNormalizerInstance(){}

	virtual NormalizerExecutionContextInterface* createExecutionContext() const
	{
		return new OrigNormalizerExecutionContext();
	}
};

class OrigNormalizerFunction
	:public NormalizerFunctionInterface
{
public:
	virtual NormalizerFunctionInstanceInterface* createInstance( const std::vector<std::string>& args, const TextProcessorInterface*) const
	{
		if (args.size()) throw std::runtime_error("no arguments expected for 'orig' normalizer");
		return new OrigNormalizerInstance();
	}
};

class ContentTokenizerExecutionContext
	:public TokenizerExecutionContextInterface
{
public:
	ContentTokenizerExecutionContext(){}

	virtual std::vector<analyzer::Token>
			tokenize( const char* src, std::size_t srcsize)
	{
		std::vector<analyzer::Token> rt;
		rt.push_back( analyzer::Token( 0, 0, srcsize));
		return rt;
	}
};

class ContentTokenizerInstance
	:public TokenizerFunctionInstanceInterface
{
public:
	virtual TokenizerExecutionContextInterface* createExecutionContext() const
	{
		return new ContentTokenizerExecutionContext();
	}
};

class ContentTokenizerFunction
	:public TokenizerFunctionInterface
{
public:
	virtual TokenizerFunctionInstanceInterface* createInstance( const std::vector<std::string>& args, const TextProcessorInterface* tp) const
	{
		if (args.size()) throw std::runtime_error("no arguments expected for 'content' normalizer");
		return new ContentTokenizerInstance();
	}
};


static ContentTokenizerFunction contentTokenizer;
static OrigNormalizerFunction origNormalizer;
static EmptyNormalizerFunction emptyNormalizer;


TextProcessor::TextProcessor()
{
	defineTokenizer( "content", &contentTokenizer);
	defineNormalizer( "orig", &origNormalizer);
	defineNormalizer( "empty", &emptyNormalizer);
}

const TokenizerFunctionInterface* TextProcessor::getTokenizer( const std::string& name) const
{
	std::map<std::string,const TokenizerFunctionInterface*>::const_iterator
		ti = m_tokenizer_map.find( utils::tolower( name));
	if (ti == m_tokenizer_map.end())
	{
		throw std::runtime_error(std::string("no tokenizer defined with name '") + name + "'");
	}
	return ti->second;
}

const NormalizerFunctionInterface* TextProcessor::getNormalizer( const std::string& name) const
{
	std::map<std::string,const NormalizerFunctionInterface*>::const_iterator
		ni = m_normalizer_map.find( utils::tolower( name));
	if (ni == m_normalizer_map.end())
	{
		throw std::runtime_error(std::string("no normalizer defined with name '") + name + "'");
	}
	return ni->second;
}

void TextProcessor::defineTokenizer( const std::string& name, const TokenizerFunctionInterface* tokenizer)
{
	m_tokenizer_map[ utils::tolower( name)] = tokenizer;
}

void TextProcessor::defineNormalizer( const std::string& name, const NormalizerFunctionInterface* normalizer)
{
	m_normalizer_map[ utils::tolower( name)] = normalizer;
}

void TextProcessor::addResourcePath( const std::string& path)
{
	char const* cc = path.c_str();
	char const* ee = std::strchr( cc, STRUS_RESOURCE_PATHSEP);
	for (; ee!=0; cc=ee+1,ee=std::strchr( cc, STRUS_RESOURCE_PATHSEP))
	{
		m_resourcePaths.push_back( utils::trim( std::string( cc, ee)));
	}
	m_resourcePaths.push_back( utils::trim( std::string( cc)));
}

std::string TextProcessor::getResourcePath( const std::string& filename) const
{
	std::vector<std::string>::const_iterator
		pi = m_resourcePaths.begin(), pe = m_resourcePaths.end();
	for (; pi != pe; ++pi)
	{
		std::string absfilename( *pi + STRUS_RESOURCE_DIRSEP + filename);
		if (utils::isFile( absfilename))
		{
			return absfilename;
		}
	}
	throw std::runtime_error( std::string( "resource file '") + filename + "' not found");
}


