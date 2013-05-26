// Utility per gli script dei materiali (.ADM), luci (.ADL),
// oggetti e patch (.ADO), dummy (.ADD)
// Made by [AD]Turbo


#ifndef _UTILSSCRIPT_H_
#define _UTILSSCRIPT_H_

#include "ADrender.h"
#include "scene3D.h"
#include "patch.h"

// -------------------------------------------------
// procedure di utilita' generale a tutti gli script
// -------------------------------------------------
char *get_file_ext(char *nomefile);  // si ottiene l'estensione da un nomefile (es: "pippo.pcx" -> "PCX")
int find_first_section(char* memfile, int pos);
int find_next_section(char* memfile, int pos);
char *get_section_type(char* memfile, int pos);
char *get_section_value(char* memfile, int pos);
int get_subsection(char* memfile, int pos);
int get_first_key(char* memfile, int pos);
int get_next_key(char* memfile, int pos);
char *get_key_type(char* memfile, int pos);
char *get_key_value(char* memfile, int pos);

// alloca in memoria il file togliendo commenti, spazi inutili e andate a capo
// serve per mettere in memoria uno script
char *alloc_script(char *filename);



// ----------------------------------------------
// procedure proprie per gli script dei materiali
// ----------------------------------------------
int load_ADM (char *nfile, AD_Scene3D *s);



// ----------------------------------------------
// procedure proprie per gli script delle luci
// ----------------------------------------------
int load_ADL (char *nfile, AD_Scene3D *s);



// ----------------------------------------------
// procedure proprie per gli script degli oggetti
// ----------------------------------------------
int load_ADO (char *nfile, AD_Scene3D *s);



// ----------------------------------------------
// procedure proprie per gli script dei Dummy$
// ----------------------------------------------
int load_ADD (char *nfile, AD_Scene3D *s);


// -----------------------------------------------------
// procedure proprie per gli script dei particle systems
// -----------------------------------------------------
int load_ADP (char *nfile, AD_Scene3D *s);


#endif
