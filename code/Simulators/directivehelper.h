#ifndef DIRECTIVEHELPER_H
#define DIRECTIVEHELPER_H


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

   DirectiveHelper();

   static bool resolveSectionTypeAndIfChanged(const Expressions::Directive& directive, SectionType& sectionType);
};

}
#endif // DIRECTIVEHELPER_H
