/*
 * Copyright (c) 2014 Patrick P. Frey
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
/// \brief Exported functions of the strus segmenter library
#include "strus/lib/segmenter_textwolf.hpp"
#include "strus/analyzerErrorBufferInterface.hpp"
#include "segmenter.hpp"
#include "private/dll_tags.hpp"
#include "private/internationalization.hpp"
#include "private/errorUtils.hpp"

static bool g_intl_initialized = false;

using namespace strus;

DLL_PUBLIC SegmenterInterface* strus::createSegmenter_textwolf( AnalyzerErrorBufferInterface* errorhnd)
{
	try
	{
		if (!g_intl_initialized)
		{
			strus::initMessageTextDomain();
			g_intl_initialized = true;
		}
		return new Segmenter( errorhnd);
	}
	CATCH_ERROR_MAP_RETURN( _TXT("cannot create segmenter based on textwolf: %s"), *errorhnd, 0);
}


