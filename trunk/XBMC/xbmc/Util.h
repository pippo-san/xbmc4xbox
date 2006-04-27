#pragma once
#include "playlist.h"
#include "filesystem/rarmanager.h"
#include "settings.h"

class CTrainer;

struct network_info
{
  char ip[32];
  char gateway[32];
  char subnet[32];
  char DNS1[32];
  char DNS2[32];
  char DHCP;
};

// for 'cherry' patching
typedef enum
{
  COUNTRY_NULL = 0,
  COUNTRY_USA,
  COUNTRY_JAP,
  COUNTRY_EUR
} F_COUNTRY;

typedef enum
{
  VIDEO_NULL = 0,
  VIDEO_NTSCM,
  VIDEO_NTSCJ,
  VIDEO_PAL50,
  VIDEO_PAL60
} F_VIDEO;

static BYTE OriginalData[57]=
{
  0x55,0x8B,0xEC,0x81,0xEC,0x04,0x01,0x00,0x00,0x8B,0x45,0x08,0x3D,0x04,0x01,0x00,
  0x00,0x53,0x75,0x32,0x8B,0x4D,0x18,0x85,0xC9,0x6A,0x04,0x58,0x74,0x02,0x89,0x01,
  0x39,0x45,0x14,0x73,0x0A,0xB8,0x23,0x00,0x00,0xC0,0xE9,0x59,0x01,0x00,0x00,0x8B,
  0x4D,0x0C,0x89,0x01,0x8B,0x45,0x10,0x8B,0x0D
};

static BYTE PatchData[70]=
{
  0x55,0x8B,0xEC,0xB9,0x04,0x01,0x00,0x00,0x2B,0xE1,0x8B,0x45,0x08,0x53,0x3B,0xC1,
  0x74,0x0C,0x49,0x3B,0xC1,0x75,0x2F,0xB8,0x00,0x03,0x80,0x00,0xEB,0x05,0xB8,0x04,
  0x00,0x00,0x00,0x50,0x8B,0x4D,0x18,0x6A,0x04,0x58,0x85,0xC9,0x74,0x02,0x89,0x01,
  0x8B,0x4D,0x0C,0x89,0x01,0x59,0x8B,0x45,0x10,0x89,0x08,0x33,0xC0,0x5B,0xC9,0xC2,
  0x14,0x00,0x00,0x00,0x00,0x00
};

extern "C"	int __stdcall MmQueryAddressProtect(void *Adr);
extern "C"	void __stdcall MmSetAddressProtect(void *Adr, int Size, int Type);  

// for trainers
#define KERNEL_STORE_ADDRESS 0x8000000C // this is address in kernel we store the address of our allocated memory block
#define KERNEL_START_ADDRESS 0x80010000 // base addy of kernel
#define KERNEL_ALLOCATE_ADDRESS 0x7FFD2200 // where we want to put our allocated memory block (under kernel so it works retail)
#define KERNEL_SEARCH_RANGE 0x02AF90 // used for loop control base + search range to look xbe entry point bytes

#define XBTF_HEAP_SIZE 15360 // plenty of room for trainer + xbtf support functions
#define ETM_HEAP_SIZE 2400  // just enough room to match evox's etm spec limit (no need to give them more room then evox does)
// magic kernel patch (asm included w/ source)
static unsigned char trainerloaderdata[167] =
{
       0x60, 0xBA, 0x34, 0x12, 0x00, 0x00, 0x60, 0x6A, 0x01, 0x6A, 0x07, 0xE8, 0x67, 0x00, 0x00, 0x00,
       0x6A, 0x0C, 0x6A, 0x08, 0xE8, 0x5E, 0x00, 0x00, 0x00, 0x61, 0x8B, 0x35, 0x18, 0x01, 0x01, 0x00,
       0x83, 0xC6, 0x08, 0x8B, 0x06, 0x8B, 0x72, 0x12, 0x03, 0xF2, 0xB9, 0x03, 0x00, 0x00, 0x00, 0x3B,
       0x06, 0x74, 0x0C, 0x83, 0xC6, 0x04, 0xE2, 0xF7, 0x68, 0xF0, 0x00, 0x00, 0x00, 0xEB, 0x29, 0x8B,
       0xEA, 0x83, 0x7A, 0x1A, 0x00, 0x74, 0x05, 0x8B, 0x4A, 0x1A, 0xEB, 0x03, 0x8B, 0x4A, 0x16, 0x03,
       0xCA, 0x0F, 0x20, 0xC0, 0x50, 0x25, 0xFF, 0xFF, 0xFE, 0xFF, 0x0F, 0x22, 0xC0, 0xFF, 0xD1, 0x58,
       0x0F, 0x22, 0xC0, 0x68, 0xFF, 0x00, 0x00, 0x00, 0x6A, 0x08, 0xE8, 0x08, 0x00, 0x00, 0x00, 0x61,
       0xFF, 0x15, 0x28, 0x01, 0x01, 0x00, 0xC3, 0x55, 0x8B, 0xEC, 0x66, 0xBA, 0x04, 0xC0, 0xB0, 0x20,
       0xEE, 0x66, 0xBA, 0x08, 0xC0, 0x8A, 0x45, 0x08, 0xEE, 0x66, 0xBA, 0x06, 0xC0, 0x8A, 0x45, 0x0C,
       0xEE, 0xEE, 0x66, 0xBA, 0x02, 0xC0, 0xB0, 0x1A, 0xEE, 0x50, 0xB8, 0x40, 0x42, 0x0F, 0x00, 0x48,
       0x75, 0xFD, 0x58, 0xC9, 0xC2, 0x08, 0x00,
};

#define SIZEOFLOADERDATA 167// loaderdata is our kernel hack to handle if trainer (com file) is executed for title about to run

extern "C" XBOXAPI PVOID * WINAPI MmAllocateContiguousMemoryEx(IN SIZE_T NumberOfBytes, IN ULONG_PTR LowestAcceptableAddress, IN ULONG_PTR HighestAcceptableAddress, IN ULONG_PTR Alignment, IN ULONG Protect);
extern "C" XBOXAPI DWORD WINAPI MmPersistContiguousMemory(IN PVOID BaseAddress, IN SIZE_T NumberOfBytes, IN BOOLEAN Persist);

static unsigned char trainerdata[XBTF_HEAP_SIZE] = { NULL }; // buffer to hold trainer in mem - needs to be global?
// SM_Bus function for xbtf trainers
static unsigned char sm_bus[48] =
{
	0x55, 0x8B, 0xEC, 0x66, 0xBA, 0x04, 0xC0, 0xB0, 0x20, 0xEE, 0x66, 0xBA, 0x08, 0xC0, 0x8A, 0x45, 
	0x08, 0xEE, 0x66, 0xBA, 0x06, 0xC0, 0x8A, 0x45, 0x0C, 0xEE, 0xEE, 0x66, 0xBA, 0x02, 0xC0, 0xB0, 
	0x1A, 0xEE, 0x50, 0xB8, 0x40, 0x42, 0x0F, 0x00, 0x48, 0x75, 0xFD, 0x58, 0xC9, 0xC2, 0x08, 0x00, 
};
 
// PatchIt dynamic patching
static unsigned char patch_it_toy[33] =
{
	0x55, 0x8B, 0xEC, 0x60, 0x0F, 0x20, 0xC0, 0x50, 0x25, 0xFF, 0xFF, 0xFE, 0xFF, 0x0F, 0x22, 0xC0, 
	0x8B, 0x4D, 0x0C, 0x8B, 0x55, 0x08, 0x89, 0x0A, 0x58, 0x0F, 0x22, 0xC0, 0x61, 0xC9, 0xC2, 0x08, 
	0x00, 
};

// HookIt
static unsigned char hookit_toy[43] =
{
	0x55, 0x8B, 0xEC, 0x60, 0x8B, 0x55, 0x08, 0x8B, 0x45, 0x0C, 0xBD, 0x0C, 0x00, 0x00, 0x80, 0x8B, 
	0x6D, 0x00, 0x83, 0xC5, 0x02, 0x8B, 0x6D, 0x00, 0x8D, 0x44, 0x05, 0x00, 0xC6, 0x02, 0x68, 0x89, 
	0x42, 0x01, 0xC6, 0x42, 0x05, 0xC3, 0x61, 0xC9, 0xC2, 0x08, 0x00, 
};

// in game keys (main)
static unsigned char igk_main_toy[403] =
{
	0x81, 0x3D, 0x4C, 0x01, 0x01, 0x00, 0xA4, 0x01, 0x00, 0x00, 0x74, 0x30, 0xC7, 0x05, 0x4C, 0x01, 
	0x01, 0x00, 0xA4, 0x01, 0x00, 0x00, 0xC7, 0x05, 0x14, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 
	0x60, 0xBA, 0x50, 0x01, 0x01, 0x00, 0xB9, 0x05, 0x00, 0x00, 0x00, 0xC7, 0x02, 0x00, 0x00, 0x00, 
	0x00, 0x83, 0xC2, 0x04, 0xE2, 0xF5, 0x61, 0xE9, 0x4B, 0x01, 0x00, 0x00, 0x51, 0x8D, 0x4A, 0x08, 
	0x60, 0x33, 0xC0, 0x8A, 0x41, 0x0C, 0x50, 0x8D, 0x15, 0x14, 0x00, 0x00, 0x80, 0x8B, 0x0A, 0x89, 
	0x42, 0x04, 0x3B, 0xC1, 0x0F, 0x84, 0x1F, 0x01, 0x00, 0x00, 0x66, 0x25, 0x81, 0x00, 0x66, 0x3D, 
	0x81, 0x00, 0x75, 0x0C, 0x80, 0x35, 0x51, 0x01, 0x01, 0x00, 0x01, 0xE9, 0x09, 0x01, 0x00, 0x00, 
	0x58, 0x50, 0x66, 0x25, 0x82, 0x00, 0x66, 0x3D, 0x82, 0x00, 0x75, 0x0C, 0x80, 0x35, 0x52, 0x01, 
	0x01, 0x00, 0x01, 0xE9, 0xF1, 0x00, 0x00, 0x00, 0x58, 0x50, 0x66, 0x25, 0x84, 0x00, 0x66, 0x3D, 
	0x84, 0x00, 0x75, 0x0C, 0x80, 0x35, 0x53, 0x01, 0x01, 0x00, 0x01, 0xE9, 0xD9, 0x00, 0x00, 0x00, 
	0x58, 0x50, 0x66, 0x25, 0x88, 0x00, 0x66, 0x3D, 0x88, 0x00, 0x75, 0x0C, 0x80, 0x35, 0x54, 0x01, 
	0x01, 0x00, 0x01, 0xE9, 0xC1, 0x00, 0x00, 0x00, 0x58, 0x50, 0x66, 0x83, 0xE0, 0x41, 0x66, 0x83, 
	0xF8, 0x41, 0x75, 0x0C, 0x80, 0x35, 0x55, 0x01, 0x01, 0x00, 0x01, 0xE9, 0xA9, 0x00, 0x00, 0x00, 
	0x58, 0x50, 0x66, 0x83, 0xE0, 0x42, 0x66, 0x83, 0xF8, 0x42, 0x75, 0x0C, 0x80, 0x35, 0x56, 0x01, 
	0x01, 0x00, 0x01, 0xE9, 0x91, 0x00, 0x00, 0x00, 0x58, 0x50, 0x66, 0x83, 0xE0, 0x44, 0x66, 0x83, 
	0xF8, 0x44, 0x75, 0x09, 0x80, 0x35, 0x57, 0x01, 0x01, 0x00, 0x01, 0xEB, 0x7C, 0x58, 0x50, 0x66, 
	0x83, 0xE0, 0x48, 0x66, 0x83, 0xF8, 0x48, 0x75, 0x09, 0x80, 0x35, 0x58, 0x01, 0x01, 0x00, 0x01, 
	0xEB, 0x67, 0x58, 0x50, 0x66, 0x25, 0xC0, 0x00, 0x66, 0x3D, 0xC0, 0x00, 0x75, 0x09, 0x80, 0x35, 
	0x59, 0x01, 0x01, 0x00, 0x01, 0xEB, 0x52, 0x58, 0x50, 0x66, 0x83, 0xE0, 0x60, 0x66, 0x83, 0xF8, 
	0x60, 0x75, 0x09, 0x80, 0x35, 0x5A, 0x01, 0x01, 0x00, 0x01, 0xEB, 0x3D, 0x58, 0x50, 0x66, 0x83, 
	0xE0, 0x50, 0x66, 0x83, 0xF8, 0x50, 0x75, 0x09, 0x80, 0x35, 0x5B, 0x01, 0x01, 0x00, 0x01, 0xEB, 
	0x28, 0x58, 0x50, 0x66, 0x25, 0xA0, 0x00, 0x66, 0x3D, 0xA0, 0x00, 0x75, 0x09, 0x80, 0x35, 0x5C, 
	0x01, 0x01, 0x00, 0x01, 0xEB, 0x13, 0x58, 0x50, 0x66, 0x25, 0x90, 0x00, 0x66, 0x3D, 0x90, 0x00, 
	0x75, 0x07, 0x80, 0x35, 0x5D, 0x01, 0x01, 0x00, 0x01, 0x58, 0x8D, 0x15, 0x14, 0x00, 0x00, 0x80, 
	0x8B, 0x42, 0x04, 0x89, 0x02, 0x61, 0x59, 0x5B, 0xB9, 0x10, 0x00, 0x00, 0x80, 0xFF, 0x11, 0x53, 
	0x33, 0xDB, 0xC3, 
};

// HOOKIGK (moves stock pad call and patches game igk_main to use correct register)
static unsigned char hook_igk_toy[76] =
{
	0x55, 0x8B, 0xEC, 0x60, 0xBA, 0x34, 0x12, 0x00, 0x00, 0x8B, 0x45, 0x08, 0xB9, 0x20, 0x00, 0x00, 
	0x00, 0x8A, 0x18, 0x80, 0xFB, 0x50, 0x7C, 0x07, 0x80, 0xFB, 0x53, 0x7F, 0x02, 0xEB, 0x05, 0x48, 
	0xE2, 0xEF, 0xEB, 0x23, 0x80, 0xEB, 0x08, 0x88, 0x5A, 0x3E, 0x83, 0x45, 0x08, 0x01, 0x8B, 0x45, 
	0x08, 0x50, 0x03, 0x00, 0x83, 0xC0, 0x04, 0x2B, 0x55, 0x08, 0x83, 0xEA, 0x04, 0xBB, 0x10, 0x00, 
	0x00, 0x80, 0x89, 0x03, 0x58, 0x89, 0x10, 0x61, 0xC9, 0xC2, 0x04, 0x00, 
};

// SmartXX / Aladin4 functions
static unsigned char lcd_toy_xx[378] =
{
	0x55, 0x8B, 0xEC, 0x90, 0x66, 0x8B, 0x55, 0x08, 0x90, 0x8A, 0x45, 0x0C, 0x90, 0xEE, 0x90, 0x90, 
	0xC9, 0xC2, 0x08, 0x00, 0x55, 0x8B, 0xEC, 0x60, 0x33, 0xC0, 0x33, 0xDB, 0x0F, 0xB6, 0x45, 0x08, 
	0xC1, 0xF8, 0x02, 0x83, 0xE0, 0x28, 0xA2, 0x40, 0x01, 0x01, 0x00, 0x0F, 0xB6, 0x45, 0x08, 0x83, 
	0xE0, 0x50, 0x0F, 0xB6, 0x0D, 0x40, 0x01, 0x01, 0x00, 0x0B, 0xC8, 0x88, 0x0D, 0x40, 0x01, 0x01, 
	0x00, 0x0F, 0xB6, 0x45, 0x08, 0xC1, 0xE0, 0x02, 0x83, 0xE0, 0x28, 0xA2, 0x41, 0x01, 0x01, 0x00, 
	0x0F, 0xB6, 0x45, 0x08, 0xC1, 0xE0, 0x04, 0x83, 0xE0, 0x50, 0x0F, 0xB6, 0x0D, 0x41, 0x01, 0x01, 
	0x00, 0x0B, 0xC8, 0x88, 0x0D, 0x41, 0x01, 0x01, 0x00, 0x0F, 0xB6, 0x45, 0x0C, 0x83, 0xE0, 0x02, 
	0x0F, 0xB6, 0x0D, 0x40, 0x01, 0x01, 0x00, 0x0B, 0xC1, 0x50, 0x68, 0x00, 0xF7, 0x00, 0x00, 0xE8, 
	0x7C, 0xFF, 0xFF, 0xFF, 0x0F, 0xB6, 0x45, 0x0C, 0x83, 0xE0, 0x02, 0x83, 0xC8, 0x04, 0x0F, 0xB6, 
	0x0D, 0x40, 0x01, 0x01, 0x00, 0x0B, 0xC1, 0x50, 0x68, 0x00, 0xF7, 0x00, 0x00, 0xE8, 0x5E, 0xFF, 
	0xFF, 0xFF, 0x0F, 0xB6, 0x45, 0x0C, 0x83, 0xE0, 0x02, 0x0F, 0xB6, 0x0D, 0x40, 0x01, 0x01, 0x00, 
	0x0B, 0xC1, 0x50, 0x68, 0x00, 0xF7, 0x00, 0x00, 0xE8, 0x43, 0xFF, 0xFF, 0xFF, 0x0F, 0xB6, 0x45, 
	0x0C, 0x83, 0xE0, 0x01, 0x75, 0x6A, 0x0F, 0xB6, 0x45, 0x0C, 0x83, 0xE0, 0x02, 0x0F, 0xB6, 0x0D, 
	0x41, 0x01, 0x01, 0x00, 0x0B, 0xC1, 0x50, 0x68, 0x00, 0xF7, 0x00, 0x00, 0xE8, 0x1F, 0xFF, 0xFF, 
	0xFF, 0x0F, 0xB6, 0x45, 0x0C, 0x83, 0xE0, 0x02, 0x83, 0xC8, 0x04, 0x0F, 0xB6, 0x0D, 0x41, 0x01, 
	0x01, 0x00, 0x0B, 0xC1, 0x50, 0x68, 0x00, 0xF7, 0x00, 0x00, 0xE8, 0x01, 0xFF, 0xFF, 0xFF, 0x0F, 
	0xB6, 0x45, 0x0C, 0x83, 0xE0, 0x02, 0x0F, 0xB6, 0x0D, 0x41, 0x01, 0x01, 0x00, 0x0B, 0xC1, 0x50, 
	0x68, 0x00, 0xF7, 0x00, 0x00, 0xE8, 0xE6, 0xFE, 0xFF, 0xFF, 0x0F, 0xB6, 0x45, 0x08, 0x25, 0xFC, 
	0x00, 0x00, 0x00, 0x75, 0x0B, 0xF4, 0x90, 0x90, 0x90, 0x90, 0x90, 0xF4, 0x90, 0x90, 0x90, 0x90, 
	0xF4, 0x90, 0x90, 0x90, 0x90, 0x90, 0xF4, 0x90, 0x90, 0x90, 0x90, 0x90, 0x61, 0xC9, 0xC2, 0x08, 
	0x00, 0x6A, 0x00, 0x6A, 0x01, 0xE8, 0xCA, 0xFE, 0xFF, 0xFF, 0xC3, 0x55, 0x8B, 0xEC, 0x60, 0xB1, 
	0x80, 0x0A, 0x4D, 0x0C, 0x6A, 0x00, 0x8A, 0xC1, 0x50, 0xE8, 0xB6, 0xFE, 0xFF, 0xFF, 0x33, 0xC9, 
	0x8B, 0x55, 0x08, 0x8A, 0x04, 0x11, 0x3C, 0x00, 0x74, 0x0B, 0x6A, 0x02, 0x50, 0xE8, 0xA2, 0xFE, 
	0xFF, 0xFF, 0x41, 0xEB, 0xEE, 0x61, 0xC9, 0xC2, 0x08, 0x00, 
};

// X3 LCD functions
static unsigned char lcd_toy_x3[246] =
{
	0x55, 0x8B, 0xEC, 0x90, 0x66, 0x8B, 0x55, 0x08, 0x90, 0x8A, 0x45, 0x0C, 0x90, 0xEE, 0x90, 0x90, 
	0xC9, 0xC2, 0x08, 0x00, 0x55, 0x8B, 0xEC, 0x60, 0x50, 0x33, 0xC0, 0x8A, 0x45, 0x0C, 0x24, 0x02, 
	0x3C, 0x00, 0x74, 0x05, 0x32, 0xE4, 0x80, 0xCC, 0x01, 0x8A, 0x45, 0x08, 0x24, 0xF0, 0x50, 0x68, 
	0x04, 0xF5, 0x00, 0x00, 0xE8, 0xC7, 0xFF, 0xFF, 0xFF, 0x8A, 0xC4, 0x50, 0x68, 0x05, 0xF5, 0x00, 
	0x00, 0xE8, 0xBA, 0xFF, 0xFF, 0xFF, 0x50, 0x80, 0xCC, 0x04, 0x8A, 0xC4, 0x50, 0x68, 0x05, 0xF5, 
	0x00, 0x00, 0xE8, 0xA9, 0xFF, 0xFF, 0xFF, 0x58, 0x8A, 0xC4, 0x50, 0x68, 0x05, 0xF5, 0x00, 0x00, 
	0xE8, 0x9B, 0xFF, 0xFF, 0xFF, 0x8A, 0x45, 0x0C, 0x24, 0x01, 0x3C, 0x00, 0x75, 0x3D, 0x8A, 0x45, 
	0x08, 0xC0, 0xE0, 0x04, 0x50, 0x68, 0x04, 0xF5, 0x00, 0x00, 0xE8, 0x81, 0xFF, 0xFF, 0xFF, 0x8A, 
	0xC4, 0x50, 0x68, 0x05, 0xF5, 0x00, 0x00, 0xE8, 0x74, 0xFF, 0xFF, 0xFF, 0x50, 0x80, 0xCC, 0x04, 
	0x8A, 0xC4, 0x50, 0x68, 0x05, 0xF5, 0x00, 0x00, 0xE8, 0x63, 0xFF, 0xFF, 0xFF, 0x58, 0x8A, 0xC4, 
	0x50, 0x68, 0x05, 0xF5, 0x00, 0x00, 0xE8, 0x55, 0xFF, 0xFF, 0xFF, 0x58, 0xF4, 0x90, 0x90, 0x90, 
	0x90, 0x90, 0xF4, 0x90, 0x90, 0x90, 0x90, 0x90, 0x61, 0xC9, 0xC2, 0x08, 0x00, 0x6A, 0x00, 0x6A, 
	0x01, 0xE8, 0x4E, 0xFF, 0xFF, 0xFF, 0xC3, 0x55, 0x8B, 0xEC, 0x60, 0xB1, 0x80, 0x0A, 0x4D, 0x0C, 
	0x6A, 0x00, 0x8A, 0xC1, 0x50, 0xE8, 0x3A, 0xFF, 0xFF, 0xFF, 0x33, 0xC9, 0x8B, 0x55, 0x08, 0x8A, 
	0x04, 0x11, 0x3C, 0x00, 0x74, 0x0B, 0x6A, 0x02, 0x50, 0xE8, 0x26, 0xFF, 0xFF, 0xFF, 0x41, 0xEB, 
	0xEE, 0x61, 0xC9, 0xC2, 0x08, 0x00, 
};

using namespace std;
using namespace PLAYLIST;

class CUtil
{
public:
  CUtil(void);
  virtual ~CUtil(void);
  static const CStdString GetExtension(const CStdString& strFileName);
  static void RemoveExtension(CStdString& strFileName);
  static bool GetVolumeFromFileName(const CStdString& strFileName, CStdString& strFileTitle, CStdString& strVolumeNumber);
  static void CleanFileName(CStdString& strFileName);
  static const CStdString GetFileName(const CStdString& strFileNameAndPath);  
  static CStdString GetTitleFromPath(const CStdString& strFileNameAndPath);
  static bool IsHD(const CStdString& strFileName);
  static bool IsBuiltIn(const CStdString& execString);
  static void GetBuiltInHelp(CStdString &help);
  static int ExecBuiltIn(const CStdString& execString);
  static bool GetParentPath(const CStdString& strPath, CStdString& strParent);
  static void GetQualifiedFilename(const CStdString &strBasePath, CStdString &strFilename);
  static bool InstallTrainer(CTrainer& trainer);
  static bool RemoveTrainer();
  static bool PatchCountryVideo(F_COUNTRY Country, F_VIDEO Video);
  static void RunXBE(const char* szPath, char* szParameters = NULL, F_VIDEO ForceVideo=VIDEO_NULL, F_COUNTRY ForceCountry=COUNTRY_NULL);
  static void LaunchXbe(const char* szPath, const char* szXbe, const char* szParameters, F_VIDEO ForceVideo=VIDEO_NULL, F_COUNTRY ForceCountry=COUNTRY_NULL); 
  static void GetDirectory(const CStdString& strFilePath, CStdString& strDirectoryPath);
  static void GetThumbnail(const CStdString& strFileName, CStdString& strThumb);
  static void GetCachedThumbnail(const CStdString& strFileName, CStdString& strCachedThumb);
  static void GetHomePath(CStdString& strPath);
  static bool InitializeNetwork(int iAssignment, const char* szLocalAddress, const char* szLocalSubnet, const char* szLocalGateway, const char* szNameServer);
  static bool IsEthernetConnected();
  static void GetTitleIP(CStdString& ip);
  static void ConvertTimeTToFileTime(__int64 sec, long nsec, FILETIME &ftTime);
  static __int64 CompareSystemTime(const SYSTEMTIME *a, const SYSTEMTIME *b);
  static void ReplaceExtension(const CStdString& strFile, const CStdString& strNewExtension, CStdString& strChangedFile);
  static void GetExtension(const CStdString& strFile, CStdString& strExtension);
  static bool HasSlashAtEnd(const CStdString& strFile);
  static bool IsRemote(const CStdString& strFile);
  static bool IsOnDVD(const CStdString& strFile);
  static bool IsDVD(const CStdString& strFile);
  static bool IsVirtualPath(const CStdString& strFile);
  static bool IsStack(const CStdString& strFile);
  static bool IsRAR(const CStdString& strFile);
  static bool IsInRAR(const CStdString& strFile);
  static bool IsZIP(const CStdString& strFile);
  static bool IsInZIP(const CStdString& strFile);
  static bool IsCDDA(const CStdString& strFile);
  static void GetFileAndProtocol(const CStdString& strURL, CStdString& strDir);
  static int GetDVDIfoTitle(const CStdString& strPathFile);
  static void UrlDecode(CStdString& strURLData);
  static void URLEncode(CStdString& strURLData);
  static bool LoadString(string &strTxt, FILE *fd);
  static int LoadString(CStdString &strTxt, byte* pBuffer);
  static void SaveString(const CStdString &strTxt, FILE *fd);
  static void SaveInt(int iValue, FILE *fd);
  static int LoadInt( FILE *fd);
  static void LoadDateTime(SYSTEMTIME& dateTime, FILE *fd);
  static void SaveDateTime(SYSTEMTIME& dateTime, FILE *fd);
  static void GetSongInfo(const CStdString& strFileName, CStdString& strSongCacheName);
  static void GetAlbumFolderThumb(const CStdString& strFileName, CStdString& strAlbumThumb, bool bTempDir = false);
  static void GetAlbumThumb(const CStdString& strAlbumName, const CStdString& strFileName, CStdString& strThumb, bool bTempDir = false);
  static void GetAlbumInfo(const CStdString& strFileName, CStdString& strAlbumThumb);
  static void GetAlbumDatabase(const CStdString& strFileName, CStdString& strAlbumThumb);
  static bool GetXBEIcon(const CStdString& strFilePath, CStdString& strIcon);
  static bool GetXBEDescription(const CStdString& strFileName, CStdString& strDescription);
  static bool SetXBEDescription(const CStdString& strFileName, const CStdString& strDescription);
  static bool GetDirectoryName(const CStdString& strFileName, CStdString& strDescription);
  static DWORD GetXbeID( const CStdString& strFilePath);
  static void CreateShortcuts(CFileItemList &items);
  static void CreateShortcut(CFileItem* pItem);
  static void GetArtistDatabase(const CStdString& strFileName, CStdString& strArtistDBS);
  static void GetGenreDatabase(const CStdString& strFileName, CStdString& strGenreDBS);
  static void GetFatXQualifiedPath(CStdString& strFileNameAndPath);
  static void ShortenFileName(CStdString& strFileNameAndPath);
  static bool IsISO9660(const CStdString& strFile);
  static bool IsSmb(const CStdString& strFile);
  static bool IsDAAP(const CStdString& strFile);
  static void ConvertPathToUrl( const CStdString& strPath, const CStdString& strProtocol, CStdString& strOutUrl );
  static void GetDVDDriveIcon( const CStdString& strPath, CStdString& strIcon );
  static void RemoveTempFiles();
  static void DeleteTDATA();
  static bool GetFolderThumb(const CStdString& strFolder, CStdString& strThumb);

  static void RemoveIllegalChars( CStdString& strText);
  static void CacheSubtitles(const CStdString& strMovie, CStdString& strExtensionCached, XFILE::IFileCallback *pCallback = NULL);
  static bool CacheRarSubtitles(std::vector<CStdString>& vecExtensionsCached, const CStdString& strRarPath, const CStdString& strCompare, const CStdString& strExtExt="");
  static void ClearSubtitles();
  static void PrepareSubtitleFonts();
  static __int64 ToInt64(DWORD dwHigh, DWORD dwLow);
  static void AddFileToFolder(const CStdString& strFolder, const CStdString& strFile, CStdString& strResult);
  static void AddSlashAtEnd(CStdString& strFolder);
  static void RemoveSlashAtEnd(CStdString& strFolder);
  static void GetPath(const CStdString& strFileName, CStdString& strPath);
  static void Split(const CStdString& strFileNameAndPath, CStdString& strPath, CStdString& strFileName);
  static void CreateRarPath(CStdString& strUrlPath, const CStdString& strRarPath, 
    const CStdString& strFilePathInRar,  const WORD wOptions = EXFILE_AUTODELETE , 
    const CStdString& strPwd = RAR_DEFAULT_PASSWORD, const CStdString& strCachePath = RAR_DEFAULT_CACHE);
  static bool ThumbExists(const CStdString& strFileName, bool bAddCache = false);
  static bool ThumbCached(const CStdString& strFileName);
  static void ThumbCacheAdd(const CStdString& strFileName, bool bFileExists);
  static void ThumbCacheClear();
  static void PlayDVD();
  static DWORD SetUpNetwork( bool resetmode, struct network_info& networkinfo );
  static void GetVideoThumbnail(const CStdString& strIMDBID, CStdString& strThumb);
  static CStdString GetNextFilename(const char* fn_template, int max);
  static void TakeScreenshot();
  static void TakeScreenshot(const char* fn, bool flash);
  static void SetBrightnessContrastGamma(float Brightness, float Contrast, float Gamma, bool bImmediate);
  static void SetBrightnessContrastGammaPercent(int iBrightNess, int iContrast, int iGamma, bool bImmediate);
  static void Tokenize(const CStdString& path, vector<CStdString>& tokens, const string& delimiters);
  static void FlashScreen(bool bImmediate, bool bOn);
  static void RestoreBrightnessContrastGamma();
  static void InitGamma();
  static void ClearCache();
  static bool IsNetworkUp() { return m_bNetworkUp; }
  static void StatToStatI64(struct _stati64 *result, struct stat *stat);
  static void Stat64ToStatI64(struct _stati64 *result, struct __stat64 *stat);
  static void StatI64ToStat64(struct __stat64 *result, struct _stati64 *stat);
  static void Stat64ToStat(struct _stat *result, struct __stat64 *stat);
  static bool CreateDirectoryEx(const CStdString& strPath);
  static CStdString MakeLegalFileName(const char* strFile, bool bKeepExtension, bool isFATX = true);
  static void ConvertFileItemToPlayListItem(const CFileItem *pItem, CPlayList::CPlayListItem &playlistitem);
  static void AddDirectorySeperator(CStdString& strPath);
  static char GetDirectorySeperator(const CStdString& strFile);

  static bool IsUsingTTFSubtitles();
  static void SplitExecFunction(const CStdString &execString, CStdString &strFunction, CStdString &strParam);
  static int GetMatchingShare(const CStdString& strPath, VECSHARES& vecShares, bool& bIsBookmarkName);
  static CStdString TranslateSpecialDir(const CStdString &strSpecial);
  static void DeleteDatabaseDirectoryCache();
  static CStdString MusicPlaylistsLocation();
  static CStdString VideoPlaylistsLocation();
  static CStdString SubstitutePath(const CStdString& strFileName);

  //GeminiServer
  static bool IsLeapYear(int iLYear, int iLMonth, int iLTag, int &iMonMax, int &iWeekDay);
  static bool SetSysDateTimeYear(int iYear, int iMonth, int iDay, int iHour, int iMinute);
  static bool XboxAutoDetectionPing(bool bRefresh, CStdString strFTPUserName, CStdString strFTPPass, CStdString strNickName, int iFTPPort, CStdString &strHasClientIP, CStdString &strHasClientInfo, CStdString &strNewClientIP, CStdString &strNewClientInfo );
  static bool XboxAutoDetection();
  static bool IsFTP(const CStdString& strFile);
  static bool GetFTPServerUserName(int iFTPUserID, CStdString &strFtpUser1, int &iUserMax );
  static bool SetFTPServerUserPassword(CStdString strFtpUserName, CStdString strFtpUserPassword);
  static bool SetXBOXNickName(CStdString strXboxNickNameIn, CStdString &strXboxNickNameOut);
  static bool GetXBOXNickName(CStdString &strXboxNickNameOut);

  static void GetRecursiveListing(const CStdString& strPath, CFileItemList& items, const CStdString& strMask);
  
  static void GetRecursiveDirsListing(const CStdString& strPath, CFileItemList& items);
  static void ForceForwardSlashes(CStdString& strPath);

  static double AlbumRelevance(const CStdString& strAlbumTemp1, const CStdString& strAlbum1, const CStdString& strArtistTemp1, const CStdString& strArtist1);
  static bool MakeShortenPath(CStdString StrInput, CStdString& StrOutput, int iTextMaxLength);
  static float CurrentCpuUsage();
  static bool SupportsFileOperations(const CStdString& strPath);
  static void GetDHCPInfo(CStdString& dns2, CStdString& dhcpserver);
  
private:
  static bool m_bNetworkUp;
  static HANDLE m_hCurrentCpuUsage;
};
