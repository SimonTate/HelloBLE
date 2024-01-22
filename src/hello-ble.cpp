#include <iostream>
#include <iomanip>
#include <simpleble/SimpleBLE.h>

void print_byte_array(const SimpleBLE::ByteArray &bytes)
{
	for (auto b : bytes)
	{
		std::cout << std::hex << std::setfill('0') << std::setw(2) << (uint32_t)((uint8_t)b) << " ";
	}
	std::cout << std::endl;
}

int main(int argc, char **argv)
{
	if (!SimpleBLE::Adapter::bluetooth_enabled())
	{
		std::cout << "Bluetooth is not enabled" << std::endl;
		return 1;
	}

	auto adapters = SimpleBLE::Adapter::get_adapters();
	if (adapters.empty())
	{
		std::cout << "No Bluetooth adapters found" << std::endl;
		return 1;
	}

	// Use the first adapter - might be others in the future.
	auto adapter = adapters[0];

	std::cout << "Adapter identifier: " << adapter.identifier() << std::endl;
	std::cout << "Adapter address: " << adapter.address() << std::endl;

	// Scan for peripherals for 5000 milliseconds

	// Get the list of peripherals found
	std::vector<SimpleBLE::Peripheral> peripherals;

	adapter.set_callback_on_scan_found([&](SimpleBLE::Peripheral peripheral) {

		peripherals.push_back(peripheral);
	});

	adapter.set_callback_on_scan_start([]() {
		std::cout << "Scan started." << std::endl;
	});
	adapter.set_callback_on_scan_stop([](){
		std::cout << "Scan stopped." << std::endl;
	});
	adapter.scan_for(5000);

	for (auto peripheral : peripherals) {
		std::cout << "Peripheral:" << std::endl;
		std::cout << "\tIdentifier: " << peripheral.identifier() << std::endl;
		std::cout << "\tAddress: " << peripheral.address() << std::endl;
		std::cout << std::dec << "\tRSSI: " << peripheral.rssi() << std::endl;
		std::cout << std::dec << "\tIs Connectable: " << peripheral.is_connectable() << std::endl;

		std::map<uint16_t, SimpleBLE::ByteArray> manufacturer_data = peripheral.manufacturer_data();
		for (auto &[manufacturer_id, data] : manufacturer_data)
		{
			std::cout << "\tManufacturer ID: " << manufacturer_id << std::endl;
			std::cout << "\tManufacturer Data: ";
			print_byte_array(data);
			std::cout << std::endl;
		}
	}

	return 0;
}
