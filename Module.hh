/*
 * Module.h -- header for the Module class.
 *
 * Module is the simulation class from which all module classes are derived.
 */

// Here's the list of all modules.
GLOBAL(List all_modules);

// Here's the list of modules which need to be cycled -- that is, called
// at their cycle routines on each machine cycle.  When components are
// initialized (their constructors are called) they may add themselves to
// this list.
GLOBAL(List modules_to_cycle);

// Definition of the module class.
class Module {
public:
	Module() {
		all_modules.add(this);
	}
	virtual void init() {}
	virtual void cycle() {}
};
