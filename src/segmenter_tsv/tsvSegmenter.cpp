/*
 * Copyright (C) 2016 Andreas Baumann
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "tsvSegmenter.hpp"

#define LOWLEVEL_DEBUG

#ifdef LOWLEVEL_DEBUG
#include <iostream>
#endif

#include <algorithm>
#include <cstring>

// TSVParserDefinition

TSVParserDefinition::TSVParserDefinition( )
	: m_map( ), m_it( m_map.end( ) ), m_end( m_map.end( ) )
{
}

void TSVParserDefinition::addDefinition( int id, const std::string &definition )
{
#ifdef LOWLEVEL_DEBUG	
	std::cout << "DEBUG: adding selector expression: " << id << ", " << definition << std::endl;
#endif
	m_map.insert( std::make_pair( definition, id ) );

#ifdef LOWLEVEL_DEBUG	
	std::cout << "DEBUG: definition multimap contains: ";
	for( std::multimap<std::string, int>::const_iterator it = m_map.begin( ); it != m_map.end( ); it++ ) {
		std::cout << "[" << it->first << ", " << it->second << "], ";
	}
	std::cout << std::endl;
#endif
}

int TSVParserDefinition::getNextId( const std::string &definition )
{
#ifdef LOWLEVEL_DEBUG	
	std::cout << "DEBUG: lookup expression for: " << definition << std::endl;
#endif
	int id = 0;
	if( m_it != m_map.end( ) ) {
		m_it++;
		if( m_it != m_end ) {
			id = m_it->second;
		} else {
			m_it = m_map.end( );
			m_end = m_map.end( );
		}
	} else {		
		std::pair <std::multimap<std::string, int>::const_iterator, std::multimap<std::string, int>::const_iterator> ret;
		ret = m_map.equal_range( definition );
		if( ret.first->first == definition ) {
			m_it = ret.first;
			m_end = ret.second;
			id = m_it->second;
		} else {
			m_it = m_map.end( );
			m_end = m_map.end( );
		}
	}
#ifdef LOWLEVEL_DEBUG	
	std::cout << "DEBUG: got number for '" << definition << "' to be " << id << std::endl;
#endif
	return id;
}

bool TSVParserDefinition::moreOftheSame( )
{
#ifdef LOWLEVEL_DEBUG	
	std::cout << "DEBUG: moreOftheSame is " <<  ( m_it != m_map.end( ) && m_it != m_end ) << std::endl;
#endif
	return m_it != m_map.end( ) && m_it != m_end;
}

// TSVSegmenterContext

TSVSegmenterContext::TSVSegmenterContext( TSVParserDefinition *parserDefinition, strus::ErrorBufferInterface *errbuf, bool errorReporting )
	: m_errbuf( errbuf ), m_errorReporting( errorReporting), m_buf( ),
	m_eof( false ), m_is( m_buf ), m_currentLine( ),
	m_parserDefinition( parserDefinition ), m_data( ), m_pos( -2 ), m_linepos( 0 ),
	m_parseState( TSV_PARSE_STATE_HEADER ), m_headers( )
{
}
	
TSVSegmenterContext::~TSVSegmenterContext( )
{
}
			
void TSVSegmenterContext::putInput( const char *chunk, std::size_t chunksize, bool eof )
{
	if( m_eof ) {
		m_errbuf->report( "fed chunk after declared end of input" );
		return;
	}
	
	m_buf.append( chunk, chunksize );
	m_eof |= eof;
	
	m_is.str( m_buf );
}

static std::vector<std::string> splitLine( const std::string &s, const std::string &delimiter, bool keepEmpty )
{
	std::vector<std::string> result; 

	if( delimiter.empty( ) ) { 
		result.push_back( s );
		return result;
	}

	std::string::const_iterator b = s.begin( ), e;
	while( true ) {
		e = std::search( b, s.end( ), delimiter.begin( ), delimiter.end( ) );
		std::string tmp( b, e );
		if( keepEmpty || !tmp.empty( ) ) {
			result.push_back( tmp );
		}
		if( e == s.end( ) ) {
			break;
		}
		b = e + delimiter.size( );
	}

	return result;
}

bool TSVSegmenterContext::parseHeader( )
{
	m_headers = splitLine( m_currentLine, "\t", true );
	return true;
}

bool TSVSegmenterContext::parseData( int &id, strus::SegmenterPosition &pos, const char *&segment, std::size_t &segmentsize )
{
	// hard-coded field line number (not present in the TSV file itself)
	if( m_pos == -1 ) {
		id = m_parserDefinition->getNextId( "lineno" );
		if( id != 0 ) {
			if( !m_parserDefinition->moreOftheSame( ) ) {
				m_pos++;
			}
			pos = m_linepos * m_headers.size( );
			snprintf( m_lineposStr, 11, "%d", m_linepos );
			segment = m_lineposStr;
			segmentsize = strlen( m_lineposStr );
			return true;
		}

		if( !m_parserDefinition->moreOftheSame( ) ) {
			m_pos++;
		}
	}

	if( m_pos == 0 ) {
		m_data = splitLine( m_currentLine, "\t", true );
	}
		
	do {
		std::string headerName = m_headers[m_pos];
#ifdef LOWLEVEL_DEBUG	
		std::cout << "DEBUG: field " << m_linepos << ":" << m_pos << "'" << headerName << "': '" << m_data[m_pos] << "'" << std::endl;
#endif
		
		id = m_parserDefinition->getNextId( headerName );
		
		if( id == 0 ) {
			m_pos++;
		}
		
	} while( id == 0 && (size_t)m_pos < m_headers.size( ) );

	if( id != 0 ) {
		// TODO: think whether we should not return the character position into the TSV here
		pos = m_linepos * m_headers.size( ) + m_pos;
		segment = m_data[m_pos].c_str( );
		segmentsize = m_data[m_pos].size( );
		if( !m_parserDefinition->moreOftheSame( ) ) {
			m_pos++;
		}
		if( (size_t)m_pos >= m_headers.size( ) ) {
			m_pos = -2;
		}
		return true;
	}

	m_pos++;
	if( (size_t)m_pos >= m_headers.size( ) ) {
		m_pos = -2;
	}
	
	return false;
}

bool TSVSegmenterContext::getNext( int &id, strus::SegmenterPosition &pos, const char *&segment, std::size_t &segmentsize )
{
NEXTLINE:
	
	bool hasNext = false;
	switch( m_parseState ) {
		case TSV_PARSE_STATE_HEADER:
			std::getline( m_is, m_currentLine );
			if( m_is.eof( ) ) {
				// we don't have a complete line to work with, so wait for more data
				return false;
			}
			if( !( hasNext = parseHeader( ) ) ) {
				m_parseState = TSV_PARSE_STATE_EOF;
				return false;
			}
			m_parseState = TSV_PARSE_STATE_DATA;
			m_buf.clear( );
			goto NEXTLINE;
			
		case TSV_PARSE_STATE_DATA:
			if( m_pos < -1 ) {
				std::getline( m_is, m_currentLine );
				if( m_is.eof( ) ) {
					if( m_eof ) {
						m_parseState = TSV_PARSE_STATE_EOF;
						return false;
					}	
					// we don't have a complete line to work with, so wait for more data
					return false;
				}
				m_linepos++;
				m_pos = -1;
			}
				
			if( ( hasNext = parseData( id, pos, segment, segmentsize ) ) ) {
				return hasNext;
			} else {
				goto NEXTLINE;
			}
			break;

		case TSV_PARSE_STATE_EOF:
#ifdef LOWLEVEL_DEBUG
			std::cout << "DEBUG: end of data" << std::endl;
#endif
			return false;
	}
	
	return false;
}

// TSVSegmenterInstance

TSVSegmenterInstance::TSVSegmenterInstance( strus::ErrorBufferInterface *errbuf, bool errorReporting )
	: m_errbuf( errbuf ), m_errorReporting( errorReporting )
{
	m_parserDefinition.reset( new TSVParserDefinition( ) );
}

void TSVSegmenterInstance::defineSelectorExpression( int id, const std::string &expression )
{
	m_parserDefinition->addDefinition( id, expression );
}
		
void TSVSegmenterInstance::defineSubSection( int startId, int endId, const std::string &expression )
{
	// TODO
#ifdef LOWLEVEL_DEBUG	
	std::cout << "DEBUG: selector subsection: " << startId << ", " 
		<< endId << ", " << expression << std::endl;
#endif
}

strus::SegmenterContextInterface* TSVSegmenterInstance::createContext( const strus::DocumentClass &dclass ) const
{
	return new TSVSegmenterContext( m_parserDefinition.get( ), m_errbuf, m_errorReporting );
}

// TSVSegmenter

TSVSegmenter::TSVSegmenter( strus::ErrorBufferInterface *errbuf, bool errorReporting )
	: m_errbuf( errbuf ), m_errorReporting( errorReporting )
{
}

const char* TSVSegmenter::mimeType( ) const
{
	return "text/tab-separated-values";
}

strus::SegmenterInstanceInterface* TSVSegmenter::createInstance( ) const
{
	return new TSVSegmenterInstance( m_errbuf, m_errorReporting );
}
