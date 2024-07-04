`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/05/29 17:25:40
// Design Name: 
// Module Name: Pipeline_IF
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


module Pipeline_IF(
    input clk_IF,
    input rst_IF,
    input en_IF,
    input [31:0] PC_in_IF,
    input PCSrc,
    output [31:0] PC_out_IF
    );
    wire [31:0] mux_out;
    wire [31:0] add_out;
    
    MUX2T1_32 MUX(
    .I0(add_out),
    .I1(PC_in_IF),
    .s(PCSrc),
    .o(mux_out)
    );
    
    REG32 PC(
    .clk(clk_IF),
    .rst(rst_IF),
    .CE(en_IF),
    .D(mux_out),
    .Q(PC_out_IF)
    );
    
    ADD32 ADD(
    .a(32'h00000004),
    .b(PC_out_IF),
    .o(add_out)
    );
endmodule
