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
#ifndef _STRUS_DOCUMENT_ANALYZER_HPP_INCLUDED
#define _STRUS_DOCUMENT_ANALYZER_HPP_INCLUDED
#include "strus/documentAnalyzerInterface.hpp"
#include "strus/documentAnalyzerInstanceInterface.hpp"
#include "strus/segmenterInterface.hpp"
#include "strus/segmenterInstanceInterface.hpp"
#include "strus/normalizerFunctionInstanceInterface.hpp"
#include "strus/tokenizerFunctionInstanceInterface.hpp"
#include "private/utils.hpp"
#include <vector>
#include <string>
#include <map>
#include <utility>
#include <stdexcept>

namespace strus
{

/// \brief Document analyzer implementation
class DocumentAnalyzer
	:public DocumentAnalyzerInterface
{
public:
	DocumentAnalyzer( SegmenterInterface* segmenter_);

	virtual ~DocumentAnalyzer()
	{
		delete m_segmenter;
	}

	virtual void addSearchIndexFeature(
			const std::string& type,
			const std::string& selectexpr,
			TokenizerFunctionInstanceInterface* tokenizer,
			const std::vector<NormalizerFunctionInstanceInterface*>& normalizers,
			const FeatureOptions& options)
	{
		defineFeature( FeatSearchIndexTerm, type, selectexpr, tokenizer, normalizers, options);
	}

	virtual void addForwardIndexFeature(
			const std::string& type,
			const std::string& selectexpr,
			TokenizerFunctionInstanceInterface* tokenizer,
			const std::vector<NormalizerFunctionInstanceInterface*>& normalizers,
			const FeatureOptions& options)
	{
		defineFeature( FeatForwardIndexTerm, type, selectexpr, tokenizer, normalizers, options);
	}

	virtual void defineMetaData(
			const std::string& fieldname,
			const std::string& selectexpr,
			TokenizerFunctionInstanceInterface* tokenizer,
			const std::vector<NormalizerFunctionInstanceInterface*>& normalizers)
	{
		defineFeature( FeatMetaData, fieldname, selectexpr, tokenizer, normalizers, FeatureOptions());
	}

	virtual void defineAttribute(
			const std::string& attribname,
			const std::string& selectexpr,
			TokenizerFunctionInstanceInterface* tokenizer,
			const std::vector<NormalizerFunctionInstanceInterface*>& normalizers)
	{
		defineFeature( FeatAttribute, attribname, selectexpr, tokenizer, normalizers, FeatureOptions());
	}

	virtual void defineSubDocument(
			const std::string& subDocumentTypeName,
			const std::string& selectexpr);

	virtual analyzer::Document analyze( const std::string& content) const;

	virtual DocumentAnalyzerInstanceInterface* createInstance() const;

public:
	enum FeatureClass
	{
		FeatMetaData,
		FeatAttribute,
		FeatSearchIndexTerm,
		FeatForwardIndexTerm
	};
	static const char* featureClassName( FeatureClass i)
	{
		static const char* ar[] = {"MetaData", "Attribute", "SearchIndexTerm", "ForwardIndexTerm"};
		return  ar[i];
	}

	class FeatureConfig
	{
	public:
		FeatureConfig( const std::string& name_,
				TokenizerFunctionInstanceInterface* tokenizer,
				const std::vector<NormalizerFunctionInstanceInterface*>& normalizers,
				FeatureClass featureClass_,
				const FeatureOptions& options_);

		FeatureConfig( const FeatureConfig& o)
			:m_name(o.m_name)
			,m_tokenizer(o.m_tokenizer)
			,m_normalizerlist(o.m_normalizerlist)
			,m_featureClass(o.m_featureClass)
			,m_options(o.m_options){}
	
		typedef utils::SharedPtr<NormalizerFunctionInstanceInterface> NormalizerReference;
		typedef utils::SharedPtr<TokenizerFunctionInstanceInterface> TokenizerReference;

		const std::string& name() const					{return m_name;}
		const TokenizerReference& tokenizer() const			{return m_tokenizer;}
		const std::vector<NormalizerReference>& normalizerlist() const	{return m_normalizerlist;}
		FeatureClass featureClass() const				{return m_featureClass;}
		FeatureOptions options() const					{return m_options;}

	private:
		std::string m_name;
		TokenizerReference m_tokenizer;
		std::vector<NormalizerReference> m_normalizerlist;
		FeatureClass m_featureClass;
		FeatureOptions m_options;
	};

private:
	void defineFeature(
		FeatureClass featureClass,
		const std::string& name,
		const std::string& expression,
		TokenizerFunctionInstanceInterface* tokenizer,
		const std::vector<NormalizerFunctionInstanceInterface*>& normalizers,
		const FeatureOptions& options);

	const FeatureConfig& featureConfig( int featidx) const;

private:
	friend class DocumentAnalyzerInstance;
	SegmenterInterface* m_segmenter;
	std::vector<FeatureConfig> m_featurear;
	std::vector<std::string> m_subdoctypear;
};


class ParserContext
{
public:
	ParserContext( const std::vector<DocumentAnalyzer::FeatureConfig>& config);
	ParserContext( const ParserContext& o)
		:m_featureContextAr(o.m_featureContextAr){}
	~ParserContext(){}

	struct FeatureContext
	{
		typedef utils::SharedPtr<NormalizerExecutionContextInterface> NormalizerExecutionContextReference;
		typedef std::vector<NormalizerExecutionContextReference> NormalizerExecutionContextArray;
		typedef utils::SharedPtr<TokenizerExecutionContextInterface> TokenizerExecutionContextReference;

		FeatureContext( const DocumentAnalyzer::FeatureConfig& config);
		FeatureContext( const FeatureContext& o)
			:m_config(o.m_config)
			,m_normalizerContextAr(o.m_normalizerContextAr)
			,m_tokenizerContext(o.m_tokenizerContext){}

		std::string normalize( const char* tok, std::size_t toksize);

		const DocumentAnalyzer::FeatureConfig* m_config;
		NormalizerExecutionContextArray m_normalizerContextAr;
		TokenizerExecutionContextReference m_tokenizerContext;
	};

	FeatureContext& featureContext( int featidx)
	{
		if (featidx <= 0) throw std::logic_error("array bound write (document analyzer parser context)");
		return m_featureContextAr[ featidx-1];
	}

private:
	std::vector<FeatureContext> m_featureContextAr;
};


class DocumentAnalyzerInstance
	:public DocumentAnalyzerInstanceInterface
{
public:
	explicit DocumentAnalyzerInstance( const DocumentAnalyzer* analyzer_);

	virtual ~DocumentAnalyzerInstance()
	{
		delete m_segmenter;
	}

	virtual void putInput(const char* chunk, std::size_t chunksize, bool eof);

	virtual bool analyzeNext( analyzer::Document& doc);

private:
	void clearTermMaps();
	void mapPositions( analyzer::Document& res) const;
	void processDocumentSegment( analyzer::Document& res, int featidx, std::size_t rel_position, const char* elem, std::size_t elemsize);
	void concatDocumentSegment( int featidx, std::size_t rel_position, const char* elem, std::size_t elemsize);
	void processConcatenated( analyzer::Document& res);

private:
	struct Chunk
	{
		Chunk()
			:position(0){}
		Chunk( std::size_t position_, const std::string& content_)
			:position(position_),content(content_){}
		Chunk( const Chunk& o)
			:position(o.position),content(o.content){}
	
		std::size_t position;
		std::string content;
	};
	
	typedef std::map<int,Chunk> ConcatenatedMap;

	struct SuccPositionChunk
	{
		SuccPositionChunk( int featidx_, const char* elem_, std::size_t elemsize_)
			:featidx(featidx_),elem(elem_,elemsize_){}
		SuccPositionChunk( const SuccPositionChunk& o)
			:featidx(o.featidx),elem(o.elem){}
		int featidx;
		std::string elem;
	};

private:
	const DocumentAnalyzer* m_analyzer;
	SegmenterInstanceInterface* m_segmenter;
	ParserContext m_parserContext;
	std::vector<analyzer::Document> m_subdocstack;

	ConcatenatedMap m_concatenatedMap;

	std::vector<analyzer::Term> m_searchTerms;
	std::vector<analyzer::Term> m_forwardTerms;
	bool m_eof;
	SegmenterPosition m_last_position;
	SegmenterPosition m_curr_position;
	SegmenterPosition m_start_position;
	std::vector<SuccPositionChunk> m_succChunks;
};

}//namespace
#endif

