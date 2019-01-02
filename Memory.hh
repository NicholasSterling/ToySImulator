/*
 * Memory.h -- header for various simulation memory module classes.
 *
 * Ram		simulates read-write memory.
 * Rom		simulates read-only  memory.
 *
 * Because Ram takes an optional parameter of a file from which to
 * load its initial contents, it can serve to simulate EEPROM.
 */

class Ram : public Bus_slave {
	Byte*	contents;
	Address	addr_mask;	// AND this with address to get contents index
	Uint	read_delay, write_delay;
public:
	Ram( Ulong size, Uint read_delay = 0, Uint write_delay = 0,
			const char* init_file = 0 );
	virtual void read (Read_write_info& info, Access_status& status);
	virtual void write(Read_write_info& info, Access_status& status);
	virtual void write_part(Write_part_info& info, Access_status& status);
};

class Rom : public Ram {
public:
	Rom(Ulong size, Uint read_delay = 0, const char* init_file = 0) :
		 Ram(size, read_delay, 0, init_file) {};
	virtual void write(Read_write_info& info, Access_status& status);
	virtual void write_part(Write_part_info& info, Access_status& status);
};
