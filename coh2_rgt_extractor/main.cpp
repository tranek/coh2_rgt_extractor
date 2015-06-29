#include <iostream>

#include "Rainman_src/CRgtFile.h"
//#include "Rainman_src/CMemoryStore.h"
#include "Rainman_src/Exception.h"
//#include "Rainman_src/CFileMap.h"
#include "Rainman_src/CFileSystemStore.h"

int main(int argc, char* argv[]) {
	std::cout << "Hell World!" << std::endl;

	std::cout << "Num args: " << argc << std::endl;

	if (argc != 2) {
		std::cout << "Invalid number of arguments. Requires 2." << std::endl;
		return 1;
	}

	char* infile = argv[1];

	std::cout << "Input filename: " << infile << std::endl;

	IFileStore::IStream *pIn = 0;
	IFileStore::IOutputStream *pOut = 0;

	/*CModuleFile* pMod = TheConstruct->GetModule();
	try
	{
		pIn = pMod->VOpenStream(saFile);
	}
	catch(CRainmanException *pE)
	{
		delete pIn;
		throw new CModStudioException(pE, __FILE__, __LINE__, "Cannot open input stream for \'%s\'", saFile);
	}*/

	// Need to call MemoryRange() on infile
	//char* CMemoryStore::MemoryRange(void* pBegin, unsigned long iLength)

	//CMemoryStore cmstore;
	//cmstore.VInit();

	//CFileMap cfilemap;
	//cfilemap.VInit();

	CFileSystemStore cfilestore;
	cfilestore.VInit();

	try {
		//pIn = cmstore.VOpenStream(infile);
		//pIn = cfilemap.VOpenStream(infile);
		pIn = cfilestore.VOpenStream(infile);
	} catch(CRainmanException *pE) {
		delete pIn;
		std::cout << "Error: Cannot open input stream." << std::endl;
		return 1;
	}


	CRgtFile oRgt;
	try {
		oRgt.Load(pIn); // void Load(IFileStore::IStream *pFile);
	} catch(CRainmanException *pE) {
		delete pIn;
		delete pOut;
		//throw new CModStudioException(pE, __FILE__, __LINE__, "Cannot load file \'%s\'", saFile);
		std::cout << "Error: Cannot load file." << std::endl;
		return 1;
	}

	char* saOutFile = _strdup(infile);
	//if(!saOutFile) throw new CModStudioException(__FILE__, __LINE__, "Memory allocation error");

	if (!saOutFile) {
		std::cout << "Error: Memory allocation." << std::endl;
		return 1;
	}

	switch(oRgt.GetImageFormat()) {
		case CRgtFile::IF_Tga:
			strcpy(strchr(saOutFile, '.'), ".tga");
			//sOutFormat = wxT("TGA 32bit RGBA");
			std::cout << "Format: TGA 32bit RGBA" << std::endl;
			break;
		case CRgtFile::IF_Dxtc:
			//oRgt.setDXTCodec((CRgtFile::tfnDXTCodec)TheConstruct->oSquishLibrary.GetSymbol(wxT("CompressImage")), (CRgtFile::tfnDXTCodec)TheConstruct->oSquishLibrary.GetSymbol(wxT("DecompressImage")));
			strcpy(strchr(saOutFile, '.'), ".dds");
			//sOutFormat = wxT("DDS ");
			switch(oRgt.GetProperty(CRgtFile::IP_CompressionLevel)) {
				case 1:
					//sOutFormat.Append(wxT("DXT1"));
					std::cout << "Format: DXT1" << std::endl;
					break;

				case 2:
					//sOutFormat.Append(wxT("DXT2"));
					std::cout << "Format: DXT2" << std::endl;
					break;

				case 3:
					//sOutFormat.Append(wxT("DXT3"));
					std::cout << "Format: DXT3" << std::endl;
					break;

				case 4:
					//sOutFormat.Append(wxT("DXT4"));
					std::cout << "Format: DXT4" << std::endl;
					break;

				case 5:
					//sOutFormat.Append(wxT("DXT5"));
					std::cout << "Format: DXT5" << std::endl;
					break;

				default:
					//sOutFormat.Append(wxT("unknown compression"));
					std::cout << "Format: unknown" << std::endl;
					break;
			};

			break;
		default:
			delete pIn;
			free(saOutFile);
			//throw new CModStudioException(0, __FILE__, __LINE__, "Uknown image format of \'%s\'", saFile);
			std::cout << "Error unknown image format" << std::endl;
			return 1;
	}
	/*if(oRgt.GetProperty(CRgtFile::IP_MipLevelCount) > 1)
	{
		sOutFormat.Append(wxT(" with mip levels"));
	}*/

	std::cout << "Output file: " << saOutFile << std::endl;


	try {
		//pOut = pMod->VOpenOutputStream(saOutFile, true);
		//pOut = cmstore.VOpenOutputStream(saOutFile, true);
		//pOut = cfilemap.VOpenOutputStream(saOutFile, true);
		pOut = cfilestore.VOpenOutputStream(saOutFile, true);
	} catch(CRainmanException *pE) {
		delete pIn;
		free(saOutFile);
		//throw new CModStudioException(pE, __FILE__, __LINE__, "Cannot open output stream for \'%s\'", saFile);
		std::cout << "Error: Cannot open output stream." << std::endl;
		return 1;
	}

	try {
		//oRgt.SaveTGA(pOut);
		oRgt.SaveGeneric(pOut);
	} catch(CRainmanException *pE) {
		delete pIn;
		delete pOut;
		free(saOutFile);
		//throw new CModStudioException(pE, __FILE__, __LINE__, "Cannot save generic of \'%s\'", saFile);
		std::cout << "Error: Cannot save generic." << std::endl;
	}

	delete pIn;
	delete pOut;
	free(saOutFile);

	return 0;
}