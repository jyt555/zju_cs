`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/04/10 16:28:03
// Design Name: 
// Module Name: SCPU
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


module SCPU(
    input MIO_ready,
    input [31:0] Data_in,
    input clk,
    input [31:0] Inst_in,
    input rst,
    output MemRW,
    output CPU_MIO,
    output [31:0] Addr_out,
    output [31:0] Data_out,
    output [31:0] PC_out
    );
    wire [2:0] ImmSel;
    wire ALUSrc_B;
    wire [1:0] MemtoReg;
    wire [1:0] Jump;
    wire Branch;
    wire BranchN;
    wire RegWrite;
    wire [3:0] ALU_Control;
    
    SCPU_ctrl Controller(
        .OPcode(Inst_in[6:2]),.Fun3(Inst_in[14:12]),.Fun7(Inst_in[30]),.MIO_ready(MIO_ready),
        .ImmSel(ImmSel),.ALUSrc_B(ALUSrc_B),.MemtoReg(MemtoReg),.Jump(Jump),
        .Branch(Branch),.BranchN(BranchN),.RegWrite(RegWrite),.MemRW(MemRW),.ALU_Control(ALU_Control),.CPU_MIO(CPU_MIO)
        );
        
    DataPath DP(
        .ALUSrc_B(ALUSrc_B),.ALU_operation(ALU_Control),.Branch(Branch),.BranchN(BranchN),.Data_in(Data_in),
        .ImmSel(ImmSel),.Jump(Jump),.MemtoReg(MemtoReg),.RegWrite(RegWrite),.clk(clk),
        .inst_field(Inst_in),.rst(rst),.ALU_out(Addr_out),.Data_out(Data_out),.PC_out(PC_out)
        );
endmodule
