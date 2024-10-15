#pragma once

#include "instructiontranslatorbase.h"

#include <Expressions/instruction.h>

namespace Translators
{
class Translator
{
public:
   static void Translate(const std::string& targetISAName, const Expressions::ExpressionList& from, Expressions::ExpressionList& to);
};

}
