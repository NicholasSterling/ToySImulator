/*
 * Timer.h -- Declaration of Timer module class.
 *
 * A Timer is a Bus_slave module simulating hardware which appears on the
 * bus as a Word-long register.  If a nonzero number is written to this
 * register, the value will be counted down toward zero on each cycle.
 * When zero is reached, an interrupt is generated.  This interrupt can be
 * cleared by writing to the register again.  Note that by writing zero to
 * the register, the interrupt will be cancelled and no other interrupt will
 * be forthcoming.
 */

class Timer : public Bus_slave {
	Interruptee* interruptee;
	Word count;
	Bool interrupting;
public:
	Timer(Interruptee&);
	void cycle();
	void read (Read_write_info&, Access_status&);
	void write(Read_write_info&, Access_status&);
	void write_part(Write_part_info&, Access_status&);
};
