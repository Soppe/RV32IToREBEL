#ifndef INSTRUCTIONCONVERTERBASE_H
#define INSTRUCTIONCONVERTERBASE_H

#include <vector>
#include <list>
#include <string>
#include <unordered_map>
#include <functional>

class Expression;

namespace Converters
{
using StringList = std::vector<std::string>;
using ExpressionList = std::list<Expression*>;
using ConverterFunc = std::function<void(const StringList& bOperands, ExpressionList& expressions)>;

class InstructionConverterBase : public std::unordered_map<std::string, ConverterFunc>
{
public:
   InstructionConverterBase() = default;
   virtual ~InstructionConverterBase() = default;

   virtual void initMap() = 0;
};
}

#endif // INSTRUCTIONCONVERTERBASE_H
