`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/04/30 20:58:09
// Design Name: 
// Module Name: tb
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


module soc_test_wrapper(
    input rst,
    input clk
    );
    wire [31:0] PC_out;
    wire [31:0] Inst_in;
    wire MemRW;
    wire [31:0] Addr_out;
    wire [31:0] Data_in;
    wire [31:0] Data_out;
    wire CPU_MIO;
    dist_mem_gen_0 ROM(
        .a(PC_out[11:2]),.spo(Inst_in)
        );
        
    RAM_B memory(
        .clka(~clk),.wea(MemRW),.addra(Addr_out[11:2]),.dina(Data_out),.douta(Data_in)
        );
        
    SCPU CPU(
        .Data_in(Data_in),.MIO_ready(CPU_MIO),.clk(clk),
        .Inst_in(Inst_in),.rst(rst),
        .Addr_out(Addr_out),.CPU_MIO(CPU_MIO),.Data_out(Data_out),
        .MemRW(MemRW),.PC_out(PC_out)
        );
endmodule
