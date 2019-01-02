/*
 * simulation.c
 *
 * This is a C++ program which uses various classes to simulate a worthless
 * computer.  The point (for now) is not to simulate anything really useful,
 * but just to get down how things would connect together and interact.
 *
 * Following are the major class hierarchies defined (note that the
 * indentation of the class names shows the tree structure):
 *
 * Module
 *   Interruptee
 *     Cpu
 *   Bus_master
 *     Cpu
 *   Bus_slave
 *     Bus_tracer
 *     Ram
 *       Rom
 *     Single_master_bus
 *     Timer
 *
 * List
 *
 * This file is the main driver; it declares the components of the machine
 * to be simulated, connects them together, and then ticks off the cycles.
 */

#define GLOBALS	// allocate storage for globals in this module

#include <iostream>
#include <stdlib.h>

#include "goodies.hh"
#include "simulation.hh"

#include "List.hh"
#include "Module.hh"
#include "Interrupt.hh"
#include "Bus.hh"
#include "Cpu.hh"
#include "Memory.hh"
#include "Timer.hh"

// Declare the components being simulated.
Single_master_bus bus;
Rom		rom(1 K, 3, "rom.init");
Ram		ram(1 K, 2, 2);
Bus_tracer	tracer(bus, "from_cpu", "cpu.trc");
Cpu		cpu(tracer);
Timer		timer(cpu);

#pragma argsused
int main(int argc, char** argv) {

	std::cout << all_modules.length() << " modules, " << modules_to_cycle.length() << " to cycle\n";
	std::cout << std::hex;

	// Connect memory to the bus.
	bus.add_slave(rom,   PROG, 0xf0000000, 0x00000000); //   rom at 0x0..
	bus.add_slave(rom,   DATA, 0xf0000000, 0x00000000);
	bus.add_slave(ram,   PROG, 0xf0000000, 0x10000000); //   ram at 0x1..
	bus.add_slave(ram,   DATA, 0xf0000000, 0x10000000);
	bus.add_slave(timer, DATA, 0xf0000000, 0xf0000000); // timer at 0xf..

	simulate();
}

void simulate() {
	Module*	module;

	// Let all the modules initialize themselves.
	all_modules.seek();
	while ((module = (Module*) all_modules.get_next()) != NULL)
		module->init();

	// Keep cycling until the simulation is over.
	int cycle_num = 0;
	while (simulating) {
		std::cout << "\n===== CYCLE " << cycle_num++ << " =====\n\n";
		modules_to_cycle.seek();
		while ((module = (Module*) modules_to_cycle.get_next()) != NULL) {
			module->cycle();
		}
		cycle++;
	}

	// We're done; say so.
	std::cout << "Simulation complete.\n";
}
