#include <iostream>

#include "Rainman_src/CRgtFile.h"
#include "Rainman_src/Exception.h"
#include "Rainman_src/CFileSystemStore.h"

int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cout << "Invalid number of arguments. Requires 2." << std::endl;
		return 1;
	}

	char* infile = argv[1];

	std::cout << "Input filename: " << infile << std::endl;

	IFileStore::IStream *pIn = 0;
	IFileStore::IOutputStream *pOut = 0;

	CFileSystemStore cfilestore;
	cfilestore.VInit();

	try {
		pIn = cfilestore.VOpenStream(infile);
	} catch(CRainmanException *pE) {
		delete pIn;
		std::cout << "Error: Cannot open input stream." << std::endl;
		return 1;
	}

	CRgtFile oRgt;
	try {
		oRgt.Load(pIn);
	} catch(CRainmanException *pE) {
		delete pIn;
		delete pOut;
		std::cout << "Error: Cannot load file." << std::endl;
		return 1;
	}

	char* saOutFile = _strdup(infile);
	if (!saOutFile) {
		std::cout << "Error: Memory allocation." << std::endl;
		return 1;
	}

	switch(oRgt.GetImageFormat()) {
		case CRgtFile::IF_Tga:
			strcpy(strchr(saOutFile, '.'), ".tga");
			std::cout << "Format: TGA 32bit RGBA" << std::endl;
			break;
		case CRgtFile::IF_Dxtc:
			strcpy(strchr(saOutFile, '.'), ".dds");
			switch(oRgt.GetProperty(CRgtFile::IP_CompressionLevel)) {
				case 1:
					std::cout << "Format: DXT1" << std::endl;
					break;

				case 2:
					std::cout << "Format: DXT2" << std::endl;
					break;

				case 3:
					std::cout << "Format: DXT3" << std::endl;
					break;

				case 4:
					std::cout << "Format: DXT4" << std::endl;
					break;

				case 5:
					std::cout << "Format: DXT5" << std::endl;
					break;

				default:
					std::cout << "Format: unknown" << std::endl;
					break;
			};

			break;
		default:
			delete pIn;
			free(saOutFile);
			std::cout << "Error unknown image format" << std::endl;
			return 1;
	}

	std::cout << "Output file: " << saOutFile << std::endl;


	try {
		pOut = cfilestore.VOpenOutputStream(saOutFile, true);
	} catch(CRainmanException *pE) {
		delete pIn;
		free(saOutFile);
		std::cout << "Error: Cannot open output stream." << std::endl;
		return 1;
	}

	try {
		oRgt.SaveGeneric(pOut);
	} catch(CRainmanException *pE) {
		delete pIn;
		delete pOut;
		free(saOutFile);
		std::cout << "Error: Cannot save generic." << std::endl;
	}

	delete pIn;
	delete pOut;
	free(saOutFile);

	return 0;
}