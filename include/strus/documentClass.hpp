/*
---------------------------------------------------------------------
    The C++ library strus implements basic operations to build
    a search engine for structured search on unstructured data.

    Copyright (C) 2015 Patrick Frey

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation; either
    version 3 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

--------------------------------------------------------------------

	The latest version of strus can be found at 'http://github.com/patrickfrey/strus'
	For documentation see 'http://patrickfrey.github.com/strus'

--------------------------------------------------------------------
*/
/// \brief Structure describing the MIME type plus some attributes that could be relevant for analysis of a document
/// \file documentClass.hpp
#ifndef _STRUS_ANALYZER_DOCUMENT_CLASS_HPP_INCLUDED
#define _STRUS_ANALYZER_DOCUMENT_CLASS_HPP_INCLUDED
#include <vector>
#include <string>
#include <cstring>

/// \brief strus toplevel namespace
namespace strus {

/// \brief Defines a description of the properties of an original document processed by the segmenter
class DocumentClass
{
public:
	/// \brief Default constructor
	DocumentClass(){}
	/// \brief Constructor
	explicit DocumentClass(
			const std::string& mimeType_)		:m_mimeType(mimeType_){}
	/// \brief Constructor
	DocumentClass(
			const std::string& mimeType_,
			const std::string& encoding_)		:m_mimeType(mimeType_),m_encoding(encoding_){}
	/// \brief Constructor
	DocumentClass(
			const std::string& mimeType_,
			const std::string& encoding_,
			const std::string& scheme_)		:m_mimeType(mimeType_),m_scheme(scheme_),m_encoding(encoding_){}
	/// \brief Copy constructor
	DocumentClass( const DocumentClass& o)			:m_mimeType(o.m_mimeType),m_scheme(o.m_scheme),m_encoding(o.m_encoding){}

	/// \brief Set the MIME type of the document class
	/// \param[in] the document MIME type string
	void setMimeType( const std::string& mimeType_)		{m_mimeType = mimeType_;}
	/// \brief Set the scheme identifier of the document class
	/// \param[in] the document scheme identifier
	void setScheme( const std::string& scheme_)		{m_scheme = scheme_;}
	/// \brief Set the character set encoding of the document class
	/// \param[in] the character set encoding string
	void setEncoding( const std::string& encoding_)		{m_encoding = encoding_;}

	/// \brief Get the MIME type of the document class
	/// \return the document MIME type string
	const std::string& mimeType() const			{return m_mimeType;}
	/// \brief Get the scheme identifier of the document class
	/// \return the document scheme identifier
	const std::string& scheme() const			{return m_scheme;}
	/// \brief Get the character set encoding of the document class
	/// \return the character set encoding string
	const std::string& encoding() const			{return m_encoding;}

private:
	std::string m_mimeType;
	std::string m_scheme;
	std::string m_encoding;
};

}//namespace
#endif

