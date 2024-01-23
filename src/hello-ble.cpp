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

void print_peripheral(SimpleBLE::Peripheral peripheral)
{
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

	adapter.set_callback_on_scan_found([&](SimpleBLE::Peripheral peripheral)
									   { peripherals.push_back(peripheral); });

	adapter.set_callback_on_scan_start([]()
									   { std::cout << "Scan started." << std::endl; });
	adapter.set_callback_on_scan_stop([]()
									  { std::cout << "Scan stopped." << std::endl; });
	adapter.scan_for(5000);

	for (auto peripheral : peripherals)
	{
		print_peripheral(peripheral);
	}

	// Look for the one I want, which is my Zwift Hub. This could be anything of your choosing.
	auto search_id = "Zwift Hub";
	auto search_result = std::find_if(peripherals.begin(), peripherals.end(), [&](SimpleBLE::Peripheral peripheral)
									  { return peripheral.identifier() == search_id; });
	if (search_result == peripherals.end())
	{
		std::cout << "Failed to find " << search_id << std::endl;
		return -1;
	}

	std::cout << "Found: " << std::endl;
	print_peripheral(*search_result);

	// Connect, to find the services of the searched peripheral
	// TODO: error handling of connect...
	std::cout << "Connecting to " << search_result->identifier() << std::endl;
	search_result->connect();

	std::cout << "Connected." << std::endl;

	// Find out the services
	for (auto &service : search_result->services())
	{
		// Now find the characteristics of each service.
		std::cout << "Service: " << service.uuid() << std::endl;

		for (auto &characteristic : service.characteristics())
		{
			std::cout << "\tCharacteristic: " << characteristic.uuid() << std::endl;
			// And then capabilities...
			std::cout << "\t\tCapabilities: ";
			for (auto &capability : characteristic.capabilities())
			{
				std::cout << capability << " ";
			}
			std::cout << std::endl;

			for (auto &descriptor : characteristic.descriptors())
			{
				std::cout << "\t\tDescriptor: " << descriptor.uuid() << std::endl;
			}
		}
	}

	search_result->disconnect();

	return 0;
}
