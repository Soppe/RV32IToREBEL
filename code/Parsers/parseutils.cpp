#include "parseutils.h"

#include <logger.h>

#include <regex>
#include <iostream>

// Define taken from https://github.com/riscv-software-src/riscv-tests/tree/master/isa/macros/scalar/test_macros.h
//#define PARSEUTILS_SEXT_IMM(imm, bits) ((imm) | (-(((imm) >> (bits - 1)) & 1) << (bits - 1)))

// Adapted based on how sign extension is included when bitshifting in c++ (since c++20 - undefined prior to it, but most compilers seem to do arithmetic right shift on a negative number)
#define PARSEUTILS_SEXT_IMM(imm, bits) ((imm << bits) >> bits)

std::string ParseUtils::TEMP_LABEL_PREFIX = ".temp_label_";

bool ParseUtils::parseAssemblerModifier(const std::string& in, ASSEMBLER_MODIFIER& type, std::string& value)
{
   std::string rsDummy;
   return parseAssemblerModifierRs(in, type, value, rsDummy);
}

bool ParseUtils::parseAssemblerModifierRs(const std::string& in, ASSEMBLER_MODIFIER& type, std::string& value, std::string& rs)
{
   // E.g. %pcrel_hi(some_label_123)(optional registry)
   static std::regex modifierRegex("([a-z_%]*)\\(([.a-zA-Z_0-9]*)\\)(\\([a-zA-Z_\\-0-9]*\\))?");

   std::smatch matches;
   bool retVal = false;
   bool regSuccess = false;
   type = ASSEMBLER_MODIFIER::UNDEFINED;

   regSuccess = std::regex_match(in, matches, modifierRegex);

   int numMatches = matches.size();
   if(regSuccess && ((numMatches == 3) || (numMatches == 4)))
   {
      std::string modifier = matches[1];
      value = matches[2];
      if(numMatches == 4)
      {
         rs = matches[3];
      }
      retVal = true;

      if     (modifier == "%pcrel_hi") type = ASSEMBLER_MODIFIER::PCRELHI;
      else if(modifier == "%pcrel_lo") type = ASSEMBLER_MODIFIER::PCRELLO;
      else if(modifier == "%pcrel") type = ASSEMBLER_MODIFIER::PCREL;
      else if(modifier == "%hi") type = ASSEMBLER_MODIFIER::HI;
      else if(modifier == "%lo") type = ASSEMBLER_MODIFIER::LO;
      else
      {
         std::cerr << __PRETTY_FUNC__ << ": Unsupported assembler modifier " << modifier << std::endl;
         abort();
      }
   }

   return retVal;
}

bool ParseUtils::parseRegisterOffset(const std::string& in, std::string& offset, std::string& rs1)
{
   // E.g. -323(x23)
   static std::regex integerOffsetRegex("(-?[0-9]+)\\(([a-zA-Z_\\-0-9]*)\\)");
   // E.g. %pcrel_hi(some_label_123)(x23)
   static std::regex modifierOffsetRegex("([a-z_%]*\\([.a-zA-Z_0-9]*\\))\\(([a-zA-Z_\\-0-9]*)\\)");

   std::smatch matches;
   bool retVal = false;
   bool regSuccess = false;

   if(in[0] == '%')
   {
      regSuccess = std::regex_match(in, matches, modifierOffsetRegex);
   }
   else
   {
      regSuccess = std::regex_match(in, matches, integerOffsetRegex);
   }

   if(regSuccess && (matches.size() == 3))
   {
      offset = matches[1];
      rs1 = matches[2];
      retVal = true;
   }

   return retVal;
}

bool ParseUtils::parseImmediate(std::uint8_t immediateSizeBits, const std::string& in, std::int32_t& out)
{
   std::int32_t number = 0;
   bool retVal = false;

   try {
      if(in.starts_with("0x") || in.starts_with("0X"))
      {
         number = static_cast<std::int32_t>(std::stoul(in, nullptr, 16));
         retVal = true;
      }
      else
      {
         number = static_cast<std::int32_t>(std::stoul(in));
         retVal = true;
      }
      out = PARSEUTILS_SEXT_IMM(number, (32 - immediateSizeBits));
   }
   catch(std::exception& e)
   {
      retVal = false;
   }

   return retVal;
}

bool ParseUtils::parseImmediate(std::uint8_t immediateSizeBits, std::int32_t in, std::int32_t& out)
{
   return parseImmediate(immediateSizeBits, std::to_string(in), out);
}

void ParseUtils::generateTempLabel(std::string& out)
{
   static std::uint32_t counter = 0;
   out = TEMP_LABEL_PREFIX + std::to_string(counter);
   counter++;
}
