#include"patching.h"
#include"kernel.h"
#include<lib.h>

uint32_t write_code_to_codecave(uint32_t instruction){
  auto location = allocate_codecave_instruction();

  KernelWriteU32(location, instruction);

  return location;
}
// this is litterally just a more descriptive version of kernel write lol


uint32_t generate_codecave_longjump(uint32_t destination){
  auto upper_bits = (destination >> 16) & 0xFFFF;
  auto lower_bits = destination & 0xFFFF;

  auto entry = write_code_to_codecave(LoadImmediateShifted(13,upper_bits)); 
  write_code_to_codecave(OrImmediate(13,13, lower_bits));
  write_code_to_codecave(BranchCounterRegister());

  return entry;
}

uint32_t allocate_codecave_instruction(){
  auto current_location = current_codecave_writer_location;
  current_codecave_writer_location++;
  return (uint32_t)current_location;
}

void generate_longjump_from_text(uint32_t offset, void* destination){
  auto longjump_entry = generate_codecave_longjump((uint32_t)destination);

  auto absolute_text_location = offset + (uint32_t)game_text_region;  

  patch_text_location(offset, Branch::from_to(absolute_text_location, longjump_entry));
}


void overwrite_codecave_instruction(uint32_t location, uint32_t instruction){
  KernelWriteU32(location, instruction);
  
}
void patch_text_location(uint32_t offset, uint32_t instruction){
  KernelWriteU32((uint32_t)game_text_region + offset, instruction);
}

void patch_data_location(uint32_t offset, uint8_t data){
  // the data region is by definition writable from the space we are in currently so we dont need kernel permissions to write
  *((uint8_t*)game_data_region + offset) = data; 
}

uint32_t cemu_pointer_to_actual(uint32_t cemu_ptr){
  if(0x10000000 < cemu_ptr){
    return cemu_ptr - 0x10000000 + (uint32_t)game_data_region;
  }
  return cemu_ptr - 0x02000000 + (uint32_t)game_text_region;
}
