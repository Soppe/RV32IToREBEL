#ifndef ASSEMBLERANDLINKER_H
#define ASSEMBLERANDLINKER_H

#include <list>
#include <vector>
#include <map>
#include <expressionparser.h>
#include <parseutils.h>
#include <Simulators/directivehelper.h>

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
   AssemblerAndLinker(std::list<Expression*>& expressions, Converters::InstructionConverterBase& converter, ExecutableProgram* executable);

   void init();
   void run();

private:

   void handleTextSection(Expression* expr);
   void handleDataSection(Expression* expr);
   void handleBssSection(Expression* expr);
   void handleRoDataSection(Expression* expr);

   void resolveInstruction(const Expressions::Instruction* instr, std::list<Expression*>& out, Converters::InstructionConverterBase& converter);
   void resolveDataDirective(const Expressions::Directive* directive);

   bool resolveIfObject(const Expressions::Directive* directive, int& byteSizePerElement, std::vector<int>& values);

   void resolveOperands();
   int resolveAssemblerModifier(const ParseUtils::ASSEMBLER_MODIFIER& modifier, const std::string& imm, int pc);

   Converters::InstructionConverterBase& m_converter;
   ExpressionParser m_parser;
   DirectiveHelper::SectionType m_sectionType;
   ExecutableProgram* m_executable;

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
