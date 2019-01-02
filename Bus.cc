/*
 * Bus.c -- Member functions for classes defined in Bus.h.
 */

#include <iostream>
#include <fstream>
#include "goodies.hh"
#include "List.hh"
#include "simulation.hh"
#include "Module.hh"
#include "Bus.hh"

// Values for write_mask.  Index into arrays with low bits of address.
const Word word_mask = 0xffffffff;
const Word half_masks[] = {0xffff0000, 0x0000ffff};
const Word byte_masks[] = {0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff};

/*
 * Decoder is a simple little class which is used by multi-slave busses to
 * select from a set of slaves based upon address.
 */
class Decoder {
	Asi		asi;
	Address		mask;
	Address		match;
public:
	Bus_slave*	slave;
	Decoder(Bus_slave* slave, Asi asi, Address mask, Address match) {
		this->slave = slave;
		this->asi   = asi;
		this->mask  = mask;
		this->match = match;
	}
	Bool is_selected(Access_info& info) {
		return (info.asi == asi) && ((info.addr & mask) == match);
	}
};

void Single_master_bus::
  add_slave(Bus_slave& slave, Asi asi, Address mask, Address match) {
	Decoder* decoder = new Decoder(&slave, asi, mask, match);
	slaves.add(decoder);

	// TODO: check for conflicts with other slaves.
}

/*
 * The implementations of read(), write(), and write_part() for class
 * Single_master_bus simply look through all of the decoders in the list
 * of slaves until they find one that is selected by the access.  Then
 * they simply call upon the corresponding slave to do the access.
 */

void Single_master_bus::read(Read_write_info& info, Access_status& status) {
	Decoder* decoder;
	slaves.seek();
	while ((decoder = (Decoder*) slaves.get_next()) != NULL)
		if (decoder->is_selected(info)) {
			decoder->slave->read(info, status);
			return;
		}
	status.set(1, FAULT);
}

void Single_master_bus::write(Read_write_info& info, Access_status& status) {
	Decoder* decoder;
	slaves.seek();
	while ((decoder = (Decoder*) slaves.get_next()) != NULL)
		if (decoder->is_selected(info)) {
			decoder->slave->write(info, status);
			return;
		}
	status.set(1, FAULT);
}

void Single_master_bus::
		write_part(Write_part_info& info, Access_status& status) {
	Decoder* decoder;
	slaves.seek();
	while ((decoder = (Decoder*) slaves.get_next()) != NULL)
		if (decoder->is_selected(info)) {
			decoder->slave->write_part(info, status);
			return;
		}
	status.set(1, FAULT);
}

/*
 * A Bus_tracer traces bus accesses to a file or stderr.
 */
Bus_tracer::Bus_tracer(Bus_slave& slave, const char* label, const char* trace_file) {
	this->label = label;
	if (trace_file) {
		out = new std::ofstream(trace_file);
		if (!out)
			fatal("error creating trace file");
	} else
		out = &std::cerr;
	this->slave = &slave;
}

void Bus_tracer::read(Read_write_info& info, Access_status& status) {
	*out << ::cycle << " - " << label
		 << " read:  asi="   << info.asi
		 <<      ", addr=0x" << info.addr
		 <<     ", count=0x" << info.count << '\n';
	slave->read(info, status);
	trace_status("read", status);
}

void Bus_tracer::write(Read_write_info& info, Access_status& status) {
	*out << ::cycle << " - " << label
		 << " write:  asi="   << info.asi
		 <<       ", addr=0x" << info.addr
		 <<      ", count=0x" << info.count << '\n';
	slave->write(info, status);
	trace_status("write", status);
}

void Bus_tracer::write_part(Write_part_info& info, Access_status& status) {
	*out << ::cycle << " - " << label
		 << " write_part: asi="   << info.asi
		 <<           ", addr=0x" << info.addr
		 <<           ", word=0x" << info.word
		 <<           ", mask=0x" << info.mask << '\n';
	slave->write_part(info, status);
	trace_status("write_part", status);
}

void Bus_tracer::trace_status(const char* access_type, Access_status& status) {
	*out << ::cycle << " - " << label << ' ' << access_type << " status: "
		 << ((status.result == OK) ? "OK" : "FAULT")
		 << " at cycle " << status.when << '\n';
}
