/*
 * simulation.h -- globals and defines for the simulator.
 */

// Turn this off to stop the simulation.
GLOBAL_INIT(Bool simulating, TRUE);

// Cycle count.
GLOBAL_INIT(Ulong cycle, 0);

void simulate();
