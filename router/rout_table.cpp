#include "rout_table.hpp"
#include <string>


Routeur::Routeur(string name){
    _name = name;
    _tab = new RoutTable(name);
}

Routeur::Routeur(const Routeur * data){
    _name   = data->_name;
    _tab    = new RoutTable(data->_tab);
}

Routeur::~Routeur(){
    delete _tab;
}

RoutTable::RoutTable(const string name){
    _head = new Entry(name, NULL, 0);
}

RoutTable::RoutTable(const RoutTable * data){
    _head = new Entry(data->_head);
}

RoutTable::~RoutTable(){
    delete _head;
}


Entry::Entry(string name, string next_op){
    _name       = name;
    _next_op    = next_op;
    _dist       = -1;
    _next_entry  = NULL;
    _prev_entry  = NULL;
}

Entry::Entry(string name, string next_op, int dist){
    _name       = name;
    _next_op    = next_op;
    _dist       = dist;
    _next_entry  = NULL;
    _prev_entry  = NULL;
}


Entry::Entry(const Entry * data){
    _name       = data->_name;
    _next_op    = data->_next_op;
    _dist       = data->_dist;
    _next_entry  = data->_next_entry;
    _prev_entry  = data->_prev_entry;

}

Entry::~Entry(){
    Entry * tmp1 = _next_entry;
    Entry * tmp2 = tmp1->_next_entry;

    while(tmp1 != NULL){
        delete tmp1;
        tmp1 = tmp2;
        tmp2 = tmp2->_next_entry;
    }
}
