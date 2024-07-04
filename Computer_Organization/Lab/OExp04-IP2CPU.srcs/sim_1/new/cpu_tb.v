`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/04/30 21:10:58
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
    soc_test_wrapper u(
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
