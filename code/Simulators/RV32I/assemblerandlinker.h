#pragma once

#include <Parsers/expressionparser.h>
#include <Parsers/parseutils.h>
#include <Simulators/directivehelper.h>

#include <vector>
#include <map>

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

   bool resolveIfObject(const Expressions::Directive* directive, std::uint8_t& byteSizePerElement, std::vector<std::int32_t>& values);

   void resolveOperands();
   std::int32_t resolveAssemblerModifier(const ParseUtils::ASSEMBLER_MODIFIER& modifier, const std::string& imm, std::uint32_t pc);

   ExpressionParser m_parser;
   DirectiveHelper::SectionType m_sectionType;
   ExecutableProgram& m_executable;

   using HeapLabelMap = std::map<std::string, std::uint32_t>; // Label | address
   HeapLabelMap m_tempHeapLabels;
   std::uint32_t m_pc; // Program counter
   std::uint32_t m_hc; // Heap counter - used to generate the correct value for labels related to data/bss/rodata objects

   using RelocationTableMap = std::map<std::uint32_t, std::string>; // Address | Label
   RelocationTableMap m_relocationTable;
};

}
}
