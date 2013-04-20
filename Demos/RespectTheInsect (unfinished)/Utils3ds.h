// procedure utili per caricare roba da .3DS by HereBit

#ifndef _UTILS3DS_H_
#define _UTILS3DS_H_

#include "keyframe.h"
#include "camera.h"
#include "object3d.h"
#include "scene3d.h"
#include "lights.h"
#include "ADbase.h"

 //*** DEFINIZIONI DEI CHUNKS *********************************

 //>------ Primary chunk
 #define MAIN3DS       0x4D4D

 //>------ Main Chunks
 #define VERSION3DS    0x0002
 #define EDIT3DS       0x3D3D  // this is the start of the editor config
 #define KEYF3DS       0xB000  // this is the start of the keyframer config

 //>------ sub defines of EDIT3DS
 #define EDIT_MATERIAL 0xAFFF
 #define EDIT_OBJECT   0x4000

 #define MAT_NAME 0xA000
 #define MAT_MAP1 0xA200
 #define MAT_MAP2 0xA34C
 #define MAP_FILE exA300

 //>------ sub defines of EDIT_OBJECT
 #define OBJ_TRIMESH   0x4100
 #define OBJ_LIGHT     0x4600
 #define OBJ_SPOTLIGHT 0x4610
 #define OBJ_CAMERA    0x4700

 //>------ sub defines of OBJ_TRIMESH
 #define TRI_VERTEXL   0x4110
 #define TRI_FACEL1    0x4120
 #define TRI_MATERIAL  0x4130
 #define TRI_MAPCOORD  0x4140
 #define TRI_SMOOTH    0x4150
 #define TRI_LOCAL     0x4160
 #define TRI_VISIBLE   0x4165

 //>>------ sub defs of KEYF3DS
 // i seguenti 7 chunk sono quelli che fanno scattare l'aumrnto del c_id
 #define KEYF_AMBIENT  0xB001
 #define KEYF_MESH   0xB002
 #define KEYF_CAMERA 0xB003
 #define KEYF_TARGET 0xB004
 #define KEYF_OMNI   0xB005
 #define KEYF_SPOTTARGET 0xB006
 #define KEYF_SPOT   0xB007


 #define KEYF_FRAMES     0xB008
 #define KEYF_OBJNAME    0xB010
 #define KEYF_PIVOT      0xB013
 #define KEYF_POSTRACK   0xB020
 #define KEYF_ROTTRACK   0xB021
 #define KEYF_SCALETRACK 0xB022
 #define KEYF_FOVTRACK   0xB023
 #define KEYF_ROLLTRACK  0xB024
 #define KEYF_COLORTRACK 0xB025
 

 //>>------  these define the different color chunk types
 #define fCOL_RGB  0x0010  // la f iniziale sta per float, cioe' i dati sono dei float
 #define bCOL_RGB  0x0011  // la b iniziale sta per byte, cioe' i dati sono dei byte


// costanti messe da Turbo, usate nella riconosci oggetto
 #define CAMERA 1
 #define OMNILIGHT 2
 #define SPOTLIGHT 3


//*** PROCEDURE UTILISSIME HEREBITTOLOSE ****************************************
class File3DS
{
public:

	File3DS()
	{}


    // ********************************************************
    // *************  PROCEDURE DI BASSO LIVELLO  *************
    // ********************************************************

	unsigned short int get_chunk_type(char *file3DS, int offset);
    char *alloc3DS(char *filename);
    // alloca in memoria il file aggiungendo un word di valore 0 alla fine 
    // (usato per capire quando sono finiti i chunks)
    // restituisce la dimensione allocata in byte o -1 se c'e' stato un errore
	int get_chunk_fullsize(char *file3DS, int offset);
    // restituisce la dimensione del chunk + i subchunks + i 6 byte iniziali
    int get_chunk_size(char *file3DS, int offset);
    // restituisce la dimensione del chunk, esclusi i 6 byte iniziali e i subchunks
    // purtroppo alcuni chunks hanno dimensione variabile, per questo servono
    // i valori di *file3DS e offset ...
    // ATTENZIONE: per ora ho fatto solo quelli che mi pare che ci servono!!

    int find_next_chunk(char *file3DS, int offset);
    // restituisce l'offset del prossimo chunk di stesso livello o inferiore
    // a partire da offset dove offset punta a un chunk-id
    // (salta i subchunks)
    // ATTENZIONE: non controlla se si va oltre la fine del file!!!

    int find_next_chunk_oftype(char *file3DS, int offset, short int cktype);
    // restituisce l'offset del prossimo chunk a partire da offset del tipo cktype
    // dove offset punta a un chunk-id
    // (salta i subchunks) e restituisce -1 se non trovato

    int find_first_subchunk(char *file3DS, int offset);
    // restituisce la posizione del primo subchunk del chunk offset
    // dove offset punta a un chunk-id
    // se il chunk non ha subchunks ritorna -1

    int find_first_subchunk_oftype(char *file3DS, int offset, short int cktype);
    // restituisce la posizione del primo subchunk del chunk offset
    // dove offset punta a un chunk-id, del tipo cktype
    // se il chunk non ha subchunks o quel tipo non viene trovato ritorna -1

    int riconosci_oggetto (char *file3DS, int offset);
    // riconosce il tipo di oggetto; file3ds+offset puntano ad un chunk_id
    // 0x4000 (EDIT_OBJECT)



    // ********************************************************
    // *************  PROCEDURE DI MEDIO LIVELLO  *************
    // ********************************************************	
	
	void camera_load_settings (AD_Camera *cam, char *file3DS, int offset);
    void camera_load_keytracks (AD_Camera *cam, char *file3DS, int offset);
    void camera_load_keytargettracks (AD_Camera *cam, char *file3DS, int offset);

    void omnilight_load_settings(AD_Omnilight *ol, char *file3DS, int offset);
    void omnilight_load_keytracks (AD_Omnilight *ol, char *file3DS, int offset);
    
    void spotlight_load_settings(AD_Spotlight *sl, char *file3DS, int offset);
    void spotlight_load_keytracks (AD_Spotlight *sl, char *file3DS, int offset);
	void spotlight_load_keytargettracks (AD_Spotlight *sl, char *file3DS, int offset);

    void object_load_vertex_list(AD_Object3D *obj, char *file3DS, int offset);
    void object_load_face_list(AD_Object3D *obj, char *file3DS, int offset);
    void object_load_material_list(AD_Object3D *obj, char *file3DS, int offset);
    void object_load_smooth_list(AD_Object3D *obj, char *file3DS, int offset);
    void object_load_mapcoords_list(AD_Object3D *obj, char *file3DS, int offset);
    void object_load_local_coordinate_system(AD_Object3D *obj, char *file3DS, int offset);
    void object_load_mesh(AD_Object3D *obj, char *file3DS, int offset);
    void object_load_dummy(AD_Object3D *obj, char *file3DS, int offset);
    void object_load_keytracks(AD_Object3D *obj, char *file3DS, int offset);

	void keyframe_load_postrack(postrack *pt, char *file3DS, int offset);
	void keyframe_load_rottrack(rottrack *rt, char *file3DS, int offset);
	void keyframe_load_rolltrack(rolltrack *lt, char *file3DS, int offset);



    // ********************************************************
    // **************  PROCEDURE DI ALTO LIVELLO  *************
    // ********************************************************

	void LoadFromMemory(AD_Scene3D *s, char *ptr);
	void LoadFromFile(AD_Scene3D *s, char *filename);
	void SaveToMemory(char *ptr, unsigned int *size);
	void SaveToFile(char *filename);
	
	AD_Scene3D *myscene;   // copia del parametro di LoadFromFile
};

#endif
