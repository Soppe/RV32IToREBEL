#include "assemblerandlinker.h"
#include "executableprogram.h"
#include "simulatorutils.h"

#include <Expressions/all_expressions.h>
#include <Simulators/directivehelper.h>
#include <logger.h>

#include <iostream>

namespace Simulators
{
namespace RV32I
{
AssemblerAndLinker::AssemblerAndLinker(Expressions::ExpressionList& expressions, ExecutableProgram& executable)
    : m_parser(expressions)
    , m_sectionType(DirectiveHelper::SectionType::TEXT) // Until otherwise specified, the asm manual states the default is .text
    , m_executable(executable)
    , m_pc(0)
    , m_hc(0)
{
}

void AssemblerAndLinker::init()
{
   m_sectionType = DirectiveHelper::SectionType::TEXT;
   m_pc = 0;
   m_hc = 0;
   m_tempHeapLabels.clear();
   m_relocationTable.clear();
}

void AssemblerAndLinker::run()
{
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
         std::cerr << __PRETTY_FUNC__ << ": Unknown section type " << static_cast<int>(m_sectionType) << std::endl;
         abort();
         break;
      }

      expr = m_parser.nextExpression();
   }

   m_executable.finishUp();

   // Finished setting up the instruction memory, now we need to set up heap labels
   HeapLabelMap::const_iterator it = m_tempHeapLabels.begin();
   int instructionsSize = m_executable.getInstructionsSize();

   while(it != m_tempHeapLabels.end())
   {
      m_executable.addSymbol(it->first, it->second + instructionsSize);
      ++it;
   }

   // Now that we finally know the position of everything we need to resolve the operands
   resolveOperands();
}

void AssemblerAndLinker::handleTextSection(const Expressions::Expression* expr)
{
   std::cout << "In text section" << std::endl;

   while(expr != nullptr)
   {
      switch(expr->getExpressionType())
      {
      case Expressions::Expression::ExpressionType::DIRECTIVE:
      {
         const Expressions::Directive* directive = static_cast<const Expressions::Directive*>(expr);
         // No expecting any directives while handling .text
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

void AssemblerAndLinker::handleDataSection(const Expressions::Expression* expr)
{
   std::cout << "In data section" << std::endl;
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
         std::cerr << __PRETTY_FUNC__ << ": Found instruction \"";
         expr->print();
         std::cerr << "\" in data section" << std::endl;
         abort();
         break;

      default:
         // Don't care
         break;
      }

      expr = m_parser.nextExpression();
   }
}

void AssemblerAndLinker::handleBssSection(const Expressions::Expression* expr)
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
         break;
      }
      case Expressions::Expression::ExpressionType::LABEL:
         break;
      case Expressions::Expression::ExpressionType::INSTRUCTION:
         std::cerr << __PRETTY_FUNC__ << ": Found instruction \"";
         expr->print();
         std::cerr << "\" in bss section" << std::endl;
         abort();
         break;
      default:
         // Don't care
         break;
      }

      expr = m_parser.nextExpression();
   }
}

void AssemblerAndLinker::handleRoDataSection(const Expressions::Expression* expr)
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
         break;
      }
      case Expressions::Expression::ExpressionType::LABEL:
         break;
      case Expressions::Expression::ExpressionType::INSTRUCTION:
         std::cerr << __PRETTY_FUNC__ << ": Found instruction \"";
         expr->print();
         std::cerr << "\" in rodata section" << std::endl;
         abort();
         break;
      default:
         // Don't care
         break;
      }

      expr = m_parser.nextExpression();
   }
}

bool AssemblerAndLinker::resolveIfObject(const Expressions::Directive* directive, int& byteSizePerElement, std::vector<int>& values)
{
   bool retVal = true;
   const std::string& name = directive->getDirectiveName();
   const std::vector<std::string> parameters = directive->getDirectiveParameters();
   static int value;

   if(name == ".ascii") // 8 bits per letter
   {
      // 7.5 .ascii "string"…
      // .ascii expects zero or more string literals (see Strings) separated by commas. It assembles each string (with no automatic trailing zero byte) into consecutive addresses.

      std::cout << "Unsupported directive object type: " << name << std::endl;
      abort();
   }
   else if(name == ".asciz") // 8 bits per letter, + terminating zero which is also 8 bits
   {
      //7.6 .asciz "string"…
      // .asciz is just like .ascii, but each string is followed by a zero byte. The “z” in ‘.asciz’ stands for “zero”.
      // Note that multiple string arguments not separated by commas will be concatenated together and only one final zero byte will be stored.

      std::cout << "Unsupported directive object type: " << name << std::endl;
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
      // 7.49 .int expressions
      // Expect zero or more expressions, of any section, separated by commas. For each expression, emit a number that, at run time, is the value of that expression.
      // The byte order and bit size of the number depends on what kind of target the assembly is for.

      // Note - this directive is not intended for encoding instructions, and it will not trigger effects like DWARF line number generation.
      // Instead some targets support special directives for encoding arbitrary binary sequences as instructions such as eg .insn or .inst.

      std::cout << "Unsupported directive object type: " << name << std::endl;
      abort();
   }
   else if(name == ".long") // 32 bits according to ABI
   {
      // 7.62 .long expressions
      // .long is the same as ‘.int’. See .int.

      std::cout << "Unsupported directive object type: " << name << std::endl;
      abort();
   }
   else if(name == ".short") // 16 bits according to ABI
   {
      // 7.87 .short expressions
      // .short is normally the same as ‘.word’. See .word.

      // In some configurations, however, .short and .word generate numbers of different lengths. See Machine Dependent Features.

      // Note - this directive is not intended for encoding instructions, and it will not trigger effects like DWARF line number generation.
      // Instead some targets support special directives for encoding arbitrary binary sequences as instructions such as .insn or .inst.

      std::cout << "Unsupported directive object type: " << name << std::endl;
      abort();
   }
   /*else if(name == ".size") // Currenly not supported, but is normally generated by GCC. It's relevance, however, is unknown.
   {

   }*/
   else if(name == ".skip")
   {
      // 7.90 .skip size [,fill]
      // This directive emits size bytes, each of value fill. Both size and fill are absolute expressions. If the comma and fill are omitted, fill is assumed to be zero. This is the same as ‘.space’.

      std::cout << "Unsupported directive object type: " << name << std::endl;
      abort();
   }
   else if(name == ".space")
   {
      // 7.92 .space size [,fill]
      // This directive emits size bytes, each of value fill. Both size and fill are absolute expressions. If the comma and fill are omitted, fill is assumed to be zero. This is the same as ‘.skip’.


      std::cout << "Unsupported directive object type: " << name << std::endl;
      abort();
   }
   else if(name == ".string") // 8 bits per letter, + terminating zero which is also 8 bits
   {
      // 7.94 .string "str", .string8 "str", .string16
      // "str", .string32 "str", .string64 "str"

      // Copy the characters in str to the object file. You may specify more than one string to copy, separated by commas.
      // Unless otherwise specified for a particular machine, the assembler marks the end of each string with a 0 byte. You can use any of the escape sequences described in Strings.

      // The variants string16, string32 and string64 differ from the string pseudo opcode in that each 8-bit character from str is copied and expanded to 16, 32 or 64 bits respectively.
      //The expanded characters are stored in target endianness byte order.

      //Example:

         //.string32 "BYE"
      //expands to:
         //.string   "B\0\0\0Y\0\0\0E\0\0\0"  /* On little endian targets.  */
         //.string   "\0\0\0B\0\0\0Y\0\0\0E"  /* On big endian targets.  */

      std::cout << "Unsupported directive object type: " << name << std::endl;
      abort();
   }
   else if(name == ".word") // 32 bits according to ISA
   {
      // 7.111 .word expressions
      // This directive expects zero or more expressions, of any section, separated by commas.

      // The size of the number emitted, and its byte order, depend on what target computer the assembly is for.

      byteSizePerElement = 4;
      for(const std::string& str: parameters)
      {
         ParseUtils::parseImmediate(32, str, value);
         values.push_back(value);
      }
   }
   else if(name == ".zero") // 8 bits
   {
      // 7.112 .zero size
      // This directive emits size 0-valued bytes. size must be an absolute expression.
      // This directive is actually an alias for the ‘.skip’ directive so it can take an optional second argument of the value to store in the bytes instead of zero.
      // Using ‘.zero’ in this way would be confusing however.

      std::cout << "Unsupported directive object type: " << name << std::endl;
      abort();
   }
   else if((name == ".2byte") || (name == ".4byte") || (name == ".8byte") || (name == ".string8") || (name == ".string16") || (name == ".string32"))
   {
      std::cout << "Unsupported directive object type: " << name << std::endl;
      abort();
   }
   else
   {
      retVal = false;
   }

   return retVal;
}

void AssemblerAndLinker::resolveOperands()
{
   int pc = 0;
   int instrSize;
   Expressions::Instruction* instr = m_executable.loadInstruction(pc, instrSize);
   while(instr != nullptr)
   {
      std::vector<std::string>& operands = instr->getInstructionOperands();
      for(std::string& operand: operands)
      {
         if(operand[0] == '%')
         {
            std::string value;
            ParseUtils::ASSEMBLER_MODIFIER type;
            if(ParseUtils::parseAssemblerModifier(operand, type, value))
            {
               //std::cout << "Operand: " << operand << "; value: " << value << "; type: " << static_cast<int>(type) << std::endl;
               try
               {
                  int imm = resolveAssemblerModifier(type, value, pc);
                  operand = std::to_string(imm);
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
            int dummy;
            ParseUtils::parseImmediate(32, operand, dummy);
            operand = std::to_string(dummy);
            std::cout << "Operand = " << operand << std::endl;
         }
         else if((std::isalpha(operand[0])) || (operand[0] == '.'))
         {
            try
            {
               int imm = m_executable.loadSymbolValue(operand);
               operand = std::to_string(imm);
            }
            catch(std::exception&)
            {
               // Assume it is a register value
            }
         }
      }

      // Offset branching and jumps based on PC
      SimulatorUtils::InstructionType instrType = SimulatorUtils::getInstructionType(instr->getInstructionName());
      if((instrType == SimulatorUtils::InstructionType::BRANCH) || (instrType == SimulatorUtils::InstructionType::JUMP))
      {
         int opInt = stoi(operands.back());
         opInt = opInt - pc;
         if((opInt % 2) != 0)
         {
            std::cerr << __PRETTY_FUNC__ << ": offset not 2 byte aligned: ";
            instr->print();
            std::cout << std::endl;
            abort();
         }
         // Last bit should always be 0 since every instruction should be at least 2-byte aligned - we discard it because the CPU adds it automatically.
         // This correlates with the values of machine instructions from dissassembled code. This also means that the immediate should always be considered to be size 13, not 12.
         opInt >>= 1;
         operands.back() = std::to_string(opInt);
      }

      pc += instrSize;
      instr = m_executable.loadInstruction(pc, instrSize);
   }
}

int AssemblerAndLinker::resolveAssemblerModifier(const ParseUtils::ASSEMBLER_MODIFIER& modifier, const std::string& imm, int pc)
{
   int immi = m_executable.loadSymbolValue(imm);

   switch(modifier)
   {
   case ParseUtils::ASSEMBLER_MODIFIER::HI:
      // Compensate for signedness from %lo, don't sign extend as it makes lui grumpy
      immi = ((immi >> 12) + ((immi & 0x800) ? 1 : 0)) & 0xfffff;
      break;
   case ParseUtils::ASSEMBLER_MODIFIER::LO:
      ParseUtils::parseImmediate(0xfff, immi, immi);
      break;
   case ParseUtils::ASSEMBLER_MODIFIER::PCRELHI:
   {
      // Store what symbol %pcrel_hi points to, as %pcrel_lo points to the the address of the instruction related to %pcrel_hi rather than the symbol itself, but is still interested in the symbol value
      // pc is the address of the %pcrel_hi-related instruction, whereas imm is the symbol name
      m_relocationTable.insert({pc, imm});

      int delta = immi - pc;
      // Compensate for signedness from %pcrel_lo, don't sign extend as it makes lui grumpy
      immi = ((delta >> 12) + ((delta & 0x800) ? 1 : 0)) & 0xfffff;
      break;
   }
   case ParseUtils::ASSEMBLER_MODIFIER::PCRELLO:
   {
      if(imm.starts_with(ParseUtils::RELO_LABEL_PREFIX))
      {
         pc = immi;
         try
         {
            std::string symbol = m_relocationTable.at(pc);
            immi = m_executable.loadSymbolValue(symbol);
         }
         catch(std::exception& e)
         {
            std::cerr << __PRETTY_FUNC__ << ": Failed to retrieve %pcrel_lo data for PC = " << pc << " where relo label = " << imm << "; e.what() = " << e.what() << std::endl;
         }
      }
      immi = (immi - pc);
      ParseUtils::parseImmediate(0xfff, immi, immi);
      break;
   }
   default:
      immi = 0;
      break;
   }

   return immi;
}

void AssemblerAndLinker::resolveDataDirective(const Expressions::Directive* directive)
{
   int byteSizePerElement;
   std::vector<int> values;
   if(resolveIfObject(directive, byteSizePerElement, values))
   {
      for(int value: values)
      {
         m_executable.addToHeap(value, byteSizePerElement);
      }
      m_hc += byteSizePerElement;
   }
}
}
}
