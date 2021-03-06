/*
 * Copyright (c) 2014 Patrick P. Frey
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
/// \brief Structure describing a document attribute
/// \file documentAttribute.hpp
#ifndef _STRUS_ANALYZER_DOCUMENT_ATTRIBUTE_HPP_INCLUDED
#define _STRUS_ANALYZER_DOCUMENT_ATTRIBUTE_HPP_INCLUDED
#include <string>

/// \brief strus toplevel namespace
namespace strus {
/// \brief analyzer parameter and return value objects namespace
namespace analyzer {

/// \brief Structure describing a document attribute
class DocumentAttribute
{
public:
	/// \brief Default constructor
	DocumentAttribute(){}
	/// \brief Constructor
	/// \param[in] n name of the attribute
	/// \param[in] v value of the attribute
	DocumentAttribute( const std::string& n, const std::string& v)
		:m_name(n),m_value(v){}
	/// \brief Copy constructor
#if __cplusplus >= 201103L
	DocumentAttribute( DocumentAttribute&& ) = default;
	DocumentAttribute( const DocumentAttribute& ) = default;
	DocumentAttribute& operator= ( DocumentAttribute&& ) = default;
	DocumentAttribute& operator= ( const DocumentAttribute& ) = default;
#else
	DocumentAttribute( const DocumentAttribute& o)
		:m_name(o.m_name),m_value(o.m_value){}
#endif

	/// \brief Get the name of the attribute
	/// \return Name of the attribute
	const std::string& name() const		{return m_name;}
	/// \brief Get the value of the attribute
	/// \return Value of the attribute
	const std::string& value() const	{return m_value;}

private:
	std::string m_name;
	std::string m_value;
};

}}//namespace
#endif

