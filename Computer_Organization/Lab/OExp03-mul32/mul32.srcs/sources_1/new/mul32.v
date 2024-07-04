`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2021/01/25 14:03:11
// Design Name: 
// Module Name: mul32
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


module mul32(
  input clk,
  input rst,
  input [31:0] multiplicand,
  input [31:0] multiplier,
  input start,
  output reg [63:0] product,
  output reg finish
);

reg [5:0] counter;
reg [63:0] op1;
reg [63:0] op2;

always @(posedge clk or posedge rst) begin
    if (start) begin
        finish <= 0;
        product <= 0;
        counter <= 0;
        op1 <= {32'b0, multiplicand};
        op2 <= {32'b0, multiplier};
    end
    else if (counter < 32) begin
        if(op2[0] == 1) 
            product <= product + op1; 
        op1 <= {op1[62:0],1'b0};
        op2 <= {1'b0, op2[63:1]};
        counter <= counter + 1;
    end
    else begin
        finish <= 1;
    end
end

endmodule
