#include <iostream>
#include "httplib.h"

unsigned long long seed[0x100]{};

static void generateSymbolSeed()
{
	unsigned long long num1 = 0;
	unsigned long long num2 = 0;

	unsigned long long i = 0;
	do {
		if ((i & 0x80) != 0) {
			num1 = 0x2b5926535897936a;
		}
		else {
			num1 = 0;
		}

		if ((i & 0x40) != 0) {
			num1 = 0xbef5b57af4dc5adf;
			if ((i & 0x80) == 0) {
				num1 = 0x95ac9329ac4bc9b5;
			}
		}

		num2 = num1 * 2 ^ 0x95ac9329ac4bc9b5;
		if ((i & 0x20) == 0) {
			num2 = num1 * 2;
		}
		num1 = num2 * 2 ^ 0x95ac9329ac4bc9b5;
		if ((i & 0x10) == 0) {
			num1 = num2 * 2;
		}
		num2 = num1 * 2 ^ 0x95ac9329ac4bc9b5;
		if ((i & 8) == 0) {
			num2 = num1 * 2;
		}
		num1 = num2 * 2 ^ 0x95ac9329ac4bc9b5;
		if ((i & 4) == 0) {
			num1 = num2 * 2;
		}
		num2 = num1 * 2 ^ 0x95ac9329ac4bc9b5;
		if ((i & 2) == 0) {
			num2 = num1 * 2;
		}
		num1 = num2 * 2 ^ 0x95ac9329ac4bc9b5;
		if ((i & 1) == 0) {
			num1 = num2 * 2;
		}
		seed[i] = num1 * 2;
		i += 1;
	} while (i < 0x100);

	return;
}

static unsigned long long generateSymbol(const char* name) {
	// Check if name is valid
	if (name == NULL || *name == '\0')
		return -1;

	unsigned long long symbol = -1;
	do {
		// Make the character lowercase
		char currentChar = *name;
		char adjustedChar = *name + ' ';

		// If the character is already lowercase, use it
		if (currentChar < 'A')
			adjustedChar = *name;
		if (currentChar >= '[')
			adjustedChar = *name;

		// XOR the new symbol based on the seed
		symbol = adjustedChar ^ seed[symbol >> 0x38 & 0xff] ^ symbol << 8;

		// Run until the end of the string
		name += 1;
	} while (*name != '\0');

	// Return the symbol
	return symbol;
}

int main()
{
	// Generate seed
	generateSymbolSeed();

	// Create server
	httplib::Server svr;

	// Listen to any GET request
	svr.Get(R"(/(.*))", [](const httplib::Request& req, httplib::Response& res) {
		// Extract name (path) from request
		std::string name = req.matches[1];
		std::string realIp = req.get_header_value("X-Real-IP");
		std::string ip = realIp.empty() ? req.remote_addr : realIp;

		// Generate symbol
		unsigned long long symbol = generateSymbol(name.c_str());

		// Return symbol in little endian
		res.set_content(std::string((char*)&symbol, 8), "application/octet-stream");

		// Log the hex value of the symbol
		std::cout << ip << " - " << name << " - 0x" << std::hex << symbol << std::endl;
		});

	// Default port to 8080
	unsigned short port = 8080;

	// Get the port from env
	const char* portString = getenv("PORT");
	if (portString != NULL)
	{
		// Convert to int
		port = atoi(portString);
	}

	// Start server
	svr.listen("0.0.0.0", port);
}