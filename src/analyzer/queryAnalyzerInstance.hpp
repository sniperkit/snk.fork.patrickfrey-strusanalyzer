/*
 * Copyright (c) 2014 Patrick P. Frey
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef _STRUS_QUERY_ANALYZER_HPP_INCLUDED
#define _STRUS_QUERY_ANALYZER_HPP_INCLUDED
#include "strus/queryAnalyzerInstanceInterface.hpp"
#include "strus/normalizerFunctionInstanceInterface.hpp"
#include "strus/tokenizerFunctionInstanceInterface.hpp"
#include "strus/analyzer/token.hpp"
#include "featureConfigMap.hpp"
#include "patternFeatureConfigMap.hpp"
#include "patternMatchConfigMap.hpp"
#include <vector>
#include <string>
#include <utility>
#include <map>
#include <set>

namespace strus
{
/// \brief Forward declaration
class ErrorBufferInterface;

/// \brief Query analyzer implementation
class QueryAnalyzerInstance
	:public QueryAnalyzerInstanceInterface
{
public:
	explicit QueryAnalyzerInstance( ErrorBufferInterface* errorhnd)
		:m_featureConfigMap()
		,m_preProcPatternMatchConfigMap(),m_postProcPatternMatchConfigMap(),m_patternFeatureConfigMap()
		,m_fieldTypeFeatureMap()
		,m_fieldTypePatternMap()
		,m_searchIndexTermTypeSet()
		,m_errorhnd(errorhnd){}
	virtual ~QueryAnalyzerInstance(){}

	virtual void addElement(
			const std::string& termtype,
			const std::string& fieldtype,
			TokenizerFunctionInstanceInterface* tokenizer,
			const std::vector<NormalizerFunctionInstanceInterface*>& normalizers,
			int priority);

	virtual void addPatternLexem(
			const std::string& termtype,
			const std::string& fieldtype,
			TokenizerFunctionInstanceInterface* tokenizer,
			const std::vector<NormalizerFunctionInstanceInterface*>& normalizers,
			int priority);

	virtual void defineTokenPatternMatcher(
			const std::string& patternTypeName,
			PatternMatcherInstanceInterface* matcher,
			PatternTermFeederInstanceInterface* feeder);

	virtual void defineContentPatternMatcher(
			const std::string& patternTypeName,
			PatternMatcherInstanceInterface* matcher,
			PatternLexerInstanceInterface* lexer,
			const std::vector<std::string>& fieldTypeNames);

	virtual void addElementFromPatternMatch(
			const std::string& type,
			const std::string& patternTypeName,
			const std::vector<NormalizerFunctionInstanceInterface*>& normalizers,
			int priority);

	virtual std::vector<std::string> queryTermTypes() const;

	virtual std::vector<std::string> queryFieldTypes() const;

	virtual QueryAnalyzerContextInterface* createContext() const;

	virtual analyzer::QueryAnalyzerView view() const;

public:/*QueryAnalyzerContext*/
	typedef std::pair<std::string,int> FieldTypeFeatureDef;
	typedef std::multimap<std::string,int> FieldTypeFeatureMap;
	typedef std::multimap<std::string,int> FieldTypePatternMap;
	typedef std::set<std::string> TermTypeSet;

	const FeatureConfigMap& featureConfigMap() const				{return m_featureConfigMap;}
	const FieldTypeFeatureMap& fieldTypeFeatureMap() const				{return m_fieldTypeFeatureMap;}
	const FieldTypePatternMap& fieldTypePatternMap() const				{return m_fieldTypePatternMap;}
	const PreProcPatternMatchConfigMap& preProcPatternMatchConfigMap() const	{return m_preProcPatternMatchConfigMap;}
	const PostProcPatternMatchConfigMap& postProcPatternMatchConfigMap() const	{return m_postProcPatternMatchConfigMap;}
	const PatternFeatureConfigMap& patternFeatureConfigMap() const			{return m_patternFeatureConfigMap;}

private:
	FeatureConfigMap m_featureConfigMap;
	PreProcPatternMatchConfigMap m_preProcPatternMatchConfigMap;
	PostProcPatternMatchConfigMap m_postProcPatternMatchConfigMap;
	PatternFeatureConfigMap m_patternFeatureConfigMap;
	FieldTypeFeatureMap m_fieldTypeFeatureMap;
	FieldTypePatternMap m_fieldTypePatternMap;
	TermTypeSet m_searchIndexTermTypeSet;
	ErrorBufferInterface* m_errorhnd;
};

}//namespace
#endif

