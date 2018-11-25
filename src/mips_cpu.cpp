






#include "mips.h"
#include <stdlib.h>
#include<iostream>

using namespace std;

uint32_t bitswanted(int num_bits,int index_bit,uint32_t instruction){		//this function zero extends the unwanted bits

	instruction=instruction<<index_bit;
	instruction=instruction>>(31-num_bits+1);


	return instruction;

}
uint32_t bytechange(uint32_t instruction){

	uint32_t instruction1=0;

	instruction1+=bitswanted(8,24,instruction)<<24;
	instruction1+=bitswanted(8,16,instruction)<<16;
	instruction1+=bitswanted(8,8,instruction)<<8;
	instruction1+=bitswanted(8,0,instruction)<<0;

	return instruction1;
}


uint32_t signextend(uint32_t immediate,int sign_bit){
	uint32_t result,k;
	if(bitswanted(1,sign_bit,immediate)==0){
		result=immediate & ( (0x00000001<<(32-sign_bit))-1 );
	}
	if(bitswanted(1,sign_bit,immediate)==1){
		k=((0x00000001<<(32-sign_bit))-1);
		k=0xFFFFFFFF-k;
		result=immediate | k ;
	}
	return result;
}

struct mips_cpu_impl{
    
    uint32_t pc;
    uint32_t pcN;
    uint32_t regs[32];
    
    mips_mem_h mem;

    uint32_t HI,LO;
};

mips_cpu_h mips_cpu_create(mips_mem_h mem)
{
    unsigned i;
    mips_cpu_h res=(mips_cpu_h)malloc(sizeof(struct mips_cpu_impl));
    
    res->mem=mem;
    
    res->pc=0;
    res->pcN=4;	// NOTE: why does this make sense?
    
    for( i=0;i<32;i++){                 //reset reg
        res->regs[i]=0;
    }
    res->HI=0;
    res->LO=0;
    
    return res;
}

void mips_cpu_free(mips_cpu_h state)
{
    free(state);
}

mips_error mips_cpu_get_register(
                                 mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
                                 unsigned index,		//!< Index from 0 to 31
                                 uint32_t *value		//!< Where to write the value to
)
{
    if(state==0)
        return mips_ErrorInvalidHandle;
    if(index>=32)
        return mips_ErrorInvalidArgument;
    if(value==0)
        return mips_ErrorInvalidArgument;
    
    *value = state->regs[index];
    return mips_Success;
}

/*! Modifies one of the 32 general purpose MIPS registers. */
mips_error mips_cpu_set_register(
                                 mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
                                 unsigned index,		//!< Index from 0 to 31
                                 uint32_t value		//!< New value to write into register file
)
{
    if(state==0)
        return mips_ErrorInvalidHandle;
    if(index>=32)
        return mips_ErrorInvalidArgument;
    
    if(index==0){
    	return  mips_ExceptionAccessViolation;			//??
    }
    state->regs[index]=value;
    
    return mips_Success;
}

mips_error mips_cpu_set_pc(mips_cpu_h state,uint32_t pc){
	 if(state==0)
	        return mips_ErrorInvalidHandle;

	state->pc=pc;
	state->pcN=pc+4;	//setting pcN to the next memory location

return	mips_Success;
}

mips_error mips_cpu_get_pc(mips_cpu_h state, uint32_t *pc){
	 if(state==0)
		        return mips_ErrorInvalidHandle;

	 *pc=state->pc;

	 return	mips_Success;
}

mips_error mips_cpu_step(mips_cpu_h state)
{
    uint32_t pc=state->pc;
    
    if(state==0)
        return mips_ErrorInvalidHandle;
    
    uint32_t instruction;
    mips_error err = mips_mem_read(state->mem, pc, 4, (uint8_t*)&instruction);
    instruction=bytechange(instruction);

    if(!err){
        
        if (bitswanted(6,0,instruction)==0){		//R type




            if(bitswanted(6,26,instruction)==36){		//bitwise AND

            	uint32_t reg1,reg2,reg3;
            	err = mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg1);
                if(!err){
                err =  mips_cpu_get_register(state,bitswanted(5,11,instruction), &reg2);
                }
                if(!err){
                	reg3 = reg1 & reg2;
                }
                if(!err){
                	err = mips_cpu_set_register(state,bitswanted(5,16,instruction), reg3);
                }
            }


            if(bitswanted(6,26,instruction)==37){	//bitwise OR
            	uint32_t reg1,reg2,reg3;
            	err = mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg1);
                if(!err){
                err =  mips_cpu_get_register(state,bitswanted(5,11,instruction), &reg2);
                        }
                if(!err){
                    reg3 = reg1 | reg2;
                        }
                if(!err){
                err = mips_cpu_set_register(state,bitswanted(5,16,instruction), reg3);
                        }
            }



            if(bitswanted(6,26,instruction)==38){	//bitwise XOR
            	uint32_t reg1,reg2,reg3;
            	err = mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg1);
                if(!err){
                err =  mips_cpu_get_register(state,bitswanted(5,11,instruction), &reg2);
                        }
                if(!err){
                    reg3 = reg1 ^ reg2;
                        }
                if(!err){
                err = mips_cpu_set_register(state,bitswanted(5,16,instruction), reg3);
                        }
            }

            if(bitswanted(6,26,instruction)==33){	//ADD unsinged (no overflow)
            	uint32_t reg1,reg2,reg3;
            	err = mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg1);
                if(!err){
                err =  mips_cpu_get_register(state,bitswanted(5,11,instruction), &reg2);
                        }
                if(!err){
                    reg3 = reg1 + reg2;
                        }
                if(!err){
                err = mips_cpu_set_register(state,bitswanted(5,16,instruction), reg3);
                        }
            }

            if(bitswanted(6,26,instruction)==35){		//Substract (unsinged)

            	uint32_t reg1,reg2,reg3;
            	err = mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg1);
                if(!err){
                err =  mips_cpu_get_register(state,bitswanted(5,11,instruction), &reg2);
                }
                if(!err){
                	reg3 = reg1 - reg2;
                }
                if(!err){
                	err = mips_cpu_set_register(state,bitswanted(5,16,instruction), reg3);
                }
            }

            if(bitswanted(6,26,instruction)==43){		//set on  less than (unsinged)

            	uint32_t reg1,reg2,reg3;
            	err = mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg1);
                if(!err){
                err =  mips_cpu_get_register(state,bitswanted(5,11,instruction), &reg2);
                }
                if(!err){
                	if(reg1<reg2){
                		reg3 = 1;
                	}
                	else{
                		reg3=0;
                	}
                }
                if(!err){
                	err = mips_cpu_set_register(state,bitswanted(5,16,instruction), reg3);
                }
            }

            if(bitswanted(6,26,instruction)==32){   //ADD (with overflow)
                uint32_t reg1,reg2,reg3;		//??

                err = mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg1);

                if(!err){
                err =  mips_cpu_get_register(state,bitswanted(5,11,instruction), &reg2);

                        }
                if(!err){
                    reg3 = reg1 + reg2;
                    if(bitswanted(1,0,reg1)==1 && bitswanted(1,0,reg2)==1 && bitswanted(1,0,reg3)==0){
                        err= mips_ExceptionArithmeticOverflow;
                    }
                    if(bitswanted(1,0,reg1)==0 && bitswanted(1,0,reg2)==0 && bitswanted(1,0,reg3)==1){
                        err= mips_ExceptionArithmeticOverflow;
                    }
                        }
                if(!err){
                err = mips_cpu_set_register(state,bitswanted(5,16,instruction), reg3);
                        }
            }

            if(bitswanted(6,26,instruction)==34){   //SUB (with overflow)
                uint32_t reg1,reg2,reg3;		//??
                err = mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg1);
                if(!err){
                err =  mips_cpu_get_register(state,bitswanted(5,11,instruction), &reg2);
                        }
                if(!err){
                    reg3 = reg1 - reg2;
                    if(bitswanted(1,0,reg1)==1 && bitswanted(1,0,-reg2)==1 && bitswanted(1,0,reg3)==0){
                        err= mips_ExceptionArithmeticOverflow;
                    }
                    if(bitswanted(1,0,reg1)==0 && bitswanted(1,0,-reg2)==0 && bitswanted(1,0,reg3)==1){
                        err= mips_ExceptionArithmeticOverflow;
                    }
                        }
                if(!err){
                err = mips_cpu_set_register(state,bitswanted(5,16,instruction), reg3);
                        }
            }
            if(bitswanted(6,26,instruction)==0){   //Shift word left logical
                uint32_t reg1,reg2;		//??
                uint32_t sa=bitswanted(5,21,instruction);

                err =  mips_cpu_get_register(state,bitswanted(5,11,instruction), &reg1);

                if(!err){
                    reg2 = reg1<<sa;

                        }
                if(!err){
                err = mips_cpu_set_register(state,bitswanted(5,16,instruction), reg2);
                        }
            }

            if(bitswanted(6,26,instruction)==2){   //Shift word right logical
                uint32_t reg1,reg2;		//??
                uint32_t sa=bitswanted(5,21,instruction);

                err =  mips_cpu_get_register(state,bitswanted(5,11,instruction), &reg1);

                if(!err){
                    reg2 = reg1>>sa;

                        }
                if(!err){
                err = mips_cpu_set_register(state,bitswanted(5,16,instruction), reg2);
                        }
            }

            if(bitswanted(6,26,instruction)==3){   //Shift word right arithmetic
                uint32_t reg1,reg2;		//??
                uint32_t sa=bitswanted(5,21,instruction);

                err =  mips_cpu_get_register(state,bitswanted(5,11,instruction), &reg1);

                if(!err){
                    reg2 = reg1>>sa;
                    reg2=signextend(reg2,sa);		//sign extending (notice bit 31 is now at bit sa
                        }
                if(!err){
                err = mips_cpu_set_register(state,bitswanted(5,16,instruction), reg2);
                        }
            }

            if(bitswanted(6,26,instruction)==4){   //Shift word left logical variable
                uint32_t reg1,reg2,reg3;		//??


                err =  mips_cpu_get_register(state,bitswanted(5,11,instruction), &reg1);

                if(!err){
                err =  mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg2);
                        }
                if(err==0)
                	reg3=reg1<<(reg2 & 0x0000001F);	//all logical shifts are defined so we logical shift by the remainder of divison by 32
                if(!err){
                err = mips_cpu_set_register(state,bitswanted(5,16,instruction), reg3);
                        }
            }

            if(bitswanted(6,26,instruction)==6){   //Shift word right logical variable
                uint32_t reg1,reg2,reg3;		//??


                err =  mips_cpu_get_register(state,bitswanted(5,11,instruction), &reg1);

                if(!err){
                err =  mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg2);
                        }
                if(err==0)
                	reg3=reg1>>(reg2 & 0x0000001F);	//all logical shifts are defined so we logical shift by the remainder of divison by 32
                if(!err){
                err = mips_cpu_set_register(state,bitswanted(5,16,instruction), reg3);
                        }
            }

            if(bitswanted(6,26,instruction)==42){   //set on less than signed
                uint32_t reg1,reg2,reg3;		//??


                err =  mips_cpu_get_register(state,bitswanted(5,11,instruction), &reg2);

                if(!err){
                	err =  mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg1);
                        }
                int32_t reg_1=reg1;	//changing to sign numbers
                int32_t reg_2=reg2;

                if(reg_1<reg_2)
                	reg3=1;
                else
                	reg3=0;

                if(err==0)
                	err = mips_cpu_set_register(state,bitswanted(5,16,instruction), reg3);
            }

            if(bitswanted(21,11,instruction)==8){   //Jump register
                uint32_t reg1;

                err = mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg1);

                if(reg1%4!=0){	//must be divisible by 4
                	err=mips_ExceptionInvalidAddress;

                }
                if(err==0){
                	err=mips_cpu_set_pc(state,reg1);
                	return err;
                }
            }
            if(bitswanted(16,16,instruction)==26){   //divide
                uint32_t reg1,reg2;
                err =  mips_cpu_get_register(state,bitswanted(5,11,instruction), &reg2);

                if(!err){
                	err =  mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg1);
                        }
                int32_t reg_1=reg1;	//changing to sign numbers
                int32_t reg_2=reg2;

                state->LO=reg_1/reg_2;
                state->HI=reg_1%reg_2;

            }

            if(bitswanted(16,16,instruction)==24){   //multiply
                uint32_t reg1,reg2;

                err =  mips_cpu_get_register(state,bitswanted(5,11,instruction), &reg2);

                if(!err){
                	err =  mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg1);
                        }
                int64_t reg_1=*(int32_t*)&reg1;	//changing to unsigned numbers
                int64_t reg_2=*(int32_t*)&reg2;

                int64_t result=reg_1*reg_2;

                state->LO=result;
                state->HI=result>>32;
            }

            if(bitswanted(16,16,instruction)==25){   //multiply unsigned
                uint32_t reg1,reg2;
                uint64_t result;
                err =  mips_cpu_get_register(state,bitswanted(5,11,instruction), &reg2);

                if(err==0){
                	err =  mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg1);
                        }
                uint64_t reg_1=*(uint32_t*)&reg1;	//changing to unsigned numbers
                uint64_t reg_2=*(uint32_t*)&reg2;


                result=reg_1*reg_2;

                state->LO=result;
                state->HI=result>>32;
            }



            if(bitswanted(16,16,instruction)==27){   //divide unsigned
                uint32_t reg1,reg2;
                err =  mips_cpu_get_register(state,bitswanted(5,11,instruction), &reg2);

                if(!err){
                	err =  mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg1);
                        }
                uint32_t reg_1=reg1;	//changing to unsigned numbers
                uint32_t reg_2=reg2;

                state->LO=reg_1/reg_2;
                state->HI=reg_1%reg_2;

            }
            if(bitswanted(6,10,instruction)==0 && bitswanted(11,21,instruction)==16){   //Move from HI register

            	uint32_t HI=state->HI;
                err =  mips_cpu_set_register(state,bitswanted(5,16,instruction), HI);
            }


            if(bitswanted(6,10,instruction)==0 && bitswanted(11,21,instruction)==18){   //Move from LO register

               uint32_t LO=state->LO;
                err =  mips_cpu_set_register(state,bitswanted(5,16,instruction), LO);
            }

        }

      if(bitswanted(6,0,instruction)==13){			//OR immediate(I type)
        	uint32_t reg1,reg2;
        	uint16_t immediate1=bitswanted(16,16,instruction);	//zero extending the bits from 15 to 0
        	uint32_t immediate=*(uint16_t *)&immediate1;
        	err = mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg1);

            if(!err){
                reg2 = reg1 | immediate;
                    }
            if(!err){
            err = mips_cpu_set_register(state,bitswanted(5,11,instruction),reg2);
                    }
        }



      if(bitswanted(6,0,instruction)==12){			//AND immediate(I type)
        	uint32_t reg1,reg2;
        	uint16_t immediate1=bitswanted(16,16,instruction);	//zero extending the bits from 15 to 0
        	uint32_t immediate=*(uint16_t *)&immediate1;
        	err = mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg1);

            if(!err){
                reg2 = reg1 & immediate;
                    }
            if(!err){
            err = mips_cpu_set_register(state,bitswanted(5,11,instruction),reg2);
                    }
        }



      if(bitswanted(6,0,instruction)==14){			//XOR immediate(I type)
        	uint32_t reg1,reg2;
        	uint16_t immediate1=bitswanted(16,16,instruction);	//zero extending the bits from 15 to 0
        	uint32_t immediate=*(uint16_t *)&immediate1;
        	err = mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg1);

            if(!err){
                reg2 = reg1 ^ immediate;
                    }
            if(!err){
            err = mips_cpu_set_register(state,bitswanted(5,11,instruction),reg2);
                    }
        }

      if(bitswanted(6,0,instruction)==9){			//ADD immediate unsinged	(I type)
        	uint32_t reg1,reg2;
        	uint16_t immediate1=bitswanted(16,16,instruction);	//bits from 16 to 31
        	//immediate=signextend(immediate,16);
        	int32_t immediate=*(int16_t*) &immediate1;
        	err = mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg1);

            if(!err){
                reg2 = reg1 + immediate;
                    }
            if(!err){
            err = mips_cpu_set_register(state,bitswanted(5,11,instruction),reg2);
                    }
        }

      if(bitswanted(6,0,instruction)==8){			//ADD immediate (with overflow)	(I type)
        	uint32_t reg1,reg2;
        	uint16_t immediate1=bitswanted(16,16,instruction);	//bits from 16 to 31
        	int32_t immediate=*(int16_t*) &immediate1;
        //	immediate=signextend(immediate,16);

        	err = mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg1);

            if(!err){
                reg2 = reg1 + immediate;
                if(bitswanted(1,0,reg1)==1 && bitswanted(1,0,immediate)==1 && bitswanted(1,0,reg2)==0){
                    err= mips_ExceptionArithmeticOverflow;
                }
                if(bitswanted(1,0,reg1)==0 && bitswanted(1,0,immediate)==0 && bitswanted(1,0,reg2)==1){
                    err= mips_ExceptionArithmeticOverflow;
                }
                    }
            if(!err){
            err = mips_cpu_set_register(state,bitswanted(5,11,instruction),reg2);
                    }
        }



      if(bitswanted(11,0,instruction)==480){			//Load upper immediate 	(I type)

        	uint32_t reg1;
        	uint32_t immediate=bitswanted(16,16,instruction);	//bits from 16 to 31


            reg1 =immediate<<16;		//bits 16 to 31 are all zeros

            err = mips_cpu_set_register(state,bitswanted(5,11,instruction),reg1);

        }


      if(bitswanted(6,0,instruction)==35){			//Load word 	(I type)

        	uint32_t reg1,reg2;
        	uint16_t offset1=bitswanted(16,16,instruction);	//bits from 16 to 31
        	int32_t offset=*(int16_t*)&offset1;
          	err = mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg1);

        	if( (reg1+offset)%4!=0 ){		//addition of offset and base must be divisible by 4
        	 err=mips_ExceptionInvalidAddress;
        	}


        	if(err==0){
        	err = mips_mem_read(state->mem, reg1+offset, 4, (uint8_t*)&reg2);
        	}
            reg2=bytechange(reg2);

            if(!err){
            err = mips_cpu_set_register(state,bitswanted(5,11,instruction),reg2);
                    }
        }

      if(bitswanted(6,0,instruction)==11){			//set on less than immediate unsinged
        	uint32_t reg1,reg2;
        	int16_t immediate1=bitswanted(16,16,instruction);	//zero extending the bits from 15 to 0
        	int32_t immediate=*(int16_t *)&immediate1;	//sign extention
        	uint32_t immediate3=immediate;				//treating it as unsinged

        	err = mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg1);

            if(!err){
                if(reg1<immediate3)
                	reg2=1;
                else{
                	reg2=0;
                }
                    }
            if(!err){
            err = mips_cpu_set_register(state,bitswanted(5,11,instruction),reg2);
                    }
        }

      if(bitswanted(6,0,instruction)==10){			//set on less than immediate singed
        	uint32_t reg1,reg2;
        	int16_t immediate1=bitswanted(16,16,instruction);	//zero extending the bits from 15 to 0
        	int32_t immediate=*(int16_t *)&immediate1;	//sign extention


        	err = mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg1);

        	int32_t reg_1=reg1;		//converting to sign
            if(!err){
                if(reg_1<immediate)
                	reg2=1;
                else{
                	reg2=0;
                }
                    }
            if(!err){
            err = mips_cpu_set_register(state,bitswanted(5,11,instruction),reg2);
                    }
        }

      if(bitswanted(6,0,instruction)==43){			//store word 	(I type)

        	uint32_t reg1,reg2;
        	uint16_t offset1=bitswanted(16,16,instruction);	//bits from 16 to 31
        	int32_t offset=*(int16_t*)&offset1;
          	err = mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg1);

        	if( (reg1+offset)%4!=0 ){		//addition of offset and base must be divisible by 4
        	return	mips_ExceptionInvalidAddress;
        	}
        	if(err==0)
        		err = mips_cpu_get_register(state,bitswanted(5,11,instruction), &reg2);
         	if(err==0)
         		reg2=bytechange(reg2);
        		err = mips_mem_write(state->mem, reg1+offset, 4, (uint8_t*)&reg2);

        }

      if(bitswanted(6,0,instruction)==4){			//Branch on equal
        	uint32_t reg1,reg2;
        	uint16_t immediate1=bitswanted(16,16,instruction);
        	int32_t immediate=*(int16_t *)&immediate1;
        	immediate=immediate<<2;	//logical shifting by 2



        	err = mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg1);

            if(!err){
             	err = mips_cpu_get_register(state,bitswanted(5,11,instruction), &reg2);
                    }
            if(!err){
            	if(reg1==reg2){
            		state->pc=state->pcN;
            		state->pcN+=immediate;	//setting pcN to the offset
            		return err;
            	}
            }

        }


      if(bitswanted(6,0,instruction)==1 &&  bitswanted(5,11,instruction)==1){			//Branch on greater than or equal to zero
        	uint32_t reg1;
        	uint16_t immediate1=bitswanted(16,16,instruction);
        	int32_t immediate=*(int16_t *)&immediate1;
        	immediate=immediate<<2;	//logical shifting by 2

        	err = mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg1);

        	int32_t reg_1=reg1;	//changing to sign number

            if(!err){
            	if(reg_1>=0){
            		state->pc=state->pcN;
            		state->pcN+=immediate;	//setting pcN to the offset
            		return err;
            	}
            }
        }
      if(bitswanted(6,0,instruction)==1 &&  bitswanted(5,11,instruction)==17){			//Branch on non negative and link
        	uint32_t reg1;
        	uint16_t immediate1=bitswanted(16,16,instruction);
        	int32_t immediate=*(int16_t *)&immediate1;
        	immediate=immediate<<2;	//logical shifting by 2

        	err = mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg1);

        	int32_t reg_1=reg1;	//changing to sign number

        	if(err==0)
        		err=mips_cpu_set_register(state,31,(state->pc)+8);	//putting the address of the next 2 instruction to register 31
            if(!err){
            	if(reg_1>=0){
            		state->pc=state->pcN;
            		state->pcN+=immediate;	//setting pcN to the offset
            		return err;
            	}
            }
        }

      if(bitswanted(6,0,instruction)==1 &&  bitswanted(5,11,instruction)==16){			//Branch on less than zero and link
        	uint32_t reg1;
        	uint16_t immediate1=bitswanted(16,16,instruction);
        	int32_t immediate=*(int16_t *)&immediate1;
        	immediate=immediate<<2;	//logical shifting by 2

        	err = mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg1);

        	int32_t reg_1=reg1;	//changing to sign number

        	if(err==0)
        		err=mips_cpu_set_register(state,31,(state->pc)+8);	//putting the address of the next 2 instruction to register 31
            if(!err){
            	if(reg_1<0){
            		state->pc=state->pcN;
            		state->pcN+=immediate;	//setting pcN to the offset
            		return err;
            	}
            }
        }


      if(bitswanted(6,0,instruction)==7 &&  bitswanted(5,11,instruction)==0){			//Branch on greater than zero
        	uint32_t reg1;
        	uint16_t immediate1=bitswanted(16,16,instruction);
        	int32_t immediate=*(int16_t *)&immediate1;
        	immediate=immediate<<2;	//logical shifting by 2

        	err = mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg1);

        	int32_t reg_1=reg1;	//changing to sign number

            if(!err){
            	if(reg_1>0){
            		state->pc=state->pcN;
            		state->pcN+=immediate;	//setting pcN to the offset
            		return err;
            	}
            }
        }

      if(bitswanted(6,0,instruction)==6 && bitswanted(5,11,instruction)==0){			//Branch on less than or equal to zero
        	uint32_t reg1;
        	uint16_t immediate1=bitswanted(16,16,instruction);
        	int32_t immediate=*(int16_t *)&immediate1;
        	immediate=immediate<<2;	//logical shifting by 2

        	err = mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg1);

        	int32_t reg_1=reg1;	//changing to sign number

            if(!err){
            	if(reg_1<=0){
            		state->pc=state->pcN;
            		state->pcN+=immediate;	//setting pcN to the offset
            		return err;
            	}
            }
        }

      if(bitswanted(6,0,instruction)==1 && bitswanted(5,11,instruction)==0){			//Branch on less than  zero
        	uint32_t reg1;
        	uint16_t immediate1=bitswanted(16,16,instruction);
        	int32_t immediate=*(int16_t *)&immediate1;
        	immediate=immediate<<2;	//logical shifting by 2

        	err = mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg1);

        	int32_t reg_1=reg1;	//changing to sign number

            if(!err){
            	if(reg_1<0){
            		state->pc=state->pcN;
            		state->pcN+=immediate;	//setting pcN to the offset
            		return err;
            	}
            }
        }



      if(bitswanted(6,0,instruction)==5){			//Branch on not equal
        	uint32_t reg1,reg2;
        	uint16_t immediate1=bitswanted(16,16,instruction);
        	int32_t immediate=*(int16_t *)&immediate1;
        	immediate=immediate<<2;	//logical shifting by 2



        	err = mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg1);

            if(!err){
             	err = mips_cpu_get_register(state,bitswanted(5,11,instruction), &reg2);
                    }
            if(!err){
            	if(reg1!=reg2){
            		state->pc=state->pcN;
            		state->pcN+=immediate;	//setting pcN to the offset
            		return err;
            	}

            }

        }

      if(bitswanted(6,0,instruction)==2){			//JUMP

        	uint32_t immediate=bitswanted(26,6,instruction);

        	immediate=immediate<<2;	//logical shifting by 2


    		state->pc=state->pcN;
    		state->pcN=immediate;	//setting pcN to the offset
    		return err;
      }
      if(bitswanted(6,0,instruction)==3){			//JUMP and link

        	uint32_t immediate=bitswanted(26,6,instruction);

        	err=mips_cpu_set_register(state,31,(state->pc)+8);	//putting the address of the next 2 instruction to register 31
        	immediate=immediate<<2;	//logical shifting by 2


    		state->pc=state->pcN;
    		state->pcN=immediate;	//setting pcN to the offset
    		return err;
      }




      if(bitswanted(6,0,instruction)==32){			//Load Byte 	(I type)

        	uint32_t reg1;
        	uint32_t reg2;

        	uint16_t offset1=bitswanted(16,16,instruction);	//bits from 16 to 31
        	int32_t offset=*(int16_t*)&offset1;
          	err = mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg1);


          	if(err==0){
          	uint32_t address=(reg1+offset)-((reg1+offset)%4);	//reading and writing address must be divisible by 4 so I find the last 4k multiple of the byte address
        	err = mips_mem_read(state->mem,address,4, (uint8_t*)&reg2);
        	reg2=bytechange(reg2);
          	}
          	reg2=bitswanted(8,((reg1+offset)%4)*8,reg2);	//extracting the byte needed from the register
        	int32_t reg_2=*(int8_t*)&reg2;	//signed 32 bit(sign extended of what is in memory)


            if(!err){
            err = mips_cpu_set_register(state,bitswanted(5,11,instruction),reg_2);
                    }
        }



      if(bitswanted(6,0,instruction)==36){			//Load Byte Unsinged	(I type)

        	uint32_t reg1;
        	uint32_t reg2;

        	uint16_t offset1=bitswanted(16,16,instruction);	//bits from 16 to 31
        	int32_t offset=*(int16_t*)&offset1;
          	err = mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg1);


          	if(err==0){
          	uint32_t address=(reg1+offset)-((reg1+offset)%4);	//reading and writing address must be divisible by 4 so I find the last 4k multiple of the byte address
        	err = mips_mem_read(state->mem,address,4, (uint8_t*)&reg2);
        	reg2=bytechange(reg2);
          	}
          	reg2=bitswanted(8,((reg1+offset)%4)*8,reg2);	//extracting the byte needed from the register
        	uint32_t reg_2=*(uint8_t*)&reg2;	//unsigned 32 bit

            if(!err){
            err = mips_cpu_set_register(state,bitswanted(5,11,instruction),reg_2);
                    }
        }



      if(bitswanted(6,0,instruction)==40){			//store byte 	(I type)

        	uint32_t reg1,reg2,reg3;
        	uint32_t shift;
        	uint16_t offset1=bitswanted(16,16,instruction);	//bits from 16 to 31
        	int32_t offset=*(int16_t*)&offset1;
          	err = mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg1);
        	uint32_t address1=(reg1+offset)-((reg1+offset)%4);

        	if(err==0){
        		err = mips_cpu_get_register(state,bitswanted(5,11,instruction), &reg2);
        		reg2=bitswanted(8,24,reg2);		//extracting the least significant 8 bits

        	}
        	if(err==0){

        		err = mips_mem_read(state->mem,address1,4, (uint8_t*)&reg3);
        		reg3=bytechange(reg3);

        	}
         	if(err==0){

         		shift=24-(  8 *  (   (reg1+offset)%4    ) );

         		reg3=reg3 & (~(0x000000FF<<shift));

         		reg3=reg3 | (reg2<<shift);

         		reg3=bytechange(reg3);
        		err = mips_mem_write(state->mem, address1, 4, (uint8_t*)&reg3);
         	}
        }



      if(bitswanted(6,0,instruction)==41){			//store half word 	(I type)

        	uint32_t reg1,reg2,reg3;

        	uint16_t offset1=bitswanted(16,16,instruction);	//bits from 16 to 31
        	int32_t offset=*(int16_t*)&offset1;

        	uint32_t shift;

          	err = mips_cpu_get_register(state,bitswanted(5,6,instruction), &reg1);
        	uint32_t address1=(reg1+offset)-((reg1+offset)%4);

          	if((reg1+offset)%2 != 0){
          		return mips_ExceptionInvalidAddress;
          	}
        	if(err==0){
        		err = mips_cpu_get_register(state,bitswanted(5,11,instruction), &reg2);
        		reg2=bitswanted(16,16,reg2);		//extracting the least significant 16 bits
        	}
        	if(err==0){

        		err = mips_mem_read(state->mem,address1,4, (uint8_t*)&reg3);
        		reg3=bytechange(reg3);

        	}
         	if(err==0){

         		shift=(  16 * (   ( -0.5*(   (reg1+offset)%4    ) )  +    1 )   );

         		reg3=reg3 & (~(0x0000FFFF<<shift));

         		reg3=reg3 | (reg2<<shift);

         		reg3=bytechange(reg3);
        		err = mips_mem_write(state->mem, address1, 4, (uint8_t*)&reg3);
         	}

      }


    }

    //TODO: Here is where the magic happens
    // - Fetch the instruction from memory (mips_mem_read)
    // - Decode the instruction (is it R, I, J)?
    // - Execute the instruction (do maths, access memory, ...)
    // - Writeback the results (update registers, advance pc)
    
    state->pc=state->pcN;
    state-> pcN += 4;		//pc must be incremented by 4 in most of the instructions
    
    return err;
 }




