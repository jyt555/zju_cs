`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/05/25 02:10:14
// Design Name: 
// Module Name: cpu_tb
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

module cpu_tb();
    reg clk;
    reg rst;
    wrapper u(
        .clk(clk),.rst(rst)
    );
    always #5 clk = ~clk;
    initial begin
        clk = 0;
        rst = 1;
        #4;
        rst = 0;
    end
endmodule
