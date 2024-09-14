#pragma once

#include <Parsers/ternarylogic.h>

namespace Simulators
{
namespace REBEL6
{

class ExecutableProgram;

namespace InstructionExecutor
{
//======================================
// Register instructions
//======================================

// Binary
void executeAdd(Tint& rd, const Tint& rs1, const Tint& rs2);
void executeSub(Tint& rd, const Tint& rs1, const Tint& rs2);
void executeSll(Tint& rd, const Tint& rs1, const Tint& rs2);
void executeSrl(Tint& rd, const Tint& rs1, const Tint& rs2);
void executeSra(Tint& rd, const Tint& rs1, const Tint& rs2);
void executeSltu(Tint& rd, const Tint& rs1, const Tint& rs2);
void executeOr(Tint& rd, const Tint& rs1, const Tint& rs2);
void executeXor(Tint& rd, const Tint& rs1, const Tint& rs2);
void executeAnd(Tint& rd, const Tint& rs1, const Tint& rs2);

// Ternary
void executeAdd_t(Tint& rd, const Tint& rs1, const Tint& rs2);
void executeSub_t(Tint& rd, const Tint& rs1, const Tint& rs2);
void executeSl_t(Tint& rd, const Tint& rs1, const Tint& rs2);
void executeSr_t(Tint& rd, const Tint& rs1, const Tint& rs2);
void executeSlt_t(Tint& rd, const Tint& rs1, const Tint& rs2);
void executeOr_t(Tint& rd, const Tint& rs1, const Tint& rs2);
void executeXor_t(Tint& rd, const Tint& rs1, const Tint& rs2);
void executeAnd_t(Tint& rd, const Tint& rs1, const Tint& rs2);

//======================================
// Immediate instructions
//======================================

// Binary
void executeAddi(Tint& rd, const Tint& rs1, const Tint& imm);
void executeSlli(Tint& rd, const Tint& rs1, const Tint& imm);
void executeSrli(Tint& rd, const Tint& rs1, const Tint& imm);
void executeSrai(Tint& rd, const Tint& rs1, const Tint& imm);
void executeSltiu(Tint& rd, const Tint& rs1, const Tint& imm);
void executeOri(Tint& rd, const Tint& rs1, const Tint& imm);
void executeXori(Tint& rd, const Tint& rs1, const Tint& imm);
void executeAndi(Tint& rd, const Tint& rs1, const Tint& imm);

// Ternary
void executeAddi_t(Tint& rd, const Tint& rs1, const Tint& imm);
void executeSli_t(Tint& rd, const Tint& rs1, const Tint& imm);
void executeSri_t(Tint& rd, const Tint& rs1, const Tint& imm);
void executeSlti_t(Tint& rd, const Tint& rs1, const Tint& imm);
void executeOri_t(Tint& rd, const Tint& rs1, const Tint& imm);
void executeXori_t(Tint& rd, const Tint& rs1, const Tint& imm);
void executeAndi_t(Tint& rd, const Tint& rs1, const Tint& imm);

//======================================
// Load Immediate instructions
//======================================

// Ternary
void executeLi_t(Tint& rd, const Tint& imm);
void executeAipc_t(Tint& rd, const Tint& imm, const std::int32_t& pc);

//======================================
// Branch instructions
//======================================

// Binary
void executeBgeu(const Tint& rs1, const Tint& rs2, std::int32_t offset, std::int32_t& pc, std::uint8_t instructionSize);
void executeBltu(const Tint& rs1, const Tint& rs2, std::int32_t offset, std::int32_t& pc, std::uint8_t instructionSize);

// Ternary
void executeBeq_t(const Tint& rs1, const Tint& rs2, std::int32_t offset, std::int32_t& pc, std::uint8_t instructionSize);
void executeBne_t(const Tint& rs1, const Tint& rs2, std::int32_t offset, std::int32_t& pc, std::uint8_t instructionSize);
void executeBlt_t(const Tint& rs1, const Tint& rs2, std::int32_t offset, std::int32_t& pc, std::uint8_t instructionSize);
void executeBge_t(const Tint& rs1, const Tint& rs2, std::int32_t offset, std::int32_t& pc, std::uint8_t instructionSize);

//======================================
// Jump instructions
//======================================

// Ternary
void executeJal_t(Tint& rd, std::int32_t offset, std::int32_t& pc, std::uint8_t instructionSize);

//======================================
// Jump register instructions
//======================================

// Ternary
void executeJalr_t(Tint& rd, const Tint& offset, const Tint& rs1, std::int32_t& pc, std::uint8_t instructionSize);

//======================================
// Load instructions
//======================================

// Binary
void executeLw(Tint& rd, const Tint& offset, const Tint& rs1, Simulators::REBEL6::ExecutableProgram& program);
void executeLh(Tint& rd, const Tint& offset, const Tint& rs1, Simulators::REBEL6::ExecutableProgram& program);
void executeLb(Tint& rd, const Tint& offset, const Tint& rs1, Simulators::REBEL6::ExecutableProgram& program);
void executeLhu(Tint& rd, const Tint& offset, const Tint& rs1, Simulators::REBEL6::ExecutableProgram& program);
void executeLbu(Tint& rd, const Tint& offset, const Tint& rs1, Simulators::REBEL6::ExecutableProgram& program);

// Ternary
void executeLw_t(Tint& rd, const Tint& offset, const Tint& rs1, Simulators::REBEL6::ExecutableProgram& program);
void executeLh_t(Tint& rd, const Tint& offset, const Tint& rs1, Simulators::REBEL6::ExecutableProgram& program);
void executeLt_t(Tint& rd, const Tint& offset, const Tint& rs1, Simulators::REBEL6::ExecutableProgram& program);

//======================================
// Store instructions
//======================================

// Binary
void executeSw(const Tint& rs2, const Tint& offset, const Tint& rs1, Simulators::REBEL6::ExecutableProgram& program);
void executeSh(const Tint& rs2, const Tint& offset, const Tint& rs1, Simulators::REBEL6::ExecutableProgram& program);
void executeSb(const Tint& rs2, const Tint& offset, const Tint& rs1, Simulators::REBEL6::ExecutableProgram& program);

// Ternary
void executeSw_t(const Tint& rs2, const Tint& offset, const Tint& rs1, Simulators::REBEL6::ExecutableProgram& program);
void executeSh_t(const Tint& rs2, const Tint& offset, const Tint& rs1, Simulators::REBEL6::ExecutableProgram& program);
void executeSt_t(const Tint& rs2, const Tint& offset, const Tint& rs1, Simulators::REBEL6::ExecutableProgram& program);


}
}
}
