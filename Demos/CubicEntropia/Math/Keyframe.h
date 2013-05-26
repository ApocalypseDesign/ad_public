#ifndef _KEYFRAME_H_
#define _KEYFRAME_H_

#include <stdio.h>
#include "A3DFile.h"
#include "vect3D.h"
#include "quater.h"

// BITs di flag per il campo type delle tracce
// definiscono il tipo di traccia di rotazione; una assoluta
// hai i quaternioni assoluti, quella relativa ha i quaternioni
// ognuno che realizza una rotazione rispetto al precedente
#define ABSOLUTE_TRACK 1
#define RELATIVE_TRACK 2

// indicano di che tipo e' la traccia (da come era nel max)
#define TCB_TRACK 4
#define BEZIER_TRACK 8
#define LINEAR_TRACK 16
#define BOOLEAN_TRACK 32

// prototipo della funzione di ease che e' globale
float4 spline_ease (float4 t, float4 easefrom, float4 easeto);

// DEFINIZIONE DI UNA KEY DI POSIZIONE
typedef struct
{
  float4 tension, bias, continuity; // parametri della b-spline
  float4 easeto, easefrom;
  AD_Vect3D incomtg;    // tangente incoming
  AD_Vect3D outcomtg;   // tangente outcoming
  int32 posintrack;
  AD_Vect3D p;          // valore di posizione
} POSKEY;  // puo' essere usata da un oggetto, oppure dalla telecamera (in
	       // questo caso sia per la position, che per il target)


// DEFINIZIONE DI UNA KEY DI HIDING
typedef int32 HIDEKEY;

// DEFINIZIONE DI UNA KEY DI ROTAZIONE
typedef struct
{
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
  float4 tension, bias, continuity; // parametri della b-spline
  float4 easeto, easefrom;
  float4 incomtg;
  float4 outcomtg;
  float4 roll;    // questo valore e' in gradi nel 3ds !!! con la init()
		  // lo si trasforma in radianti
  int32 posintrack;
} ROLLKEY;        // usata solo dalla telecamera


// DEFINIZIONE DI UNA TRACCIA GENERICA (classe base)
class CKeyTrack
{
private:
	virtual void m_SortKeys(void) { return; };

public:
	int32 p_ORTBefore, p_ORTAfter;
	int32 p_Type;
	int32 p_NumKeys;

    float4 m_CycleTimeFloat(int32 start, int32 end, float4 t);
    int32 m_CycleTimeInt(int32 start, int32 end, int32 t);
	int32 m_NumCycles(int32 start, int32 end, float4 t);
	
	CKeyTrack()
	{ 
		p_NumKeys=-1;
		p_Type=0;
        p_ORTBefore=OOR_HOLD;
        p_ORTBefore=OOR_HOLD;
	}
	virtual int32 m_Init(float4 scale) { return (1); };
	virtual void m_GetTimeRange(float4 *timeStart, float4 *timeEnd) { return; };
};



// DEFINIZIONE DI UNA TRACCIA DI VISIBILITA' (hiding)
class CHideTrack : public CKeyTrack
{
private:
	void m_SortKeys(void);

public:
	HIDEKEY *p_Keys;         // array delle key di hiding
	int32 p_InitialState;

	CHideTrack();
	int32 m_Init(float4 scale);
	void m_GetData(float4 framepos, int32 *out);
	void m_GetTimeRange(float4 *timeStart, float4 *timeEnd);
};



// DEFINIZIONE DI UNA TRACCIA DI POSIZIONI (per oggetti e telecamera)
class CPosTrack : public CKeyTrack
{
private:
	void m_SortKeys(void);

public:
	POSKEY *p_Keys;         // array delle key di posizione

	CPosTrack();
	int32 m_Init(float4 scale);
	void m_GetData(float4 framepos, AD_Vect3D *out);
	void m_GetTimeRange(float4 *timeStart, float4 *timeEnd);
};



// DEFINIZIONE DI UNA TRACCIA DI ROTAZIONI (solo per gli oggetti !!!)
class CRotTrack : public CKeyTrack
{
private:
	void m_SortKeys(void);

public:
	ROTKEY *p_Keys;             // array delle key di rotazione

	CRotTrack();
	int32 m_Init(float4 scale);
	void m_GetData(float4 framepos, AD_Quaternion *out);
	void m_GetData(float4 framepos, AD_Matrix *out);
	void m_GetTimeRange(float4 *timeStart, float4 *timeEnd);
};




// DEFINIZIONE DI UNA TRACCIA DI ROLL (solo per la telecamera !!!)
class CRollTrack : public CKeyTrack
{
private:
	void m_SortKeys(void);

public:
	ROLLKEY *p_Keys;      // array delle key di roll

	CRollTrack();
	int32 m_Init(float4 scale);
	void m_GetData(float4 framepos, float4 *out);     // numero del frame
	void m_GetTimeRange(float4 *timeStart, float4 *timeEnd);
};

#endif