/*
 * List.h -- header for the List class.
 *
 * A List is a simple list of ListEntry variables.  One can ask a List to do
 * the following things:
 *
 *    add        an item
 *    drop       an item
 *    seek       a particular item in the list
 *    get_next   item
 */

class List {
	struct ListEntry {
		ListEntry*   next;
		void*        item;
	};
	ListEntry*   first;
	ListEntry*   last;
	ListEntry*   next;
	ListEntry* find_entry(void* item);
public:
	List() { first = last = next = 0; }
	~List();
	Bool add(void* item, void* item_to_follow = 0);
	Bool drop(void* item);
	Bool seek(void* item = 0);
	int length();
	void* get_next() {
		ListEntry* entry = next;
		if (!entry)
			return 0;
		next = entry->next;
		return entry->item;
	}
};