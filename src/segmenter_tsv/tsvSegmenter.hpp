/*
 * Copyright (C) 2016 Andreas Baumann
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef TSV_SEGMENTER_H_INLCUDED
#define TSV_SEGMENTER_H_INLCUDED

#include "boost/scoped_ptr.hpp"

#include "strus/errorBufferInterface.hpp"
#include "strus/segmenterInterface.hpp"
#include "strus/segmenterInstanceInterface.hpp"
#include "strus/segmenterContextInterface.hpp"

#include <string> 
#include <map>
#include <stdexcept>
#include <sstream>

class TSVParserException : public std::runtime_error {
	
	public:
	
		TSVParserException( const std::string &what )
			: std::runtime_error( what ) { }
};

class TSVParserDefinition
{
	public:
	
		TSVParserDefinition( );
		
		void addDefinition( int id, const std::string &definition );
		int getNextId( const std::string &definition );
		bool moreOftheSame( );
	
	private:
	
		std::multimap<std::string, int> m_map;
		std::multimap<std::string, int>::const_iterator m_it, m_end;
		
};

class TSVSegmenterContext : public strus::SegmenterContextInterface
{		
	public:
	
		TSVSegmenterContext( TSVParserDefinition *parserDefinition, strus::ErrorBufferInterface *errbuf, const bool errorReporting );
		
		virtual ~TSVSegmenterContext( );
		
		virtual void putInput( const char *chunk, std::size_t chunksize, bool eof );
		
		virtual bool getNext( int &id, strus::SegmenterPosition &pos, const char *&segment, std::size_t &segmentsize );
	
	private:
	
		enum TSVParseState {
			TSV_PARSE_STATE_HEADER,
			TSV_PARSE_STATE_BODY,
			TSV_PARSE_STATE_EOF
		};
	
		strus::ErrorBufferInterface *m_errbuf;
		bool m_errorReporting;
		std::string m_buf;
		bool m_eof;
		TSVParserDefinition *m_parserDefinition;
		std::string m_currentValue;
		std::size_t m_pos;
		enum TSVParseState m_parseState;
		std::istringstream m_is;
		
		bool parseHeader( int &id, strus::SegmenterPosition &pos, const char *&segment, std::size_t &segmentsize );
		bool parseBody( int &id, strus::SegmenterPosition &pos, const char *&segment, std::size_t &segmentsize );
};


class TSVSegmenterInstance : public strus::SegmenterInstanceInterface
{
	public:

		TSVSegmenterInstance( strus::ErrorBufferInterface *errbuf, const bool errorReporting );
		
		virtual void defineSelectorExpression( int id, const std::string &expression );
		
		virtual void defineSubSection( int startId, int endId, const std::string &expression );

		virtual strus::SegmenterContextInterface* createContext( const strus::DocumentClass &dclass) const;

	private:

		strus::ErrorBufferInterface *m_errbuf;
		bool m_errorReporting;
		boost::scoped_ptr<TSVParserDefinition> m_parserDefinition;
};

class TSVSegmenter : public strus::SegmenterInterface
{
	public:

		TSVSegmenter( strus::ErrorBufferInterface *errbuf, const bool errorReporting = true );
		
		virtual const char* mimeType( ) const;

		virtual strus::SegmenterInstanceInterface* createInstance( ) const;

	private:

		strus::ErrorBufferInterface *m_errbuf;
		bool m_errorReporting;
};

#endif
