// Utility per gli script dei materiali (.ADM), luci (.ADL),
// oggetti e patch (.ADO), dummy (.ADD)
// Made by [AD]Turbo


#ifndef _UTILSSCRIPT_H_
#define _UTILSSCRIPT_H_

#include "Scene3D.h"
//#include "patch.h"

// -------------------------------------------------
// procedure di utilita' generale a tutti gli script
// -------------------------------------------------
void get_file_ext(char8 *nomefile, char8 *out);  // si ottiene l'estensione da un nomefile (es: "pippo.pcx" -> "PCX")
int32 find_first_section(char8 *memfile, int32 pos);
int32 find_next_section(char8 *memfile, int32 pos);
void get_section_type(char8 *memfile, int32 pos, char8 *out);
void get_section_value(char8 *memfile, int32 pos, char8 *out);
int32 get_subsection(char8 *memfile, int32 pos);
int32 get_first_key(char8 *memfile, int32 pos);
int32 get_next_key(char8 *memfile, int32 pos);
void get_key_type(char8 *memfile, int32 pos, char8 *out);
int32 get_key_value(char8 *memfile, int32 pos, int32 quale, char8 *out);

// alloca in memoria il file togliendo commenti, spazi inutili e andate a capo
// serve per mettere in memoria uno script
char8 *alloc_script(char8 *filename);



// ----------------------------------------------
// procedure proprie per gli script dei materiali
// ----------------------------------------------
int32 load_ADM (char8 *nfile, CScene3D *s);



// ----------------------------------------------
// procedure proprie per gli script delle luci
// ----------------------------------------------
int32 load_ADL (char8 *nfile, CScene3D *s);



// ----------------------------------------------
// procedure proprie per gli script degli oggetti
// ----------------------------------------------
int32 load_ADO (char8 *nfile, CScene3D *s);



// ----------------------------------------------
// procedure proprie per gli script dei Dummy$
// ----------------------------------------------
// int32 load_ADD (char8 *nfile, CScene3D *s);


// -----------------------------------------------------
// procedure proprie per gli script dei particle systems
// -----------------------------------------------------
int32 load_ADP (char8 *nfile, CScene3D *s);


#endif