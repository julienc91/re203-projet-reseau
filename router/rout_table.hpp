#include <string>

using namespace std;
class Entry{
	public:
		Entry(string name, string next_op);
		Entry(string name, string next_op, int dist = NULL);
		Entry(const Entry * data);
		~Entry();

//    private:
		string _name;
		string _next_op;
		int _dist;
		Entry * _next_entry;
		Entry * _prev_entry;


};


class RoutTable{

	public:
		RoutTable(const string name);
		RoutTable(const RoutTable * data);
		~RoutTable();

//    private:
		Entry *_head;


};
