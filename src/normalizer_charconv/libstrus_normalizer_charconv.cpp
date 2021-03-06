/*
 * Copyright (c) 2014 Patrick P. Frey
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "strus/lib/normalizer_charconv.hpp"
#include "strus/errorBufferInterface.hpp"
#include "normalizerCharConv.hpp"
#include "strus/base/dll_tags.hpp"
#include "private/internationalization.hpp"
#include "private/errorUtils.hpp"

static bool g_intl_initialized = false;

using namespace strus;


DLL_PUBLIC NormalizerFunctionInterface* strus::createNormalizer_lowercase( ErrorBufferInterface* errorhnd)
{
	try
	{
		if (!g_intl_initialized)
		{
			strus::initMessageTextDomain();
			g_intl_initialized = true;
		}
		return new LowercaseNormalizerFunction( errorhnd);
	}
	CATCH_ERROR_ARG1_MAP_RETURN( _TXT("cannot create '%s' normalizer: %s"), "lc", *errorhnd, 0);
}

DLL_PUBLIC NormalizerFunctionInterface* strus::createNormalizer_uppercase( ErrorBufferInterface* errorhnd)
{
	try
	{
		if (!g_intl_initialized)
		{
			strus::initMessageTextDomain();
			g_intl_initialized = true;
		}
		return new UppercaseNormalizerFunction( errorhnd);
	}
	CATCH_ERROR_ARG1_MAP_RETURN( _TXT("cannot create '%s' normalizer: %s"), "uc", *errorhnd, 0);
}

DLL_PUBLIC NormalizerFunctionInterface* strus::createNormalizer_convdia( ErrorBufferInterface* errorhnd)
{
	try
	{
		if (!g_intl_initialized)
		{
			strus::initMessageTextDomain();
			g_intl_initialized = true;
		}
		return new DiacriticalNormalizerFunction( errorhnd);
	}
	CATCH_ERROR_ARG1_MAP_RETURN( _TXT("cannot create '%s' normalizer: %s"), "convdia", *errorhnd, 0);
}

DLL_PUBLIC NormalizerFunctionInterface* strus::createNormalizer_charselect( ErrorBufferInterface* errorhnd)
{
	try
	{
		if (!g_intl_initialized)
		{
			strus::initMessageTextDomain();
			g_intl_initialized = true;
		}
		return new CharSelectNormalizerFunction( errorhnd);
	}
	CATCH_ERROR_ARG1_MAP_RETURN( _TXT("cannot create '%s' normalizer: %s"), "charselect", *errorhnd, 0);
}





