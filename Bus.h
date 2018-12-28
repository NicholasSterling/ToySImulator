/*
 * Bus.h -- classes and other stuff relating to bus accesses.
 *
 * Requires goodies.h.
 *
 * The following classes are declared in this file:
 *   Bus_master		a module which can access the bus
 *   Bus_slave		a module which can be accessed over the bus
 *   Single_master_bus	a module which allows one master to talk to many slaves
 *   Bus_tracer		a module which traces bus activity
 *   Access_info	a collection of info about the access
 *   Access_status	info about how the access went
 *
 * A Bus_master is always directly connected to a Bus_slave.  In most cases
 * this is a 1-1 connection and there is no explicit bus -- just the master
 * and slave.  However, by attaching the Bus_master to a Single_master_bus
 * (which is actually a special form of Bus_slave) and connecting the latter
 * to many slaves, a master can talk to multiple slaves based on the addresses
 * it emits.  A multi-master, multi-slave bus remains to be written.
 *
 * A Bus_slave may be asked to do the following things:
 *   read		initiate a read  request for some # of words
 *   write		initiate a write request for some # of words
 *   write_part		initiate a write request for part of a word
 *
 * A Single_master_bus may be asked to do any of those and:
 *   add_slave		add a Bus_slave at a particular address range
 */

enum Asi {PROG, DATA};		// address space identifier
enum Access_result {FAULT=-1, OK=0};	// how the access went
typedef Ulong  Address;		// address
typedef Uchar  Byte;		// smallest addressible memory unit
typedef Ushort Half;
typedef Ulong  Word;

// Information common to all access types.
struct Access_info {
	Asi	asi;
	Address	addr;
};

// Extra info for a read() or write().
struct Read_write_info : public Access_info {
	Word*	buff;	// ptr to master's buffer
	int	count;	// # of words to read into or write from buffer
};

// Extra info for a write_part().
struct Write_part_info : public Access_info {
	Word	word;	// word containing data to write
	Word	mask;	// mask of bits the slave should affect
};

// Values for write_mask.  Index into arrays with low bits of address.
extern const Word word_mask;
extern const Word half_masks[];
extern const Word byte_masks[];

class Bus_tracer;

class Access_status {
	Ulong		when;
public:
	Access_result	result;
	void set(Ulong delay, Access_result result) {// for callee to set status
		this->when   = cycle + delay;
		this->result = result;
	}
	Bool done(void) {	// returns TRUE when caller may look at status
		return cycle == when;
	}
	friend Bus_tracer;
};

class Bus_master;

class Bus_slave : public Module {
public:
	virtual void read (Read_write_info&, Access_status&) = 0;
	virtual void write(Read_write_info&, Access_status&) = 0;
	virtual void write_part(Write_part_info&, Access_status&) = 0;
};

class Bus_master : public Module {
public:
	Bus_slave* slave;
	Bus_master(Bus_slave& slave) {
		this->slave = &slave;
	}
};

class Single_master_bus : public Bus_slave {
	List slaves;
public:
	void add_slave(Bus_slave&, Asi, Address mask, Address match);
	void read (Read_write_info&, Access_status&);
	void write(Read_write_info&, Access_status&);
	void write_part(Write_part_info&, Access_status&);
};

class Bus_tracer : public Bus_slave {
	char*		label;
	ostream*	out;
	Bus_slave*	slave;
	void trace_status(char*, Access_status&);
public:
	Bus_tracer(Bus_slave& slave, char* label = _, char* trace_file = _);
	void read (Read_write_info&, Access_status&);
	void write(Read_write_info&, Access_status&);
	void write_part(Write_part_info&, Access_status&);
};
