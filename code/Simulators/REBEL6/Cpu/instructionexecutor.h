#pragma once

#include <Simulators/REBEL6/ternarylogic.h>

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
void  executeAddi(Tint& rd, const Tint& rs1, const Tint& imm);
void  executeSlli(Tint& rd, const Tint& rs1, const Tint& imm);
void  executeSrli(Tint& rd, const Tint& rs1, const Tint& imm);
void  executeSrai(Tint& rd, const Tint& rs1, const Tint& imm);
void  executeOri(Tint& rd, const Tint& rs1, const Tint& imm);
void  executeXori(Tint& rd, const Tint& rs1, const Tint& imm);
void  executeAndi(Tint& rd, const Tint& rs1, const Tint& imm);

// Ternary
void  executeAddi_t(Tint& rd, const Tint& rs1, const Tint& imm);
void  executeSli_t(Tint& rd, const Tint& rs1, const Tint& imm);
void  executeSri_t(Tint& rd, const Tint& rs1, const Tint& imm);
void  executeSlti_t(Tint& rd, const Tint& rs1, const Tint& imm);
void  executeOri_t(Tint& rd, const Tint& rs1, const Tint& imm);
void  executeXori_t(Tint& rd, const Tint& rs1, const Tint& imm);
void  executeAndi_t(Tint& rd, const Tint& rs1, const Tint& imm);

//======================================
// Branch instructions
//======================================

// Ternary
void  executeBeq_t(const Tint& rs1, const Tint& rs2, std::int32_t offset, std::uint32_t& pc);
void  executeBne_t(const Tint& rs1, const Tint& rs2, std::int32_t offset, std::uint32_t& pc);
void  executeBlt_t(const Tint& rs1, const Tint& rs2, std::int32_t offset, std::uint32_t& pc);
void  executeBge_t(const Tint& rs1, const Tint& rs2, std::int32_t offset, std::uint32_t& pc);

//======================================
// Jump instructions
//======================================

// Ternary
void  executeJal_t(Tint& rd, std::int32_t offset, std::uint32_t& pc);

//======================================
// Jump register instructions
//======================================

// Ternary
void  executeJalr_t(Tint& rd, std::int32_t target, std::uint32_t& pc);

//======================================
// Load instructions
//======================================

// Binary
void  executeLw(Tint& rd, std::int32_t srcAddress, Simulators::REBEL6::ExecutableProgram& program);
void  executeLh(Tint& rd, std::int32_t srcAddress, Simulators::REBEL6::ExecutableProgram& program);
void  executeLb(Tint& rd, std::int32_t srcAddress, Simulators::REBEL6::ExecutableProgram& program);
void  executeLhu(Tint& rd, std::int32_t srcAddress, Simulators::REBEL6::ExecutableProgram& program);
void  executeLbu(Tint& rd, std::int32_t srcAddress, Simulators::REBEL6::ExecutableProgram& program);

// Ternary
void  executeLw_t(Tint& rd, std::int32_t srcAddress, Simulators::REBEL6::ExecutableProgram& program);
void  executeLh_t(Tint& rd, std::int32_t srcAddress, Simulators::REBEL6::ExecutableProgram& program);
void  executeLb_t(Tint& rd, std::int32_t srcAddress, Simulators::REBEL6::ExecutableProgram& program);

//======================================
// Store instructions
//======================================

// Binary
void  executeSw(const Tint& rs, std::int32_t targetAddress, Simulators::REBEL6::ExecutableProgram& program);
void  executeSh(const Tint& rs, std::int32_t targetAddress, Simulators::REBEL6::ExecutableProgram& program);
void  executeSb(const Tint& rs, std::int32_t targetAddress, Simulators::REBEL6::ExecutableProgram& program);

// Ternary
void  executeSw_t(const Tint& rs, std::int32_t targetAddress, Simulators::REBEL6::ExecutableProgram& program);
void  executeSh_t(const Tint& rs, std::int32_t targetAddress, Simulators::REBEL6::ExecutableProgram& program);
void  executeSb_t(const Tint& rs, std::int32_t targetAddress, Simulators::REBEL6::ExecutableProgram& program);


}
}
}
