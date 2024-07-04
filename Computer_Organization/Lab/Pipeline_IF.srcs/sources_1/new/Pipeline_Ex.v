`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/05/31 13:25:45
// Design Name: 
// Module Name: Pipeline_Ex
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


module Pipeline_Ex(
    input[31:0] PC_in_EX,
    input[31:0] Rs1_in_EX,
    input[31:0] Rs2_in_EX,
    input[31:0] Imm_in_EX,
    input ALUSrc_B_in_EX,
    input[2:0] ALU_control_in_EX,
    output wire [31:0] PC_out_EX,
    output wire [31:0] PC4_out_EX,
    output wire zero_out_EX,
    output wire [31:0] ALU_out_EX,
    output wire [31:0] Rs2_out_EX
    );
    
    wire [31:0]mux_out;

    assign Rs2_out_EX = Rs2_in_EX;

    ADD32 ADD1(
    .a(32'h00000004),
    .b(PC_in_EX),
    .c(PC4_out_EX)
    );

    ADD32 ADD2(
    .a(PC_in_EX),
    .b(Imm_in_EX),
    .c(PC_out_EX)
    );

    MUX2T1_32 MUX(
    .I0(Rs2_in_EX),
    .I1(Imm_in_EX),
    .s(ALUSrc_B_in_EX),
    .o(mux_out)
    );

    ALU ALU(
    .A(Rs1_in_EX),
    .B(mux_out),
    .ALU_operation(ALU_control_in_EX),
    .res(ALU_out_EX),
    .zero(zero_out_EX)
    );
endmodule