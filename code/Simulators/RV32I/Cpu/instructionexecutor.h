#pragma once

#include <cstdint>

namespace Simulators
{
namespace RV32I
{

class ExecutableProgram;

namespace InstructionExecutor
{
//======================================
// Register instructions
//======================================
void executeAdd(std::int32_t& rd, std::int32_t rs1, std::int32_t rs2);
void executeSub(std::int32_t& rd, std::int32_t rs1, std::int32_t rs2);
void executeSll(std::int32_t& rd, std::int32_t rs1, std::int32_t rs2);
void executeSrl(std::int32_t& rd, std::int32_t rs1, std::int32_t rs2);
void executeSra(std::int32_t& rd, std::int32_t rs1, std::int32_t rs2);
void executeSlt(std::int32_t& rd, std::int32_t rs1, std::int32_t rs2);
void executeSltu(std::int32_t& rd, std::int32_t rs1, std::int32_t rs2);
void executeOr(std::int32_t& rd, std::int32_t rs1, std::int32_t rs2);
void executeXor(std::int32_t& rd, std::int32_t rs1, std::int32_t rs2);
void executeAnd(std::int32_t& rd, std::int32_t rs1, std::int32_t rs2);

//======================================
// Immediate instructions
//======================================
void  executeAddi(std::int32_t& rd, std::int32_t rs1, std::int32_t imm);
void  executeSlli(std::int32_t& rd, std::int32_t rs1, std::int32_t imm);
void  executeSrli(std::int32_t& rd, std::int32_t rs1, std::int32_t imm);
void  executeSrai(std::int32_t& rd, std::int32_t rs1, std::int32_t imm);
void  executeSlti(std::int32_t& rd, std::int32_t rs1, std::int32_t imm);
void  executeSltiu(std::int32_t& rd, std::int32_t rs1, std::int32_t imm);
void  executeOri(std::int32_t& rd, std::int32_t rs1, std::int32_t imm);
void  executeXori(std::int32_t& rd, std::int32_t rs1, std::int32_t imm);
void  executeAndi(std::int32_t& rd, std::int32_t rs1, std::int32_t imm);

//======================================
// Upper instructions
//======================================
void  executeLui(std::int32_t& rd, std::int32_t imm20);
void  executeAuipc(std::int32_t& rd, std::int32_t imm20, std::int32_t pc);

//======================================
// Branch instructions
//======================================
void  executeBeq(std::int32_t rs1, std::int32_t rs2, std::int32_t offset, std::uint32_t& pc);
void  executeBne(std::int32_t rs1, std::int32_t rs2, std::int32_t offset, std::uint32_t& pc);
void  executeBlt(std::int32_t rs1, std::int32_t rs2, std::int32_t offset, std::uint32_t& pc);
void  executeBltu(std::int32_t rs1, std::int32_t rs2, std::int32_t offset, std::uint32_t& pc);
void  executeBge(std::int32_t rs1, std::int32_t rs2, std::int32_t offset, std::uint32_t& pc);
void  executeBgeu(std::int32_t rs1, std::int32_t rs2, std::int32_t offset, std::uint32_t& pc);

//======================================
// Jump instructions
//======================================
void  executeJal(std::int32_t& rd, std::int32_t offset, std::uint32_t& pc);

//======================================
// Jump register instructions
//======================================
void  executeJalr(std::int32_t& rd, std::int32_t target, std::uint32_t& pc);

//======================================
// Load instructions
//======================================
void  executeLw(std::int32_t& rd, std::int32_t srcAddress, Simulators::RV32I::ExecutableProgram& program);
void  executeLh(std::int32_t& rd, std::int32_t srcAddress, Simulators::RV32I::ExecutableProgram& program);
void  executeLb(std::int32_t& rd, std::int32_t srcAddress, Simulators::RV32I::ExecutableProgram& program);
void  executeLhu(std::int32_t& rd, std::int32_t srcAddress, Simulators::RV32I::ExecutableProgram& program);
void  executeLbu(std::int32_t& rd, std::int32_t srcAddress, Simulators::RV32I::ExecutableProgram& program);

//======================================
// Store instructions
//======================================
void  executeSw(std::int32_t rs, std::int32_t targetAddress, Simulators::RV32I::ExecutableProgram& program);
void  executeSh(std::int32_t rs, std::int32_t targetAddress, Simulators::RV32I::ExecutableProgram& program);
void  executeSb(std::int32_t rs, std::int32_t targetAddress, Simulators::RV32I::ExecutableProgram& program);
}
}
}
