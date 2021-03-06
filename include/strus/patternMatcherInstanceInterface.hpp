/*
 * Copyright (c) 2016 Patrick P. Frey
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
/// \brief Interface for building the automaton for detecting patterns in text
/// \file "patternMatcherInstanceInterface.hpp"
#ifndef _STRUS_ANALYZER_PATTERN_MATCHER_INSTANCE_INTERFACE_HPP_INCLUDED
#define _STRUS_ANALYZER_PATTERN_MATCHER_INSTANCE_INTERFACE_HPP_INCLUDED
#include "strus/analyzer/functionView.hpp"
#include <string>

namespace strus
{

/// \brief Forward declaration
class PatternMatcherContextInterface;

/// \brief Interface for building the automaton for detecting patterns in text
class PatternMatcherInstanceInterface
{
public:
	/// \brief Destructor
	virtual ~PatternMatcherInstanceInterface(){}

	/// \brief Define an option value for the compilation
	/// \param[in] name option name
	/// \param[in] value option value
	virtual void defineOption( const std::string& name, double value)=0;

	/// \brief Define a relative document term frequency used for optimization of the automaton
	/// \param[in] termid term identifier
	/// \param[in] df document frequency (only compared relatively, value between 0 and a virtual collection size)
	virtual void defineTermFrequency( unsigned int termid, double df)=0;

	/// \brief Push a term on the stack
	/// \param[in] termid term identifier
	virtual void pushTerm( unsigned int termid)=0;

	///\brief Join operations (similar meaning as in query evaluation)
	enum JoinOperation
	{
		OpSequence,		///< The argument patterns must appear in the specified (strict) order (ordinal span) within a specified proximity range of ordinal positions for the completion of the rule.
		OpSequenceImm,		///< Same as OpSequence, but does not allow any gap in between the elements.
		OpSequenceStruct,	///< The argument patterns must appear in the specified (strict) order (ordinal span) within a specified proximity range of ordinal positions for the completion of the rule without a structure element appearing before the last argument pattern needed for then completion of the rule.
		OpWithin,		///< The argument patterns must appear within a specified proximity range of ordinal positions without overlapping ordinal spans for the completion of the rule.
		OpWithinStruct,		///< The argument patterns must appear within a specified proximity range of ordinal positions without overlapping ordinal spans for the completion of the rule without a structure element appearing before the last element for then completion of the rule.
		OpAny,			///< At least one of the argument patterns must appear for the completion of the rule.
		OpAnd			///< All of the argument patterns must appear for the completion of the rule at the same ordinal position.
	};
	static const char* joinOperationName( JoinOperation op)
	{
		static const char* ar[] = {"sequence","sequence_imm","sequence_struct","within","within_struct","any","and",0};
		return ar[ op];
	}

	/// \brief Take the topmost elements from the stack, build an expression out of them and replace the argument elements with the created element on the stack
	/// \param[in] operation identifier of the operation to perform as string
	/// \param[in] argc number of arguments of this operation
	/// \param[in] range position proximity range of the expression
	/// \param[in] cardinality specifies a result dimension requirement (e.g. minimum number of elements of any input subset selection that builds a result) (0 for use default). Interpretation depends on operation, but in most cases it specifies the required size for a valid result.
	/// \note The operation identifiers should if possible correspond to the names used for the standard posting join operators in the query evaluation of the strus core.
	virtual void pushExpression(
			JoinOperation operation,
			std::size_t argc, unsigned int range, unsigned int cardinality)=0;

	/// \brief Push a reference to a pattern on the stack
	/// \param[in] name name of the referenced pattern
	virtual void pushPattern( const std::string& name)=0;

	/// \brief Attaches a variable to the top expression or term on the stack.
	/// \param[in] name name of the variable attached
	/// \remark The stack is not changed
	virtual void attachVariable( const std::string& name)=0;

	/// \brief Create a pattern that can be referenced by the given name and can be declared as part of the result
	/// \param[in] name name of the pattern and the result if declared as visible
	/// \param[in] formatstring format string for the value associated with the pattern (empty if undefined).
	///		The formatstring contains variable references to pattern parts in curly brackets '{' '}'.
	///		A variable reference is an identifier optionally followed by an or '|' and a separator value
	///		used for separate multiple occurrencies of the referenced variable.
	/// \param[in] visible true, if the pattern result should be exported (be visible in the final result)
	virtual void definePattern( const std::string& name, const std::string& formatstring, bool visible)=0;

	/// \brief Compile all patterns defined
	/// \param[in] opt optimization options
	/// \note Tries to optimize the program if possible by setting initial key events of the programs to events that are relative rare
	virtual bool compile()=0;

	/// \brief Create the context to process a document with the pattern matcher
	/// \return the pattern matcher context
	/// \remark The context cannot be reset. So the context has to be recreated for every processed unit (document)
	virtual PatternMatcherContextInterface* createContext() const=0;

	/// \brief Get the definition of the function as structure for introspection
	/// \return structure for introspection
	virtual analyzer::FunctionView view() const=0;
};

} //namespace
#endif


