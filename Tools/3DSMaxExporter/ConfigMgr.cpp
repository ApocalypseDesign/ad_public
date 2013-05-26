/*===========================================================================*\
 | 
 |  FILE:	ConfigMgr.cpp
 |			A3D Scene Exporter 
 |			for 3D Studio MAX R3.0
 | 
 |  AUTH:   Turbo / Apocalypse Design
 |
 |  HIST:	Started 10-8-99
 | 
\*===========================================================================*/

#include "SceneExport.h"


// change the name when using this skeleton
#define		CFG_FILENAME		"A3D_Export.cfg"
#define		CFG_VERSION			0x001


/*===========================================================================*\
 |  Get the config file's full name on the disk
\*===========================================================================*/

TSTR SkeletonExporter::GetConfigFilename()
{
	TSTR filename;
	
	filename += ip->GetDir(APP_PLUGCFG_DIR);
	filename += "\\";
	filename += CFG_FILENAME;

	return filename;
}

/*===========================================================================*\
 |  Load and save the values
\*===========================================================================*/

void SkeletonExporter::SaveExporterConfig()
{
	// Open the configuration file for writing
	TSTR filename = GetConfigFilename();
	FILE* cfgStream;

	cfgStream = fopen(filename, "wb");
	if (!cfgStream) return;
	// Write CFG version
	_putw(CFG_VERSION, cfgStream);
	fputc(makeADM, cfgStream);
	fputc(makeADL, cfgStream);
	fputc(makeADO, cfgStream);
	//fputc(makeADD, cfgStream);
	fputc(makeADP, cfgStream);
	fputc(makeADS, cfgStream);
	fputc(makeRAY, cfgStream);
	fclose(cfgStream);
}


BOOL SkeletonExporter::LoadExporterConfig()
{
	// Open the configuration file for reading
	TSTR filename = GetConfigFilename();

	// If the file doesn't exist yet, write out the defaults
	if(!DoesFileExist(filename)) SaveExporterConfig();

	FILE* cfgStream;
	cfgStream = fopen(filename, "rb");
	if (!cfgStream)
		return FALSE;

	// First item is a file version
	int fileVersion = _getw(cfgStream);
	if (fileVersion > CFG_VERSION) {
		// Unknown version
		fclose(cfgStream);
		return FALSE;
	}
	makeADM = fgetc(cfgStream);
	makeADL = fgetc(cfgStream);
	makeADO = fgetc(cfgStream);
	//makeADD = fgetc(cfgStream);
	makeADP = fgetc(cfgStream);
	makeADS = fgetc(cfgStream);
	makeRAY = fgetc(cfgStream);
	fclose(cfgStream);
	return TRUE;
}