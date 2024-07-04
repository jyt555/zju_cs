`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/05/29 17:53:54
// Design Name: 
// Module Name: Pipeline_ID
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module Pipeline_ID(
    input clk_ID,
    input rst_ID,
    input RegWrite_in_ID,
    input [4:0] Rd_addr_ID,
    input [31:0] Wt_data_ID,
    input [31:0] Inst_in_ID,
    output [31:0] Rd_addr_out_ID,
    output [31:0] Rs1_out_ID,
    output [31:0] Rs2_out_ID,

    output [4:0] Rs1_addr_ID,
    output [4:0] Rs2_addr_ID,
    output Rs1_used,
    output Rs2_used,

    output [31:0] Imm_out_ID,
    output ALUSrc_B_ID,
    output [2:0] ALU_control_ID,
    output Branch_ID,
    output BranchN_ID,
    output MemRW_ID,
    output Jump_ID,
    output [1:0] MemtoReg_ID,
    output RegWrite_out_ID,

    output [31:0] zero,
    output [31:0] ra,
    output [31:0] sp,
    output [31:0] gp,
    output [31:0] tp,
    output [31:0] t0,
    output [31:0] t1,
    output [31:0] t2,
    output [31:0] s0,
    output [31:0] s1,
    output [31:0] a0,
    output [31:0] a1,
    output [31:0] a2,
    output [31:0] a3,
    output [31:0] a4,
    output [31:0] a5,
    output [31:0] a6,
    output [31:0] a7,
    output [31:0] s2,
    output [31:0] s3,
    output [31:0] s4,
    output [31:0] s5,
    output [31:0] s6,
    output [31:0] s7,
    output [31:0] s8,
    output [31:0] s9,
    output [31:0] s10,
    output [31:0] s11,
    output [31:0] t3,
    output [31:0] t4,
    output [31:0] t5,
    output [31:0] t6
    );
    
    wire [2:0] ImmSel; //*
    assign Rd_addr_out_ID[4:0] = Inst_in_ID[11:7];
    assign Rs1_addr_ID = Inst_in_ID[19:15];
    assign Rs2_addr_ID = Inst_in_ID[24:20];
    
    regs regs(
    .clk(clk_ID),
    .rst(rst_ID),
    .Rs1_addr(Inst_in_ID[19:15]),
    .Rs2_addr(Inst_in_ID[24:20]),
    .Wt_addr(Rd_addr_ID),
    .Wt_data(Wt_data_ID),
    .RegWrite(RegWrite_in_ID),
    .Rs1_data(Rs1_out_ID),
    .Rs2_data(Rs2_out_ID),
    .zero(zero),
    .ra(ra),
    .sp(sp),
    .gp(gp),
    .tp(tp),
    .t0(t0),
    .t1(t1),
    .t2(t2),
    .s0(s0),
    .s1(s1),
    .a0(a0),
    .a1(a1),
    .a2(a2),
    .a3(a3),
    .a4(a4),
    .a5(a5),
    .a6(a6),
    .a7(a7),
    .s2(s2),
    .s3(s3),
    .s4(s4),
    .s5(s5),
    .s6(s6),
    .s7(s7),
    .s8(s8),
    .s9(s9),
    .s10(s10),
    .s11(s11),
    .t3(t3),
    .t4(t4),
    .t5(t5),
    .t6(t6)
    );

    ImmGen ImmGen(
    .ImmSel(ImmSel),
    .inst_field(Inst_in_ID),
    .Imm_out(Imm_out_ID)
    );

    SCPU_ctrl SCPU_ctrl(
    .OPcode(Inst_in_ID[6:2]),
    .Fun3(Inst_in_ID[14:12]),
    .Fun7(Inst_in_ID[30]),
    .ImmSel(ImmSel),
    .ALUSrc_B(ALUSrc_B_ID),
    .MemtoReg(MemtoReg_ID),
    .Jump(Jump_ID),
    .Branch(Branch_ID),
    .BranchN(BranchN_ID),
    .RegWrite(RegWrite_out_ID),
    .MemRW(MemRW_ID),
    .ALU_Control(ALU_control_ID)
    );

endmodule
