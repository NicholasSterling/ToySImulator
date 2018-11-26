/*
 * Bus_info.h -- description of Bus_info struct, and other bus-related stuff.
 */

typedef unsigned char Asi;      // address space identifier
typedef unsigned long Address;  // address
typedef unsigned char Byte;     // smallest addressible memory unit
typedef Byte Word[4];           // memory that can be read/written in one gulp
typedef enum { OK, FAULT, BUSY, IDLE } Bus_status;  // status of bus access

struct Bus_info {
	Bus_status  status;
	Asi         asi;
	Address     addr;
	int         count;
	Byte *      buff;
}
