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
    output reg [1:0]Jump,
    output reg Branch,
    output reg BranchN,
    output reg RegWrite,
    output reg MemRW,
    output reg [3:0]ALU_Control,
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
        5'b01100:begin {ALUSrc_B,MemtoReg,RegWrite,MemRW,Jump,ALUop} = 9'b000100010; end    // R
        5'b01000:begin {ALUSrc_B,MemtoReg,RegWrite,MemRW,Jump,ALUop} = 9'b100010000; end    // sw
        5'b11000:begin {ALUSrc_B,MemtoReg,RegWrite,MemRW,Jump,ALUop} = 9'b000000001; end    // beq,bne
        5'b01101:begin {ALUSrc_B,MemtoReg,RegWrite,MemRW,Jump,ALUop} = 9'b011100000; end    // lui
        5'b00101:begin {ALUSrc_B,MemtoReg,RegWrite,MemRW,Jump,ALUop} = 9'b011100000; end    // U
        5'b00000:begin {ALUSrc_B,MemtoReg,RegWrite,MemRW,Jump,ALUop} = 9'b101100000; end    // lw - data_in from memory
        5'b11001:begin {ALUSrc_B,MemtoReg,RegWrite,MemRW,Jump,ALUop} = 9'b110101000; end    // jalr - store next PC
        5'b11011:begin {ALUSrc_B,MemtoReg,RegWrite,MemRW,Jump,ALUop} = 9'b110100100; end    // jal
        5'b00100:begin {ALUSrc_B,MemtoReg,RegWrite,MemRW,Jump,ALUop} = 9'b100100011; end    // I
        default: begin {ALUSrc_B,MemtoReg,RegWrite,MemRW,Jump,ALUop} = 9'b100100011; end
        endcase
        Branch = 1'b0;
        BranchN = 1'b0;
        if(OPcode==5'b11000 && Fun3==3'b000) Branch =  1'b1;
        if(OPcode==5'b11000 && Fun3==3'b001) BranchN = 1'b1;
    end
    
    always @* begin
        case(ALUop)
        2'b00: ALU_Control = 4'b0010 ;  // add
        2'b01: ALU_Control = 4'b0110 ;  // sub
        2'b10:  // R
            case({Fun3,Fun7})
            4'b0000: ALU_Control = 4'b0010 ;    // add
            4'b0001: ALU_Control = 4'b0110 ;    // sub
            4'b0010: ALU_Control = 4'b1110 ;    // sll
            4'b0100: ALU_Control = 4'b0111 ;    // slt
            4'b0110: ALU_Control = 4'b1001 ;    // sltu
            4'b1000: ALU_Control = 4'b1100 ;    // xor
            4'b1010: ALU_Control = 4'b1101 ;    // srl
            4'b1011: ALU_Control = 4'b1111 ;    // sra
            4'b1100: ALU_Control = 4'b0001 ;    // or
            4'b1110: ALU_Control = 4'b0000 ;    // and
            default: ALU_Control = 4'b0000 ;
            endcase
        2'b11:  // I
            case(Fun3)
            3'b000: ALU_Control = 4'b0010 ;
            3'b010: ALU_Control = 4'b0111 ;
            3'b011: ALU_Control = 4'b1001 ;
            3'b100: ALU_Control = 4'b1100 ;
            3'b110: ALU_Control = 4'b0001 ;
            3'b111: ALU_Control = 4'b0000 ;
            3'b001: ALU_Control = 4'b1110 ;
            3'b101: 
                case(Fun7)
                1'b0: ALU_Control = 4'b1101 ;
                1'b1: ALU_Control = 4'b1111 ;
                endcase
            default:ALU_Control = 4'b1110 ;
            endcase
        endcase
        CPU_MIO = 0;
    end
endmodule