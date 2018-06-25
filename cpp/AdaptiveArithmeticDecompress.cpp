/* 
 * Decompression application using adaptive arithmetic coding
 * 
 * Usage: AdaptiveArithmeticDecompress InputFile OutputFile
 * This decompresses files generated by the "AdaptiveArithmeticCompress" application.
 * 
 * Copyright (c) Project Nayuki
 * 
 * https://www.nayuki.io/page/reference-arithmetic-coding
 * https://github.com/nayuki/Reference-arithmetic-coding
 */

#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include "ArithmeticCoder.hpp"
#include "BitIoStream.hpp"
#include "FrequencyTable.hpp"

using std::uint32_t;


int main(int argc, char *argv[]) {
	// Handle command line arguments
	if (argc != 3) {
		std::cerr << "Usage: " << argv[0] << " InputFile OutputFile" << std::endl;
		return EXIT_FAILURE;
	}
	const char *inputFile  = argv[1];
	const char *outputFile = argv[2];
	
	// Perform file decompression
	std::ifstream in(inputFile, std::ios::binary);
	std::ofstream out(outputFile, std::ios::binary);
	BitInputStream bin(in);
	try {
		
		std::vector<uint32_t> temp;
		for (int i = 0; i < 257; i++)
			temp.push_back(1);
		SimpleFrequencyTable freqs(temp);
		
		ArithmeticDecoder dec(bin);
		while (true) {
			// Decode and write one byte
			int symbol = static_cast<int>(dec.read(freqs));
			if (symbol == 256)  // EOF symbol
				break;
			out.put(symbol);
			freqs.increment(symbol);
		}
		return EXIT_SUCCESS;
		
	} catch (const char *msg) {
		std::cerr << msg << std::endl;
		return EXIT_FAILURE;
	}
}
