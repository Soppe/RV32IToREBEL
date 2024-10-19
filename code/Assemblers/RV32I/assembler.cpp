#include "assembler.h"

#include "executableprogram.h"
#include "assemblerutils.h"

#include <Expressions/all_expressions.h>
#include <Assemblers/directivehelper.h>
#include <logger.h>

#include <iostream>
#include <fstream>

namespace Assemblers
{
namespace RV32I
{
Assembler::Assembler(Expressions::ExpressionList& expressions, ExecutableProgram& executable)
    : m_parser(expressions)
    , m_sectionType(DirectiveHelper::SectionType::TEXT) // Until otherwise specified, the asm manual states the default is .text
    , m_executable(executable)
    , m_pc(0)
    , m_hc(0)
{
}

void Assembler::init()
{
   m_sectionType = DirectiveHelper::SectionType::TEXT;
   m_pc = 0;
   m_hc = 0;
   m_tempHeapLabels.clear();
   m_tempLabelsTable.clear();
   m_heapRelocations.clear();
}

void Assembler::run()
{
   m_parser.reset();
   const Expressions::Expression* expr = m_parser.nextExpression();

   while(expr != nullptr)
   {
      switch(m_sectionType)
      {
      case DirectiveHelper::SectionType::TEXT:
         handleTextSection(expr);
         break;
      case DirectiveHelper::SectionType::DATA:
         handleDataSection(expr);
         break;
      case DirectiveHelper::SectionType::BSS:
         handleBssSection(expr);
         break;
      case DirectiveHelper::SectionType::RODATA:
         handleRoDataSection(expr);
         break;
      default:
         std::cerr << __PRETTY_FUNC__ << ": Unknown section type " << static_cast<std::int32_t>(m_sectionType) << std::endl;
         abort();
         break;
      }

      expr = m_parser.nextExpression();
   }

   m_executable.calculateMemorySize();

   // Finished setting up the instruction memory, now we need to resolve the heap labels
   HeapLabelMap::const_iterator it = m_tempHeapLabels.begin();
   std::uint32_t instructionsSizeBytes = m_executable.getInstructionsSizeBytes();

   while(it != m_tempHeapLabels.end())
   {
      m_executable.addSymbol(it->first, it->second + instructionsSizeBytes);
      ++it;
   }

   // Now that we finally know the position of everything we need to resolve the operands
   resolveOperands();
   resolveUnresolvedHeapData();
}

void Assembler::handleTextSection(const Expressions::Expression* expr)
{
   while(expr != nullptr)
   {
      switch(expr->getExpressionType())
      {
      case Expressions::Expression::ExpressionType::DIRECTIVE:
      {
         const Expressions::Directive* directive = static_cast<const Expressions::Directive*>(expr);
         // Not expecting any directives while handling .text
         if(DirectiveHelper::resolveSectionTypeAndIfChanged(*directive, m_sectionType))
         {
            return;
         }
         break;
      }
      case Expressions::Expression::ExpressionType::LABEL:
      {
         const Expressions::Label* label = static_cast<const Expressions::Label*>(expr);
         m_executable.addSymbol(label->getLabelName(), m_pc);
         break;
      }
      case Expressions::Expression::ExpressionType::INSTRUCTION:
      {
         const Expressions::Instruction* instr = static_cast<const Expressions::Instruction*>(expr);
         m_executable.addInstruction(new Expressions::Instruction(*instr), 4);
         m_pc += 4;
         break;
      }
      default:
         // Don't care
         break;
      }

      expr = m_parser.nextExpression();
   }
}

void Assembler::handleDataSection(const Expressions::Expression* expr)
{
   while(expr != nullptr)
   {
      switch(expr->getExpressionType())
      {
      case Expressions::Expression::ExpressionType::DIRECTIVE:
      {
         const Expressions::Directive* directive = static_cast<const Expressions::Directive*>(expr);
         // If section has changed, stop handling it as data
         if(DirectiveHelper::resolveSectionTypeAndIfChanged(*directive, m_sectionType))
         {
            return;
         }

         resolveDataDirective(directive);
         break;
      }
      case Expressions::Expression::ExpressionType::LABEL:
      {
         const Expressions::Label* label = static_cast<const Expressions::Label*>(expr);
         m_tempHeapLabels.insert({label->getLabelName(), m_hc});
         break;
      }
      case Expressions::Expression::ExpressionType::INSTRUCTION:
         std::cerr << __PRETTY_FUNC__ << ": Found instruction \"" << *expr << "\" in data section" << std::endl;
         abort();
         break;

      default:
         // Don't care
         break;
      }

      expr = m_parser.nextExpression();
   }
}

void Assembler::handleBssSection(const Expressions::Expression* expr)
{
   while(expr != nullptr)
   {
      switch(expr->getExpressionType())
      {
      case Expressions::Expression::ExpressionType::DIRECTIVE:
      {
         const Expressions::Directive* directive = static_cast<const Expressions::Directive*>(expr);
         // If section has changed, stop handling it as bss
         if(DirectiveHelper::resolveSectionTypeAndIfChanged(*directive, m_sectionType))
         {
            return;
         }

         resolveDataDirective(directive);
         break;
      }
      case Expressions::Expression::ExpressionType::LABEL:
      {
         const Expressions::Label* label = static_cast<const Expressions::Label*>(expr);
         m_tempHeapLabels.insert({label->getLabelName(), m_hc});
         break;
      }
      case Expressions::Expression::ExpressionType::INSTRUCTION:
         std::cerr << __PRETTY_FUNC__ << ": Found instruction \"" << *expr << "\" in bss section" << std::endl;
         abort();
         break;
      default:
         // Don't care
         break;
      }

      expr = m_parser.nextExpression();
   }
}

void Assembler::handleRoDataSection(const Expressions::Expression* expr)
{
   while(expr != nullptr)
   {
      switch(expr->getExpressionType())
      {
      case Expressions::Expression::ExpressionType::DIRECTIVE:
      {
         const Expressions::Directive* directive = static_cast<const Expressions::Directive*>(expr);
         // If section has changed, stop handling it as rodata
         if(DirectiveHelper::resolveSectionTypeAndIfChanged(*directive, m_sectionType))
         {
            return;
         }

         resolveDataDirective(directive);
         break;
      }
      case Expressions::Expression::ExpressionType::LABEL:
      {
         const Expressions::Label* label = static_cast<const Expressions::Label*>(expr);
         m_tempHeapLabels.insert({label->getLabelName(), m_hc});
         break;
      }
      case Expressions::Expression::ExpressionType::INSTRUCTION:
         std::cerr << __PRETTY_FUNC__ << ": Found instruction \"" << *expr << "\" in rodata section" << std::endl;
         abort();
         break;
      default:
         // Don't care
         break;
      }

      expr = m_parser.nextExpression();
   }
}

bool Assembler::resolveIfObject(const Expressions::Directive* directive, std::uint8_t& byteSizePerElement, std::vector<std::int32_t>& values)
{
   bool retVal = true;
   const std::string& name = directive->getDirectiveName();
   const std::vector<std::string> parameters = directive->getDirectiveParameters();
   std::int32_t value;

   if(name == ".ascii") // 8 bits per letter
   {
      // 7.5 .ascii "string"…
      // .ascii expects zero or more string literals (see Strings) separated by commas. It assembles each string (with no automatic trailing zero byte) into consecutive addresses.

      std::cout << __PRETTY_FUNC__ << ": Unsupported directive object type: " << name << std::endl;
      abort();
   }
   else if(name == ".asciz") // 8 bits per letter, + terminating zero which is also 8 bits
   {
      //7.6 .asciz "string"…
      // .asciz is just like .ascii, but each string is followed by a zero byte. The “z” in ‘.asciz’ stands for “zero”.
      // Note that multiple string arguments not separated by commas will be concatenated together and only one final zero byte will be stored.

      std::cout << __PRETTY_FUNC__ << ": Unsupported directive object type: " << name << std::endl;
      abort();
   }
   else if(name == ".byte") // 8 bits
   {
      // 7.11 .byte expressions
      // .byte expects zero or more expressions, separated by commas. Each expression is assembled into the next byte.

      // Note - this directive is not intended for encoding instructions, and it will not trigger effects like DWARF line number generation.
      // Instead some targets support special directives for encoding arbitrary binary sequences as instructions such as .insn or .inst.

      byteSizePerElement = 1;
      for(const std::string& str: parameters)
      {
         ParseUtils::parseImmediate(8, str, value);
         values.push_back(value);
      }
   }
   else if((name == ".hword") || (name == ".half")) // 16 bits according to ISA
   {
      // 7.44 .hword expressions
      // This expects zero or more expressions, and emits a 16 bit number for each.

      // This directive is a synonym for ‘.short’; depending on the target architecture, it may also be a synonym for ‘.word’.
      byteSizePerElement = 2;
      for(const std::string& str: parameters)
      {
         ParseUtils::parseImmediate(16, str, value);
         values.push_back(value);
      }
   }
   else if(name == ".int") // 32 bits according to ABI
   {
      // 7.49 .std::int32_t expressions
      // Expect zero or more expressions, of any section, separated by commas. For each expression, emit a number that, at run time, is the value of that expression.
      // The byte order and bit size of the number depends on what kind of target the assembly is for.

      // Note - this directive is not intended for encoding instructions, and it will not trigger effects like DWARF line number generation.
      // Instead some targets support special directives for encoding arbitrary binary sequences as instructions such as eg .insn or .inst.

      std::cout << __PRETTY_FUNC__ << ": Unsupported directive object type: " << name << std::endl;
      abort();
   }
   else if(name == ".long") // 32 bits according to ABI
   {
      // 7.62 .long expressions
      // .long is the same as ‘.int’. See .int.

      std::cout << __PRETTY_FUNC__ << ": Unsupported directive object type: " << name << std::endl;
      abort();
   }
   else if(name == ".short") // 16 bits according to ABI
   {
      // 7.87 .short expressions
      // .short is normally the same as ‘.word’. See .word.

      // In some configurations, however, .short and .word generate numbers of different lengths. See Machine Dependent Features.

      // Note - this directive is not intended for encoding instructions, and it will not trigger effects like DWARF line number generation.
      // Instead some targets support special directives for encoding arbitrary binary sequences as instructions such as .insn or .inst.

      std::cout << __PRETTY_FUNC__ << ": Unsupported directive object type: " << name << std::endl;
      abort();
   }
   /*else if(name == ".size") // Currenly not supported, but is normally generated by GCC. It's relevance, however, is unknown.
   {

   }*/
   else if(name == ".skip")
   {
      // 7.90 .skip size [,fill]
      // This directive emits size bytes, each of value fill. Both size and fill are absolute expressions. If the comma and fill are omitted, fill is assumed to be zero. This is the same as ‘.space’.

      std::cout << __PRETTY_FUNC__ << ": Unsupported directive object type: " << name << std::endl;
      abort();
   }
   else if(name == ".space")
   {
      // 7.92 .space size [,fill]
      // This directive emits size bytes, each of value fill. Both size and fill are absolute expressions. If the comma and fill are omitted, fill is assumed to be zero. This is the same as ‘.skip’.


      std::cout << __PRETTY_FUNC__ << ": Unsupported directive object type: " << name << std::endl;
      abort();
   }
   else if(name == ".string") // 8 bits per letter, + terminating zero which is also 8 bits
   {
      // 7.94 .string "str", .string8 "str", .string16
      // "str", .string32 "str", .string64 "str"

      // Copy the characters in str to the object file. You may specify more than one string to copy, separated by commas.
      // Unless otherwise specified for a particular machine, the assembler marks the end of each string with a 0 byte. You can use any of the escape sequences described in Strings.

      // The variants string16, string32 and string64 differ from the string pseudo opcode in that each 8-bit character from str is copied and expanded to 16, 32 or 64 bits respectively.
      // The expanded characters are stored in target endianness byte order.

      //Example:

         //.string32 "BYE"
      //expands to:
         //.string   "B\0\0\0Y\0\0\0E\0\0\0"  /* On little endian targets.  */
         //.string   "\0\0\0B\0\0\0Y\0\0\0E"  /* On big endian targets.  */

      byteSizePerElement = 1;
      for(const std::string& param: parameters)
      {
         for(int i = 0; i <= param.size(); ++i) // Include terminating zero
         {
            ParseUtils::parseImmediate(8, param.c_str()[i], value);
            values.push_back(value);
            // std::cout << "Letter = " << param.c_str()[i] << "; value = " << value << std::endl;
         }
      }
   }
   else if(name == ".word") // 32 bits according to ISA
   {
      // 7.111 .word expressions
      // This directive expects zero or more expressions, of any section, separated by commas.

      // The size of the number emitted, and its byte order, depend on what target computer the assembly is for.

      byteSizePerElement = 4;
      std::uint32_t addr = m_hc;
      for(const std::string& str: parameters)
      {
         // Can refer to labels. In such cases, remember this and resolve the .word value once we know the value of the label.
         // Still allocate space on the heap.
         if(!ParseUtils::parseImmediate(32, str, value))
         {
            value = 0;
            m_heapRelocations.push_back(RelocationItem(addr, 4, str));
         }
         values.push_back(value);
         addr += 4;
      }
   }
   else if(name == ".zero") // 8 bits
   {
      // 7.112 .zero size
      // This directive emits size 0-valued bytes. size must be an absolute expression.
      // This directive is actually an alias for the ‘.skip’ directive so it can take an optional second argument of the value to store in the bytes instead of zero.
      // Using ‘.zero’ in this way would be confusing however.

      std::uint32_t zeroSize = stoul(parameters[0]);

      std::uint32_t numElements = 1;
      if(zeroSize > 4) // It's an array
      {
         switch(zeroSize % 4)
         {
         case 0: // int array
            numElements = zeroSize / 4;
            byteSizePerElement = 4;
            break;
         case 2: // short array
            numElements = zeroSize / 2;
            byteSizePerElement = 2;
            break;
         default: // byte/char array
            numElements = zeroSize;
            byteSizePerElement = 1;
            break;
         }
      }
      else
      {
         byteSizePerElement = zeroSize;
      }

      value = 0;
      if(parameters.size() == 2)
      {
         value = ParseUtils::parseImmediate(byteSizePerElement * 8, parameters[1], value);
      }

      values.resize(numElements, value);
   }
   else if((name == ".2byte") || (name == ".4byte") || (name == ".8byte") || (name == ".string8") || (name == ".string16") || (name == ".string32"))
   {
      std::cout << __PRETTY_FUNC__ << ": Unsupported directive object type: " << name << std::endl;
      abort();
   }
   else
   {
      retVal = false;
   }

   return retVal;
}

void Assembler::resolveOperands()
{
   std::uint32_t pc = 0;
   std::uint8_t instrSize;
   Expressions::Instruction* instr = m_executable.loadInstruction(pc, instrSize);
   while(instr != nullptr)
   {
      std::vector<std::string>& operands = instr->getInstructionOperands();
      for(std::string& operand: operands)
      {
         if(operand[0] == '%')
         {
            std::string value;
            std::string rs;
            ParseUtils::ASSEMBLER_MODIFIER type;
            if(ParseUtils::parseAssemblerModifierRs(operand, type, value, rs))
            {
               try
               {
                  std::int32_t imm = resolveAssemblerModifier(type, value, pc);
                  operand = std::to_string(imm);
                  if(!rs.empty())
                  {
                     operand += rs;
                  }
               }
               catch(std::exception&)
               {
                  // Assume it is a register value
               }
            }
            else
            {
               std::cerr << __PRETTY_FUNC__ << ": Failed to parse " << operand << std::endl;
            }
         }
         else if(operand.starts_with("0x") || operand.starts_with("0X"))
         {
            std::int32_t dummy;
            ParseUtils::parseImmediate(32, operand, dummy);
            operand = std::to_string(dummy);
         }
         else if((std::isalpha(operand[0])) || (operand[0] == '.'))
         {
            try
            {
               std::uint32_t imm = m_executable.loadSymbolAddress(operand);
               operand = std::to_string(imm);
            }
            catch(std::exception&)
            {
               // Assume it is a register value
            }
         }
      }

      // Offset branching and jumps based on PC
      AssemblerUtils::InstructionType instrType = AssemblerUtils::getInstructionType(instr->getInstructionName());
      if((instrType == AssemblerUtils::InstructionType::BRANCH) || (instrType == AssemblerUtils::InstructionType::JUMP))
      {
         std::int32_t opInt = stoi(operands.back());
         opInt = opInt - pc;
         if((opInt % 2) != 0)
         {
            std::cerr << __PRETTY_FUNC__ << ": offset not 2 byte aligned: " << *instr << std::endl;
            abort();
         }
         // Last bit should always be 0 since every instruction should be at least 2-byte aligned - we discard it because the CPU "adds" it back again automatically.
         opInt >>= 1;
         operands.back() = std::to_string(opInt);
      }

      pc += instrSize;
      instr = m_executable.loadInstruction(pc, instrSize);
   }
}

void Assembler::resolveUnresolvedHeapData()
{
   for(const RelocationItem& item: m_heapRelocations)
   {
      try
      {
         std::uint32_t value = m_executable.loadSymbolAddress(item.label);
         m_executable.storeToHeap(item.address + m_executable.getInstructionsSizeBytes(), value, item.sizeInBytes);
      }
      catch(std::exception&)
      {
         std::cerr << __PRETTY_FUNC__ << ": Failed to resolve label " << item.label << std::endl;
         abort();
      }
   }
}

std::int32_t Assembler::resolveAssemblerModifier(const ParseUtils::ASSEMBLER_MODIFIER& modifier, const std::string& imm, std::uint32_t pc)
{
   std::int32_t immi = 0;
   try
   {
      immi = m_executable.loadSymbolAddress(imm);
   }
   catch(std::exception&)
   {
      std::cerr << __PRETTY_FUNC__ << ": Failed to load symbol address of symbol " << imm << std::endl;
      abort();
   }

   switch(modifier)
   {
   case ParseUtils::ASSEMBLER_MODIFIER::HI:
      // Compensate for signedness from %lo, don't sign extend as it makes lui grumpy
      immi = ((immi >> 12) + ((immi & 0x800) ? 1 : 0)) & 0xfffff;
      break;
   case ParseUtils::ASSEMBLER_MODIFIER::LO:
      ParseUtils::parseImmediate(12, immi, immi);
      break;
   case ParseUtils::ASSEMBLER_MODIFIER::PCRELHI:
   {
      // Store what symbol %pcrel_hi points to, as %pcrel_lo points to the the address of the instruction related to %pcrel_hi rather than the symbol itself,
      // but is still interested in the symbol value.
      // Pc is the address of the %pcrel_hi-related instruction, whereas imm is the symbol name
      m_tempLabelsTable.insert({pc, imm});

      std::int32_t delta = immi - pc;
      // Compensate for signedness from %pcrel_lo by adding +1 if its sign bit is set, but don't sign extend as it makes lui grumpy
      immi = ((delta >> 12) + ((delta & 0x800) ? 1 : 0)) & 0xfffff;
      break;
   }
   case ParseUtils::ASSEMBLER_MODIFIER::PCRELLO:
   {
      pc = immi;
      try
      {
         std::string symbol = m_tempLabelsTable.at(pc);
         immi = m_executable.loadSymbolAddress(symbol);
      }
      catch(std::exception& e)
      {
         std::cerr << __PRETTY_FUNC__ << ": Failed to retrieve %pcrel_lo data for PC = " << pc << " where relo label = " << imm << "; e.what() = " << e.what() << std::endl;
      }

      immi = immi - pc;
      ParseUtils::parseImmediate(12, immi, immi);
      break;
   }
   default:
      immi = 0;
      break;
   }

   return immi;
}

void Assembler::resolveDataDirective(const Expressions::Directive* directive)
{
   std::uint8_t byteSizePerElement;
   std::vector<std::int32_t> values;
   if(resolveIfObject(directive, byteSizePerElement, values))
   {
      for(std::int32_t value: values)
      {
         m_executable.addToHeap(value, byteSizePerElement);
         m_hc += byteSizePerElement;
      }
   }
}

void Assembler::printExpressionsToFile(const std::string& fileName)
{
   std::ofstream file(fileName + ".bs");

   m_parser.reset();
   const Expressions::Expression* expr = m_parser.nextExpression();

   while(expr != nullptr)
   {
      switch(expr->getExpressionType())
      {
      case Expressions::Expression::ExpressionType::DIRECTIVE:
      {
         const Expressions::Directive* d = static_cast<const Expressions::Directive*>(expr);
         file << "\t" << (*d);
         break;
      }
      case Expressions::Expression::ExpressionType::LABEL:
      {
         const Expressions::Label* l = static_cast<const Expressions::Label*>(expr);
         file << (*l);
         break;
      }
      case Expressions::Expression::ExpressionType::INSTRUCTION:
      {
         const Expressions::Instruction* i = static_cast<const Expressions::Instruction*>(expr);
         file << "\t" << (*i);
         break;
      }
      case Expressions::Expression::ExpressionType::COMMENT:
         // std::cout << expr);
         break;
      case Expressions::Expression::ExpressionType::UNDEFINED:
         std::cerr << "Found undefined expression: " << expr << std::endl;
         abort();
         break;
      default:
         std::cerr << "Unsupported expression type with value = " << static_cast<std::int32_t>(expr->getExpressionType()) << std::endl;
         abort();
         break;
      }

      file << std::endl;

      expr = m_parser.nextExpression();
   }

   file.close();
   std::cout << "Finished writing binary assembly to " << fileName << ".bs" << std::endl;
}

}
}
