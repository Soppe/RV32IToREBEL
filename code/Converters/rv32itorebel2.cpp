#include "rv32itorebel2.h"

#include <Expressions/instruction.h>
#include <Parsers/parseutils.h>
#include <logger.h>

#include <iostream>



// THIS FILE IS OUTDATED AND SHOULD NOT BE USED. IT IS ONLY HERE FOR HISTORICAL PURPOSES


namespace
{
const std::string pcrel_hi(const std::string& in) { return "%pcrel_hi(" + in +")"; }
const std::string pcrel_lo(const std::string& in) { return "%pcrel_lo(" + in +")"; }
std::string zeroOffset(const std::string& in) { return "0(" + in + ")"; }
std::string createOffset(const std::string& offset, const std::string& base) { return offset + "(" + base + ")"; }

void createTernaryInstruction(Expressions::ExpressionList& el, const std::string& name)
{
   el.push_back(new Expressions::Instruction(name, std::vector<std::string>()));
}

void createTernaryInstruction(Expressions::ExpressionList& el, const std::string& name, const std::string& rs1, const std::string& rs2, const std::string& rd1, const std::string& rd2)
{
   std::vector<std::string> operands = {rs1, rs2, rd1, rd2};
   el.push_back(new Expressions::Instruction(name, operands));
}

// l{b|h|w}{u} rd, offset(rs) --> addi rs, offset, rd, 0
// l{b|h|w} rd, symbol -->
//    auipc rd, %pcrel_hi(symbol)
//    l{b|h|w} rd, %pcrel_lo(symbol)(rd) --> addi rd, %pcrel_lo(symbol), rd, 0
void handleLoad(const Converters::RV32IToREBEL2* conv, const Converters::StringList& op, Expressions::ExpressionList& el)
{
   std::string offset;
   std::string rs1;
   if(ParseUtils::parseRegisterOffset(op[1], offset, rs1)) { createTernaryInstruction(el, "addi", rs1, offset, op[0], "0"); }
   else // Pseudoinstruction
   {
      conv->at("auipc")({op[0], pcrel_hi(op[1])}, el);
      createTernaryInstruction(el, "addi", op[0], pcrel_lo(op[1]), op[0], "0");
   }
}

// s{b|h|w} rd, offset(rs) --> addi rs, offset, rd, 0
// s{b|h|w} rd, symbol, rt -->
//    auipc rt, %pcrel_hi(symbol)
//    s{b|h|w} rd, %pcrel_lo(symbol)(rt) --> addi rt, %pcrel_lo(symbol), rd, 0
void handleStore(const Converters::RV32IToREBEL2* conv, const Converters::StringList& op, Expressions::ExpressionList& el)
{
   std::string offset;
   std::string rs1;
   if(ParseUtils::parseRegisterOffset(op[1], offset, rs1)) { createTernaryInstruction(el, "addi", rs1, offset, op[0], "0"); }
   else // Pseudoinstruction
   {
      conv->at("auipc")({op[2], pcrel_hi(op[1])}, el);
      createTernaryInstruction(el, "addi", op[2], pcrel_lo(op[1]), op[0], "0");
   }
}

}
namespace Converters
{
RV32IToREBEL2::RV32IToREBEL2()
{
   RV32IToREBEL2::fillExpressionMap();
}

// Instructions that are commented out are untested.
// Instructions in capital letters are placeholders for instructions that REBEL-2 currently has no support for.
//
void RV32IToREBEL2::fillExpressionMap()
{
   // BASE INSTRUCTIONS (including pseudoinstructions in the cases where the base and pseudoinstruction has the same name)

   // add rd, rs1, rs2 --> add rs1, rs2, rd, x0
   m_instructionMap.insert( {"add", [] (const StringList& op, Expressions::ExpressionList& el) { createTernaryInstruction(el, "add", op[1], op[2], op[0], "x0"); } } );

   // sub rd, rs1, rs2 --> add rs1, rs2, rd1, x1
   m_instructionMap.insert( {"sub", [] (const StringList& op, Expressions::ExpressionList& el) { createTernaryInstruction(el, "add", op[1], op[2], op[0], "x1"); } } );

   // addi rd, rs1, imm --> addi rs1, imm, rd, 0
   m_instructionMap.insert( {"addi", [] (const StringList& op, Expressions::ExpressionList& el) { createTernaryInstruction(el, "addi", op[1], op[2], op[0], "0"); } } );

   // TODO: Undefined in REBEL2. Two ways to handle it: Either use imm as is and trust compiler to add %hi, or enforce %hi to the imm. Currently trusting the compiler and letting the
   // operand modifier figure it out as it has to convert the value either way
   // lui rd, imm --> addi X0, imm, rd, 0
   m_instructionMap.insert( {"lui", [] (const StringList& op, Expressions::ExpressionList& el){ createTernaryInstruction(el, "addi", "x0", op[1], op[0], "0"); } } );

   // TODO: Unsupported in REBEL2. Current solution is just placeholder wher auipc is meant to be replaced by addi - if it works-, but with the current operands.
   // auipc rd, imm --> auipc pc, imm, rd, 0
   m_instructionMap.insert( {"auipc", [] (const StringList& op, Expressions::ExpressionList& el){ createTernaryInstruction(el, "AUIPC", "pc", op[1], op[0], "0"); } } );

   // jal offset --> jal x1, offset --> jal x0, x1, x1, offset
   // jal rd, offset --> jal x0, x1, rd, offset
   m_instructionMap.insert( {"jal", [this] (const StringList& op, Expressions::ExpressionList& el)
           {
              if(op.size() == 1) at("jal")({"x1", op[0]}, el); // Pseudoinstruction
              else               createTernaryInstruction(el, "jal", "x0", "x1", op[0], op[1]);
           } } );


   // jalr rs --> jalr x1, 0(rs) --> jal rs, x0, x1, 0
   // jalr rd, rs, offset --> jalr rd, offset(rs) --> jal rs, x0, rd, offset // This is an older format, e.g. from version v2.2
   // jalr rd, offset(rs) --> jal rs, x0, rd, offset
   m_instructionMap.insert( {"jalr", [this] (const StringList& op, Expressions::ExpressionList& el)
           {
              if(op.size() == 1) at("jalr")({"x1", zeroOffset(op[0])}, el); // Pseudoinstruction
              else if(op.size() == 3) at("jalr")({op[0], createOffset(op[2], op[1])}, el); // Older format, e.g. from v2.2
              else
              {
                 std::string offset;
                 std::string rs1;
                 if(ParseUtils::parseRegisterOffset(op[1], offset, rs1)) { createTernaryInstruction(el, "jal", rs1, "x0", op[0], offset); }
                 else{std::cout << op[0] << "; " << op[1] << std::endl; std::cerr << "Failed to parse instruction jalr: " << std::endl; }
              }
           } } );

   // l{b|h|w} rd, offset(rs) --> addi rs, offset, rd, 0
   // l{b|h|w} rd, symbol -->
   //    auipc rd, %pcrel_hi(symbol)
   //    l{b|h|w} rd, %pcrel_lo(symbol)(rd) --> addi rd, %pcrel_lo(symbol), rd, 0
   //insert( {"lb", [] (const StringList& op, Expressions::ExpressionList& el) { handleLoad(op, el); } } );
   //insert( {"lh", [] (const StringList& op, Expressions::ExpressionList& el) { handleLoad(op, el); } } );
   m_instructionMap.insert( {"lw", [this] (const StringList& op, Expressions::ExpressionList& el) { handleLoad(this, op, el); } } );


   // lbu rd, offset(rs) --> addi rs, offset, rd, 0
   m_instructionMap.insert( {"lbu", [this] (const StringList& op, Expressions::ExpressionList& el) { handleLoad(this, op, el); } } );
           /*{
              std::string offset;
              std::string rs1;
              if(ParseUtils::parseRegisterOffset(op[1], offset, rs1)) { createTernaryInstruction(el, "addi", rs1, offset, op[0], "0"); }
              else{std::cerr << "Failed to parse instruction lbu" << std::endl; }
           } } );*/

   // lhu rd, offset(rs) --> addi rs, offset, rd, 0
   /*insert( {"lhu", [this] (const StringList& op, Expressions::ExpressionList& el) { handleLoad(this, op, el); } } );*/
           /*{
              std::string offset;
              std::string rs1;
              if(parseOffset(op[1], offset, rs1)) { createTernaryInstruction(el, "addi", rs1, offset, op[0], "0"); }
              else{std::cerr << "Failed to parse instruction lhu" << std::endl;}
           } } );*/


   // s{b|h|w} rd, offset(rs) --> addi rs, offset, rd, 0
   // s{b|h|w} rd, symbol, rt -->
   //    auipc rt, %pcrel_hi(symbol)
   //    s{b|h|w} rd, %pcrel_lo(symbol)(rt) --> addi rt, %pcrel_lo(symbol), rd, 0
   //insert( {"sb", [] (const StringList& op, Expressions::ExpressionList& el) { handleStore(op, el); } } );
   //insert( {"sh", [] (const StringList& op, Expressions::ExpressionList& el) { handleStore(op, el); } } );
   m_instructionMap.insert( {"sw", [this] (const StringList& op, Expressions::ExpressionList& el) { handleStore(this, op, el); } } );

   // beq rs1, rs2, offset --> bceg rs1, rs2, offset, x0
   m_instructionMap.insert( {"beq", [] (const StringList& op, Expressions::ExpressionList& el) { createTernaryInstruction(el, "bceg", op[0], op[1], op[2], "x0"); } } );

   // TODO: Document this
   // bne rs1, rs2, offset -->
   //    comp rs1, rs2, t6, x0
   //    mudi t6, t6, t6, x0
   //    bceg t6, x0, x0, offset
   m_instructionMap.insert( {"bne", [] (const StringList& op, Expressions::ExpressionList& el)
           {
              createTernaryInstruction(el, "comp", op[0], op[1], "t6", "x0");
              createTernaryInstruction(el, "mudi", "t6", "t6", "t6", "x0");
              createTernaryInstruction(el, "bceg", "t6", "x0", "x0", op[2]);
           } } );

   // blt rs1, rs2, offset --> bceg rs2, rs1, x0, offset
   m_instructionMap.insert( {"blt", [] (const StringList& op, Expressions::ExpressionList& el) { createTernaryInstruction(el, "bceg", op[1], op[0], "x0", op[2]); } } );

   // bge rs1, rs2, offset --> bceg rs1, rs2, offset, offset
   m_instructionMap.insert( {"bge", [] (const StringList& op, Expressions::ExpressionList& el) { createTernaryInstruction(el, "bceg", op[0], op[1], op[2], op[2]); } } );

   // bltu rs1, rs2, offset --> bceg rs2, rs1, x0, offset
   m_instructionMap.insert( {"bltu", [] (const StringList& op, Expressions::ExpressionList& el) { createTernaryInstruction(el, "bceg", op[1], op[0], "x0", op[2]); } } );

   // bgeu rs1, rs2, offset --> bceg rs1, rs2, offset, offset
   m_instructionMap.insert( {"bgeu", [] (const StringList& op, Expressions::ExpressionList& el) { createTernaryInstruction(el, "bceg", op[0], op[1], op[2], op[2]); } } );

// TODO: slti

// TODO: sltiu

// TODO: xori

// TODO: ori

// TODO: andi

   // slli rd, rs1, imm --> shi rs1, imm, rd, "-"
   m_instructionMap.insert( {"slli", [] (const StringList& op, Expressions::ExpressionList& el) { createTernaryInstruction(el, "shi", op[1], op[2], op[0], "-"); } } );

   // srli rd, rs1, imm --> shi rs1, imm, rd, "+"
   m_instructionMap.insert( {"srli", [] (const StringList& op, Expressions::ExpressionList& el) { createTernaryInstruction(el, "shi", op[1], op[2], op[0], "+"); } } );

   // srai rd, rs1, imm --> shi rs1, imm, rd, "+"
   m_instructionMap.insert( {"srai", [] (const StringList& op, Expressions::ExpressionList& el) { createTernaryInstruction(el, "shi", op[1], op[2], op[0], "+"); } } );

// TODO: sll

   //TODO: Not sure if the last param should be x0 for word-wise comparison or something else for trit-wise comparison
   // slt rd, rs1, rs2 --> comp rs1, rs2, rd, x1
   m_instructionMap.insert( {"slt", [] (const StringList& op, Expressions::ExpressionList& el) { createTernaryInstruction(el, "comp", op[1], op[2], op[0], "x1"); } } );

   //TODO: Not sure if the last param should be x0 for word-wise comparison or something else for trit-wise comparison
   // sltu rd, rs1, rs2 --> comp rs1, rs2, rd, x1
   m_instructionMap.insert( {"sltu", [] (const StringList& op, Expressions::ExpressionList& el) { createTernaryInstruction(el, "comp", op[1], op[2], op[0], "x1"); } } );

// TODO: xor

// TODO: srl

// TODO: sra

   // or rd, rs1, rs2 --> mima rs1, rs2, rd, "+0"
   m_instructionMap.insert( {"or", [] (const StringList& op, Expressions::ExpressionList& el) { createTernaryInstruction(el, "mima", op[1], op[2], op[0], "+0"); } } );

   // and rd, rs1, rs2 --> mima rs1, rs2, rd, "-0"
   m_instructionMap.insert( {"and", [] (const StringList& op, Expressions::ExpressionList& el) { createTernaryInstruction(el, "mima", op[1], op[2], op[0], "-0"); } } );

   // fence --> nop
   m_instructionMap.insert( { "fence", [this] (const StringList&, Expressions::ExpressionList& el) { at("nop")({}, el); } });

   // ecall --> ecall
   m_instructionMap.insert( { "ecall", [] (const StringList&, Expressions::ExpressionList& el) { createTernaryInstruction(el, "ecall"); } });

   // ebreak --> ebreak
   //insert( { "ebreak", [] (const StringList&, Expressions::ExpressionList& el) { createTernaryInstruction(el, "ebreak"); } });


      // PSEUDOINSTRUCTIONS

   // la rd, symbol -->
   //    auipc rd, %pcrel_hi(symbol)
   //    addi rd, rd, %pcrel_lo(symbol)
   m_instructionMap.insert( { "la", [this] (const StringList& op, Expressions::ExpressionList& el)
           {
              at("auipc")({op[0], pcrel_hi(op[1])}, el);
              at("addi")({op[0], op[0], pcrel_lo(op[1])}, el); // For nopic only. Modify when/if pic-support is added
           } } );

   // lla rd, symbol -->
   //    auipc rd, %pcrel_hi(symbol)
   //    addi rd, rd, %pcrel_lo(symbol)
   m_instructionMap.insert( { "lla", [this] (const StringList& op, Expressions::ExpressionList& el)
           {
              at("auipc")({op[0], pcrel_hi(op[1])}, el);
              at("addi")({op[0], op[0], pcrel_lo(op[1])}, el);
           } } );

   // nop --> addi x0, x0, 0
   m_instructionMap.insert( { "nop", [this] (const StringList&, Expressions::ExpressionList& el) { at("addi")({"x0", "x0", "0"}, el); } } );

   // li rd, imm -->
   //    lui rd, imm
   //    addi rd, rd, imm
   m_instructionMap.insert( { "li", [this] (const StringList& op, Expressions::ExpressionList& el) // TODO: Can possibly be converted to a single addi and then let operand converter deal with too large numbers
           {
              std::int32_t val;
              if(ParseUtils::parseImmediate(32, op[1], val))
              {
                 // Compensate for signedness for botVal
                 std::int32_t topVal = (val >> 12) + ((val & 0x800) ? 1 : 0);
                 std::int32_t botVal;
                 ParseUtils::parseImmediate(12, val, botVal);

                 at("lui")({op[0], std::to_string(topVal)}, el);
                 at("addi")({op[0], op[0], std::to_string(botVal)}, el);
              }
              else
              {
                 std::cerr << __PRETTY_FUNC__ << ": Unable to parse immediate " << op[1] << " for li instruction" << std::endl;
              }

              /*try
              {
                 std::int32_t val = stoi(op[1]);
                 std::int32_t topVal = (val & 0xfffff000);
                 std::int32_t botVal = (val & 0xfff);

                 at("lui")({op[0], std::to_string(topVal)}, el);
                 at("addi")({op[0], op[0], std::to_string(botVal)}, el);
              }
              catch(std::exception&)
              {
                 at("lui")({op[0], hi(op[1])}, el);
                 at("addi")({op[0], op[0], lo(op[1])}, el);
              }*/
           } });

   // mv rd, rs --> addi rd, rs, 0
   m_instructionMap.insert( {"mv", [this] (const StringList& op, Expressions::ExpressionList& el) { at("addi")({op[0], op[1], "0"}, el); } } );


// TODO: "not" - requires xori

   //TODO: Could this be achieved with an instruction that flips each trit, since it's balanced ternary?
   // neg rd, rs --> sub rd, x0, rs
   //insert( {"neg", [this] (const StringList& op, Expressions::ExpressionList& el) { at("sub")({ op[0], "x0", op[1] }, el); } } );

// TODO: seqz - requires sltiu

   // snez rd, rs --> sltu rd, x0, rs
   //insert( {"snez", [this] (const StringList& op, Expressions::ExpressionList& el) { at("sltu")({ op[0], "x0", op[1] }, el); } } );

   // sltz rd, rs --> slt rd, rs, x0
   //insert( {"sltz", [this] (const StringList& op, Expressions::ExpressionList& el) { at("slt")({ op[0], op[1], "x0" }, el); } } );

   // sgtz rd, rs --> slt rd, x0, rs
   //insert( {"sgtz", [this] (const StringList& op, Expressions::ExpressionList& el) { at("slt")({ op[0], "x0", op[1] }, el); } } );

   // beqz rs, offset --> beq rs, x0, offset
   //insert( {"beqz", [this] (const StringList& op, Expressions::ExpressionList& el) { at("beq")({ op[0], "x0", op[1] }, el); } } );

   // bnez rs, offset --> bne rs, x0, offset
   //insert( {"bnez", [this] (const StringList& op, Expressions::ExpressionList& el) { at("bne")({ op[0], "x0", op[1] }, el); } } );

   // blez rs, offset --> bge x0, rs, offset
   //insert( {"blez", [this] (const StringList& op, Expressions::ExpressionList& el) { at("bge")({ "x0", op[0], op[1] }, el); } } );

   // bgez rs, offset --> bge rs, x0, offset
   //insert( {"bgez", [this] (const StringList& op, Expressions::ExpressionList& el) { at("bge")({ op[0], "x0", op[1] }, el); } } );

   // bltz rs, offset --> blt rs, x0, offset
   //insert( {"bltz", [this] (const StringList& op, Expressions::ExpressionList& el) { at("blt")({ op[0], "x0", op[1] }, el); } } );

   // bgtz rs, offset --> blt x0, rs, offset
   //insert( {"bgtz", [this] (const StringList& op, Expressions::ExpressionList& el) { at("blt")({ "x0", op[0], op[1] }, el); } } );

   // bgt rs, rt, offset --> blt rt, rs, offset
   //insert( {"bgt", [this] (const StringList& op, Expressions::ExpressionList& el) { at("blt")({ op[1], op[0], op[2] }, el); } } );

   // ble rs, rt, offset --> bge rt, rs, offset
   m_instructionMap.insert( {"ble", [this] (const StringList& op, Expressions::ExpressionList& el) { at("bge")({op[1], op[0], op[2]}, el); } } );

   // bgtu rs, rt, offset --> bltu rt, rs, offset
   //insert( {"bgtu", [this] (const StringList& op, Expressions::ExpressionList& el) { at("bltu")({ op[1], op[0], op[2] }, el); } } );

   // bleu rs, rt, offset --> bgeu rt, rs, offset
   //insert( {"bleu", [this] (const StringList& op, Expressions::ExpressionList& el) { at("bgeu")({op[1], op[0], op[2]}, el); } } );

   // j offset --> jal x0, offset
   m_instructionMap.insert( {"j", [this] (const StringList& op, Expressions::ExpressionList& el) { at("jal")({"x0", op[0]}, el); } } );

   // jr rs --> jalr x0, 0(rs)
   m_instructionMap.insert( {"jr", [this] (const StringList& op, Expressions::ExpressionList& el) { at("jalr")({"x0", zeroOffset(op[0])}, el); } } );

   // ret --> jalr x0, 0(x1)
   //insert( {"ret", [this] (const StringList&, Expressions::ExpressionList& el) { at("jalr")({"x0", zeroOffset("x1")}, el); } } );

   // call offset -->
   //    auipc x1, %pcrel_hi(offset)
   //    jalr x1, %pcrel_lo(offset)(x1)
   m_instructionMap.insert( {"call", [this] (const StringList& op, Expressions::ExpressionList& el)
           {
              at("auipc")({"x1", pcrel_hi(op[0])}, el);
              at("jalr")({"x1", createOffset(pcrel_lo(op[0]), "x1")}, el);
           } } );

   // tail offset -->
   //    auipc x6, %pcrel_hi(offset)
   //    jalr x0, %pcrel_lo(offset)(x6)
   m_instructionMap.insert( {"tail", [this] (const StringList& op, Expressions::ExpressionList& el)
           {
              at("auipc")({"x6", pcrel_hi(op[0])}, el);
              at("jalr")({"x0", createOffset(pcrel_lo(op[0]), "x6")}, el);
           } } );

}

}
