#include <cstddef>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>

#include <boost/config.hpp>
#if defined(BOOST_NO_STDC_NAMESPACE)
namespace std{
	using ::remove;
}
#endif

#include <boost/serialization/string.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/assume_abstract.hpp>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

class person {
	friend std::ostream & operator<<(std::ostream &os, const person &ps);
	friend class boost::serialization::access;
	virtual std::string desc() const = 0;
	std::string name;
	std::string gender;
	int age;

	template<class Archive>
	void serialize (Archive & ar, const unsigned int /* file_version */) {
		ar 	& BOOST_SERIALIZATION_NVP(name) 
			& BOOST_SERIALIZATION_NVP(gender)
			& BOOST_SERIALIZATION_NVP(age);
	}
public:
	person() {};
	person(const std::string _n, const std::string _g, int _a) :
		name(_n), gender(_g), age(_a)
	{}
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(person)

std::ostream & operator<<(std::ostream &os, const person &ps) {
	os << ps.name << ", ";
	os << ps.gender << ", ";
	os << ps.age << "years old, ";
	os << ps.desc();
	return os;
}

class student : public person {
	friend class boost::serialization::access;
	std::string group;
	std::string avr_grade;
	virtual std::string desc() const {
		return "group " + group + ", averege grade " + avr_grade;
	}

	template<class Archive>
	void serialize(Archive &ar, const unsigned int version) {
		ar 	& BOOST_SERIALIZATION_BASE_OBJECT_NVP(person)
			& BOOST_SERIALIZATION_NVP(group)
			& BOOST_SERIALIZATION_NVP(avr_grade);
	}
public:
	student() {};
	student(const std::string & _n, const std::string & _gr, int _a, const std::string & _gp, const std::string & _ag) :
		person(_n, _gr, _a), group(_gp), avr_grade(_ag)
	{}
};

class teacher : public person {
	friend class boost::serialization::access;
	std::string chair;
	std::string academic_status;
	virtual std::string desc() const {
		return "chair " + chair + ", academic status is " + academic_status;
	}

	template<class Archive>
	void serialize(Archive &ar, const unsigned int version) {
		ar 	& BOOST_SERIALIZATION_BASE_OBJECT_NVP(person)
			& BOOST_SERIALIZATION_NVP(chair)
			& BOOST_SERIALIZATION_NVP(academic_status);
	}
public:
	teacher() {};
	teacher(const std::string & _n, const std::string & _gr, int _a, const std::string & _ch, const std::string & _as) :
		person( _n, _gr, _a), chair(_ch), academic_status(_as)
	{}
};

class person_list {
	friend class boost::serialization::access;
	friend std::ostream & operator<<(std::ostream &os, const person_list &pl);
	typedef person * person_pointer;
	std::list<person_pointer> persons;

	template<class Archive>
	void serialize(Archive &ar, const unsigned int version) {
		ar.register_type(static_cast<student *>(NULL));
		ar.register_type(static_cast<teacher *>(NULL));
		ar & BOOST_SERIALIZATION_NVP(persons);
	}
public:
	person_list() {};
	void append(person *_ps) {
		persons.insert(persons.end(), _ps);
	}
//	void load(const char * filename);
//	void save(const char * filename);
};

BOOST_CLASS_VERSION(person_list, 2);

std::ostream & operator<<(std::ostream &os, const person_list &pl) {
	std::list<person *>::const_iterator it;
	for (it = pl.persons.begin(); it != pl.persons.end(); it++) {
		os << **it << "\n"; 
	}
	return os;
}
/*
void person_list::load(const char * filename) {
	std::ifstream ifs(filename);
	assert(ifs.good());
	boost::archive::xml_iarchive ia(ifs);
	ia >> BOOST_SERIALIZATION_NVP(*this);
}

void person_list::save(const char * filename) {
	std::ofstream ofs(filename);
	assert(ofs.good());
	boost::archive::xml_oarchive oa(ofs);
	oa << BOOST_SERIALIZATION_NVP(*(this));
}*/
