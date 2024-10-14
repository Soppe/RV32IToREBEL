#pragma once

#include <Parsers/expressionparser.h>
#include <Parsers/parseutils.h>
#include <Assemblers/directivehelper.h>

#include <vector>
#include <map>

namespace Assemblers
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

   void printExpressionsToFile(const std::string& fileName);
private:

   void handleTextSection(const Expressions::Expression* expr);
   void handleDataSection(const Expressions::Expression* expr);
   void handleBssSection(const Expressions::Expression* expr);
   void handleRoDataSection(const Expressions::Expression* expr);

   void resolveDataDirective(const Expressions::Directive* directive);

   bool resolveIfObject(const Expressions::Directive* directive, std::uint8_t& byteSizePerElement, std::vector<std::int32_t>& values);

   void resolveOperands();
   void resolveUnresolvedHeapData();
   std::int32_t resolveAssemblerModifier(const ParseUtils::ASSEMBLER_MODIFIER& modifier, const std::string& imm, std::uint32_t pc);

   ExpressionParser m_parser;
   DirectiveHelper::SectionType m_sectionType;
   ExecutableProgram& m_executable;

   using HeapLabelMap = std::map<std::string, std::uint32_t>; // Label | address
   HeapLabelMap m_tempHeapLabels;
   std::uint32_t m_pc; // Program counter
   std::uint32_t m_hc; // Heap counter - used to generate the correct value for labels related to data/bss/rodata objects

   struct RelocationItem
   {
      std::uint32_t address;
      std::uint8_t sizeInBytes;
      std::string label;
   };
   using HeapRelocationVector = std::vector<RelocationItem>;
   HeapRelocationVector m_heapRelocations;

   using TempLabelTable = std::map<std::uint32_t, std::string>; // Address | Label
   TempLabelTable m_tempLabelsTable;
};

}
}
