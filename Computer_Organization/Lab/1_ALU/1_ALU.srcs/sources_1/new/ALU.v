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
    input [3:0] ALU_operation,
    input [31:0] B,
    output reg [31:0] res,
    output zero
    );
    wire [31:0] res_and, res_or, res_add, res_sub, res_xor;
    wire [31:0] res_sll, res_slt, res_sltu, res_srl, res_sra;
    parameter one = 32'h00000001, zero_0 = 32'h00000000;
    
    assign res_and = A & B;
    assign res_or = A | B;
    assign res_add = A + B;
    assign res_sub = A - B;
    assign res_xor = A ^ B;
    assign res_sll = A << B[4:0];
    assign res_srl = A >> B[4:0];
    assign res_sra = $signed(A) >>> B[4:0];
    assign res_slt = ($signed(A) < $signed(B)) ? one : zero_0;
    assign res_sltu = (A < B) ? one : zero_0;

    always @(A or B or ALU_operation)
        case(ALU_operation)
            3'b0010: res = res_add;
            3'b0110: res = res_sub;
            3'b1110: res = res_sll;
            3'b0111: res = res_slt;
            3'b1001: res = res_sltu;
            3'b1100: res = res_xor;
            3'b1101: res = res_srl;
            3'b1111: res = res_sra;
            3'b0000: res = res_and;
            3'b0001: res = res_or;
            default: res = 32'hx;
        endcase
        assign zero = (res == 0) ? 1:0;
endmodule
