/*
 * Copyright (c) 2014 Patrick P. Frey
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "strus/lib/aggregator_vsm.hpp"
#include "strus/aggregatorFunctionInterface.hpp"
#include "strus/aggregatorFunctionInstanceInterface.hpp"
#include "strus/errorBufferInterface.hpp"
#include "strus/analyzer/documentTerm.hpp"
#include "strus/analyzer/functionView.hpp"
#include "strus/base/dll_tags.hpp"
#include "strus/base/string_conv.hpp"
#include "private/errorUtils.hpp"
#include "private/internationalization.hpp"
#include <vector>
#include <string>
#include <map>
#include <cmath>

using namespace strus;
using namespace strus::analyzer;

static bool g_intl_initialized = false;

#define MODULE_NAME "VSM aggregator"

// Function types implemented for VSM aggregation
typedef double (*AggregatorFunctionCall)( const std::vector<double>& input);

double sumSquareTfAggregatorFunctionCall( const std::vector<double>& input)
{
	double sum=0.0;
	std::vector<double>::const_iterator ii=input.begin(), ie=input.end();
	for (; ie!=ie;++ie)
	{
		sum += (*ii) * (*ii);
	}
	return sqrt( sum);
}

class VsmAggregatorFunctionInstance
	:public AggregatorFunctionInstanceInterface
{
public:
	/// \brief Constructor
	VsmAggregatorFunctionInstance( const std::string& featuretype_, AggregatorFunctionCall call_, const std::string& name_, ErrorBufferInterface* errorhnd)
		:m_featuretype( string_conv::tolower( featuretype_)),m_call(call_),m_name(name_),m_errorhnd(errorhnd){}

	virtual NumericVariant evaluate( const analyzer::Document& document) const
	{
		try
		{
			std::map<std::string,std::size_t> termmap;
			std::vector<double> tfar;
			std::vector<DocumentTerm>::const_iterator
				si = document.searchIndexTerms().begin(),
				se = document.searchIndexTerms().end();
	
			for (; si != se; ++si)
			{
				if (si->type() == m_featuretype)
				{
					std::map<std::string,std::size_t>::const_iterator ti = termmap.find( si->value());
					if (ti == termmap.end())
					{
						termmap[ si->value()] = tfar.size();
						tfar.push_back( 1.0);
					}
					else
					{
						++tfar[ ti->second];
					}
				}
			}
			return m_call( tfar);
		}
		CATCH_ERROR_ARG1_MAP_RETURN( _TXT("error in '%s' aggregator: %s"), m_name.c_str(), *m_errorhnd, (NumericVariant::IntType)0);
	}

	virtual analyzer::FunctionView view() const
	{
		try
		{
			const char* function = "";
			if (m_call == &sumSquareTfAggregatorFunctionCall)
			{
				function = "sumsqaretf";
			}
			return analyzer::FunctionView( "vsm")
				( "featuretype", m_featuretype)
				( "aggregationtype", m_name)
				( "function", function)
			;
		}
		CATCH_ERROR_MAP_RETURN( _TXT("error in introspection: %s"), *m_errorhnd, analyzer::FunctionView());
	}

private:
	std::string m_featuretype;
	AggregatorFunctionCall m_call;
	std::string m_name;
	ErrorBufferInterface* m_errorhnd;
};

class VsmAggregatorFunction
	:public AggregatorFunctionInterface
{
public:
	explicit VsmAggregatorFunction( AggregatorFunctionCall call_, const char* name_, const char* description_, ErrorBufferInterface* errorhnd_)
		:m_description(description_),m_name(name_),m_call(call_),m_errorhnd(errorhnd_){}

	virtual AggregatorFunctionInstanceInterface* createInstance( const std::vector<std::string>& args) const
	{
		if (args.size() == 0)
		{
			m_errorhnd->report( ErrorCodeIncompleteDefinition, _TXT("feature type name as argument expected for '%s' aggregator function"), m_name);
			return 0;
		}
		if (args.size() > 1)
		{
			m_errorhnd->report( ErrorCodeInvalidArgument, _TXT("too many arguments passed to '%s' aggregator function"), m_name);
			return 0;
		}
		try
		{
			return new VsmAggregatorFunctionInstance( args[0], m_call, m_name, m_errorhnd);
		}
		CATCH_ERROR_ARG1_MAP_RETURN( _TXT("error in '%s' aggregator: %s"), m_name, *m_errorhnd, 0);
	}

	virtual const char* getDescription() const
	{
		return m_description;
	}

private:
	const char* m_description;
	const char* m_name;
	AggregatorFunctionCall m_call;
	ErrorBufferInterface* m_errorhnd;
};


DLL_PUBLIC AggregatorFunctionInterface* strus::createAggregator_sumSquareTf( ErrorBufferInterface* errorhnd)
{
	try
	{
		if (!g_intl_initialized)
		{
			strus::initMessageTextDomain();
			g_intl_initialized = true;
		}
		return new VsmAggregatorFunction( &sumSquareTfAggregatorFunctionCall, "sumsqaretf", _TXT("aggregator for calculating the sum of the square of the tf of all selected elements"), errorhnd);
	}
	CATCH_ERROR_ARG1_MAP_RETURN( _TXT("cannot create '%s' aggregator: %s"), "sumsqaretf", *errorhnd, 0);
}



