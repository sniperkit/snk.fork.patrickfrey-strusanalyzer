/*
 * Copyright (c) 2014 Patrick P. Frey
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
/// \brief Interface for a parameterized normalizer function
/// \file normalizerFunctionInstanceInterface.hpp
#ifndef _STRUS_ANALYZER_NORMALIZER_FUNCTION_INSTANCE_INTERFACE_HPP_INCLUDED
#define _STRUS_ANALYZER_NORMALIZER_FUNCTION_INSTANCE_INTERFACE_HPP_INCLUDED
#include "strus/analyzer/functionView.hpp"
#include <string>

/// \brief strus toplevel namespace
namespace strus
{

/// \class NormalizerFunctionInstanceInterface
/// \brief Interface for a parameterized normalization function
class NormalizerFunctionInstanceInterface
{
public:
	/// \brief Destructor
	virtual ~NormalizerFunctionInstanceInterface(){}

	/// \brief Normalization of a token
	/// \param[in] src start of the token to normalize
	/// \param[in] srcsize size of the token in bytes
	/// \return list of normalized tokens
	virtual std::string normalize( const char* src, std::size_t srcsize) const=0;

	/// \brief Get the definition of the function as structure for introspection
	/// \return structure for introspection
	virtual analyzer::FunctionView view() const=0;
};

}//namespace
#endif

