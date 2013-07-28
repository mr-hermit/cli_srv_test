#include "person.h"

std::ostream & operator<<(std::ostream &os, const person &_person) {
	return os << _person.name << _person.gender << _person.age;
}
