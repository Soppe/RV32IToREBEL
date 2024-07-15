#ifndef INSTRUCTIONCONVERTERBASE_H
#define INSTRUCTIONCONVERTERBASE_H

#include <Expressions/expression.h>

#include <unordered_map>
#include <functional>


namespace Converters
{
using StringList = std::vector<std::string>;
using ConverterFunc = std::function<void(const StringList& bOperands, Expressions::ExpressionList& expressions)>;

class InstructionConverterBase
{
public:
   InstructionConverterBase() = default;
   virtual ~InstructionConverterBase() = default;

   const ConverterFunc& at(const std::string& name) const;

protected:
   std::unordered_map<std::string, ConverterFunc> m_expressionMap;
};
}

#endif // INSTRUCTIONCONVERTERBASE_H
