/*
 * Interrupt.h -- Declaration of virtual class Interruptee.
 *
 * An Interruptee is a Module which can respond to an interrupt.  Two member
 * functions provide the mechanism by which interrupts are delivered:
 *	assert_interrupt()
 *	release_interrupt()
 * A memory-mapped device will have registers visible through some other
 * interface (e.g. as a Bus_slave); this interface only provides for the
 * device to interrupt another Module.
 */

class Interruptee : public Module {
protected:
	unsigned	pending_interrupts;
public:
	Interruptee() {
		pending_interrupts = 0;
	}
	void  assert_interrupt() {
		pending_interrupts++;
	}
	void release_interrupt() {
		pending_interrupts--;
	}
};
