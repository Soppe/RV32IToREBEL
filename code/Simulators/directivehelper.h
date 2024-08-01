#pragma once


namespace Expressions
{
class Directive;
}

namespace Simulators
{
class DirectiveHelper
{
public:
   enum class SectionType {BSS, DATA, TEXT, RODATA};

   DirectiveHelper() = delete;

   static bool resolveSectionTypeAndIfChanged(const Expressions::Directive& directive, SectionType& sectionType);
};

}
