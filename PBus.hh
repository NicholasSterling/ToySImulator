/*
 * Pbus_master and Pbus_slave classes.
 *
 * Requires goodies.h, Module.h, bus_info.h.
 *
 * "Pbus" refers to "processor bus," the bus leading out of a Cpu that
 * connects it to memory, etc.  On this bus there are two modules, one of
 * each of these classes:
 *
 *   Pbus_master	the module which originates requests
 *   Pbus_slave     the module which satisfies  requests
 *
 * Note that only one master and one slave are allowed.  In the realistic
 * case where there are multiple slaves connected to a single master, an
 * intermediary module must be used which selects between the various
 * slaves based on address or whatever.
 *
 * A Pbus_slave may be asked to do the following things:
 *
 *   connect_to     connect to a Pbus master
 *   start_read     initiate a read request
 *   start_write    initiate a write request
 */

class Pbus_slave : public Module;

class Pbus_master : public Module {
public:
	Pbus_slave *pbus_slave;
	Pbus_master(Pbus_slave *module) {
		pbus_slave = module;
		pbus_slave.connect_to(this);
	}
}

class Pbus_slave : public Module {
public:
	Pbus_master *pbus_master;
	void connect_to(Pbus_master *module) {
		pbus_master = module;
	}
	virtual void start_read (Bus_info *) = 0;
	virtual void start_write(Bus_info *) = 0;
}
