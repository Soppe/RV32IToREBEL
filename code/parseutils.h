#ifndef PARSEUTILS_H
#define PARSEUTILS_H

#include <string>

class ParseUtils
{
public:
   static std::string RELO_LABEL_PREFIX;

   enum class ASSEMBLER_MODIFIER{HI, LO, PCRELHI, PCRELLO, UNDEFINED};
   static bool parseAssemblerModifier(const std::string& in, ASSEMBLER_MODIFIER& type, std::string& value);
   static bool parseRegisterOffset(const std::string& in, std::string& offset, std::string& rs1);
   static bool parseImmediate(int immediateSize, const std::string& in, int& out);
   static bool parseImmediate(int immediateSize, int in, int &out);
   static void generateReloLabel(std::string& out);
};


#endif // PARSEUTILS_H
