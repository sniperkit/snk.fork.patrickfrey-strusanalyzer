/*
 * Copyright (c) 2014 Patrick P. Frey
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "normalizerDictMap.hpp"
#include "strus/normalizerFunctionInterface.hpp"
#include "strus/normalizerFunctionInstanceInterface.hpp"
#include "strus/errorBufferInterface.hpp"
#include "strus/base/fileio.hpp"
#include "strus/base/symbolTable.hpp"
#include "private/errorUtils.hpp"
#include "private/internationalization.hpp"
#include <cstring>
#include <cstring>
#include <cstdio>
#include <cerrno>
#include <sys/stat.h>

using namespace strus;

#define NORMALIZER_NAME "dictmap"

class KeyMap
{
public:
	virtual ~KeyMap(){}
	virtual bool set( const std::string& key, const std::string& value)=0;
	virtual bool get( const std::string& key, std::string& value) const=0;
};


class DictMap :public KeyMap
{
public:
	DictMap(){}
	virtual ~DictMap(){}

	virtual bool set( const std::string& key, const std::string& value);
	virtual bool get( const std::string& key, std::string& value) const;

private:
	conotrie::CompactNodeTrie m_map;
	std::string m_value_strings;
};


bool DictMap::set( const std::string& key, const std::string& value)
{
	std::size_t validx = m_value_strings.size();
	if (!m_map.set( key.c_str(), validx)) return false;
	m_value_strings.append( value);
	m_value_strings.push_back( '\0');
	return true;
}

bool DictMap::get( const std::string& key, std::string& value) const
{
	conotrie::CompactNodeTrie::NodeData validx;
	if (!m_map.get( key.c_str(), validx)) return false;
	value.append( m_value_strings.c_str() + validx);
	return true;
}


class HashMap :public KeyMap
{
public:
	HashMap(){}
	virtual ~HashMap(){}

	virtual bool set( const std::string& key, const std::string& value);
	virtual bool get( const std::string& key, std::string& value) const;

private:
	SymbolTable m_symtab;
	std::vector<unsigned int> m_value_refs;
	std::string m_value_strings;
};


bool HashMap::set( const std::string& key, const std::string& value)
{
	uint32_t id = m_symtab.getOrCreate( key);
	if (!m_symtab.isNew()) throw strus::runtime_error(_TXT("duplicate definition of symbol '%s'"), key.c_str());
	if (id != m_value_refs.size()+1) throw strus::runtime_error(_TXT("internal: inconsistency in data"));
	m_value_strings.push_back('\0');
	m_value_refs.push_back( m_value_strings.size());
	m_value_strings.append( value);
	return true;
}

bool HashMap::get( const std::string& key, std::string& value) const
{
	uint32_t id = m_symtab.get( key);
	if (!id) return false;
	value.append( m_value_strings.c_str() + m_value_refs[ id-1]);
	return true;
}

static void loadFile( KeyMap* keymap, const std::string& filename)
{
	std::string content;
	unsigned int ec = readFile( filename, content);
	if (ec) throw strus::runtime_error(_TXT("failed to load file '%s': %s"), filename.c_str(), ::strerror(ec));
	char delim = ' ';
	char const* cc = content.c_str();
	if (content[0] >= 32 && (content[1] == '\r' || content[1] == '\n'))
	{
		delim = content[0];
		++cc;
		while (*cc == '\r' || *cc == '\n') ++cc;
	}
	while (*cc)
	{
		const char* eoln = std::strchr( cc, '\n');
		if (eoln == 0) eoln = std::strchr( cc, '\0');
		char const* ci = cc;
		for (; ci != eoln && *ci == delim; ++ci){}
		if (ci == eoln)
		{
			cc = (*eoln)?(eoln+1):eoln;
			continue;
		}
		const char* mid = std::strchr( cc, delim);
		if (!mid || mid >= eoln) mid = eoln;

		std::string val( cc, mid - cc);
		std::string key;
		if (mid != eoln)
		{
			key.append( mid+1, eoln-mid-1);
			if (key.size() && key[ key.size()-1] == '\r')
			{
				key.resize( key.size()-1);
			}
		}
		if (!keymap->set( key, val))
		{
			throw strus::runtime_error(_TXT("too many term mappings inserted into structure of normalizer '%s'"), NORMALIZER_NAME);
		}
		cc = (*eoln)?(eoln+1):eoln;
	}
}


class DictMapNormalizerInstance
	:public NormalizerFunctionInstanceInterface
{
public:
	DictMapNormalizerInstance( const std::string& filename, const std::string& defaultResult_, bool defaultOrig_, const TextProcessorInterface* textproc, ErrorBufferInterface* errorhnd)
		:m_errorhnd(errorhnd),m_map(0),m_defaultResult(defaultResult_),m_defaultOrig(defaultOrig_)
	{
		std::size_t sz;
		unsigned int ec = readFileSize( filename, sz);
		if (!ec) throw strus::runtime_error(_TXT("could not open file '%s': %s"), filename.c_str(), ::strerror(ec));
		std::auto_ptr<KeyMap> map;
		if (sz > 2000000)
		{
			map.reset( new HashMap());
		}
		else
		{
			map.reset( new DictMap());
		}
		loadFile( map.get(), textproc->getResourcePath( filename));
		m_map = map.release();
	}

	/// \brief Destructor
	virtual ~DictMapNormalizerInstance()
	{
		delete m_map;
	}

	virtual std::string normalize(
			const char* src,
			std::size_t srcsize) const
	{
		try
		{
			std::string key( src, srcsize);
			std::string rt;
			if (m_map->get( key, rt))
			{
				return rt;
			}
			else if (m_defaultOrig)
			{
				return key;
			}
			else
			{
				return m_defaultResult;
			}
		}
		CATCH_ERROR_MAP_ARG1_RETURN( _TXT("error in '%s' normalizer: %s"), NORMALIZER_NAME, *m_errorhnd, std::string());
	}

private:
	ErrorBufferInterface* m_errorhnd;
	KeyMap* m_map;
	std::string m_defaultResult;
	bool m_defaultOrig;
};


NormalizerFunctionInstanceInterface* DictMapNormalizerFunction::createInstance( const std::vector<std::string>& args, const TextProcessorInterface* textproc) const
{
	if (args.size() == 0)
	{
		m_errorhnd->report( _TXT("name of file with key values expected as argument for '%s' normalizer"), NORMALIZER_NAME);
		return 0;
	}
	if (args.size() > 1)
	{
		if (args.size() > 2)
		{
			m_errorhnd->report( _TXT("too many arguments for '%s' normalizer"), NORMALIZER_NAME);
			return 0;
		}
		else
		{
			return new DictMapNormalizerInstance( args[0], args[1], false, textproc, m_errorhnd);
		}
	}
	try
	{
		return new DictMapNormalizerInstance( args[0], std::string(), true, textproc, m_errorhnd);
	}
	CATCH_ERROR_MAP_ARG1_RETURN( _TXT("error in '%s' normalizer: %s"), NORMALIZER_NAME, *m_errorhnd, 0);
}


