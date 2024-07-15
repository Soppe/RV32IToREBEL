#ifndef CONVERTER_H
#define CONVERTER_H

#include "instructionconverterbase.h"

#include <Expressions/instruction.h>

namespace Converters
{
class Converter
{
public:
   static void Convert(const std::string& targetISAName, const Expressions::ExpressionList& from, Expressions::ExpressionList& to);
};

}

#endif // CONVERTER_H
