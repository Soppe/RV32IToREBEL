#pragma once

#include <string>

class ParseUtils
{
public:
   static std::string TEMP_LABEL_PREFIX;

   enum class ASSEMBLER_MODIFIER{HI, LO, PCREL, PCRELHI, PCRELLO, UNDEFINED};
   static bool parseAssemblerModifier(const std::string& in, ASSEMBLER_MODIFIER& type, std::string& value);
   static bool parseRegisterOffset(const std::string& in, std::string& offset, std::string& rs1);
   static bool parseImmediate(std::uint8_t immediateSizeBits, const std::string& in, std::int32_t& out);
   static bool parseImmediate(std::uint8_t immediateSizeBits, std::int32_t in, std::int32_t &out);
   static void generateTempLabel(std::string& out);
};
