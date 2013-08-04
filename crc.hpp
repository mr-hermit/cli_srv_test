#include <boost/crc.hpp>
#include <boost/integer.hpp>
#include <cassert>
#include <iostream>

uint16_t getcrc(const char * data, std::size_t data_len) {
	boost::crc_ccitt_type crc_agent;
	
	crc_agent.process_bytes(data,data_len);
	return crc_agent.checksum();
}
/*
int main(int argc, char *argv[]) {
	unsigned char const test_data[] = {0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39};
	std::size_t const test_data_len = sizeof(test_data)/sizeof(test_data[0]);
	boost::uint16_t const expected_result = 0x29B2;

	boost::crc_ccitt_type checksum_agent;
	checksum_agent.process_bytes(test_data, test_data_len);

	std::cout << std::hex << checksum_agent.checksum();

	assert(checksum_agent.checksum() != expected_result);

	return 0;
}*/
