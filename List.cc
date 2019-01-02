/*
 * List.c -- definition of the List class.
 */

#include "goodies.hh"
#include "List.hh"

List::~List() {
	ListEntry*	entry = first;
	while (entry) {
		ListEntry* prev = entry;
		entry = entry->next;
		delete prev;
	}
}

// find_entry() is an internal routine which just returns the first entry
// pointing to the specified item.
List::ListEntry* List::find_entry(void* item) {
	ListEntry* entry = first;
	while (entry) {
		if (entry->item == item)
			return entry;
		entry = entry->next;
	}
	return 0;
}

// add() adds an item to the list, after the specified item or at the end
// of the list.
Bool List::add(void* item, void* item_to_follow) {
	ListEntry** entry_p;
 // If they specified the item it should follow, find it.
	if (item_to_follow) {
		ListEntry* entry = find_entry(item_to_follow);
		if (!entry)
			return FALSE;
		entry_p = &entry->next;
	// Otherwise find the end of the list.
	} else
		entry_p  =  first ? &last->next : &first;

	// Allocate new ListEntry, fill in the item, and link it in.
	ListEntry* new_entry = new ListEntry;
	new_entry->item = item;
	new_entry->next = *entry_p;
	if (!*entry_p)	// if we're adding to the end, fix the "last" ptr
		last = new_entry;
	*entry_p = new_entry;
	return TRUE;
}

// drop() drops an entry from the list.  Note that if the "next" pointer
// was pointing to it, the next pointer is bumped beyond it.
Bool List::drop(void* item) {
	ListEntry*  entry;
	ListEntry*  prev = 0;
	ListEntry** entry_p = &first;

	for (;;) {
		entry = *entry_p;
		if (!entry)
			return FALSE;	// no such item in list
		if (entry->item == item)
			break;	// found it!
		prev = entry;
		entry_p = &entry->next;
	}

	// Adjust pointers to remove it from the list.
	*entry_p = entry->next;
	if (!entry->next)	// if it was last, adjust the "last" pointer
		last = prev;
	if (next == entry) // if it was "next", advance "next"
		next = entry->next;

	// Get rid of it.
	delete entry;
	return TRUE;
}

// seek() positions the list for readback at the specified item, or at
// the beginning of the list.
Bool List::seek(void* item) {
	if (!item) {	// if no item specified, just go to the beginning
		next = first;
		return TRUE;
	}
	ListEntry* entry = find_entry(item);
	if (!entry)
		return FALSE;
	next = entry;
	return TRUE;
}

// length() returns the number of items in the list.
int List::length() {
	ListEntry* entry = first;
	int len = 0;
	while (entry) {
		len++;
		entry = entry->next;
	}
	return len;
}