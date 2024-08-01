#include "rv32itorebel6.h"

#include <logger.h>
#include <iostream>
#include <Expressions/instruction.h>
#include <Parsers/parseutils.h>

namespace
{
const std::string pcrel(const std::string& in) { return "%pcrel(" + in +")"; }
std::string zeroOffset(const std::string& in) { return "0(" + in + ")"; }
std::string createOffset(const std::string& offset, const std::string& base) { return offset + "(" + base + ")"; }

void createInstruction(Expressions::ExpressionList& el, const std::string& name)
{
   el.push_back(new Expressions::Instruction(name, std::vector<std::string>()));
}

void createInstruction(Expressions::ExpressionList& el, const std::string& name, const std::string& op0, const std::string& op1)
{
   std::vector<std::string> operands = {op0, op1};
   el.push_back(new Expressions::Instruction(name, operands));
}

void createInstruction(Expressions::ExpressionList& el, const std::string& name, const std::string& op0, const std::string& op1, const std::string& op2)
{
   std::vector<std::string> operands = {op0, op1, op2};
   el.push_back( new Expressions::Instruction(name, operands) );
}

void createInstruction(Expressions::ExpressionList& el, const std::string& name, const std::string& rs1, const std::string& rs2, const std::string& rd1, const std::string& rd2)
{
   std::vector<std::string> operands = {rs1, rs2, rd1, rd2};
   el.push_back(new Expressions::Instruction(name, operands));
}

//TODO: Implement this properly once we know how to deal with storing trits and trytes instead of bits and bytes in memory
// l{b|h|w}{u} rd, offset(rs) --> l{b|h|w}{u} rd, offset(rs)
// l{b|h|w} rd, symbol -->
//    auipc.t rd, %pcrel(symbol)
//    l{b|h|w} rd, 0(rd)
void handleLoad(const Converters::RV32IPseudoToRV32IBase* conv, const std::string& name, const Converters::StringList& op, Expressions::ExpressionList& el)
{
   std::string offsetDummy;
   std::string rs1Dummy;
   // TODO: How to handle if the offset is of type -123(rs)? The offset is in bytes, and would change with ternary addressing.
   if(ParseUtils::parseRegisterOffset(op[1], offsetDummy, rs1Dummy)) { createInstruction(el, name, op[0], op[1]); }
   else // Pseudoinstruction
   {
      conv->at("aipc.t")({op[0], pcrel(op[1])}, el);
      createInstruction(el, name, op[0], zeroOffset(op[0]));
   }
}

//TODO: Implement this properly once we know how to deal with storing trits and trytes instead of bits and bytes in memory
// s{b|h|w} rd, offset(rs) --> s{b|h|w} rd, offset(rs)
// s{b|h|w} rd, symbol, rt -->
//    auipc rt, %pcrel_hi(symbol)
//    s{b|h|w} rd, %pcrel_lo(symbol)(rt)
void handleStore(const Converters::RV32IPseudoToRV32IBase* conv, const std::string& name, const Converters::StringList& op, Expressions::ExpressionList& el)
{
   std::string offsetDummy; // Placeholder, not used here
   std::string rs1Dummy; // Placeholder, not used here
   if(ParseUtils::parseRegisterOffset(op[1], offsetDummy, rs1Dummy)) { createInstruction(el, name, op[0], op[1]); }
   else // Pseudoinstruction
   {
      conv->at("aipc.t")({op[2], pcrel(op[1])}, el);
      createInstruction(el, name, op[0], zeroOffset(op[2]));
   }
}

}

namespace Converters
{
RV32IToREBEL6::RV32IToREBEL6()
{
   fillExpressionMap();
}

void RV32IToREBEL6::fillExpressionMap()
{
   // TODO: Every instruction that deals with memory and addresses need to take into account that these are now all ternary, not binary.
   // You can't store a byte at address 0x123 since the memory is ternary, not binary. Same goes for addressing - jumping to address -123(a0) isn't the same in ternary as in
   // binary unless each instruction is 2-tryte aligned and each instruction is 32 trits etc, which isn't the case.

   // RV32I BASE INSTRUCTION OVERRIDES (including pseudoinstructions in the cases where the base and pseudoinstruction has the same name)

   // TODO: Pretty sure the commented expressions can be kept as they are in binary
   // add rd, rs1, rs2 --> add rd, rs1, rs2
   //m_instructionMap["add"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "add", op[0], op[1], op[2]); };

   // sub rd, rs1, rs2 --> sub rd, rs1, rs2
   //m_instructionMap["sub"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "sub", op[0], op[1], op[2]); };

   // addi rd, rs1, imm --> addi rd, rs1, imm
   m_instructionMap["addi"] = [this] (const StringList& op, Expressions::ExpressionList& el)
   {
      // This is a %pcrel_lo that corresponds to some auipc call that relates to %pcrel_hi. Since auipc is converted to aipc and therefor handles the whole immediate,
      // all that is left is to move the value stored in source registry op[1] to target registry op[0]
      if(op[2][0] == '%')
      {
         at("mv")({op[0], op[1]}, el);
      }
      else
      {
         createInstruction(el, "addi", op[0], op[1], op[2]);
      }
   };

   // lui rd, imm --> li.t rd, (imm << 12)
   // lui is defined as putting the 20 lsb of the immediate into the 20 msb of the target registry.
   m_instructionMap["lui"] = [this] (const StringList& op, Expressions::ExpressionList& el)
   {
      std::int32_t val = 0;
      if(ParseUtils::parseImmediate(20, op[1], val))
      {
         val <<= 12;
         at("li.t")({op[0], std::to_string(val)}, el);
      }
      else
      {
         std::cerr << __PRETTY_FUNC__ << ": Unable to parse immediate " << op[1] << " for lui instruction" << std::endl;
         abort();
      }
   };


   // auipc works the same as lui in that it operates on the 20 lsb of an immediate and places them in the 20 msb in the target register. It normally operates on %pcrel_hi.
   // Converting auipc is difficult since the logic between RV32I and REBEL-6 is not similar. As such, we do the whole conversion here,
   // and ignore the %pcrel_lo calls in load, store, and addi instructions.
   // auipc rd, imm --> auipc.t rd, imm
   m_instructionMap["auipc"] = [this] (const StringList& op, Expressions::ExpressionList& el)
   {
      if(op[1][0] == '%')
      {
         ParseUtils::ASSEMBLER_MODIFIER typeDummy;
         std::string modifierValue;

         ParseUtils::parseAssemblerModifier(op[1], typeDummy, modifierValue);
         at("aipc.t")({op[0], pcrel(modifierValue)}, el);
      }
      else
      {
         // Doesn't make sense that we'll get here
         std::cout << __PRETTY_FUNC__ << ": Unexpectedly got auipc without %pcrel_hi." << std::endl;

         std::int32_t val = 0;
         if(ParseUtils::parseImmediate(20, op[1], val))
         {
            val <<= 12;
            at("aipc.t")({op[0], std::to_string(val)}, el);
         }
      }
   };

   // TODO: CONVERT TO TERNARY jal.t?
   // jal offset --> jal x1, offset
   // jal rd, offset --> jal rd, offset
   m_instructionMap["jal"] = [this] (const StringList& op, Expressions::ExpressionList& el)
                           {
                              if(op.size() == 1) at("jal")({"x1", op[0]}, el); // Pseudoinstruction
                              else               createInstruction(el, "jal", op[0], op[1]);
                           };

   // TODO: CONVERT TO TERNARY jalr.t?
   // jalr rs --> jalr x1, 0(rs)
   // jalr rd, rs, offset --> jalr rd, offset(rs) // This is an older format, e.g. from version v2.2
   // jalr rd, offset(rs) --> jalr rd, offset(rs)
   m_instructionMap["jalr"] = [this] (const StringList& op, Expressions::ExpressionList& el)
                           {
                              if(op.size() == 1) at("jalr")({"x1", zeroOffset(op[0])}, el);
                              else if(op.size() == 3) at("jalr")({op[0], createOffset(op[2], op[1])}, el); // Older format, e.g. from v2.2
                              else               createInstruction(el, "jalr", op[0], op[1]);
                           };


   // TODO: What to replace these with? Using lw.t for all of them means an excessive waste of memory
   // l{b|h|w} rd, offset(rs) --> l{b|h|w} rd, offset(rs)
   // l{b|h|w} rd, symbol -->
   //    auipc rd, %pcrel_hi(symbol)
   //    l{b|h|w} rd, %pcrel_lo(symbol)(rd)
   m_instructionMap["lb"] = [this] (const StringList& op, Expressions::ExpressionList& el) { handleLoad(this, "lb", op, el); };
   m_instructionMap["lh"] = [this] (const StringList& op, Expressions::ExpressionList& el) { handleLoad(this, "lh", op, el); };
   m_instructionMap["lw"] = [this] (const StringList& op, Expressions::ExpressionList& el) { handleLoad(this, "lw", op, el); };

   // lbu rd, offset(rs) --> lbu rd, offset(rs)
   m_instructionMap["lbu"] = [this] (const StringList& op, Expressions::ExpressionList& el) { handleLoad(this, "lbu", op, el); };

   // lhu rd, offset(rs) --> lhu rd, offset(rs)
   m_instructionMap["lhu"] = [this] (const StringList& op, Expressions::ExpressionList& el) { handleLoad(this, "lhu", op, el); };


   // TODO: What to replace these with? Using sw.t for all of them means an excessive waste of memory
   // s{b|h|w} rd, offset(rs) --> s{b|h|w} rd, offset(rs)
   // s{b|h|w} rd, symbol, rt -->
   //    auipc rt, %pcrel_hi(symbol)
   //    s{b|h|w} rd, %pcrel_lo(symbol)(rt)
   m_instructionMap["sb"] = [this] (const StringList& op, Expressions::ExpressionList& el) { handleStore(this, "sb", op, el); };
   m_instructionMap["sh"] = [this] (const StringList& op, Expressions::ExpressionList& el) { handleStore(this, "sh", op, el); };
   m_instructionMap["sw"] = [this] (const StringList& op, Expressions::ExpressionList& el) { handleStore(this, "sw", op, el); };

   // TODO: Replace branching with ternary equivalents? Seeing as the offset can't overflow/underflow in a meaningful way.

   // beq rs1, rs2, offset --> beq rs1, rs2, offset
   m_instructionMap["beq"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "beq", op[0], op[1], op[2]); };

   // bne rs1, rs2, offset --> bne rs1, rs2, offset
   m_instructionMap["bne"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "bne", op[0], op[1], op[2]); };

   // blt rs1, rs2, offset --> blt rs1, rs2, offset
   m_instructionMap["blt"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "blt", op[0], op[1], op[2]); };

   // bge rs1, rs2, offset --> bge rs1, rs2, offset
   m_instructionMap["bge"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "bge", op[0], op[1], op[2]); };

   // bltu rs1, rs2, offset --> bltu rs1, rs2, offset
   m_instructionMap["bltu"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "bltu", op[0], op[1],op[2]); };

   // bgeu rs1, rs2, offset --> bgeu rs1, rs2, offset
   m_instructionMap["bgeu"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "bgeu", op[0], op[1], op[2]); };

   // TODO: Replace slt with ternary equivalents? Seeing as the immediate can't overflow/underflow in a meaningful way. Worst case add an if-test to check if imm is
   // larger than the allowed 12-bit immediate

   // slti rd, rs1, imm --> slti rd, rs1, imm
   m_instructionMap["slti"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "slti.t", op[0], op[1], op[2]); };

   // sltiu rd, rs1, imm --> sltiu rd, rs1, imm
   m_instructionMap["sltiu"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "slti.t", op[0], op[1], op[2]); };

   // TODO: Pretty sure the commented expressions must be kept as they are in binary
   // xori rd, rs1, imm --> xori rd, rs1, imm
   //m_instructionMap["xori"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "xori", op[0], op[1], op[2]); };

   // ori rd, rs1, imm --> ori rd, rs1, imm
   //m_instructionMap["ori"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "ori", op[0], op[1], op[2]); };

   // andi rd, rs1, imm --> andi rd, rs1, imm
   //m_instructionMap["andi"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "andi", op[0], op[1], op[2]); };

   // slli rd, rs1, imm --> slli rd, rs1, imm
   //m_instructionMap["slli"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "slli", op[0], op[1], op[2]); };

   // srli rd, rs1, imm --> srli rd, rs1, imm
   //m_instructionMap["srli"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "srli", op[0], op[1], op[2]); };

   // srai rd, rs1, imm --> srai rd, rs1, imm
   //m_instructionMap["srai"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "srai", op[0], op[1], op[2]); };

   // sll rd, rs1, rs2 --> sll rd, rs1, rs2
   //m_instructionMap["sll"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "sll", op[0], op[1], op[2]); };

   // TODO: Replace slt with ternary equivalents? Seeing as no operator can overflow/underflow in a meaningful way.
   // slt rd, rs1, rs2 --> slt rd, rs1, rs2
   m_instructionMap["slt"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "slt.t", op[0], op[1], op[2]); };

   // sltu rd, rs1, rs2 --> sltu rd, rs1, rs2
   m_instructionMap["sltu"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "slt.t", op[0], op[1], op[2]); };

   // TODO: Pretty sure the commented expressions must be kept as they are in binary
   // xor rd, rs1, rs2 --> xor rd, rs1, rs2
   //m_instructionMap["xor"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "xor", op[0], op[1], op[2]); };

   // srl rd, rs1, rs2 --> srl rd, rs1, rs2
   //m_instructionMap["srl"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "srl", op[0], op[1], op[2]); };

   // sra rd, rs1, rs2 --> sra rd, rs1, rs2
   //m_instructionMap["sra"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "sra", op[0], op[1], op[2]); };

   // or rd, rs1, rs2 --> or rd, rs1, rs2
   //m_instructionMap["or"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "or", op[0], op[1], op[2]); };

   // and rd, rs1, rs2 --> and rd, rs1, rs2
   //m_instructionMap["and"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "and", op[0], op[1], op[2]); };


   // RV32I PSEUDOINSTRUCTION OVERRIDES

   // la rd, symbol --> auipc.t rd, %pcrel(symbol)
   m_instructionMap[ "la"] = [this] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "aipc.t", op[0], pcrel(op[1])); };

   // lla rd, symbol --> auipc.t rd, %pcrel(symbol)
   m_instructionMap[ "lla"] = [this] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "aipc.t", op[0], pcrel(op[1])); };

   // nop --> addi.t x0, x0, 0
   m_instructionMap[ "nop"] = [this] (const StringList&, Expressions::ExpressionList& el) { at("addi.t")({"x0", "x0", "0"}, el); };

   // li rd, imm --> li.t rd, imm
   m_instructionMap[ "li"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "li.t", op[0], op[1]); };

   // mv rd, rs --> addi rd, rs, 0
   //m_instructionMap["mv"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("addi")({op[0], op[1], "0"}, el); };

   // not rd, rs --> xori rd, rs, -1
   //m_instructionMap["not"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("xori")({op[0], op[1], "-1"}, el); };

   // neg rd, rs --> sub rd, x0, rs
   // m_instructionMap["neg"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("sub")({ op[0], "x0", op[1] }, el); };

   // seqz rd, rs --> sltiu rd, rs, 1
   // m_instructionMap["seqz"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("sltiu")({ op[0], op[1], "1" }, el); };

   // snez rd, rs --> sltu rd, x0, rs
   // m_instructionMap["snez"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("sltu")({ op[0], "x0", op[1] }, el); };

   // sltz rd, rs --> slt rd, rs, x0
   // m_instructionMap["sltz"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("slt")({ op[0], op[1], "x0" }, el); };

   // sgtz rd, rs --> slt rd, x0, rs
   // m_instructionMap["sgtz"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("slt")({ op[0], "x0", op[1] }, el); };

   // beqz rs, offset --> beq rs, x0, offset
   // m_instructionMap["beqz"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("beq")({ op[0], "x0", op[1] }, el); };

   // bnez rs, offset --> bne rs, x0, offset
   // m_instructionMap["bnez"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("bne")({ op[0], "x0", op[1] }, el); };

   // blez rs, offset --> bge x0, rs, offset
   // m_instructionMap["blez"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("bge")({ "x0", op[0], op[1] }, el); };

   // bgez rs, offset --> bge rs, x0, offset
   // m_instructionMap["bgez"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("bge")({ op[0], "x0", op[1] }, el); };

   // bltz rs, offset --> blt rs, x0, offset
   // m_instructionMap["bltz"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("blt")({ op[0], "x0", op[1] }, el); };

   // bgtz rs, offset --> blt x0, rs, offset
   // m_instructionMap["bgtz"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("blt")({ "x0", op[0], op[1] }, el); };

   // bgt rs, rt, offset --> blt rt, rs, offset
   // m_instructionMap["bgt"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("blt")({ op[1], op[0], op[2] }, el); };

   // ble rs, rt, offset --> bge rt, rs, offset
   // m_instructionMap["ble"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("bge")({op[1], op[0], op[2]}, el); };

   // bgtu rs, rt, offset --> bltu rt, rs, offset
   // m_instructionMap["bgtu"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("bltu")({ op[1], op[0], op[2] }, el); };

   // bleu rs, rt, offset --> bgeu rt, rs, offset
   // m_instructionMap["bleu"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("bgeu")({op[1], op[0], op[2]}, el); };

   // j offset --> jal x0, offset
   // m_instructionMap["j"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("jal")({"x0", op[0]}, el); };

   // jr rs --> jalr x0, 0(rs)
   // jr rs, offset --> jalr x0, offset(rs) // This is some ancient syntax
   m_instructionMap["jr"] = [this] (const StringList& op, Expressions::ExpressionList& el)
                           {
                              if(op.size() == 2) // Some ancient syntax
                              {
                                 at("jalr")({"x0", createOffset(op[1], op[0])}, el);
                              }
                              else
                              {
                                 at("jalr")({"x0", zeroOffset(op[0])}, el);
                              }
                           };

   // ret --> jalr x0, 0(x1)
   m_instructionMap["ret"] = [this] (const StringList&, Expressions::ExpressionList& el) { at("jalr")({"x0", zeroOffset("x1")}, el); };

   // call offset -->
   //    auipc.t x1, %pcrel(offset)
   //    jalr x1, 0(x1)
   m_instructionMap["call"] = [this] (const StringList& op, Expressions::ExpressionList& el)
                           {
                              at("aipc.t")({"x1", pcrel(op[0])}, el);
                              at("jalr")({"x1", zeroOffset("x1")}, el); // TODO: Which jalr to use here?
                           };

   // tail offset -->
   //    auipc x6, %pcrel_hi(offset)
   //    jalr x0, %pcrel_lo(offset)(x6)
   /*insert( {"tail"] = [this] (const StringList& op, Expressions::ExpressionList& el)
           {
              // Add label for the auipc according to the documentation for the %pcrel-modifiers
              // Note that %pcrel_lo uses the reloLabel address as pc, finds the symbol used in the related auipc instruction, and takes the 12 LST from that symbol.
              std::string reloLabel;
              ParseUtils::generateReloLabel(reloLabel);
              el.push_back(new Expressions::Label(reloLabel));
              at("auipc")({"x6", pcrel_hi(op[0])}, el);
              at("jalr")({"x0", pcrel_lo(reloLabel) + "(x6)"}, el);
           };*/

   // REBEL-6 INSTRUCTIONS

   // Need their own because of overflows.
   m_instructionMap["add.t"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "add.t", op[0], op[1], op[2]); };
   m_instructionMap["sub.t"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "sub.t", op[0], op[1], op[2]); };
   m_instructionMap["addi.t"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "addi.t", op[0], op[1], op[2]); };;

   // Need their own because of addressing
   m_instructionMap["aipc.t"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "aipc.t", op[0], op[1]); };
   //m_instructionMap["jal.t"] = ;
   //m_instructionMap["jalr.t"] = ;

   // Need their own because of memory sizes
   //m_instructionMap["lt.t"] = [this] (const StringList& op, Expressions::ExpressionList& el) { handleLoad(this, "lt.t", op, el); };
   //m_instructionMap["lh.t"] = [this] (const StringList& op, Expressions::ExpressionList& el) { handleLoad(this, "lh.t", op, el); };;
   //m_instructionMap["lw.t"] = [this] (const StringList& op, Expressions::ExpressionList& el) { handleLoad(this, "lw.t", op, el); };

   //m_instructionMap["st.t"] = [this] (const StringList& op, Expressions::ExpressionList& el) { handleStore(this, "st.t", op, el); };
   //m_instructionMap["sh.t"] = [this] (const StringList& op, Expressions::ExpressionList& el) { handleStore(this, "sh.t", op, el); };
   //m_instructionMap["sw.t"] = [this] (const StringList& op, Expressions::ExpressionList& el) { handleStore(this, "sw.t", op, el); };;

   // Need their own because of addressing
   //m_instructionMap["beq.t"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "beq.t", op[0], op[1], op[2]); };
   //m_instructionMap["bne.t"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "bne.t", op[0], op[1], op[2]); };
   //m_instructionMap["blt.t"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "blt.t", op[0], op[1], op[2]); };
   //m_instructionMap["bge.t"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "bge.t", op[0], op[1], op[2]); };
   //m_instructionMap["bltu.t"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "bltu.t", op[0], op[1],op[2]); };
   //m_instructionMap["bgeu.t"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "bgeu.t", op[0], op[1], op[2]); };

   // Nice to have for larger immediates
   m_instructionMap["slti.t"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "slti.t", op[0], op[1], op[2]); };

   // Need their own for tritwise operations
   //m_instructionMap["xori.t"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "xori.t", op[0], op[1], op[2]); };
   //m_instructionMap["ori.t"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "ori.t", op[0], op[1], op[2]); };;
   //m_instructionMap["andi.t"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "andi.t", op[0], op[1], op[2]); };
   //m_instructionMap["sli.t"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "sli.t", op[0], op[1], op[2]); };
   //m_instructionMap["sri.t"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "sri.t", op[0], op[1], op[2]); };
   //m_instructionMap["sl.t"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "sl.t", op[0], op[1], op[2]); };

   // // Nice to have for larger immediates stored in the rs register
   m_instructionMap["slt.t"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "slt.t", op[0], op[1], op[2]); };

   // Need their own for tritwise operations
   //m_instructionMap["xor.t"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "xor.t", op[0], op[1], op[2]); };
   //m_instructionMap["sr.t"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "sr.t", op[0], op[1], op[2]); };
   //m_instructionMap["or.t"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "or.t", op[0], op[1], op[2]); };
   //m_instructionMap["and.t"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "and.t", op[0], op[1], op[2]); };

   // Nice to have as it supports the max value of a 32 bit integer without requiring an extra instruction
   m_instructionMap["li.t"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "li.t", op[0], op[1]); };
}

}
