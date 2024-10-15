#include "instructiontranslatorbase.h"

namespace Translators
{
const TranslatorFunc& InstructionTranslatorBase::at(const std::string& name) const
{
   return m_instructionMap.at(name);
}
}
