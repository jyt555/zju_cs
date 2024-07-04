`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2021/02/21 10:08:22
// Design Name: 
// Module Name: div32
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

module div32(
  input clk,
  input rst,
  input start,
  input [31:0] dividend,
  input [31:0] divisor,
  output finish,
  output [31:0] quotient,
  output [31:0] remainder
);
    reg [63:0] op;
    reg [5:0] counter;
    reg done;
    
    always @(posedge clk or posedge rst) begin
        if (rst) begin
            counter <= 0;
            done <= 0;
            op <= 0;
        end
        if(start) begin
            done <= 0;
            counter <= 0;
            op <= {32'h0,dividend};
        end
        else if(counter < 6'd32) begin
            counter <= counter + 1'b1;
            op = op << 1;
            if(op[63:32] >= divisor) begin
                op[63:32] = op[63:32] - divisor[31:0];
                op[0] <= 1;
            end else
                op[0] <= 0;
        end
        else done <= 1'b1;
    end
    
    assign finish = done;
    assign quotient = op[31:0];
    assign remainder = op[63:32];
    
endmodule

