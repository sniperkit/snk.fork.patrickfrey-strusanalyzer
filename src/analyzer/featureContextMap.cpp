/*
 * Copyright (c) 2014 Patrick P. Frey
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "featureContextMap.hpp"
#include "private/internationalization.hpp"
#include "strus/normalizerFunctionContextInterface.hpp"
#include "strus/tokenizerFunctionContextInterface.hpp"

using namespace strus;

FeatureContext::FeatureContext( const FeatureConfig& config)
	:m_config(&config)
	,m_tokenizerContext(config.tokenizer()->createFunctionContext())
{
	if (!m_tokenizerContext.get())
	{
		throw strus::runtime_error( _TXT("failed to create tokenizer context"));
	}
	std::vector<FeatureConfig::NormalizerReference>::const_iterator
		ni = config.normalizerlist().begin(),
		ne = config.normalizerlist().end();

	for (; ni != ne; ++ni)
	{
		m_normalizerContextAr.push_back( (*ni)->createFunctionContext());
		if (!m_normalizerContextAr.back().get())
		{
			throw strus::runtime_error( _TXT("failed to create normalizer context"));
		}
	}
}

std::string FeatureContext::normalize( char const* tok, std::size_t toksize)
{
	NormalizerFunctionContextArray::iterator
		ci = m_normalizerContextAr.begin(),
		ce = m_normalizerContextAr.end();

	std::string rt;
	std::string origstr;
	for (; ci != ce; ++ci)
	{
		rt = (*ci)->normalize( tok, toksize);
		if (ci + 1 != ce)
		{
			origstr.swap( rt);
			tok = origstr.c_str();
			toksize = origstr.size();
		}
	}
	return rt;
}

FeatureContextMap::FeatureContextMap( const FeatureConfigMap& configmap)
{
	FeatureConfigMap::const_iterator
		ci = configmap.begin(), ce = configmap.end();
	for (; ci != ce; ++ci)
	{
		m_featureContextAr.push_back( FeatureContext( *ci));
	}
}
