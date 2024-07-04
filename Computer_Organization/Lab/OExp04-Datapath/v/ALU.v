`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/03/06 16:25:50
// Design Name: 
// Module Name: ALU
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


module ALU(
    input [31:0] A,
    input [2:0] ALU_operation,
    input [31:0] B,
    output reg [31:0] res,
    output zero
    );
    wire [31:0] res_and, res_or, res_add, res_sub, res_nor, res_slt;
    parameter one = 32'h00000001, zero_0 = 32'h00000000;
    
    assign res_and = A & B;
    assign res_or = A | B;
    assign res_add = A + B;
    assign res_sub = A - B;
    assign res_slt = (A < B) ? one : zero_0;

    always @(A or B or ALU_operation)
        case(ALU_operation)
            3'b000: res = res_and;
            3'b001: res = res_or;
            3'b010: res = res_add;
            3'b011: res = A ^ B;
            3'b100: res = ~(A | B);
            3'b101: res = B >> 1;
            3'b110: res = res_sub;
            3'b111: res = res_slt;
            default: res = 32'hx;
        endcase
        assign zero = (res == 0) ? 1:0;
endmodule
