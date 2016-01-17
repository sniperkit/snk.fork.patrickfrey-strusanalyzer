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
/// \brief Interface for a parameterizable instance of a document segmenter
/// \file segmenterInstanceInterface.hpp
#ifndef _STRUS_ANALYZER_SEGMENTER_INSTANCE_INTERFACE_HPP_INCLUDED
#define _STRUS_ANALYZER_SEGMENTER_INSTANCE_INTERFACE_HPP_INCLUDED
#include <vector>
#include <string>

/// \brief strus toplevel namespace
namespace strus
{
/// \brief Forward declaration
class SegmenterContextInterface;
/// \brief Forward declaration
class DocumentClass;

/// \brief Defines a program for splitting a source text it into chunks with an id correspoding to a selecting expression.
class SegmenterInstanceInterface
{
public:
	/// \brief Destructor
	virtual ~SegmenterInstanceInterface(){}

	/// \brief Defines an expression for selecting chunks from a document
	/// \param[in] id identifier of the chunks that match to expression
	/// \param[in] expression expression for selecting chunks
	virtual void defineSelectorExpression( int id, const std::string& expression)=0;

	/// \brief Defines an expression for identifying a sub section of the document.
	/// \param[in] startId identifier to be returned when a sub section of this type starts
	/// \param[in] endId identifier to be returned when a sub section of this type ends
	/// \param[in] expression expression for selecting the sub section
	virtual void defineSubSection( int startId, int endId, const std::string& expression)=0;

	/// \brief Creates an instance of the segmenter
	/// \param[in] dclass description of the document type and encoding to process
	/// \return the segmenter object (with ownership, to be desposed with delete by the caller)
	virtual SegmenterContextInterface* createContext( const DocumentClass& dclass) const=0;
};

}//namespace
#endif

