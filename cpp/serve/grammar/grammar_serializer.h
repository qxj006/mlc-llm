/*!
 *  Copyright (c) 2023 by Contributors
 * \file serve/grammar/grammar_serializer.h
 * \brief The header for printing the AST of a BNF grammar.
 */

#ifndef MLC_LLM_SERVE_GRAMMAR_GRAMMAR_SERIALIZER_H_
#define MLC_LLM_SERVE_GRAMMAR_GRAMMAR_SERIALIZER_H_

#include <string>

#include "grammar.h"

namespace mlc {
namespace llm {
namespace serve {

/*!
 * \brief Serialize the abstract syntax tree of a BNF grammar to a string.
 */
class BNFGrammarSerializer {
 public:
  /*!
   * \brief Constructor.
   * \param grammar The grammar to print.
   */
  explicit BNFGrammarSerializer(const BNFGrammar& grammar) : grammar_(grammar) {}

  /*! \brief Serialize the grammar to string. */
  virtual String ToString() = 0;

 protected:
  const BNFGrammar& grammar_;
};

/*!
 * \brief Prints the BNF AST with standard BNF format.
 */
class BNFGrammarPrinter : public BNFGrammarSerializer {
 private:
  using DataKind = BNFGrammarNode::DataKind;
  using RuleExpr = BNFGrammarNode::RuleExpr;

 public:
  /*!
   * \brief Constructor.
   * \param grammar The grammar to print.
   */
  explicit BNFGrammarPrinter(const BNFGrammar& grammar) : BNFGrammarSerializer(grammar) {}

  /*! \brief Print the complete grammar. */
  String ToString() final;

  /*! \brief Print a rule_expr corresponding to the given id. */
  std::string PrintRuleExpr(int32_t rule_expr_id);

  /*! \brief Print rule_exprs for character range. */
  std::string PrintCharacterRange(const RuleExpr& rule_expr);
  /*! \brief Print rule_exprs for empty string. */
  std::string PrintEmptyStr(const RuleExpr& rule_expr);
  /*! \brief Print rule_exprs for rule reference. */
  std::string PrintRuleRef(const RuleExpr& rule_expr);
  /*! \brief Print rule_exprs for rule_expr sequence. */
  std::string PrintSequence(const RuleExpr& rule_expr);
  /*! \brief Print rule_exprs for rule_expr choices. */
  std::string PrintChoices(const RuleExpr& rule_expr);

 private:
  // Only print parentheses when necessary (i.e. when this rule_expr contains multiple elements
  // and is nested within another multi-element rule_expr)
  bool require_parentheses_ = false;
};

/*!
 * \brief Serialize the the raw representation of the BNF AST to a string with JSON format.
 * \sa BNFJSONParser::Parse for parsing the JSON string.
 * \details JSON format:
 *  {
 *    "rules": [
 *      {"name": "...", "rule_expr": rule_expr_id},
 *      {"name": "...", "rule_expr": rule_expr_id},
 *    ],
 *    "rule_expr_data": [integers...],
 *    "rule_expr_indptr": [integers...],
 *  }
 */
class BNFGrammarJSONSerializer : public BNFGrammarSerializer {
 public:
  /*!
   * \brief Constructor.
   * \param grammar The grammar to print.
   */
  explicit BNFGrammarJSONSerializer(const BNFGrammar& grammar, bool prettify = true)
      : BNFGrammarSerializer(grammar), prettify_(prettify) {}

  /*!
   * \brief Dump the raw representation of the AST to a JSON file.
   * \param prettify Whether to format the JSON string. If false, all whitespaces will be removed.
   */
  String ToString() final;

 private:
  bool prettify_;
};

}  // namespace serve
}  // namespace llm
}  // namespace mlc

#endif  // MLC_LLM_SERVE_GRAMMAR_GRAMMAR_SERIALIZER_H_
