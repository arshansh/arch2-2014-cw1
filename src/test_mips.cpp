

#include "mips_test.h"
#include<iostream>

using namespace std;

//uint32_t bitswanted(int num_bits,int index_bit,uint32_t instruction);


uint32_t bytechange(uint32_t instruction);

//uint32_t signextend(uint32_t immediate,int sign_bit);

int main()
{


    mips_mem_h mem=mips_mem_create_ram(1<<20,4); //creating a memory of 2^20 bytes,each word 4 bytes
    mips_cpu_h cpu=mips_cpu_create(mem);		//creating a cpu

    mips_test_begin_suite();
    

    int testId=mips_test_begin_test("and");
    int passed=0;
    uint32_t got;	//used to check the correct result
	uint32_t pc;	//this is used to check branches
    mips_error err = mips_cpu_set_register(cpu, 8, 0x0000FFFFul);               //unsigned long

    if(err==0)
        err = mips_cpu_set_register(cpu, 9, 0x00FFFF00ul);

    // and $10, $8, $9
    uint32_t instruction=0x1095024;
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,8,4,(uint8_t*)&instruction);

    // TODO : work out the bit-wise encoding for the instruction.
    
    // TODO : Write it into memory at a known address
    
    // TODO : Make sure the program-counter is at that address
    if(!err){
    	err=mips_cpu_set_pc(cpu,8);

    }
    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));

    passed = (err == mips_Success) && (got==0x0000FF00);

    mips_test_end_test(testId, passed, NULL);
    






    testId=mips_test_begin_test("OR");
    passed=0;


    err = mips_cpu_set_register(cpu, 8, 0x0000FFFFul);               //unsigned long

    if(err==0)
        err = mips_cpu_set_register(cpu, 9, 0x00FFFF00ul);

    // OR $10, $8, $9
    instruction=0x1095025;
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,12,4,(uint8_t*)&instruction);	//pc is now at 12

    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));

    passed = (err == mips_Success) && (got==0x00FFFFFF);

    mips_test_end_test(testId, passed, NULL);





    testId=mips_test_begin_test("ORI");
    passed=0;


    err = mips_cpu_set_register(cpu, 8, 0x0000FFFFul);               //unsigned long


    // OR immediate  $10, $8,15
    instruction=0x350A000F;
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,16,4,(uint8_t*)&instruction);	//pc is now at 16

    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));

    passed = (err == mips_Success) && (got==0x0000FFFF);

    mips_test_end_test(testId, passed, NULL);



    testId=mips_test_begin_test("ANDI");
    passed=0;


    err = mips_cpu_set_register(cpu, 8, 0xF000FFFFul);               //unsigned long


    // AND immediate  $10, $8, 0xFFFF
    instruction=0x310AFFFF;			//immediate value is FFFF so that we can test zero extended
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,20,4,(uint8_t*)&instruction);	//pc is now at 20

    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));

    passed = (err == mips_Success) && (got==0x0000FFFF);

    mips_test_end_test(testId, passed, NULL);




    testId=mips_test_begin_test("XORI");
    passed=0;


    err = mips_cpu_set_register(cpu, 8, 0xF000FFFFul);               //unsigned long


    // XOR immediate  $10, $8, 0xFFFF
    instruction=0x390AFFFF;			//immediate value is FFFF so that we can test zero extended
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,24,4,(uint8_t*)&instruction);	//pc is now at 24


    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));

    passed = (err == mips_Success) && (got==0xF0000000);

    mips_test_end_test(testId, passed, NULL);






    testId=mips_test_begin_test("XOR");
    passed=0;


    err = mips_cpu_set_register(cpu, 8, 0x0000FFFFul);               //unsigned long

    if(err==0)
        err = mips_cpu_set_register(cpu, 9, 0x00FFFF00ul);

    // XOR $10, $8, $9
    instruction=0x1095026;
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,28,4,(uint8_t*)&instruction);	//pc is now at 28

    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));

    passed = (err == mips_Success) && (got==0x00FF00FF);

    mips_test_end_test(testId, passed, NULL);


    testId=mips_test_begin_test("ADDU");
    passed=0;


    err = mips_cpu_set_register(cpu, 8, 0x00000001ul);               //unsigned long

    if(err==0)
        err = mips_cpu_set_register(cpu, 9, 0xFFFFFFFFul);	//overflow is not detected(result truncated to 32 bits)

    // ADDU $10, $8, $9
    instruction=0x1095021;
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,32,4,(uint8_t*)&instruction);	//pc is now at 32

    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));

    passed = (err == mips_Success) && (got==0x0000000);

    mips_test_end_test(testId, passed, NULL);


    testId=mips_test_begin_test("SUBU");
    passed=0;


    err = mips_cpu_set_register(cpu, 8, 0x0000FFFFul);               //unsigned long

    if(err==0)
        err = mips_cpu_set_register(cpu, 9, 0xFFFFFFFFul);	//overflow is not detected(result truncated to 32 bits)

    // SUBU $10, $8, $9
    instruction=0x1095023;
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,36,4,(uint8_t*)&instruction);	//pc is now at 36

    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));

    passed = (err == mips_Success) && (got==0x00010000);

    mips_test_end_test(testId, passed, NULL);




    testId=mips_test_begin_test("SLTU");
    passed=0;


    err = mips_cpu_set_register(cpu, 8, 0xFFFFFFFFul);               //unsigned long

    if(err==0)
        err = mips_cpu_set_register(cpu, 9, 0x00000001ul);	//checking unsinged numbers

    // SLTU $10, $8, $9
    instruction=0x109502B;
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,40,4,(uint8_t*)&instruction);	//pc is now at 40

    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));

    passed = (err == mips_Success) && (got==0x00000000);	//unsinged 0xFFFFFFFF is larger than 1

    mips_test_end_test(testId, passed, NULL);






    testId=mips_test_begin_test("ADD");
    passed=0;


    err = mips_cpu_set_register(cpu, 8,0xFFFFFFFF);               //unsigned long

    if(err==0)
        err = mips_cpu_set_register(cpu, 9,0xFFFFFFFF);      //2 value to detect singed overflow

    // ADD $10, $8, $9
    instruction=0x01095020;
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,44,4,(uint8_t*)&instruction);    //pc is now at 44

    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)

        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));

    passed = (err ==mips_Success ) && (got==0xFFFFFFFE) ;    //we do not have signed overflow,unsinged overflow is available but no detected

    mips_test_end_test(testId, passed, NULL);



    testId=mips_test_begin_test("SUB");
    passed=0;


    err = mips_cpu_set_register(cpu, 8,0x80000000);               //unsigned long

    if(err==0)
        err = mips_cpu_set_register(cpu, 9,0x00000001);      //2 value to detect singed overflow

    // SUB $10, $8, $9
    instruction=0x1095022;
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,48,4,(uint8_t*)&instruction);    //pc is now at 48

    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));

    passed = (err == mips_ExceptionArithmeticOverflow)  ;    //we have signed overflow(notice nothing is written in register 10)
    														//substracting the most negative number by 1 results in signed overflow
    mips_test_end_test(testId, passed, NULL);



    testId=mips_test_begin_test("ADDIU");
    passed=0;


    err = mips_cpu_set_register(cpu, 8, 0xFFFFFFFFul);               //unsigned long


   // ADDIU immediate  $10, $8, 0xFFFF
    instruction=0x250AFFFF;			//immediate value is FFFF so that we can test sign extended
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,52,4,(uint8_t*)&instruction);	//pc is now at 52




    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));

    passed = (err == mips_Success) && (got==0xFFFFFFFE);

    mips_test_end_test(testId, passed, NULL);




    testId=mips_test_begin_test("ADDI");
    passed=0;


    err = mips_cpu_set_register(cpu, 8, 0x80000000ul);               //unsigned long


   // ADDI immediate  $10, $8, 0xFFFF
    instruction=0x210AFFFF;			//immediate value is FFFF so that we can test sign extended
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,56,4,(uint8_t*)&instruction);	//pc is now at 56


    //adding 0x800000000 + FFFFFFFF results in signed overflow (adding 2 negative but result positive)

    if(err==0)
        err=mips_cpu_step(cpu);




    passed = (err == mips_ExceptionArithmeticOverflow) ;	//correctly detected

    mips_test_end_test(testId, passed, NULL);



    testId=mips_test_begin_test("LUI");
    passed=0;


    err = mips_cpu_set_register(cpu, 8, 0x80000000ul);               //unsigned long


   //LUI  $10,  0xFFFF
    instruction=0x3C0A8888;			//immediate value is 8888
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,60,4,(uint8_t*)&instruction);	//pc is now at 60


    //

    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));

    passed = (err == mips_Success) &&  (got==0x88880000);	//correctly detected

    mips_test_end_test(testId, passed, NULL);




    testId=mips_test_begin_test("LW");
    passed=0;

    err = mips_cpu_set_register(cpu, 4, 0x00000004ul);

    uint32_t value=0x12345678;
    value=bytechange(value);
    if(err==0)
    	err=mips_mem_write(mem,260,4,(uint8_t*)&value);

   //Lw  $10,$4(256)
    instruction=0x8C8A0100;			//loading content of register 4(4) +256(offset) to register 10
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,64,4,(uint8_t*)&instruction);	//pc is now at 64


    //

    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));

    passed = (err == mips_Success) &&  (got==0x12345678); //correctly detected

    mips_test_end_test(testId, passed, NULL);




    testId=mips_test_begin_test("SLL");
    passed=0;

    err = mips_cpu_set_register(cpu, 8, 0x00000001ul);



   //SLL  $10,$8,0xFF					logical shift left by 31
    instruction=0x000857C0;
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,68,4,(uint8_t*)&instruction);	//pc is now at 68



    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));

    passed = (err == mips_Success) &&  (got==0x80000000); //correctly detected

    mips_test_end_test(testId, passed, NULL);



    testId=mips_test_begin_test("SLLV");
    passed=0;

    err = mips_cpu_set_register(cpu, 8, 0x00000001ul);

    if(err==0)
    	err = mips_cpu_set_register(cpu, 9, 0x0000FFFFul);//FFFF is used so that we check the right most bits are selected


   //SLLV  $10,$8,$9
    instruction=0x01285004;
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,72,4,(uint8_t*)&instruction);	//pc is now at 72




    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));

    passed = (err == mips_Success) &&  (got==0x80000000); //correctly detected

    mips_test_end_test(testId, passed, NULL);





    testId=mips_test_begin_test("SLT");
    passed=0;

    err = mips_cpu_set_register(cpu, 8, 0x00000001ul);

    if(err==0)
    	err = mips_cpu_set_register(cpu, 9, 0xFFFFFFFF);		//-1 is used for checking sign


   //SLT  $10,$8,$9
    instruction=0x109502A;
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,76,4,(uint8_t*)&instruction);	//pc is now at 76


    //

    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));

    passed = (err == mips_Success) &&  (got==0x00000000); //correctly detected

    mips_test_end_test(testId, passed, NULL);


    testId=mips_test_begin_test("SLTI");
    passed=0;

    err = mips_cpu_set_register(cpu, 8, 0x0000000F);	//+ is checked to check the sign


   //SLTI  $10,$8,0xFFFE
    instruction=0x290AFFFE;			//the immediate value is -2 (0xFFFE)
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,80,4,(uint8_t*)&instruction);	//pc is now at 80


    //

    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));

    passed = (err == mips_Success) &&  (got==0x00000000); //correctly detected

    mips_test_end_test(testId, passed, NULL);



    testId=mips_test_begin_test("SLTIU");
    passed=0;

    err = mips_cpu_set_register(cpu, 8, 0x0FFFFFFF);	// is checked to check the unsigned number


   //SLTIU  $10,$8,0xFFFE
    instruction=0x2D0AFFFE;			//the immediate value is 0xFFFE (0xFFFE)unsinged
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,84,4,(uint8_t*)&instruction);	//pc is now at 84


    //

    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));

    passed = (err == mips_Success) &&  (got==0x00000001); //correctly detected

    mips_test_end_test(testId, passed, NULL);




    testId=mips_test_begin_test("SRL");
    passed=0;

    err = mips_cpu_set_register(cpu, 8, 0x80000000ul);




   //SRL  $10,$8,31
    instruction=0x000857C2;
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,88,4,(uint8_t*)&instruction);	//pc is now at 88




    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));

    passed = (err == mips_Success) &&  (got==0x00000001); //correctly detected

    mips_test_end_test(testId, passed, NULL);



    testId=mips_test_begin_test("SRLV");
    passed=0;

    err = mips_cpu_set_register(cpu, 8, 0xFFFFFFFFul);	//used to check the left most 5 bits
    if(err==0)
        err = mips_cpu_set_register(cpu, 9, 0x80000000ul);



   //SRLV  $10,$8,$9
    instruction=0x01095006;
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,92,4,(uint8_t*)&instruction);	//pc is now at 92




    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));

    passed = (err == mips_Success) &&  (got==0x00000001); //correctly detected

    mips_test_end_test(testId, passed, NULL);



    testId=mips_test_begin_test("SRA");
    passed=0;

    err = mips_cpu_set_register(cpu, 8, 0x80000000ul);	//to check sign extension after shifting




   //SRA  $10,$8,31		//shifting right by 31
    instruction=0x000857C3;
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,96,4,(uint8_t*)&instruction);	//pc is now at 96




    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));

    passed = (err == mips_Success) &&  (got==0xFFFFFFFF); //correctly detected

    mips_test_end_test(testId, passed, NULL);





    testId=mips_test_begin_test("SW");
    passed=0;

    err = mips_cpu_set_register(cpu, 4, 0x00000004ul);
    if(err==0)
        err = mips_cpu_set_register(cpu, 10, 0x12345678);



   //SW  $4(256),$10
    instruction=0xAC8A0080;			//loading content of register 10(0x12345678) to  adress 256(offset)+$4(4)
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,100,4,(uint8_t*)&instruction);	//pc is now at 100


    //

    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)

        err = (mips_error)(err | mips_mem_read(mem,260,4,(uint8_t*)&value));	//reading defined memory location
        value=bytechange(value);
    passed = (err == mips_Success) &&  (value==0x12345678); //correctly detected

    mips_test_end_test(testId, passed, NULL);



    testId=mips_test_begin_test("BEQ");
    passed=0;


    err = mips_cpu_set_register(cpu, 8, 0xFFFFFFFFul);               //unsigned long
    if(err==0)
        err = mips_cpu_set_register(cpu, 10, 0xFFFFFFFFul);               //unsigned long

    // BEQ  $10, $8,98(104+(-4))     	//note -4 is 0xFFFF<<2 immediate
    instruction=0x110AFFFF;			//immediate value is FFFF
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,104,4,(uint8_t*)&instruction);	//pc is now at 104

    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)

        err = (mips_error)(err | mips_cpu_get_pc(cpu,&pc));

    passed = (err == mips_Success) && (pc==108);	//pc is at 108 but will go to offset in the nest instruction

    mips_test_end_test(testId, passed, NULL);



    testId=mips_test_begin_test("BEQ");	//this function just checks the PC following the branch(where branch happens)
    passed=0;


    err = mips_cpu_set_register(cpu, 8,0xFFFFFFFF);               //unsigned long

    if(err==0)
        err = mips_cpu_set_register(cpu, 9,0xFFFFFFFF);      //2 value to detect singed overflow

    // ADD $10, $8, $9
    instruction=0x01095020;
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,108,4,(uint8_t*)&instruction);    //pc is now at 108

    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)

        err = (mips_error)(err |  mips_cpu_get_pc(cpu,&pc));

    passed = (err ==mips_Success ) && (pc==104) ;



    mips_test_end_test(testId, passed, NULL);

    testId=mips_test_begin_test("BGEZ");
    passed=0;
    err=mips_cpu_set_pc(cpu,108);	//setting back the pc to 108
    if(err==0)
    	err = mips_cpu_set_register(cpu, 8, 0x00000000ul);               //unsigned long checking zero


    // BGEZ  $10, $8,98(112+(2048*4))     	//note -4 is 0xFFFF<<2 immediate
    instruction=0x5010800;			//immediate value is FFFF
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,108,4,(uint8_t*)&instruction);	//pc is now at 108

    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)

        err = (mips_error)(err | mips_cpu_get_pc(cpu,&pc));

    passed = (err == mips_Success) && (pc==112);

    mips_test_end_test(testId, passed, NULL);


    testId=mips_test_begin_test("BGEZ");	//this function just checks the PC following the branch(where branch happens)
    passed=0;


    err = mips_cpu_set_register(cpu, 8,0xFFFFFFFF);               //unsigned long

    if(err==0)
        err = mips_cpu_set_register(cpu, 9,0xFFFFFFFF);      //2 value to detect singed overflow

    // ADD $10, $8, $9
    instruction=0x01095020;
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,112,4,(uint8_t*)&instruction);    //pc is now at 112

    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)

        err = (mips_error)(err |  mips_cpu_get_pc(cpu,&pc));

    passed = (err ==mips_Success ) && (pc==112+8192) ;//8192 is offset(0x800)*4

    mips_test_end_test(testId, passed, NULL);

    testId=mips_test_begin_test("BLEZ");
    passed=0;
    err=mips_cpu_set_pc(cpu,116);	//setting back the pc to 116
    if(err==0)
    	err = mips_cpu_set_register(cpu, 8, 0x00000001ul);               //unsigned long (condition not met)


    // BLEZ  $10, $8,98(112+(-4))     	//note -4 is 0xFFFF<<2 immediate
    instruction=0x1900FFFF;			//immediate value is FFFF
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,116,4,(uint8_t*)&instruction);	//pc is now at 116

    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)

        err = (mips_error)(err | mips_cpu_get_pc(cpu,&pc));

    passed = (err == mips_Success) && (pc==120);

    mips_test_end_test(testId, passed, NULL);


    testId=mips_test_begin_test("BLTZ");	//(condition not met)
    passed=0;

    if(err==0)
    	err = mips_cpu_set_register(cpu, 8, 0x00000000ul);               //unsigned long checking zero


    // BLTZ  $10, $8,98(116+(-4))     	//note -4 is 0xFFFF<<2 immediate
    instruction=0x500FFFF;			//immediate value is FFFF
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,120,4,(uint8_t*)&instruction);	//pc is now at 120

    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)

        err = (mips_error)(err | mips_cpu_get_pc(cpu,&pc));

    passed = (err == mips_Success) && (pc==124);	//condition is not met so pc+=4 as normall

    mips_test_end_test(testId, passed, NULL);




    testId=mips_test_begin_test("BNE");
    passed=0;


    err = mips_cpu_set_register(cpu, 8, 0xFFFFFFF0ul);               //unsigned long
    if(err==0)
        err = mips_cpu_set_register(cpu, 10, 0xFFFFFFFFul);               //unsigned long

    // BNE  $10, $8,98(124+16)
    instruction=0x150A0004;			//immediate value is 0004
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,124,4,(uint8_t*)&instruction);	//pc is now at 124

    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)

        err = (mips_error)(err | mips_cpu_get_pc(cpu,&pc));

    passed = (err == mips_Success) && (pc==128);		//values are equal so we do pc+=4 as usual

    mips_test_end_test(testId, passed, NULL);



    testId=mips_test_begin_test("BNE");	//this function just checks the PC following the branch(where branch happens)
    passed=0;


    err = mips_cpu_set_register(cpu, 8,0xFFFFFFFF);               //unsigned long

    if(err==0)
        err = mips_cpu_set_register(cpu, 9,0xFFFFFFFF);      //2 value to detect singed overflow

    // ADD $10, $8, $9
    instruction=0x01095020;
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,128,4,(uint8_t*)&instruction);    //pc is now at 128

    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)

        err = (mips_error)(err |  mips_cpu_get_pc(cpu,&pc));

    passed = (err ==mips_Success ) && (pc==128+16) ;



    mips_test_end_test(testId, passed, NULL);

    testId=mips_test_begin_test("J");
    passed=0;

    err=mips_cpu_set_pc(cpu,128);	//setting back the pc to 128

    // JR  0x00000008,	immediate value is 2 which is 8 when logical shifted by 2
    instruction=0x8000002;
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,128,4,(uint8_t*)&instruction);	//pc is now at 128

    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)

        err = (mips_error)(err | mips_cpu_get_pc(cpu,&pc));

    passed = (err == mips_Success) && (pc==132);		//values are equal so we do pc+=4 as usual

    mips_test_end_test(testId, passed, NULL);




    testId=mips_test_begin_test("J");	//this function just checks the PC following the branch(where branch happens)
    passed=0;


    err = mips_cpu_set_register(cpu, 8,0xFFFFFFFF);               //unsigned long

    if(err==0)
        err = mips_cpu_set_register(cpu, 9,0xFFFFFFFF);      //2 value to detect singed overflow

    // ADD $10, $8, $9
    instruction=0x01095020;
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,132,4,(uint8_t*)&instruction);    //pc is now at 132

    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)

        err = (mips_error)(err |  mips_cpu_get_pc(cpu,&pc));

    passed = (err ==mips_Success ) && (pc==8) ;



    mips_test_end_test(testId, passed, NULL);

    testId=mips_test_begin_test("JR");
    passed=0;
    err=mips_cpu_set_pc(cpu,128);	//setting back the pc to 128
    if(err==0)
    	err = mips_cpu_set_register(cpu, 8, 0xFFFFFFFFul);               //this number is not divisible by 4 so we should get an error

    // JR  $8
    instruction=0x1000008;
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,128,4,(uint8_t*)&instruction);	//pc is now at 128

    if(err==0)
        err=mips_cpu_step(cpu);



    passed = (err == mips_ExceptionInvalidAddress);

    mips_test_end_test(testId, passed, NULL);



    testId=mips_test_begin_test("LB");
    passed=0;

    err = mips_cpu_set_register(cpu, 8, 0x00000005ul);

    uint32_t value1=0x12800678;
    value=bytechange(value1);
    if(err==0)
    	err=mips_mem_write(mem,260,4,(uint8_t*)&value);

   //LB  $10,$8(256)		//loading byte 5+256=261(0x80) to register 10
    instruction=0x810A0100;
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,132,4,(uint8_t*)&instruction);	//pc is now at 132




    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));

    passed = (err == mips_Success) &&  (got==0xFFFFFF80); //correctly detected

    mips_test_end_test(testId, passed, NULL);



    testId=mips_test_begin_test("LBU");
    passed=0;

    err = mips_cpu_set_register(cpu, 8, 0x00000005ul);

    uint32_t value2=0x12800678;
    value=bytechange(value2);
    if(err==0)
    	err=mips_mem_write(mem,260,4,(uint8_t*)&value);

   //LBU  $10,$8(256)		//loading byte 5+256=261(0x80) to register 10
    instruction=0x910A0100;
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,136,4,(uint8_t*)&instruction);	//pc is now at 136




    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));

    passed = (err == mips_Success) &&  (got==0x00000080); //correctly detected (zero extended)

    mips_test_end_test(testId, passed, NULL);



    testId=mips_test_begin_test("SB");
    passed=0;

    err = mips_cpu_set_register(cpu, 10, 0x00000024ul);
    if(err==0)
    	err = mips_cpu_set_register(cpu, 8, 0x00000007ul);

    uint32_t value5=0x10233111;
    value=bytechange(value5);
    if(err==0)
    	err=mips_mem_write(mem,260,4,(uint8_t*)&value);


   //SB  $8(256),$10		//storing byte FF to  5+256=261(0x02)
    instruction=0xA10A0100;
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,140,4,(uint8_t*)&instruction);	//pc is now at 140
    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)
        err = (mips_error)(err | mips_mem_read(mem,260,4,(uint8_t*)&got) );
    got=bytechange(got);
    passed = (err == mips_Success) &&  (got==0x10233124); //correctly detected (zero extended)

    mips_test_end_test(testId, passed, NULL);




    testId=mips_test_begin_test("BGEZAL");
    passed=0;

    if(err==0)
    	err = mips_cpu_set_register(cpu, 8, 0x00000000ul);               //unsigned long checking zero


    // BGEZ  $10, $8,98(108+(2048))
    instruction=0x5110800;			//immediate value is FFFF
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,144,4,(uint8_t*)&instruction);	//pc is now at 144

    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)

        err = (mips_error)(err | mips_cpu_get_pc(cpu,&pc));

    passed = (err == mips_Success) && (pc==148);

    mips_test_end_test(testId, passed, NULL);



    testId=mips_test_begin_test("BGEZAL");	//this function just checks the PC following the branch(where branch happens)
    passed=0;


    err = mips_cpu_set_register(cpu, 8,0xFFFFFFFF);               //unsigned long

    if(err==0)
        err = mips_cpu_set_register(cpu, 9,0xFFFFFFFF);      //2 value to detect singed overflow

    // ADD $10, $8, $9
    instruction=0x01095020;
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,148,4,(uint8_t*)&instruction);    //pc is now at 148

    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)

        err = (mips_error)(err |  mips_cpu_get_pc(cpu,&pc));
    if(err==0)
    	err = (mips_error)(err |  mips_cpu_get_register(cpu,31,&got));	//checking if the value in reg[31]=orginal pc+8

    passed = (err ==mips_Success ) && (pc==148+8192) && (got==152)  ;//8192 is offset(0x800)*4

    mips_test_end_test(testId, passed, NULL);




    testId=mips_test_begin_test("BLTZAL");	//(condition not met)
    passed=0;
    err=mips_cpu_set_pc(cpu,152);
    if(err==0)
    	err = mips_cpu_set_register(cpu, 8, 0x00000000ul);               //unsigned long checking zero



    instruction=0x510FFFF;			//immediate value is FFFF
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,152,4,(uint8_t*)&instruction);	//pc is now at 152

    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)

        err = (mips_error)(err | mips_cpu_get_pc(cpu,&pc));

    passed = (err == mips_Success) && (pc==156);	//condition is not met so pc+=4 as normal(true even if condition not met)

    mips_test_end_test(testId, passed, NULL);


    testId=mips_test_begin_test("BLTZAL");	//this function just checks the PC following the branch(where branch happens)
    passed=0;


    err = mips_cpu_set_register(cpu, 8,0xFFFFFFFF);               //unsigned long

    if(err==0)
        err = mips_cpu_set_register(cpu, 9,0xFFFFFFFF);      //2 value to detect singed overflow

    // ADD $10, $8, $9
    instruction=0x01095020;
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,156,4,(uint8_t*)&instruction);    //pc is now at 156

    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)

        err = (mips_error)(err |  mips_cpu_get_pc(cpu,&pc));
    if(err==0)
    	err = (mips_error)(err |  mips_cpu_get_register(cpu,31,&got));	//checking if the value in reg[31]=orginal pc+8

    passed = (err ==mips_Success ) && (pc==160) && (got==160)  ;//checking if the value in reg[31]=orginal pc+8

    mips_test_end_test(testId, passed, NULL);


    testId=mips_test_begin_test("MFHI");	//this needs multiplying in the first place to set HI and LO
    passed=0;

    err = mips_cpu_set_register(cpu, 4, 0x0011FF00ul);
    if(err==0)
    	err=mips_cpu_set_register(cpu,8,0xFFFF1004ul);



   //MULTU  $8,$4  0011FF00 * FFFF1004 = 0x0011FEEF2137FC00
    instruction=0x880019;
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,160,4,(uint8_t*)&instruction);	//pc is now at 160


    if(err==0)
        err=mips_cpu_step(cpu);

    if(err==0)
        instruction=0x5010;	//move from high
        instruction=bytechange(instruction);
        err=mips_mem_write(mem,164,4,(uint8_t*)&instruction);	//pc is now at 164
    if(err==0){
               err=mips_cpu_step(cpu);
    }

    if(err==0)
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));

    passed = (err == mips_Success) &&  (got==0x0011FEEF); //correctly detected



    mips_test_end_test(testId, passed, NULL);

    

    testId=mips_test_begin_test("MFLO");	//this needs multiplying in the first place to set HI and LO
    passed=0;

    err = mips_cpu_set_register(cpu, 4, 0x0011FF00ul);
    if(err==0)
    	err=mips_cpu_set_register(cpu,8,0xFFFF1004ul);


   //MULTU  $8,$4  0011FF00 × FFFF1004 = 0x11FEEF2137FC00
    instruction=0x880019;
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,168,4,(uint8_t*)&instruction);	//pc is now at 168


    if(err==0)
        err=mips_cpu_step(cpu);

    if(err==0)
        instruction=0x5012;	//move from low
        instruction=bytechange(instruction);
        err=mips_mem_write(mem,172,4,(uint8_t*)&instruction);	//pc is now at 172
    if(err==0){
               err=mips_cpu_step(cpu);
    }
    if(err==0)
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));

    passed = (err == mips_Success) &&  (got==0x2137FC00); //correctly detected



    mips_test_end_test(testId, passed, NULL);





    testId=mips_test_begin_test("MULTU");
    passed=0;
 	uint32_t got1;
    if(err==0)
    	err = mips_cpu_set_register(cpu, 4, 0x0011FF00ul);               //unsigned long checking zero

    if(err==0)
     	err = mips_cpu_set_register(cpu, 8, 0xFFFF1004ul);

    instruction=0x880019;
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,176,4,(uint8_t*)&instruction);	//pc is now at 176

    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0){
        instruction=0x5010;	//move from high to register 10
        instruction=bytechange(instruction);
        err=mips_mem_write(mem,180,4,(uint8_t*)&instruction);	//pc is now at 180
    }
    if(err==0)
    	err=mips_cpu_step(cpu);


    if(err==0){
        instruction=0x4812;	//move from LOW to register 9
        instruction=bytechange(instruction);
        err=mips_mem_write(mem,184,4,(uint8_t*)&instruction);	//pc is now at 184
    }
    if(err==0)
    	err=mips_cpu_step(cpu);

    err = (mips_error)(err | mips_cpu_get_register(cpu,10,&got));

    if(err==0)
    	mips_cpu_get_register(cpu,9,&got1);

    passed = (err == mips_Success) && (got==0x0011FEEF) && (got1==0x2137FC00);

    mips_test_end_test(testId, passed, NULL);





    testId=mips_test_begin_test("MULT");
    passed=0;
 	uint32_t got2;
    if(err==0)
    	err = mips_cpu_set_register(cpu, 4, 0xFFFFFFFF);               //unsigned long checking zero

    if(err==0)
     	err = mips_cpu_set_register(cpu, 8, 0xFFFFFFFF);

    instruction=0x880018;		//multiply signed
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,188,4,(uint8_t*)&instruction);	//pc is now at 188

    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0){
        instruction=0x5010;	//move from high to register 10
        instruction=bytechange(instruction);
        err=mips_mem_write(mem,192,4,(uint8_t*)&instruction);	//pc is now at 192
    }
    if(err==0)
    	err=mips_cpu_step(cpu);


    if(err==0){
        instruction=0x4812;	//move from high to register 10
        instruction=bytechange(instruction);
        err=mips_mem_write(mem,196,4,(uint8_t*)&instruction);	//pc is now at 196
    }
    if(err==0)
    	err=mips_cpu_step(cpu);

    err = (mips_error)(err | mips_cpu_get_register(cpu,10,&got));

    if(err==0)
    	mips_cpu_get_register(cpu,9,&got2);

    passed = (err == mips_Success) && (got==0) && (got2==0x1);

    mips_test_end_test(testId, passed, NULL);



    testId=mips_test_begin_test("DIV");
    passed=0;
 	uint32_t got3;
    if(err==0)
    	err = mips_cpu_set_register(cpu, 4, 0xFFFFFFFF);               //unsigned long checking zero

    if(err==0)
     	err = mips_cpu_set_register(cpu, 8, 0xFFFFFFFE);

    instruction=0x88001A;		//divide signed
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,200,4,(uint8_t*)&instruction);	//pc is now at 200

    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0){
        instruction=0x5010;	//move from high to register 10
        instruction=bytechange(instruction);
        err=mips_mem_write(mem,204,4,(uint8_t*)&instruction);	//pc is now at 192
    }
    if(err==0)
    	err=mips_cpu_step(cpu);


    if(err==0){
        instruction=0x4812;	//move from low to register 9
        instruction=bytechange(instruction);
        err=mips_mem_write(mem,208,4,(uint8_t*)&instruction);	//pc is now at 196
    }
    if(err==0)
    	err=mips_cpu_step(cpu);

    err = (mips_error)(err | mips_cpu_get_register(cpu,10,&got));

    if(err==0)
    	mips_cpu_get_register(cpu,9,&got3);


    int32_t got_0=got;
    int32_t got_3=got3;
    passed = (err == mips_Success) && (got_0==-1) && (got_3==0);


    mips_test_end_test(testId, passed, NULL);




    testId=mips_test_begin_test("DIVU");
    passed=0;
 	uint32_t got4;
    if(err==0)
    	err = mips_cpu_set_register(cpu, 4, 0x0000000Aul);               //unsigned long checking zero

    if(err==0)
     	err = mips_cpu_set_register(cpu, 8, 0x0000003ul);

    instruction=0x88001B;		//divide unsigned
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,212,4,(uint8_t*)&instruction);	//pc is now at

    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0){
        instruction=0x5010;	//move from high to register 10
        instruction=bytechange(instruction);
        err=mips_mem_write(mem,216,4,(uint8_t*)&instruction);	//pc is now at
    }
    if(err==0)
    	err=mips_cpu_step(cpu);


    if(err==0){
        instruction=0x4812;	//move from low to register 9
        instruction=bytechange(instruction);
        err=mips_mem_write(mem,220,4,(uint8_t*)&instruction);	//pc is now at
    }
    if(err==0)
    	err=mips_cpu_step(cpu);

    err = (mips_error)(err | mips_cpu_get_register(cpu,10,&got));

    if(err==0)
    	mips_cpu_get_register(cpu,9,&got4);

    passed = (err == mips_Success) && (got==1) && (got4==3);

    mips_test_end_test(testId, passed, NULL);


    testId=mips_test_begin_test("JAL");
    passed=0;


    // JAL  0x00000008,
    instruction=0xC000002;
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,224,4,(uint8_t*)&instruction);	//pc is now at 224

    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)

        err = (mips_error)(err | mips_cpu_get_pc(cpu,&pc));

    passed = (err == mips_Success) && (pc==228);		//values are equal so we do pc+=4 as usual

    mips_test_end_test(testId, passed, NULL);




    testId=mips_test_begin_test("SH");
    passed=0;

    err = mips_cpu_set_register(cpu, 10, 0x0000ABCDul);
    if(err==0)
    	err = mips_cpu_set_register(cpu, 8, 0x00000004ul);

    uint32_t value6=0x10233111;
    value=bytechange(value6);
    if(err==0)
    	err=mips_mem_write(mem,260,4,(uint8_t*)&value);


   //SH  $8(256),$10		//storing halftbyte FFFF to  6+256=262(0x02)
    instruction=0xA50A0100;
    instruction=bytechange(instruction);
    if(err==0)
    	err=mips_mem_write(mem,228,4,(uint8_t*)&instruction);	//pc is now at 228
    if(err==0)
        err=mips_cpu_step(cpu);


    if(err==0)
        err = (mips_error)(err | mips_mem_read(mem,260,4,(uint8_t*)&got) );
    got=bytechange(got);

    err=mips_cpu_get_pc(cpu,&pc);
    passed = (err == mips_Success) &&  (got==0xABCD3111) && (pc==8); //correctly detected (zero extended)

    mips_test_end_test(testId, passed, NULL);

    mips_test_end_suite();

    mips_cpu_free(cpu);
    mips_mem_free(mem);
    
    return 0;
}
