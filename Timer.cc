/*
 * Timer.c -- implementation of the Timer class.
 *
 * The Timer is a simulator Module.  It counts down a value in a register
 * (which appears in memory and can be written to) and interrupts when it
 * reaches zero.
 */

#include <iostream>
#include "goodies.hh"
#include "List.hh"
#include "simulation.hh"

#include "Module.hh"
#include "Interrupt.hh"
#include "Bus.hh"
#include "Timer.hh"

Timer::Timer(Interruptee& interruptee) {
	this->interruptee = &interruptee;
	count = 0;
	interrupting = FALSE;
	modules_to_cycle.add(this);
}

void Timer::read(Read_write_info& info, Access_status& status) {
	Word* buff = info.buff;
	for (int i=info.count; i--; )
		*buff++ = count;
	status.set(1, OK); // arbitrarily chosen delay of 1 cycle
}

void Timer::write(Read_write_info& info, Access_status& status) {
	Word* buff = info.buff;
	for (int i=info.count; i--; ) {
		count = *buff++;
		std::cout << "Timer set to " << count << "\n";
	}
	if (interrupting)
		interruptee->release_interrupt();
	interrupting = FALSE;
	status.set(1, OK);
}

void Timer::write_part(Write_part_info& info, Access_status& status) {
	if (info.mask != word_mask) { // halfword/byte writes aren't valid
		status.set(1, FAULT);
		return;
	}
	count = info.word;
	if (interrupting)
		interruptee->release_interrupt();
	interrupting = FALSE;
	status.set(1, OK);
}

void Timer::cycle() {
	if (count && !--count) {
		interruptee->assert_interrupt();
		interrupting = TRUE;
	}
}
