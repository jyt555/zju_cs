`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/05/25 01:47:31
// Design Name: 
// Module Name: wrapper
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


module wrapper(
    input clk,
    input rst
    );
    wire [31:0] Data_in;
    wire [31:0] Inst_IF;
    wire [31:0] PC_out_ID;
    wire [31:0] PC_out_EX;
    wire [31:0] Inst_ID;
    wire [31:0] PC_out_IF;
    wire [31:0] Addr_out;
    wire [31:0] Data_out;
    wire [31:0] Data_out_WB;
    wire MemRW_Mem;
    wire MemRW_EX;
    ROM ROM(
    .a(PC_out_IF[11:2]),
    .spo(Inst_IF)
    );
    
    RAM RAM(
    .clka(~clk),
    .wea(MemRW_Mem),
    .addra(Addr_out),
    .dina(Data_out),
    .douta(Data_in)
    );
    
    Pipeline_CPU CPU(
    .clk(clk),
    .rst(rst),
    .Data_in(Data_in),
    .Inst_IF(Inst_IF),
    .PC_out_EX(PC_out_EX),
    .PC_out_ID(PC_out_ID),
    .Inst_ID(Inst_ID),
    .PC_out_IF(PC_out_IF),
    .Addr_out(Addr_out),
    .Data_out(Data_out),
    .Data_out_WB(Data_out_WB),
    .MemRW_Mem(MemRW_Mem),
    .MemRW_EX(MemRW_EX)
    );
endmodule
