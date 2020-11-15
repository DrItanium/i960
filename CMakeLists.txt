cmake_minimum_required(VERSION 3.10)

project(i960 VERSION 0.3)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_EXTRA_FLAGS}")
add_library(core960
	types.h
	opcodes.h
	Operand.h
	Operand.cc
	Instruction.h
	Instruction.cc)
add_executable(sim960jx 
	sim960jx.cc
	ArithmeticControls.h
	ConditionCode.h
	core.h
	InternalDataRam.h
	memiface.h
	MemoryMap.h
	NormalRegister.h
	opcodes.h
	PMCONRegister.h
	ProcessControls.h
	ProcessorControlBlock.h
	QuadRegister.h
	Records.h
	StartupRecord.h
	TripleRegister.h
	types.h
	DefFactQuery.def
	ExpandAllOpcodes.def
	opcodes.def
	ArithmeticControls.cc
	core.cc
	NormalRegister.cc
	ProcessControls.cc
	QuadRegister.cc
	TripleRegister.cc)
add_executable(decode960jx decode960jx.cc)

target_link_libraries(decode960jx PUBLIC core960)
target_link_libraries(sim960jx PUBLIC core960)
target_include_directories(decode960jx PUBLIC "${PROJECT_SRC_DIR}")
target_include_directories(sim960jx PUBLIC "${PROJECT_SRC_DIR}")
