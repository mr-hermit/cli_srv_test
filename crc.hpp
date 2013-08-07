#include <boost/crc.hpp>
#include <boost/integer.hpp>
#include <cassert>
#include <iostream>

boost::uint16_t getcrc(const char * data, std::size_t data_len) {
	boost::crc_ccitt_type crc_agent;
	
	crc_agent.process_bytes(data,data_len);
	return crc_agent.checksum();
}
