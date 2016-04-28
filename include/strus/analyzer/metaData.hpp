/*
 * Copyright (c) 2014 Patrick P. Frey
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
/// \brief Structure describing a document meta data element
/// \file metaData.hpp
#ifndef _STRUS_ANALYZER_METADATA_HPP_INCLUDED
#define _STRUS_ANALYZER_METADATA_HPP_INCLUDED
#include "strus/numericVariant.hpp"

/// \brief strus toplevel namespace
namespace strus {
/// \brief analyzer parameter and return value objects namespace
namespace analyzer {

/// \brief Structure describing a document meta data element
class MetaData
{
public:
	/// \brief Default constructor
	MetaData(){}
	/// \brief Copy constructor
	MetaData( const MetaData& o)
		:m_name(o.m_name),m_value(o.m_value){}

	/// \brief Constructor
	/// \param[in] n name of the meta data element
	/// \param[in] v value of the meta data element
	MetaData( const std::string& n, const NumericVariant& v)
		:m_name(n),m_value(v){}

	/// \brief Get the name of the meta data element
	/// \return Name of the meta data element
	const std::string& name() const		{return m_name;}

	/// \brief Get the value of the meta data element
	/// \return Value of the meta data element
	const NumericVariant& value() const	{return m_value;}

private:
	std::string m_name;
	NumericVariant m_value;
};

}}//namespace
#endif

