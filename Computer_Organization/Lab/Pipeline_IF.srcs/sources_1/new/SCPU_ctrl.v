`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/04/24 16:04:42
// Design Name: 
// Module Name: SCPU_ctrl
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

module SCPU_ctrl(
    input[4:0]OPcode,
    input[2:0]Fun3,
    input Fun7,
    input MIO_ready,
    output reg [2:0]ImmSel,
    output reg ALUSrc_B,
    output reg [1:0]MemtoReg,
    output reg Jump, //*
    output reg Branch,
    output reg BranchN,
    output reg RegWrite,
    output reg MemRW,
    output reg [2:0]ALU_Control, //*
    output reg CPU_MIO
    );
    
    reg [1:0]ALUop;
    
    always@* begin
        case(OPcode)
        5'b00000:begin ImmSel<=3'b001; end  // I-lw
        5'b00100:begin ImmSel<=3'b001; end  // I-addi,slti...
        5'b11001:begin ImmSel<=3'b001; end  // I-jalr
        5'b01000:begin ImmSel<=3'b010; end  // S-sw
        5'b11000:begin ImmSel<=3'b011; end  // SB-beq,bne
        5'b11011:begin ImmSel<=3'b100; end  // J-jal
        5'b00101:begin ImmSel<=3'b000; end  // U
        5'b01101:begin ImmSel<=3'b000; end  // U-lui
        default: begin ImmSel<=3'b000; end
        endcase
    end
    
    always@* begin
        case(OPcode)
        5'b01100:begin {ALUSrc_B,MemtoReg,RegWrite,MemRW,Jump,ALUop} = 9'b00010010; end    // R
        5'b01000:begin {ALUSrc_B,MemtoReg,RegWrite,MemRW,Jump,ALUop} = 9'b10001000; end    // sw
        5'b11000:begin {ALUSrc_B,MemtoReg,RegWrite,MemRW,Jump,ALUop} = 9'b00000001; end    // beq,bne
        5'b01101:begin {ALUSrc_B,MemtoReg,RegWrite,MemRW,Jump,ALUop} = 9'b01110000; end    // lui
        5'b00101:begin {ALUSrc_B,MemtoReg,RegWrite,MemRW,Jump,ALUop} = 9'b01110000; end    // U
        5'b00000:begin {ALUSrc_B,MemtoReg,RegWrite,MemRW,Jump,ALUop} = 9'b10110000; end    // lw - data_in from memory
        5'b11001:begin {ALUSrc_B,MemtoReg,RegWrite,MemRW,Jump,ALUop} = 9'b11010100; end    // jalr - store next PC
        5'b11011:begin {ALUSrc_B,MemtoReg,RegWrite,MemRW,Jump,ALUop} = 9'b11010100; end    // jal
        5'b00100:begin {ALUSrc_B,MemtoReg,RegWrite,MemRW,Jump,ALUop} = 9'b10010011; end    // I
        default: begin {ALUSrc_B,MemtoReg,RegWrite,MemRW,Jump,ALUop} = 9'b10010011; end
        endcase
        Branch = 1'b0;
        BranchN = 1'b0;
        if(OPcode==5'b11000 && Fun3==3'b000) Branch =  1'b1;
        if(OPcode==5'b11000 && Fun3==3'b001) BranchN = 1'b1;
    end
    
    always @* begin
        case(ALUop)
        2'b00: ALU_Control = 3'b010 ;  // add
        2'b01: ALU_Control = 3'b110 ;  // sub
        2'b10:  // R
            case({Fun3,Fun7})
            4'b0000: ALU_Control = 3'b010 ;    // add
            4'b0001: ALU_Control = 3'b110 ;    // sub
            4'b0100: ALU_Control = 3'b111 ;    // slt
            4'b1000: ALU_Control = 3'b011 ;    // xor
            4'b1010: ALU_Control = 3'b101 ;    // srl
            4'b1100: ALU_Control = 3'b001 ;    // or
            4'b1110: ALU_Control = 3'b000 ;    // and
            default: ALU_Control = 3'b000 ;
            endcase
        2'b11:  // I
            case(Fun3)
            3'b000: ALU_Control = 3'b010 ;
            3'b010: ALU_Control = 3'b111 ;
            3'b100: ALU_Control = 3'b011 ;
            3'b110: ALU_Control = 3'b001 ;
            3'b111: ALU_Control = 3'b000 ;
            3'b101: ALU_Control = 3'b101 ;
            default:ALU_Control = 3'b010 ;
            endcase
        endcase
        CPU_MIO = 0;
    end
endmodule