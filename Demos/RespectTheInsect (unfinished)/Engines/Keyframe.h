// Definizion dell'oggetto KeyTrack e dei suoi oggetti derivati
// By [Turbo]

#ifndef _KEYFRAME_H_
#define _KEYFRAME_H_

#include <stdio.h>
#include "..\Math\vect3D.h"
#include "..\Math\quater.h"

// BITs di flag per ilc ampo type delle tracce
// definiscono il tipo di traccia di rotazione; una assoluta
// hai i quaternioni assoluti, quella relativa ha i quaternioni
// ognuno che realizza una rotazione rispetto al precedente
#define ABSOLUTE_TRACK 1
#define RELATIVE_TRACK 2
// indicano di che tipo e' la traccia (da come era nel max)
#define TCB_TRACK 4
#define BEZIER_TRACK 8
#define LINEAR_TRACK 16

// prototipo della funzione di ease che e' globale
float4 spline_ease (float4 t, float4 easefrom, float4 easeto);

// DEFINIZIONE DI UNA KEY DI POSIZIONE
typedef struct
{
  int16 splineflags;
  float4 tension, bias, continuity; // parametri della b-spline
  float4 easeto, easefrom;

  AD_Vect3D incomtg;    // tangente incoming
  AD_Vect3D outcomtg;   // tangente outcoming
  int32 posintrack;
  AD_Vect3D p;          // valore di posizione
} POSKEY;  // puo' essere usata da un oggetto, oppure dalla telecamera (in
	       // questo caso sia per la position, che per il target)



// DEFINIZIONE DI UNA KEY DI ROTAZIONE
typedef struct
{
  int16 splineflags;    // non usati, in quanto usiamo uno SLERP per interpolare non uno SQUAD
  float4 tension, bias, continuity; // parametri della b-spline, non usati !!!
  float4 easeto, easefrom;

  float4 roll;           // e' gia' in radianti nel .3ds !!!
  AD_Quaternion rotquat; // questo AD_Quaternione lo si deve costruire dai due
		                 // dati precedenti; lo si fara con la init()
  AD_Quaternion incomtg, outcomtg;
  int32 posintrack;
} ROTKEY;  // usata solo dagli oggetti



// DEFINIZIONE DI UNA KEY DI ROLL
typedef struct
{
  int16 splineflags;
  float4 tension, bias, continuity; // parametri della b-spline
  float4 easeto, easefrom;

  float4 incomtg;
  float4 outcomtg;
  float4 roll;    // questo valore e' in gradi nel 3ds !!! con la init()
		  // lo si trasforma in radianti
  int32 posintrack;

} ROLLKEY;        // usata solo dalla telecamera


// DEFINIZIONE DI UNA TRACCIA GENERICA (classe base)
class keytrack
{
public:
	keytrack()
	{ 
		numkey=-1;
		type=0;
	}

	// inizializza le TBC spline, con codice di uscita di eventuale errore
	int init();

	int16 trackflags;   // non usati per adesso, ma molto utili !
	int type;
	int32 numkey;

protected:
private:
};



// DEFINIZIONE DI UNA TRACCIA DI POSIZIONI (per oggetti e telecamera)
class postrack : public keytrack
{
public:
	postrack()                // costruttore
	{
	  posizioni=NULL;
	  numkey=-1;
	}
	int init(void);
	void get_data(float4 framepos, AD_Vect3D *out);  // numero del frame

	POSKEY *posizioni;         // array delle key di posizione
protected:
private:
};




// DEFINIZIONE DI UNA TRACCIA DI ROTAZIONI (solo per gli oggetti !!!)
class rottrack : public keytrack
{
public:
	rottrack()                    // costruttore
	{
	  rotazioni=NULL;
	  type=RELATIVE_TRACK;
	  numkey=-1;
	}
	int init();
	void get_data(float4 framepos, AD_Quaternion *out);   // numero del frame

	ROTKEY *rotazioni;             // array delle key di rotazione
protected:
private:
};





// DEFINIZIONE DI UNA TRACCIA DI ROLL (solo per la telecamera !!!)
class rolltrack : public keytrack
{
public:
	rolltrack()
	{
	  rollazioni=NULL;
	  numkey=-1;
	}                          // costruttore
	int init(void);
	void get_data(float4 framepos, float4 *out);     // numero del frame

	ROLLKEY *rollazioni;      // array delle key di roll
protected:
private:
};




class material_parameters_tracks
{
public:
   material_parameters_tracks()
   {
      trasparency_track=(rolltrack *)NULL;
   }

   rolltrack *trasparency_track;
   void init(void);
};

#endif