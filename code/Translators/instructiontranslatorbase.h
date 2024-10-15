#pragma once

#include <Expressions/expression.h>

#include <unordered_map>
#include <functional>


namespace Translators
{
using StringList = std::vector<std::string>;
using TranslatorFunc = std::function<void(const StringList& bOperands, Expressions::ExpressionList& expressions)>;

class InstructionTranslatorBase
{
public:
   InstructionTranslatorBase() = default;
   virtual ~InstructionTranslatorBase() = default;

   const TranslatorFunc& at(const std::string& name) const;

protected:
   std::unordered_map<std::string, TranslatorFunc> m_instructionMap;
};
}
