#include "label.h"

#include <iostream>

namespace Expressions
{
Label::Label(const std::string& labelName)
    :m_name(labelName)
{
}

Expression::ExpressionType Label::getExpressionType() const
{
   return ExpressionType::LABEL;
}

std::ostream& Label::printToStream(std::ostream& stream) const
{
   stream << m_name << ":";
   return stream;
}

const std::string& Label::getLabelName() const
{
   return m_name;
}

}
