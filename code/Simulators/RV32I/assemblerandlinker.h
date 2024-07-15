#ifndef ASSEMBLERANDLINKER_H
#define ASSEMBLERANDLINKER_H

#include <Parsers/expressionparser.h>
#include <Parsers/parseutils.h>
#include <Simulators/directivehelper.h>

#include <vector>
#include <map>

namespace Converters
{
class InstructionConverterBase;
}


namespace Simulators
{
namespace RV32I
{

class ExecutableProgram;

class AssemblerAndLinker
{
public:
   AssemblerAndLinker(Expressions::ExpressionList& expressions, ExecutableProgram& executable);

   void init();
   void run();

private:

   void handleTextSection(const Expressions::Expression* expr);
   void handleDataSection(const Expressions::Expression* expr);
   void handleBssSection(const Expressions::Expression* expr);
   void handleRoDataSection(const Expressions::Expression* expr);

   void resolveInstruction(const Expressions::Instruction* instr, Expressions::ExpressionList& out);
   void resolveDataDirective(const Expressions::Directive* directive);

   bool resolveIfObject(const Expressions::Directive* directive, int& byteSizePerElement, std::vector<int>& values);

   void resolveOperands();
   int resolveAssemblerModifier(const ParseUtils::ASSEMBLER_MODIFIER& modifier, const std::string& imm, int pc);

   ExpressionParser m_parser;
   DirectiveHelper::SectionType m_sectionType;
   ExecutableProgram& m_executable;

   using HeapLabelMap = std::map<std::string, int>;
   HeapLabelMap m_tempHeapLabels;
   int m_pc; // Program counter
   int m_hc; // Heap counter - used to generate the correct value for labels related to data/bss/rodata objects

   using RelocationTableMap = std::map<int, std::string>; // Address | Label
   RelocationTableMap m_relocationTable;
};

}
}
#endif // ASSEMBLERANDLINKER_H
