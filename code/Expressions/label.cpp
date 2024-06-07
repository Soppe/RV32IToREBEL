#include "label.h"

#include <iostream>

namespace Expressions
{
Label::Label(const std::string& labelName)
    : m_name(labelName)
{

}

Expression::ExpressionType Label::getExpressionType() const
{
   return ExpressionType::LABEL;
}

void Label::print() const
{
   std::cout << m_name << ":";
}

const std::string& Label::getLabelName() const
{
   return m_name;
}

}
