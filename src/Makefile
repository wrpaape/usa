# ══════════════════════════════════════════════════════════════════════════════
# ──────────────────────────────────────────────────────────────────────────────
#				CONFIGURATION
# ──────────────────────────────────────────────────────────────────────────────
# ══════════════════════════════════════════════════════════════════════════════
#
#
## MISC
# ══════════════════════════════════════════════════════════════════════════════
# Phony Targets
# ──────────────────────────────────────────────────────────────────────────────
.PHONY: all clean


# String Utils
# ──────────────────────────────────────────────────────────────────────────────
EMPTY	       :=
SPACE	       := $(EMPTY) $(EMPTY)
TAB	       := $(EMPTY)	$(EMPTY)
TRIM		= $(subst $(SPACE),$(EMPTY),$1)
JOIN	        = $(subst $(SPACE),$2,$(strip $1))
define NEWLINE :=


endef

# SYSTEM CONFIG
# ══════════════════════════════════════════════════════════════════════════════
# extended from user 'Trevor Robinson''s response to stackoverflow question:
#
# 	http://stackoverflow.com/questions/714100/os-detecting-makefile
ifeq ($(OS),Windows_NT)
        SYSTEM_WINDOWS 	:= T
        SYS_ENV_FLAGS 	:= -DWIN32

        ifeq      (AMD64,$(PROCESSOR_ARCHITECTURE))
                ARCH_X86_64	:= T
                SYS_ARCH_FLAG	:= -arch x86_64
                SYS_ENV_FLAGS	+= -DAMD_64

        else ifeq (x86,$(PROCESSOR_ARCHITECTURE))
                ARCH_I386	:= T
                SYS_ARCH_FLAG	:= -arch i386
                SYS_ENV_FLAGS	+= -DIA_32
        endif
else
        UNAME_S := $(shell uname -s)

        ifeq      ($(UNAME_S),Linux)
                SYSTEM_LINUX 	:= T
                SYS_ENV_FLAGS 	:= -DLINUX

        else ifeq ($(UNAME_S),Darwin)
                SYSTEM_OSX 	:= T
                SYS_ENV_FLAGS 	:= -DOSX
        endif

        UNAME_M := $(shell uname -m)

        ifeq      (x86_64,$(UNAME_M))
                ARCH_X86_64	:= T
                SYS_ARCH_FLAG	:= -arch x86_64
                SYS_ENV_FLAGS	+= -DAMD_64

        else ifneq (,$(filter %86,$(UNAME_M)))
                ARCH_I386	:= T
                SYS_ARCH_FLAG	:= -arch i386
                SYS_ENV_FLAGS 	+= -DIA_32

        else ifneq (,$(filter arm%,$(UNAME_M)))

                ifneq (,$(filter %32%,$(UNAME_M)))
                        ARCH_ARM_32	:= T
                        SYS_ARCH_FLAG	:= -arch AArch32
                        SYS_ENV_FLAGS 	+= -DARM_32
                else
                        ARCH_ARM_64	:= T
                        SYS_ARCH_FLAG	:= -arch AArch64
                        SYS_ENV_FLAGS 	+= -DARM_64
                endif
        endif
endif


# Path delimiter, chain commands token, executable file extension
# ──────────────────────────────────────────────────────────────────────────────
ifeq (T,$(SYSTEM_WINDOWS))
PATH_DELIM	:= \\
CMD_DELIM 	:= &
BIN_EXT		:= .exe
else
PATH_DELIM	:= /
CMD_DELIM 	:= ;
BIN_EXT		:= $(EMPTY)
endif



# DIRECTORY CONFIG
# ══════════════════════════════════════════════════════════════════════════════
PATH_JOIN		 = $(call JOIN,$1,$(PATH_DELIM))
PROJECT_ROOT		:= ..
SOURCE_DIR		:= $(call PATH_JOIN,$(PROJECT_ROOT) src)
HEADER_DIR		:= $(SOURCE_DIR)
OBJECT_DIR		:= $(call PATH_JOIN,$(PROJECT_ROOT) obj)
BINARY_DIR		:= $(call PATH_JOIN,$(PROJECT_ROOT) bin)
DATA_DIR		:= $(realpath $(call PATH_JOIN,$(PROJECT_ROOT) data))

# PATH CONFIG
# ══════════════════════════════════════════════════════════════════════════════
FILE_PATH		= $(call PATH_JOIN,$1 $(call TRIM,$2))
SOURCE_FILE_PATH	= $(call FILE_PATH,$1,$2.c)
HEADER_FILE_PATH	= $(call FILE_PATH,$1,$2.h)
OBJECT_FILE_PATH	= $(call FILE_PATH,$1,$2.o)
BINARY_FILE_PATH	= $(call FILE_PATH,$1,$2$(BIN_EXT))
DATA_FILE_PATH		= $(call FILE_PATH,$1,$2.tsv)
SOLUTION_FILE_PATH	= $(call FILE_PATH,$1,$2.txt)

# Project
# ─────────────── source ───────────────────────────────────────────────────────
SOURCE_PATH		= $(call SOURCE_FILE_PATH,$(SOURCE_DIR),$(call PATH_JOIN,$1))# implementation
HEADER_PATH		= $(call HEADER_FILE_PATH,$(HEADER_DIR),$(call PATH_JOIN,$1))# interface
# ─────────────── build ────────────────────────────────────────────────────────
OBJECT_PATH   		= $(call OBJECT_FILE_PATH,$(OBJECT_DIR),$(call JOIN,$1,_))# object files
BINARY_PATH 		= $(call BINARY_FILE_PATH,$(BINARY_DIR),$(call JOIN,$1,_))# binary executable output
# ─────────────── data ────────────────────────────────────────────────────────
DATA_PATH		= $(call DATA_FILE_PATH,$(DATA_DIR),$1)
SOLUTION_PATH		= $(call SOLUTION_FILE_PATH,$(DATA_DIR),$1)


# DATA FILE CONFIG
# ══════════════════════════════════════════════════════════════════════════════
COORDINATES_PATH	:= $(call TRIM,"\"$(call DATA_PATH,coordinates)\"")
DISTANCES_PATH		:= $(call TRIM,"\"$(call DATA_PATH,distances)\"")
SOLUTION_PATH		:= $(call TRIM,"\"$(call SOLUTION_PATH,solution)\"")


# UTILITY CONFIG
# ══════════════════════════════════════════════════════════════════════════════
# C Compiler
# ──────────────────────────────────────────────────────────────────────────────
CC		:= gcc
CC_ENV_FLAGS	:= -D__USE_FIXED_PROTOTYPES__ -D_GNU_SOURCE $(SYS_ENV_FLAGS)
CC_BASE_FLAGS	:= -std=gnu99 -march=native $(CC_ENV_FLAGS) -I$(HEADER_DIR)
CC_FLAGS	:= -O2 -funroll-loops $(CC_BASE_FLAGS)

# Linker
# ──────────────────────────────────────────────────────────────────────────────
ifeq (T,$(SYSTEM_OSX))
LD		:= ld
LD_FLAGS	:= -macosx_version_min 10.11 -no_pie $(SYS_ARCH_FLAG)
LD_LIB_FLAGS	:= -lc
else
LD		:= gcc
LD_FLAGS	:= $(EMPTY)
LD_LIB_FLAGS	:= $(EMPTY)
endif

# Cleaner
# ──────────────────────────────────────────────────────────────────────────────
ifeq (T,$(SYSTEM_WINDOWS))
RM		:= cmd \/C del
RM_FLAGS	:= /F /Q
else
RM		:= rm
RM_FLAGS	:= -rf
endif



# COMPILATION MODULES
# ══════════════════════════════════════════════════════════════════════════════
# independent headers
# ──────────────────────────────────────────────────────────────────────────────
UTILS_TYPES_UINT128_HDR		:= $(call HEADER_PATH,utils types uint128)



# utils/utils
# ──────────────────────────────────────────────────────────────────────────────
UTILS_UTILS_SRC 	:= $(call SOURCE_PATH,utils utils)
UTILS_UTILS_HDR		:= $(call HEADER_PATH,utils utils)
UTILS_UTILS_OBJ 	:= $(call OBJECT_PATH,utils utils)
# ─────────────── target prequisites ───────────────────────────────────────────
UTILS_UTILS_OBJ_PREQS	:= $(UTILS_UTILS_SRC) 	\
		   	   $(UTILS_UTILS_HDR)
UTILS_UTILS_OBJ_GROUP	:= $(UTILS_UTILS_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(UTILS_UTILS_OBJ)



# utils/uint128_ops
# ──────────────────────────────────────────────────────────────────────────────
UTILS_UINT128_OPS_SRC 		:= $(call SOURCE_PATH,utils uint128_ops)
UTILS_UINT128_OPS_HDR		:= $(call HEADER_PATH,utils uint128_ops)
UTILS_UINT128_OPS_OBJ 		:= $(call OBJECT_PATH,utils uint128_ops)
# ─────────────── target prequisites ───────────────────────────────────────────
UTILS_UINT128_OPS_OBJ_PREQS	:= $(UTILS_UINT128_OPS_SRC) 	\
			   	   $(UTILS_UINT128_OPS_HDR)	\
			   	   $(UTILS_TYPES_UINT128_HDR)
UTILS_UINT128_OPS_OBJ_GROUP	:= $(UTILS_UINT128_OPS_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(UTILS_UINT128_OPS_OBJ)



# bitwise/bitwise_utils
# ──────────────────────────────────────────────────────────────────────────────
BITWISE_BITWISE_UTILS_SRC 	:= $(call SOURCE_PATH,bitwise bitwise_utils)
BITWISE_BITWISE_UTILS_HDR	:= $(call HEADER_PATH,bitwise bitwise_utils)
BITWISE_BITWISE_UTILS_OBJ 	:= $(call OBJECT_PATH,bitwise bitwise_utils)
# ─────────────── target prequisites ───────────────────────────────────────────
BITWISE_BITWISE_UTILS_OBJ_PREQS	:= $(BITWISE_BITWISE_UTILS_SRC) \
			      	   $(BITWISE_BITWISE_UTILS_HDR)
BITWISE_BITWISE_UTILS_OBJ_GROUP	:= $(BITWISE_BITWISE_UTILS_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(BITWISE_BITWISE_UTILS_OBJ)



# time/time_utils
# ──────────────────────────────────────────────────────────────────────────────
TIME_TIME_UTILS_SRC 		:= $(call SOURCE_PATH,time time_utils)
TIME_TIME_UTILS_HDR		:= $(call HEADER_PATH,time time_utils)
TIME_TIME_UTILS_OBJ 		:= $(call OBJECT_PATH,time time_utils)
# ─────────────── target prequisites ───────────────────────────────────────────
TIME_TIME_UTILS_OBJ_PREQS	:= $(TIME_TIME_UTILS_SRC) 	\
			   	   $(TIME_TIME_UTILS_HDR)
TIME_TIME_UTILS_OBJ_GROUP	:= $(TIME_TIME_UTILS_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(TIME_TIME_UTILS_OBJ)



# random/pcg_random
# ──────────────────────────────────────────────────────────────────────────────
RANDOM_PCG_RANDOM_UTILS_SRC 		:= $(call SOURCE_PATH,random pcg_random)
RANDOM_PCG_RANDOM_UTILS_HDR		:= $(call HEADER_PATH,random pcg_random)
RANDOM_PCG_RANDOM_UTILS_OBJ 		:= $(call OBJECT_PATH,random pcg_random)
# ─────────────── target prequisites ───────────────────────────────────────────
RANDOM_PCG_RANDOM_UTILS_OBJ_PREQS	:= $(RANDOM_PCG_RANDOM_UTILS_SRC) 	\
			   	   	   $(RANDOM_PCG_RANDOM_UTILS_HDR)	\
			   	   	   $(BITWISE_BITWISE_UTILS_HDR)		\
			   	   	   $(UTILS_UINT128_OPS_HDR)
RANDOM_PCG_RANDOM_UTILS_OBJ_GROUP	:= $(RANDOM_PCG_RANDOM_UTILS_OBJ)	\
			   	   	   $(BITWISE_BITWISE_UTILS_OBJ_GROUP)	\
			   	   	   $(UTILS_UINT128_OPS_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS					+= $(RANDOM_PCG_RANDOM_UTILS_OBJ)



# random/random
# ──────────────────────────────────────────────────────────────────────────────
RANDOM_RANDOM_UTILS_SRC 	:= $(call SOURCE_PATH,random random_utils)
RANDOM_RANDOM_UTILS_HDR		:= $(call HEADER_PATH,random random_utils)
RANDOM_RANDOM_UTILS_OBJ 	:= $(call OBJECT_PATH,random random_utils)
# ─────────────── target prequisites ───────────────────────────────────────────
RANDOM_RANDOM_UTILS_OBJ_PREQS	:= $(RANDOM_RANDOM_UTILS_SRC) 			\
			   	   $(RANDOM_RANDOM_UTILS_HDR)			\
			   	   $(RANDOM_PCG_RANDOM_UTILS_HDR)		\
			   	   $(TIME_TIME_UTILS_HDR)
RANDOM_RANDOM_UTILS_OBJ_GROUP	:= $(RANDOM_RANDOM_UTILS_OBJ)			\
			   	   $(RANDOM_PCG_RANDOM_UTILS_OBJ_GROUP)		\
			   	   $(TIME_TIME_UTILS_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(RANDOM_RANDOM_UTILS_OBJ)



# string/string_utils
# ──────────────────────────────────────────────────────────────────────────────
STRING_STRING_UTILS_SRC 	:= $(call SOURCE_PATH,string string_utils)
STRING_STRING_UTILS_HDR		:= $(call HEADER_PATH,string string_utils)
STRING_STRING_UTILS_OBJ 	:= $(call OBJECT_PATH,string string_utils)
# ─────────────── target prequisites ───────────────────────────────────────────
STRING_STRING_UTILS_OBJ_PREQS	:= $(STRING_STRING_UTILS_SRC) 	\
			   	   $(STRING_STRING_UTILS_HDR)	\
				   $(UTILS_UTILS_HDR)
STRING_STRING_UTILS_OBJ_GROUP	:= $(STRING_STRING_UTILS_OBJ)	\
				   $(UTILS_UTILS_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(STRING_STRING_UTILS_OBJ)



# file/file_utils
# ──────────────────────────────────────────────────────────────────────────────
FILE_FILE_UTILS_SRC 		:= $(call SOURCE_PATH,file file_utils)
FILE_FILE_UTILS_HDR		:= $(call HEADER_PATH,file file_utils)
FILE_FILE_UTILS_OBJ 		:= $(call OBJECT_PATH,file file_utils)
# ─────────────── target prequisites ───────────────────────────────────────────
FILE_FILE_UTILS_OBJ_PREQS	:= $(FILE_FILE_UTILS_SRC) 	\
			   	   $(FILE_FILE_UTILS_HDR)	\
			   	   $(UTILS_UTILS_HDR)
FILE_FILE_UTILS_OBJ_GROUP	:= $(FILE_FILE_UTILS_OBJ)	\
				   $(UTILS_UTILS_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(FILE_FILE_UTILS_OBJ)



# signal/signal_utils
# ──────────────────────────────────────────────────────────────────────────────
SIGNAL_SIGNAL_UTILS_SRC 	:= $(call SOURCE_PATH,signal signal_utils)
SIGNAL_SIGNAL_UTILS_HDR		:= $(call HEADER_PATH,signal signal_utils)
SIGNAL_SIGNAL_UTILS_OBJ 	:= $(call OBJECT_PATH,signal signal_utils)
# ─────────────── target prequisites ───────────────────────────────────────────
SIGNAL_SIGNAL_UTILS_OBJ_PREQS	:= $(SIGNAL_SIGNAL_UTILS_SRC) 	\
			   	   $(SIGNAL_SIGNAL_UTILS_HDR)	\
			   	   $(UTILS_UTILS_HDR)
SIGNAL_SIGNAL_UTILS_OBJ_GROUP	:= $(SIGNAL_SIGNAL_UTILS_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(SIGNAL_SIGNAL_UTILS_OBJ)



# usa/usa_utils
# ──────────────────────────────────────────────────────────────────────────────
USA_USA_UTILS_SRC 	:= $(call SOURCE_PATH,usa usa_utils)
USA_USA_UTILS_HDR	:= $(call HEADER_PATH,usa usa_utils)
USA_USA_UTILS_OBJ 	:= $(call OBJECT_PATH,usa usa_utils)
# ─────────────── target prequisites ───────────────────────────────────────────
USA_USA_UTILS_OBJ_PREQS	:= $(USA_USA_UTILS_SRC) 		\
		  	   $(USA_USA_UTILS_HDR)			\
		   	   $(STRING_STRING_UTILS_HDR)		\
		   	   $(FILE_FILE_UTILS_HDR)
USA_USA_UTILS_OBJ_GROUP	:= $(USA_USA_UTILS_OBJ)			\
		   	   $(STRING_STRING_UTILS_OBJ_GROUP)	\
		   	   $(FILE_FILE_UTILS_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(USA_USA_UTILS_OBJ)



# generate/generate_distances
# ──────────────────────────────────────────────────────────────────────────────
GENERATE_GENERATE_DISTANCES_SRC 	:= $(call SOURCE_PATH,generate generate_distances)
GENERATE_GENERATE_DISTANCES_HDR		:= $(call HEADER_PATH,generate generate_distances)
GENERATE_GENERATE_DISTANCES_OBJ 	:= $(call OBJECT_PATH,generate_distances)
GENERATE_GENERATE_DISTANCES_BIN 	:= $(call BINARY_PATH,generate_distances)
# ─────────────── target prequisites ───────────────────────────────────────────
GENERATE_GENERATE_DISTANCES_BIN_PREQS	:= $(GENERATE_GENERATE_DISTANCES_OBJ)	\
		   	   		   $(USA_USA_UTILS_OBJ_GROUP)
GENERATE_GENERATE_DISTANCES_OBJ_PREQS	:= $(GENERATE_GENERATE_DISTANCES_SRC) 	\
		  	   		   $(GENERATE_GENERATE_DISTANCES_HDR)	\
		   	   		   $(USA_USA_UTILS_HDR)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS					+= $(GENERATE_GENERATE_DISTANCES_OBJ)	\
					   $(GENERATE_GENERATE_DISTANCES_BIN)



# usa (entry point)
# ──────────────────────────────────────────────────────────────────────────────
USA_SRC 	:= $(call SOURCE_PATH,usa)
USA_HDR		:= $(call HEADER_PATH,usa)
USA_OBJ 	:= $(call OBJECT_PATH,usa)
USA_BIN 	:= $(call BINARY_PATH,usa)
# ─────────────── target prequisites ───────────────────────────────────────────
USA_BIN_PREQS	:= $(USA_OBJ)				\
		   $(USA_USA_UTILS_OBJ_GROUP)		\
		   $(RANDOM_RANDOM_UTILS_OBJ_GROUP)	\
		   $(SIGNAL_SIGNAL_UTILS_OBJ_GROUP)
USA_OBJ_PREQS	:= $(USA_SRC) 				\
		   $(USA_HDR)				\
		   $(USA_USA_UTILS_HDR)			\
		   $(RANDOM_RANDOM_UTILS_HDR)		\
		   $(SIGNAL_SIGNAL_UTILS_HDR)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS		+= $(USA_OBJ)				\
		   $(USA_BIN)



# ══════════════════════════════════════════════════════════════════════════════
# ──────────────────────────────────────────────────────────────────────────────
# 				MAKE RULES
# ──────────────────────────────────────────────────────────────────────────────
# ══════════════════════════════════════════════════════════════════════════════
#
#
## COMPOSITE RULES
# ══════════════════════════════════════════════════════════════════════════════
# make all targets
# ──────────────────────────────────────────────────────────────────────────────
all:  $(TARGETS)

# remove all targets
# ──────────────────────────────────────────────────────────────────────────────
clean:
	$(RM) $(RM_FLAGS) $(TARGETS)

## MAKE PROJECT
# ══════════════════════════════════════════════════════════════════════════════
# make usa binary
# ──────────────────────────────────────────────────────────────────────────────
$(USA_BIN): $(USA_BIN_PREQS)
	$(LD) $(LD_FLAGS) $^ $(LD_LIB_FLAGS) -o $@

# make usa object
# ──────────────────────────────────────────────────────────────────────────────
$(USA_OBJ): $(USA_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -DDISTANCES_PATH=$(DISTANCES_PATH) -DSOLUTION_PATH=$(SOLUTION_PATH) -c $< -o $@

# make generate_distances binary
# ──────────────────────────────────────────────────────────────────────────────
$(GENERATE_GENERATE_DISTANCES_BIN): $(GENERATE_GENERATE_DISTANCES_BIN_PREQS)
	$(LD) $(LD_FLAGS) $^ $(LD_LIB_FLAGS) -o $@

# make generate_distances object
# ──────────────────────────────────────────────────────────────────────────────
$(GENERATE_GENERATE_DISTANCES_OBJ): $(GENERATE_GENERATE_DISTANCES_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -DDISTANCES_PATH=$(DISTANCES_PATH) -DCOORDINATES_PATH=$(COORDINATES_PATH) -c $< -o $@

# make usa/usa_utils object
# ──────────────────────────────────────────────────────────────────────────────
$(USA_USA_UTILS_OBJ): $(USA_USA_UTILS_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make signal/signal_utils object
# ──────────────────────────────────────────────────────────────────────────────
$(SIGNAL_SIGNAL_UTILS_OBJ): $(SIGNAL_SIGNAL_UTILS_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make file/file_utils object
# ──────────────────────────────────────────────────────────────────────────────
$(FILE_FILE_UTILS_OBJ): $(FILE_FILE_UTILS_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make string/string_utils object
# ──────────────────────────────────────────────────────────────────────────────
$(STRING_STRING_UTILS_OBJ): $(STRING_STRING_UTILS_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make random/random_utils object
# ──────────────────────────────────────────────────────────────────────────────
$(RANDOM_RANDOM_UTILS_OBJ): $(RANDOM_RANDOM_UTILS_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make time/time_utils object
# ──────────────────────────────────────────────────────────────────────────────
$(TIME_TIME_UTILS_OBJ): $(TIME_TIME_UTILS_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make random/pcg_random object
# ──────────────────────────────────────────────────────────────────────────────
$(RANDOM_PCG_RANDOM_UTILS_OBJ): $(RANDOM_PCG_RANDOM_UTILS_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make bitwise/bitwise_utils object
# ──────────────────────────────────────────────────────────────────────────────
$(BITWISE_BITWISE_UTILS_OBJ): $(BITWISE_BITWISE_UTILS_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make utils/uint128_ops object
# ──────────────────────────────────────────────────────────────────────────────
$(UTILS_UINT128_OPS_OBJ): $(UTILS_UINT128_OPS_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make utils/utils object
# ──────────────────────────────────────────────────────────────────────────────
$(UTILS_UTILS_OBJ): $(UTILS_UTILS_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@
