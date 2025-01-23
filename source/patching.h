#pragma once
#include<cstdint>

inline uint32_t* current_codecave_writer_location = nullptr;
inline void* game_text_region;
inline void* game_data_region;

uint32_t write_code_to_codecave(uint32_t instruction);
uint32_t allocate_codecave_instruction();
uint32_t generate_codecave_longjump(uint32_t destination);
void generate_longjump_from_text(uint32_t offset, void* destination);
void overwrite_codecave_instruction(uint32_t location, uint32_t instruction);
void patch_text_location(uint32_t offset, uint32_t instruction);
void patch_data_location(uint32_t offset, uint8_t data);

uint32_t cemu_pointer_to_actual(uint32_t cemu_ptr);
