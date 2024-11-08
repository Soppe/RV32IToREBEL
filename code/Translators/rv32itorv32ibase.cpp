#include "rv32itorv32ibase.h"

#include <Expressions/instruction.h>
#include <Expressions/label.h>
#include <Parsers/parseutils.h>
#include <logger.h>

#include <iostream>
#include <math.h>
#include <sstream>

namespace
{
const std::string pcrel_hi(const std::string& in) { return "%pcrel_hi(" + in +")"; }
const std::string pcrel_lo(const std::string& in) { return "%pcrel_lo(" + in +")"; }
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

// l{b|h|w}{u} rd, offset(rs) --> l{b|h|w}{u} rd, offset(rs)
// l{b|h|w} rd, symbol -->
// temp_label:
//    auipc rd, %pcrel_hi(symbol)
//    l{b|h|w} rd, %pcrel_lo(temp_label)(rd)
void handleLoad(const Translators::RV32IToRV32IBase* conv, const std::string& name, const Translators::StringList& op, Expressions::ExpressionList& el)
{
   std::string offsetDummy; // Placeholder, not used here
   std::string rs1Dummy; // Placeholder, not used here
   if(ParseUtils::parseRegisterOffset(op[1], offsetDummy, rs1Dummy)) { createInstruction(el, name, op[0], op[1]); }
   else // Pseudoinstruction
   {
      // Add label for the auipc according to the documentation for the %pcrel-modifiers
      // Note that %pcrel_lo uses the temp_label address as pc, finds the symbol used in the related auipc instruction, and takes the 12 LST from that symbol.
      std::string tempLabel;
      ParseUtils::generateTempLabel(tempLabel);
      el.push_back(new Expressions::Label(tempLabel));
      conv->at("auipc")({op[0], pcrel_hi(op[1])}, el);
      createInstruction(el, name, op[0], createOffset(pcrel_lo(tempLabel), op[0]));
   }
}

// s{b|h|w} rd, offset(rs) --> s{b|h|w} rd, offset(rs)
// s{b|h|w} rd, symbol, rt -->
// temp_label:
//    auipc rt, %pcrel_hi(symbol)
//    s{b|h|w} rd, %pcrel_lo(temp_label)(rt)
void handleStore(const Translators::RV32IToRV32IBase* conv, const std::string& name, const Translators::StringList& op, Expressions::ExpressionList& el)
{
   std::string offsetDummy; // Placeholder, not used here
   std::string rs1Dummy; // Placeholder, not used here
   if(ParseUtils::parseRegisterOffset(op[1], offsetDummy, rs1Dummy)) { createInstruction(el, name, op[0], op[1]); }
   else // Pseudoinstruction
   {
      // Add label for the auipc according to the documentation for the %pcrel-modifiers
      // Note that %pcrel_lo uses the temp_label address as pc, finds the symbol used in the related auipc instruction, and takes the 12 LST from that symbol.
      std::string tempLabel;
      ParseUtils::generateTempLabel(tempLabel);
      el.push_back(new Expressions::Label(tempLabel));
      conv->at("auipc")({op[2], pcrel_hi(op[1])}, el);
      createInstruction(el, name, op[0], createOffset(pcrel_lo(tempLabel), op[2]));
   }
}

std::string intToHexString(std::int32_t in)
{
   std::stringstream str;
   str << "0x" << std::hex << in;
   return str.str();
}
}

namespace Translators
{
RV32IToRV32IBase::RV32IToRV32IBase()
{
   fillInstructionMap();
}

void RV32IToRV32IBase::fillInstructionMap()
{
   // BASE INSTRUCTIONS (including pseudoinstructions in the cases where the base and pseudoinstruction has the same name)

   // add rd, rs1, rs2 --> add rd, rs1, rs2
   m_instructionMap["add"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "add", op[0], op[1], op[2]); };

   // sub rd, rs1, rs2 --> sub rd, rs1, rs2
   m_instructionMap["sub"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "sub", op[0], op[1], op[2]); };

   // addi rd, rs1, imm --> addi rd, rs1, imm
   m_instructionMap["addi"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "addi", op[0], op[1], op[2]); };

   // lui rd, imm --> lui rd, imm
   m_instructionMap["lui"] = [] (const StringList& op, Expressions::ExpressionList& el){ createInstruction(el, "lui", op[0], op[1]); };

   // auipc rd, imm --> auipc rd, imm
   m_instructionMap["auipc"] = [] (const StringList& op, Expressions::ExpressionList& el){ createInstruction(el, "auipc", op[0], op[1]); };

   // jal offset --> jal x1, offset
   // jal rd, offset --> jal rd, offset
   m_instructionMap["jal"] = [this] (const StringList& op, Expressions::ExpressionList& el)
           {
              if(op.size() == 1) at("jal")({"x1", op[0]}, el); // Pseudoinstruction
              else               createInstruction(el, "jal", op[0], op[1]);
           };


   // jalr rs --> jalr x1, 0(rs)
   // jalr rd, rs, offset --> jalr rd, offset(rs) // This is an older format, e.g. from version v2.2
   // jalr rd, offset(rs) --> jalr rd, offset(rs)
   m_instructionMap["jalr"] = [this] (const StringList& op, Expressions::ExpressionList& el)
           {
              if(op.size() == 1) at("jalr")({"x1", zeroOffset(op[0])}, el);
              else if(op.size() == 3) at("jalr")({op[0], createOffset(op[2], op[1])}, el); // Older format, e.g. from v2.2
              else               createInstruction(el, "jalr", op[0], op[1]);
           };

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


   // s{b|h|w} rd, offset(rs) --> s{b|h|w} rd, offset(rs)
   // s{b|h|w} rd, symbol, rt -->
   //    auipc rt, %pcrel_hi(symbol)
   //    s{b|h|w} rd, %pcrel_lo(symbol)(rt)
   m_instructionMap["sb"] = [this] (const StringList& op, Expressions::ExpressionList& el) { handleStore(this, "sb", op, el); };
   m_instructionMap["sh"] = [this] (const StringList& op, Expressions::ExpressionList& el) { handleStore(this, "sh", op, el); };
   m_instructionMap["sw"] = [this] (const StringList& op, Expressions::ExpressionList& el) { handleStore(this, "sw", op, el); };

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

   // slti rd, rs1, imm --> slti rd, rs1, imm
   m_instructionMap["slti"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "slti", op[0], op[1], op[2]); };

   // sltiu rd, rs1, imm --> sltiu rd, rs1, imm
   m_instructionMap["sltiu"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "sltiu", op[0], op[1], op[2]); };

   // xori rd, rs1, imm --> xori rd, rs1, imm
   m_instructionMap["xori"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "xori", op[0], op[1], op[2]); };

   // ori rd, rs1, imm --> ori rd, rs1, imm
   m_instructionMap["ori"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "ori", op[0], op[1], op[2]); };

   // andi rd, rs1, imm --> andi rd, rs1, imm
   m_instructionMap["andi"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "andi", op[0], op[1], op[2]); };

   // slli rd, rs1, imm --> slli rd, rs1, imm
   m_instructionMap["slli"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "slli", op[0], op[1], op[2]); };

   // srli rd, rs1, imm --> srli rd, rs1, imm
   m_instructionMap["srli"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "srli", op[0], op[1], op[2]); };

   // srai rd, rs1, imm --> srai rd, rs1, imm
   m_instructionMap["srai"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "srai", op[0], op[1], op[2]); };

   // sll rd, rs1, rs2 --> sll rd, rs1, rs2
   m_instructionMap["sll"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "sll", op[0], op[1], op[2]); };

   // slt rd, rs1, rs2 --> slt rd, rs1, rs2
   m_instructionMap["slt"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "slt", op[0], op[1], op[2]); };

   // sltu rd, rs1, rs2 --> sltu rd, rs1, rs2
   m_instructionMap["sltu"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "sltu", op[0], op[1], op[2]); };

   // xor rd, rs1, rs2 --> xor rd, rs1, rs2
   m_instructionMap["xor"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "xor", op[0], op[1], op[2]); };

   // srl rd, rs1, rs2 --> srl rd, rs1, rs2
   m_instructionMap["srl"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "srl", op[0], op[1], op[2]); };

   // sra rd, rs1, rs2 --> sra rd, rs1, rs2
   m_instructionMap["sra"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "sra", op[0], op[1], op[2]); };

   // or rd, rs1, rs2 --> or rd, rs1, rs2
   m_instructionMap["or"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "or", op[0], op[1], op[2]); };

   // and rd, rs1, rs2 --> and rd, rs1, rs2
   m_instructionMap["and"] = [] (const StringList& op, Expressions::ExpressionList& el) { createInstruction(el, "and", op[0], op[1], op[2]); };

   // fence --> nop
   m_instructionMap["fence"] = [this] (const StringList&, Expressions::ExpressionList& el) { at("nop")({}, el); };

   // ecall --> ecall
   m_instructionMap["ecall"] = [] (const StringList&, Expressions::ExpressionList& el) { createInstruction(el, "ecall"); };

   // ebreak --> ebreak
   m_instructionMap["ebreak"] = [] (const StringList&, Expressions::ExpressionList& el) { createInstruction(el, "ebreak"); };


   // PSEUDOINSTRUCTIONS

   // la rd, symbol -->
   // temp_label:
   //    auipc rd, %pcrel_hi(symbol)
   //    addi rd, rd, %pcrel_lo(tempLabel)
   m_instructionMap["la"] = [this] (const StringList& op, Expressions::ExpressionList& el)
           {
              // Add label for the auipc according to the documentation for the %pcrel-modifiers
              // Note that %pcrel_lo uses the tempLabel address as pc, finds the symbol used in the related auipc instruction, and takes the 12 LST from that symbol.
              std::string tempLabel;
              ParseUtils::generateTempLabel(tempLabel);
              el.push_back(new Expressions::Label(tempLabel));
              at("auipc")({op[0], pcrel_hi(op[1])}, el);
              at("addi")({op[0], op[0], pcrel_lo(tempLabel)}, el); // For nopic only. Modify when/if pic-support is added
           };

   // lla rd, symbol -->
   // temp_label:
   //    auipc rd, %pcrel_hi(symbol)
   //    addi rd, rd, %pcrel_lo(tempLabel)
   m_instructionMap["lla"] = [this] (const StringList& op, Expressions::ExpressionList& el)
           {
              // Add label for the auipc according to the documentation for the %pcrel-modifiers
              // Note that %pcrel_lo uses the tempLabel address as pc, finds the symbol used in the related auipc instruction, and takes the 12 LST from that symbol.
              std::string tempLabel;
              ParseUtils::generateTempLabel(tempLabel);
              el.push_back(new Expressions::Label(tempLabel));
              at("auipc")({op[0], pcrel_hi(op[1])}, el);
              at("addi")({op[0], op[0], pcrel_lo(tempLabel)}, el);
           };

   // nop --> addi x0, x0, 0
   m_instructionMap[ "nop"] = [this] (const StringList&, Expressions::ExpressionList& el) { at("addi")({"x0", "x0", "0"}, el); };

   // li rd, imm -->
   //    lui rd, imm20
   //    addi rd, rd, imm12
   m_instructionMap[ "li"] = [this] (const StringList& op, Expressions::ExpressionList& el)
           {
              std::int32_t val;
              if(ParseUtils::parseImmediate(32, op[1], val))
              {
                 static std::int16_t maxAddiVal = std::pow(2, 11);

                 // Only use lui if the value can't be represented with a single addi call
                 if((val >= maxAddiVal) || (val < -maxAddiVal))
                 {
                    std::int32_t botVal;
                    ParseUtils::parseImmediate(12, val, botVal);
                   // Compensate for signedness for botVal, and mask away any of the 12 MSB from topVal as it makes lui grumpy
                    std::int32_t topVal = ((val >> 12) + ((val & 0x800) ? 1 : 0)) & 0xfffff;
                    at("lui")({op[0], intToHexString(topVal)}, el);
                    at("addi")({op[0], op[0], intToHexString(botVal)}, el);
                 }
                 else
                 {
                    at("addi")({op[0], "x0", op[1]}, el);
                 }

              }
              else
              {
                 std::cerr << __PRETTY_FUNC__ << ": Unable to parse immediate " << op[1] << " for li instruction" << std::endl;
                 abort();
              }
           };

   // mv rd, rs --> addi rd, rs, 0
   m_instructionMap["mv"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("addi")({op[0], op[1], "0"}, el); };

   // not rd, rs --> xori rd, rs, -1
   m_instructionMap["not"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("xori")({op[0], op[1], "-1"}, el); };

   // neg rd, rs --> sub rd, x0, rs
   m_instructionMap["neg"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("sub")({ op[0], "x0", op[1] }, el); };

   // seqz rd, rs --> sltiu rd, rs, 1
   m_instructionMap["seqz"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("sltiu")({ op[0], op[1], "1" }, el); };

   // snez rd, rs --> sltu rd, x0, rs
   m_instructionMap["snez"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("sltu")({ op[0], "x0", op[1] }, el); };

   // sltz rd, rs --> slt rd, rs, x0
   m_instructionMap["sltz"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("slt")({ op[0], op[1], "x0" }, el); };

   // sgtz rd, rs --> slt rd, x0, rs
   m_instructionMap["sgtz"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("slt")({ op[0], "x0", op[1] }, el); };

   // beqz rs, offset --> beq rs, x0, offset
   m_instructionMap["beqz"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("beq")({ op[0], "x0", op[1] }, el); };

   // bnez rs, offset --> bne rs, x0, offset
   m_instructionMap["bnez"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("bne")({ op[0], "x0", op[1] }, el); };

   // blez rs, offset --> bge x0, rs, offset
   m_instructionMap["blez"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("bge")({ "x0", op[0], op[1] }, el); };

   // bgez rs, offset --> bge rs, x0, offset
   m_instructionMap["bgez"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("bge")({ op[0], "x0", op[1] }, el); };

   // bltz rs, offset --> blt rs, x0, offset
   m_instructionMap["bltz"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("blt")({ op[0], "x0", op[1] }, el); };

   // bgtz rs, offset --> blt x0, rs, offset
   m_instructionMap["bgtz"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("blt")({ "x0", op[0], op[1] }, el); };

   // bgt rs, rt, offset --> blt rt, rs, offset
   m_instructionMap["bgt"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("blt")({ op[1], op[0], op[2] }, el); };

   // ble rs, rt, offset --> bge rt, rs, offset
   m_instructionMap["ble"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("bge")({op[1], op[0], op[2]}, el); };

   // bgtu rs, rt, offset --> bltu rt, rs, offset
   m_instructionMap["bgtu"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("bltu")({ op[1], op[0], op[2] }, el); };

   // bleu rs, rt, offset --> bgeu rt, rs, offset
   m_instructionMap["bleu"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("bgeu")({op[1], op[0], op[2]}, el); };

   // j offset --> jal x0, offset
   m_instructionMap["j"] = [this] (const StringList& op, Expressions::ExpressionList& el) { at("jal")({"x0", op[0]}, el); };

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
   // temp_label:
   //    auipc x1, %pcrel_hi(offset)
   //    jalr x1, %pcrel_lo(temp_label)(x1)
   m_instructionMap["call"] = [this] (const StringList& op, Expressions::ExpressionList& el)
           {
              // Add label for the auipc according to the documentation for the %pcrel-modifiers
              // Note that %pcrel_lo uses the temp_label address as pc, finds the symbol used in the related auipc instruction, and takes the 12 LST from that symbol.
              std::string tempLabel;
              ParseUtils::generateTempLabel(tempLabel);
              el.push_back(new Expressions::Label(tempLabel));
              at("auipc")({"x1", pcrel_hi(op[0])}, el);
              at("jalr")({"x1", createOffset(pcrel_lo(tempLabel), "x1")}, el);
           };

   // tail offset -->
   // temp_label:
   //    auipc x6, %pcrel_hi(offset)
   //    jalr x0, %pcrel_lo(temp_label)(x6)
   /*insert( {"tail"] = [this] (const StringList& op, Expressions::ExpressionList& el)
           {
              // Add label for the auipc according to the documentation for the %pcrel-modifiers
              // Note that %pcrel_lo uses the temp_label address as pc, finds the symbol used in the related auipc instruction, and takes the 12 LST from that symbol.
              std::string tempLabel;
              ParseUtils::generateTempLabel(tempLabel);
              el.push_back(new Expressions::Label(tempLabel));
              at("auipc")({"x6", pcrel_hi(op[0])}, el);
              at("jalr")({"x0", pcrel_lo(tempLabel) + "(x6)"}, el);
           };*/
}

}
