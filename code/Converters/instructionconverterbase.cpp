#include "instructionconverterbase.h"

const Converters::ConverterFunc& Converters::InstructionConverterBase::at(const std::string& name) const
{
   return m_instructionMap.at(name);
}
