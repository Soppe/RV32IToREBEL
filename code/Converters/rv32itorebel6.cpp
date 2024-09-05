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
//    aipc.t rd, %pcrel(symbol)
//    l{b|h|w} rd, 0(rd)
void handleLoad(const Converters::RV32IPseudoToRV32IBase* conv, const std::string& name, const Converters::StringList& op, Expressions::ExpressionList& el)
{
   std::string offsetDummy; // Placeholder, not used here
   std::string rs1Dummy; // Placeholder, not used here
   // TODO: How to handle if the offset is of type -123(rs)? The offset is in bytes, and would change with ternary addressing.
   // Possible answer: -123 means 123 bytes. However, it could also be 123 trytes, where 1 byte = 8 bit, and 1 tryte = 6 trits.
   // This means the jumps in memory can be the same, but the underlying transistors used to represent the value for each byte goes from
   // 8 to 6. This follows the same logic as the assembler where directives related to size of a variable in memory simply multiplies the amount of bytes with
   // TernaryLogic::TRITS_PER_TRYTE since our memory currently is on trit-level, and one tryte covers one byte.
   // Now, since a tryte is 6 trits, but a ternary halfword is 11 trits and a ternary word is 21 trits, we have 1 dead trit in the case of using lh and 3 dead trits in the case of using lw
   // Note that this will not work the same for instruction memory for jumping/branching. In these cases each byte is the equivalent of 8 trits, so that the addresses in binary and ternary
   // are the same. This is probably one of the reasons why REBEL-6 has exactly 32 trit instructions.
   if(ParseUtils::parseRegisterOffset(op[1], offsetDummy, rs1Dummy)) { createInstruction(el, name, op[0], op[1]); }
   else // Pseudoinstruction
   {
      conv->at("aipc.t")({op[0], pcrel(op[1])}, el); // TODO: Is there a risk that the target address offset is beyond what can be represented with 32 bits?
      createInstruction(el, name, op[0], zeroOffset(op[0]));
   }
}

//TODO: Implement this properly once we know how to deal with storing trits and trytes instead of bits and bytes in memory
// s{b|h|w} rd, offset(rs) --> s{b|h|w} rd, offset(rs)
// s{b|h|w} rd, symbol, rt -->
//    aipc rt, %pcrel(symbol)
//    s{b|h|w} rd, 0(rt)
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

   // addi rd, rs1, %pcrel_lo | %lo --> mv rd, rs1
   // addi rd, rs1, imm --> addi rd, rs1, imm
   m_instructionMap["addi"] = [this] (const StringList& op, Expressions::ExpressionList& el)
   {
      // This is a %pcrel_lo or %lo call that corresponds to some auipc or lui call that relates to %pcrel_hi or %hi.
      // Auipc is converted to aipc and lui to li and they both handle the whole immediate.
      // All that is left is to move the value stored in source registry op[1] to target registry op[0] if they're not the same
      if(op[2][0] == '%')
      {
         if(op[0] != op[1])
         {
            at("mv")({op[0], op[1]}, el);
         }
      }
      else
      {
         createInstruction(el, "addi", op[0], op[1], op[2]);
      }
   };

   // lui rd, imm --> li.t rd, imm
   // lui is defined as putting the 20 lsb of the immediate into the 20 msb of the target registry.
   // However, since li.t can take the full 32 bits, we do the whole operation here and ignore any calls to corresponding %lo calls in addi.
   m_instructionMap["lui"] = [this] (const StringList& op, Expressions::ExpressionList& el)
   {
      if(op[1][0] == '%') // %hi
      {
         ParseUtils::ASSEMBLER_MODIFIER dummyType;
         std::string modifierValue;

         ParseUtils::parseAssemblerModifier(op[1], dummyType, modifierValue);
         at("li.t")({op[0], modifierValue}, el);
      }
      else
      {
         // Doesn't make sense that we'll get here
         std::cout << __PRETTY_FUNC__ << ": Unexpectedly got lui without %hi." << std::endl;

         std::int32_t val = 0;
         if(ParseUtils::parseImmediate(20, op[1], val))
         {
            val <<= 12;
            at("li.t")({op[0], std::to_string(val)}, el);
         }
      }
   };


   // auipc rd, imm --> aipc.t rd, imm
   // auipc works the same as lui in that it operates on the 20 lsb of an immediate and places them in the 20 msb in the target register. It normally operates on %pcrel_hi.
   // Converting auipc is difficult since the logic between RV32I and REBEL-6 is not similar. As such, we do the whole conversion here,
   // and ignore the %pcrel_lo calls in load, store, and addi instructions.
   m_instructionMap["auipc"] = [this] (const StringList& op, Expressions::ExpressionList& el)
   {
      if(op[1][0] == '%') // %pcrel_hi
      {
         ParseUtils::ASSEMBLER_MODIFIER dummyType;
         std::string modifierValue;

         ParseUtils::parseAssemblerModifier(op[1], dummyType, modifierValue);
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

   // Ternary gives larger address interval to jump within.
   // jal offset --> jal x1, offset
   // jal rd, offset --> jal.t rd, offset
   m_instructionMap["jal"] = [this] (const StringList& op, Expressions::ExpressionList& el)
                           {
                              if(op.size() == 1) at("jal")({"x1", op[0]}, el); // Pseudoinstruction
                              else               at("jal.t")({op[0], op[1]}, el);
                           };

   // TODO: CONVERT TO TERNARY jalr.t?
   // jalr rs --> jalr x1, 0(rs)
   // jalr rd, rs, offset --> jalr rd, offset(rs) // This is an older format, e.g. from version v2.2
   // jalr rd, offset(rs) --> jalr.t rd, offset(rs)
   m_instructionMap["jalr"] = [this] (const StringList& op, Expressions::ExpressionList& el)
                           {
                              if(op.size() == 1) at("jalr")({"x1", zeroOffset(op[0])}, el);
                              else if(op.size() == 3) at("jalr")({op[0], createOffset(op[2], op[1])}, el); // Older format, e.g. from v2.2
                              else                    at("jalr.t")({op[0], op[1]}, el);
                           };


   // l{b|h|w} rd, offset(rs) --> l{b|h|w} rd, offset(rs)
   // l{b|h|w} rd, symbol -->
   //    aipc rd, %pcrel(symbol)
   //    l{b|h|w} rd, 0(rd)
   m_instructionMap["lb"] = [this] (const StringList& op, Expressions::ExpressionList& el) { handleLoad(this, "lb", op, el); };
   m_instructionMap["lh"] = [this] (const StringList& op, Expressions::ExpressionList& el) { handleLoad(this, "lh", op, el); };
   m_instructionMap["lw"] = [this] (const StringList& op, Expressions::ExpressionList& el) { handleLoad(this, "lw", op, el); };

   // Although values are stored in balanced ternary, we need to know if the loaded value is signed or not
   // to be able know what the max and min values are that can be loaded. Not sure if actually needed though...
   // lbu rd, offset(rs) --> lbu rd, offset(rs)
   m_instructionMap["lbu"] = [this] (const StringList& op, Expressions::ExpressionList& el) { handleLoad(this, "lbu", op, el); };

   // lhu rd, offset(rs) --> lhu rd, offset(rs)
   m_instructionMap["lhu"] = [this] (const StringList& op, Expressions::ExpressionList& el) { handleLoad(this, "lh", op, el); };

   // s{b|h|w} rd, offset(rs) --> s{b|h|w} rd, offset(rs)
   // s{b|h|w} rd, symbol, rt -->
   //    aipc rt, %pcrel(symbol)
   //    s{b|h|w} rd, 0(rt)
   m_instructionMap["sb"] = [this] (const StringList& op, Expressions::ExpressionList& el) { handleStore(this, "sb", op, el); };
   m_instructionMap["sh"] = [this] (const StringList& op, Expressions::ExpressionList& el) { handleStore(this, "sh", op, el); };
   m_instructionMap["sw"] = [this] (const StringList& op, Expressions::ExpressionList& el) { handleStore(this, "sw", op, el); };

   // beq rs1, rs2, offset --> beq.t rs1, rs2, offset
   m_instructionMap["beq"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("beq.t")({op[0], op[1], op[2]}, el); };

   // bne rs1, rs2, offset --> bne.t rs1, rs2, offset
   m_instructionMap["bne"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("bne.t")({op[0], op[1], op[2]}, el); };

   // blt rs1, rs2, offset --> blt.t rs1, rs2, offset
   m_instructionMap["blt"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("blt.t")({op[0], op[1], op[2]}, el); };

   // bge rs1, rs2, offset --> bge.t rs1, rs2, offset
   m_instructionMap["bge"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("bge.t")({op[0], op[1], op[2]}, el); };

   // bltu rs1, rs2, offset --> blt.t rs1, rs2, offset
   m_instructionMap["bltu"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("blt.t")({op[0], op[1], op[2]}, el); };

   // bgeu rs1, rs2, offset --> bge.t rs1, rs2, offset
   m_instructionMap["bgeu"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("bge.t")({op[0], op[1], op[2]}, el); };

   // slti rd, rs1, imm --> slti.t rd, rs1, imm
   m_instructionMap["slti"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("slti.t")({op[0], op[1], op[2]}, el); };

   // sltiu rd, rs1, imm --> slti.t rd, rs1, imm
   m_instructionMap["sltiu"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("slti.t")({op[0], op[1], op[2]}, el); };

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

   // slt rd, rs1, rs2 --> slt.t rd, rs1, rs2
   m_instructionMap["slt"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("slt.t")({op[0], op[1], op[2]}, el); };

   // sltu rd, rs1, rs2 --> slt.t rd, rs1, rs2
   m_instructionMap["sltu"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("slt.t")({op[0], op[1], op[2]}, el); };

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


   // System calls can stay as they are
   // fence --> nop
   //m_instructionMap["fence"] = [this] (const StringList&, Expressions::ExpressionList& el) { at("nop")({}, el); };

   // ecall --> ecall
   //m_instructionMap["ecall"] = [] (const StringList&, Expressions::ExpressionList& el) { createInstruction(el, "ecall"); };

   // ebreak --> ebreak
   //m_instructionMap["ebreak"] = [] (const StringList&, Expressions::ExpressionList& el) { createInstruction(el, "ebreak"); };

   // RV32I PSEUDOINSTRUCTION OVERRIDES

   // la rd, symbol --> aipc.t rd, %pcrel(symbol)
   m_instructionMap["la"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("aipc.t")({op[0], pcrel(op[1])}, el); };

   // lla rd, symbol --> aipc.t rd, %pcrel(symbol)
   m_instructionMap["lla"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("aipc.t")({op[0], pcrel(op[1])}, el); };

   // nop --> addi.t x0, x0, 0
   m_instructionMap["nop"] = [this] (const StringList&, Expressions::ExpressionList& el) { at("addi.t")({"x0", "x0", "0"}, el); };

   // li rd, imm --> li.t rd, imm
   m_instructionMap["li"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("li.t")({op[0], op[1]}, el); };

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
   /*m_instructionMap["jr"] = [this] (const StringList& op, Expressions::ExpressionList& el)
                           {
                              if(op.size() == 2) // Some ancient syntax
                              {
                                 at("jalr")({"x0", createOffset(op[1], op[0])}, el);
                              }
                              else
                              {
                                 at("jalr")({"x0", zeroOffset(op[0])}, el);
                              }
                           };*/

   // ret --> jalr x0, 0(x1)
   //m_instructionMap["ret"] = [this] (const StringList&, Expressions::ExpressionList& el) { at("jalr")({"x0", zeroOffset("x1")}, el); };

   // TODO: Can this be replaced with something ala jal.t x1, offset?
   // call offset -->
   //    aipc.t x1, %pcrel(offset)
   //    jalr x1, 0(x1)
   m_instructionMap["call"] = [this] (const StringList& op, Expressions::ExpressionList& el)
                           {
                              at("aipc.t")({"x1", pcrel(op[0])}, el);
                              at("jalr")({"x1", zeroOffset("x1")}, el); // TODO: Which jalr to use here?
                           };

   // tail offset -->
   //    aipc x6, %pcrel(offset)
   //    jalr x0, 0(x6)
   /*m_instructionMap["tail"] = [this] (const StringList& op, Expressions::ExpressionList& el)
                              {
                                 at("aipc.t")({"x6", pcrel(op[0])}, el);
                                 at("jalr")({"x0", zeroOffset("x6")}, el); // TODO: Which jalr to use here?
                              };*/

   // REBEL-6 INSTRUCTIONS

   // Need their own because of overflows.

   // add.t rd, rs1, rs2 --> add.t rd, rs1, rs2
   m_instructionMap["add.t"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "add.t", op[0], op[1], op[2]); };

   // sub.t rd, rs1, rs2 --> sub.t rd, rs1, rs2
   m_instructionMap["sub.t"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "sub.t", op[0], op[1], op[2]); };

   // addi.t rd, rs1, imm --> addi.t rd, rs1, imm
   m_instructionMap["addi.t"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "addi.t", op[0], op[1], op[2]); };;

   // Need their own because of addressing

   // aipc.t rd, imm --> aipc.t rd, imm
   m_instructionMap["aipc.t"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "aipc.t", op[0], op[1]); };

   // jal.t rd, offset --> jal.t rd, offset
   m_instructionMap["jal.t"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "jal.t", op[0], op[1]); };

   //jalr.t rd, offset(rs) --> jalr.t rd, offset(rs)
   m_instructionMap["jalr.t"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "jalr.t", op[0], op[1]); };

   // Need their own because of memory sizes and overholding binary overflows when storing to memory. Both load and store
   // should have their own both in binary and ternary to ensure overflows are kept consistent in binary and ternary
   //m_instructionMap["lt.t"] = [this] (const StringList& op, Expressions::ExpressionList& el) { handleLoad(this, "lt.t", op, el); };
   //m_instructionMap["lh.t"] = [this] (const StringList& op, Expressions::ExpressionList& el) { handleLoad(this, "lh.t", op, el); };;
   //m_instructionMap["lw.t"] = [this] (const StringList& op, Expressions::ExpressionList& el) { handleLoad(this, "lw.t", op, el); };

   //m_instructionMap["st.t"] = [this] (const StringList& op, Expressions::ExpressionList& el) { handleStore(this, "st.t", op, el); };
   //m_instructionMap["sh.t"] = [this] (const StringList& op, Expressions::ExpressionList& el) { handleStore(this, "sh.t", op, el); };
   //m_instructionMap["sw.t"] = [this] (const StringList& op, Expressions::ExpressionList& el) { handleStore(this, "sw.t", op, el); };;

   // Need their own because of addressing
   // beq.t rs1, rs2, offset --> beq.t rs1, rs2, offset
   m_instructionMap["beq.t"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "beq.t", op[0], op[1], op[2]); };

   // bne.t rs1, rs2, offset --> bne.t rs1, rs2, offset
   m_instructionMap["bne.t"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "bne.t", op[0], op[1], op[2]); };

   // blt.t rs1, rs2, offset --> blt.t rs1, rs2, offset
   m_instructionMap["blt.t"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "blt.t", op[0], op[1], op[2]); };

   // bge.t rs1, rs2, offset --> bge.t rs1, rs2, offset
   m_instructionMap["bge.t"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "bge.t", op[0], op[1], op[2]); };

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
   // li.t rd, imm --> li.t rd, imm
   m_instructionMap["li.t"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "li.t", op[0], op[1]); };
}

}
