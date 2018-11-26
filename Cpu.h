/*
 * Cpu.h -- header for the Cpu class.
 *
 * Cpu is a simulation module derived from Bus_master.  This version
 * doesn't do much, to save effort and because it isn't necessary
 * at this point to have a complex cpu -- we're just trying to show
 * how things connect up and interact.
 */

const unsigned NUM_REGS = 16;

class Cpu : public Bus_master, public Interruptee {
	enum	Opcodes {HALT, ADD, ADDHI, SUB, BNE, BEQ, ST, STH, STB, RTE};
	Word	regs[NUM_REGS];
	Word	pc;
	Read_write_info fetch;
	Write_part_info store;
	Access_status access_status;
	Bool	data_access;
	Word	saved_pc; // pc gets saved here on exceptions until RTE
	Bool	exceptions_enabled;
	union {
		struct {
			unsigned opcode    : 4;
			unsigned dest      : 4;	// log2(NUM_REGS)
			unsigned src1      : 4;
			unsigned src2      : 4;
			signed   immediate : 16;
		} instr;
		Word instruction;
	};
	void execute_instruction();
	void branch(Bool flag);
public:
	Cpu(Bus_slave&);
	virtual void init();
	virtual void cycle();
};
