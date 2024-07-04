`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/06/01 14:28:32
// Design Name: 
// Module Name: ID_reg_Ex
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


module ID_reg_Ex( 
        input clk_IDEX,
        input rst_IDEX,
        input en_IDEX,

		input NOP_IDEX,
		input valid_in_IDEX,
		input [31:0] inst_in_IDEX,

        input [31:0] PC_in_IDEX,
        input [4:0] Rd_addr_IDEX,
        input [31:0] Rs1_in_IDEX,
        input [31:0] Rs2_in_IDEX, 
        input [31:0] Imm_in_IDEX , 
        input ALUSrc_B_in_IDEX , 
        input [2:0] ALU_control_in_IDEX, 
        input Branch_in_IDEX, 
        input BranchN_in_IDEX, 
        input MemRW_in_IDEX, 
        input Jump_in_IDEX, //*
        input [1:0] MemtoReg_in_IDEX, 
        input RegWrite_in_IDEX, 

		output reg [31:0] inst_out_IDEX,	
		output reg valid_out_IDEX,			

		output reg [31:0] PC_out_IDEX,
        output reg [4:0] Rd_addr_out_IDEX,
        output reg [31:0] Rs1_out_IDEX,
        output reg [31:0] Rs2_out_IDEX, 
        output reg [31:0] Imm_out_IDEX , 
        output reg ALUSrc_B_out_IDEX , 
        output reg [2:0] ALU_control_out_IDEX, 
        output reg Branch_out_IDEX, 
        output reg BranchN_out_IDEX, 
        output reg MemRW_out_IDEX, 
        output reg Jump_out_IDEX, //*
        output reg [1:0] MemtoReg_out_IDEX, 
        output reg RegWrite_out_IDEX 
    ); 
	
    always @(posedge clk_IDEX or posedge rst_IDEX) begin
		if (rst_IDEX == 1'b1)  begin 
			PC_out_IDEX <= 32'b0;	
			Rs1_out_IDEX <= 32'b0;	
			Rs2_out_IDEX <= 32'b0;	
			Imm_out_IDEX <= 32'b0;	
			Rd_addr_out_IDEX <= 5'b0;		
			ALUSrc_B_out_IDEX <= 1'b0;		
			ALU_control_out_IDEX <= 3'b0;	
			Branch_out_IDEX <= 1'b0;		
			BranchN_out_IDEX <= 1'b0;		
			MemRW_out_IDEX <= 1'b0;		
			Jump_out_IDEX <= 1'b0;			
			MemtoReg_out_IDEX <= 2'b0;		
			RegWrite_out_IDEX <= 1'b0;		
			inst_out_IDEX <= 32'b0;			
			valid_out_IDEX <= 1'b0;		
		end
		else if (en_IDEX) begin
			if(NOP_IDEX) begin
				PC_out_IDEX <= 32'b0;		
				Rs1_out_IDEX <= 32'b0;	
				Rs2_out_IDEX <= 32'b0;	
				Imm_out_IDEX <= 32'b0;	
				Rd_addr_out_IDEX <= 5'b0;		
				ALUSrc_B_out_IDEX <= 1'b0;		
				ALU_control_out_IDEX <= 3'b0;
				Branch_out_IDEX <= 1'b0;			
				BranchN_out_IDEX <= 1'b0;		
				MemRW_out_IDEX <= 1'b0;						
				Jump_out_IDEX <= 1'b0;				
				MemtoReg_out_IDEX <= 2'b0;		
				RegWrite_out_IDEX <= 1'b0;					
				inst_out_IDEX <= 32'h00000013;			
				valid_out_IDEX <= 1'b0;		
			end else begin
				PC_out_IDEX <= PC_in_IDEX;	
				Rs1_out_IDEX <= Rs1_in_IDEX;
				Rs2_out_IDEX <= Rs2_in_IDEX;
				Imm_out_IDEX <= Imm_in_IDEX;
				Rd_addr_out_IDEX <= Rd_addr_IDEX;			
				ALUSrc_B_out_IDEX <= ALUSrc_B_in_IDEX;		
				ALU_control_out_IDEX <= ALU_control_in_IDEX;
				Branch_out_IDEX <= Branch_in_IDEX;			
				BranchN_out_IDEX <= BranchN_in_IDEX;		
				MemRW_out_IDEX <= MemRW_in_IDEX;			
				Jump_out_IDEX <= Jump_in_IDEX;				
				MemtoReg_out_IDEX <= MemtoReg_in_IDEX;		
				RegWrite_out_IDEX <= RegWrite_in_IDEX;		
				inst_out_IDEX <= inst_in_IDEX;				
				valid_out_IDEX <= valid_in_IDEX;			
			end
		end
	end

endmodule