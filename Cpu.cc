/*
 * Cpu.c -- implementation of the Cpu class.
 *
 * This implementation of Cpu is incredibly simple.  All we're trying to d
 * at this point is show how things connect up and interact.
 */

#include <iostream>
#include "goodies.hh"
#include "List.hh"
#include "simulation.hh"

#include "Module.hh"
#include "Interrupt.hh"
#include "Bus.hh"
#include "Cpu.hh"

Cpu::Cpu(Bus_slave& slave) : Bus_master(slave) {
	pc = 0;
	fetch.asi   = PROG;
	fetch.count = 1;
	fetch.buff  = &instruction;
	store.asi   = DATA;
	data_access = FALSE;
	exceptions_enabled = TRUE;
	for (int i=0; i<NUM_REGS; i++)
		regs[i] = 0;
	modules_to_cycle.add(this);
}

/*
 * Cpu's init routine; gets called after all modules are set up.
 */
void Cpu::init(void) {
	fetch.addr = pc; // fetch the instruction at address 0.
	slave->read(fetch, access_status);
}

/*
 * Cpu's cycle routine; gets called every cycle.
 */
void Cpu::cycle(void) {
	// If a bus access just completed...
	if (access_status.done()) {
		if (access_status.result == FAULT)
			fatal("Cpu: fault on bus access"); // fatal for now
		if (data_access)
			data_access = FALSE;
		else { // it's an instruction, so execute it
			pc += sizeof(instruction);
			execute_instruction();
		}

		// Check for interrupts.
		if (pending_interrupts && exceptions_enabled) {
			saved_pc = pc;
			pc = sizeof(Word); // 0 = reset vector, then interrupt
			exceptions_enabled = FALSE;
		}

		// If the instruction didn't use the bus, fetch the next one.
		if (!data_access) {
			fetch.addr = pc;
			slave->read(fetch, access_status);
		}
	}
}

inline void Cpu::branch(Bool flag) {
	if (flag) {
		pc  +=  regs[instr.src2] + instr.immediate;
		std::cout << "branch taken\n";
	}
}

void Cpu::execute_instruction() {
	unsigned dest  = instr.dest;
	unsigned src1  = instr.src1;
	unsigned src2  = instr.src2;
	long immediate = instr.immediate;

	std::cout << "Executing opcode " << instr.opcode << "; pc is now " << pc << "\n";
	switch (instr.opcode) {
	case HALT:
		simulating = FALSE;
		std::cout << "Halting\n";
		break;
	case ADD:
		regs[dest]  =  regs[src1] + regs[src2] + immediate;
		std::cout << "ADD: reg[" << dest << "] = " << regs[dest] << "\n";
		break;
	case ADDHI:
		regs[dest]  =  regs[src1] + regs[src2] + (immediate<<16);
		std::cout << "ADDHI: reg[" << dest << "] = " << regs[dest] << "\n";
		break;
	case SUB:
		regs[dest]  =  regs[src1] - (regs[src2] + immediate);
		std::cout << "SUB: reg[" << dest << "] = " << regs[dest] << "\n";
		break;
	case BNE:
		std::cout << "BNE\n";
		branch(regs[src1] != 0);
		break;
	case BEQ:
		std::cout << "BEQ\n";
		branch(regs[src1] == 0);
		break;
	case ST:
		data_access =  TRUE;
		store.addr  =  regs[src1] + (regs[src2]<<2) + immediate;
		store.word  =  regs[dest];
		store.mask  =  word_mask;
		slave->write_part(store, access_status);
		std::cout << "ST: at " << store.addr << " value " << store.word << "\n";
		break;
	case STH:
		data_access =  TRUE;
		store.addr  =  regs[src1] + (regs[src2]<<1) + immediate;
		store.word  =  regs[dest];
		store.mask  =  half_masks [store.addr & 1];
		slave->write_part(store, access_status);
		std::cout << "STH: at " << store.addr << " value " << store.word << "\n";
		break;
	case STB:
		data_access =  TRUE;
		store.addr  =  regs[src1] + regs[src2] + immediate;
		store.word  =  regs[dest];
		store.mask  =  byte_masks [store.addr & 3];
		slave->write_part(store, access_status);
		std::cout << "STB: at " << store.addr << " value " << store.word << "\n";
		break;
	case RTE:
		pc = saved_pc;
		exceptions_enabled = TRUE;
		std::cout << "RTE\n";
		break;
	default:
		fatal("unknown opcode"); // fatal for now
	}
	regs[0] = 0;
}
