#include "directivehelper.h"
#include <Expressions/directive.h>
#include <unordered_map>

namespace
{
std::unordered_map<std::string, Simulators::DirectiveHelper::SectionType> directiveSectionMap =
    {
        {".text", Simulators::DirectiveHelper::SectionType::TEXT},
        {".bss", Simulators::DirectiveHelper::SectionType::BSS},
        {".sbss", Simulators::DirectiveHelper::SectionType::BSS},
        {".data", Simulators::DirectiveHelper::SectionType::DATA},
        {".sdata", Simulators::DirectiveHelper::SectionType::DATA},
        {".rodata", Simulators::DirectiveHelper::SectionType::RODATA},
        {".srodata", Simulators::DirectiveHelper::SectionType::RODATA}
};
}
namespace Simulators
{
DirectiveHelper::DirectiveHelper()
{

}

bool DirectiveHelper::resolveSectionTypeAndIfChanged(const Expressions::Directive& directive, SectionType& sectionType)
{
   bool retVal = false;

   std::string section = directive.getDirectiveName();

   if(section == ".section")
   {
      section = directive.getDirectiveParameters()[0];
   }

   auto it = directiveSectionMap.find(section);
   if(it != directiveSectionMap.end())
   {
      if(it->second != sectionType)
      {
         sectionType = it->second;
         retVal = true;
      }
   }

   return retVal;
}
}
