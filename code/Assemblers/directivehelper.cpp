#include "directivehelper.h"
#include <Expressions/directive.h>
#include <unordered_map>

namespace
{
std::unordered_map<std::string, Assemblers::DirectiveHelper::SectionType> directiveSectionMap =
    {
        {".text", Assemblers::DirectiveHelper::SectionType::TEXT},
        {".bss", Assemblers::DirectiveHelper::SectionType::BSS},
        {".sbss", Assemblers::DirectiveHelper::SectionType::BSS},
        {".data", Assemblers::DirectiveHelper::SectionType::DATA},
        {".sdata", Assemblers::DirectiveHelper::SectionType::DATA},
        {".rodata", Assemblers::DirectiveHelper::SectionType::RODATA},
        {".srodata", Assemblers::DirectiveHelper::SectionType::RODATA}
};
}
namespace Assemblers
{

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
