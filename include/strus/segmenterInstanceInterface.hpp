/*
---------------------------------------------------------------------
    The C++ library strus implements basic operations to build
    a search engine for structured search on unstructured data.

    Copyright (C) 2013,2014 Patrick Frey

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

--------------------------------------------------------------------

	The latest version of strus can be found at 'http://github.com/patrickfrey/strus'
	For documentation see 'http://patrickfrey.github.com/strus'

--------------------------------------------------------------------
*/
#ifndef _STRUS_ANALYZER_SEGMENTER_INSTANCE_INTERFACE_HPP_INCLUDED
#define _STRUS_ANALYZER_SEGMENTER_INSTANCE_INTERFACE_HPP_INCLUDED
#include <utility>
#include <string>

#ifdef _MSC_VER
#include <BaseTsd.h>
namespace strus {
	///\typedef SegmenterPosition
	///\brief Byte position in scanned source
	typedef INT64 SegmenterPosition;
}//namespace
#else
#include <stdint.h>
namespace strus {
	///\typedef SegmenterPosition
	///\brief Byte position in scanned source
	typedef int64_t SegmenterPosition;
}//namespace
#endif

namespace strus
{

/// \class SegmenterInstanceInterface
/// \brief Defines an instance of a segmenter program
class SegmenterInstanceInterface
{
public:
	/// \brief Destructor
	virtual ~SegmenterInstanceInterface(){}

	/// \brief Fetch the next text segment
	/// \param[out] id identifier of the expression that addresses the text segment (defined with SegmenterInterface::defineSelectorExpression(int, const std::string&) or with SegmenterInterface::defineSubSection(int,int,const std::string&))
	/// \param[out] pos position of the segment in the original source
	/// \param[out] segment pointer to the start of the segment. Must remain a valid reference during the whole lifetime of this segmented instance.
	/// \param[out] segmentsize size of segment in bytes
	/// \return true, if a valid segment could be returned, false in case of EOF (no segments left)
	/// \remark throws on error
	virtual bool getNext( int& id, SegmenterPosition& pos, const char*& segment, std::size_t& segmentsize)=0;
};

}//namespace
#endif
