#ifndef _PERSON_H_CLASS
#define _PERSON_H_CLASS

#include <iostream>
#include <fstream>
#include <string>

#include <boost/archive/tmpdir.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

class person {
	friend std::ostream & operator<<(std::ostream &os, const person &_person);
	friend class boost::serialization::access;
	std::string name;
	std::string gender;
	int age;

	template<class Archive>
	void serialize (Archive & ar, const unsigned in /* file_version */){
		ar & name & gender & age;
	}
public:
	person() {};
	person(std::string _name, std::string _gender, int _age) :
		name(_name), gender(_gender), age(_age)
	{}
};

#endif //_PERSONS_H_CLASS
