`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/04/10 17:21:04
// Design Name: 
// Module Name: DataPath
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

module DataPath(
    input clk,
    input rst,
    input [31:0] inst_field,
    input [31:0] Data_in,
    input [3:0] ALU_operation,
    input [2:0] ImmSel,
    input [1:0] MemtoReg,
    input ALUSrc_B,
    input [1:0] Jump,
    input Branch,
    input BranchN,
    input RegWrite,
    output [31:0] PC_out,
    output [31:0] Data_out,
    output [31:0] ALU_out
    );
    wire [31:0] PC4;
    wire [31:0] NextPC;
    wire [31:0] PCi;
    wire [31:0] imm_out;
    wire [31:0] mux0_out;
    wire [31:0] Wt_data;
    wire [31:0] PC_in;
    wire [31:0] ALU_in;
    wire ALU_zero;
    wire [31:0] Rs1_data;
    assign PC4 = 32'd4;

    ImmGen ImmGen(
        .ImmSel(ImmSel),.inst_field(inst_field),
        .Imm_out(imm_out));

    ADD32 PC4_add(
        .A(PC_out),.B(PC4),.o(NextPC));

    ADD32 PCi_add(
        .A(PC_out),.B(imm_out),.o(PCi));

    MUX2T1_32 PC_MUX0(
        .I0(NextPC),.I1(PCi),
        .s((Branch & ALU_zero) | (BranchN & ~ALU_zero)),
        .o(mux0_out));
    
    MUX4T1 PC_MUX1(
        .I0(mux0_out),.I1(PCi),.I2(ALU_out),.I3(mux0_out),
        .s(Jump),.o(PC_in));
        
    MUX4T1 REG_MUX(
            .I0(ALU_out),.I1(Data_in),.I2(NextPC),.I3(imm_out),
            .s(MemtoReg),.o(Wt_data));

    MUX2T1_32 ALU_MUX(
        .I0(Data_out),.I1(imm_out),.s(ALUSrc_B),.o(ALU_in));
    
    regs Regs(
        .clk(clk),.rst(rst),
        .Rs1_addr(inst_field[19:15]),.Rs2_addr(inst_field[24:20]),
        .Wt_addr(inst_field[11:7]),.Wt_data(Wt_data),
        .RegWrite(RegWrite),
        .Rs1_data(Rs1_data),.Rs2_data(Data_out));

    ALU ALU(
        .A(Rs1_data),.B(ALU_in),.ALU_operation(ALU_operation),
        .res(ALU_out),.zero(ALU_zero));

    REG32 PC(
        .clk(clk),.rst(rst),
        .CE(1'b1),.D(PC_in),.Q(PC_out));
endmodule
