#include "utility.h"

void debug_error (HWND hWnd, char8 *messaggio)
{
	MessageBox(hWnd, messaggio,"AD-Debug", NULL); 
	exit(0);
}

// -------------------------------------------------------------------

void memsetd (void *dest, uint32 val, uint32 dwords) 
{
	_asm
	{

			mov edi, [dest]
			mov ecx, [dwords]

			shr ecx, 1
			mov eax, [val]

            lea esi, [edi+ecx*8]
			neg ecx

ciclo:
			mov [esi+ecx*8], eax
			mov [esi+ecx*8+4], eax

			inc ecx
			jnz ciclo
		}
}

// -------------------------------------------------------------------

void memcpyd(void *dest, void *sorg, int32 dwords) 
{
	_asm {
			push edi
			push ecx
			push esi
			mov edi,dest
			mov esi,sorg
			mov ecx,dwords
			rep movsd
			pop esi
			pop ecx
			pop edi
	}
}

// -------------------------------------------------------------------

void rgb_to_hsv(float4 r, float4 g, float4 b, float4 *h, float4 *s, float4 *v)
{
  float4 hh, ss, vv;
  float4 min, max;
  float4 delta;

  hh = 0;
  if (r > g)
    {
      if (r > b)
        max = r;
      else
        max = b;

      if (g < b)
        min = g;
      else
        min = b;
    }
  else
    {
      if (g > b)
        max = g;
      else
        max = b;

      if (r < b)
        min = r;
      else
        min = b;
    }

  vv = max;

  if (max != 0)
    ss = (max - min) / max;
  else
    ss = 0.0;

  if (ss == 0.0)
  {
      hh = 0.0;
  }
  else
    {
      delta = max - min;

      if (r == max)
        hh = (g - b) / delta;
      else if (g == max)
        hh = 2 + (b - r) / delta;
      else if (b == max)
        hh = 4 + (r - g) / delta;

      hh /= 6.0f;

      if (hh < 0.0)
        hh += 1.0;
      else if (hh >= 1.0)
        hh -= 1.0;
    }

  *h = hh;
  *s = ss;
  *v = vv;
}

// -------------------------------------------------------------------

void hsv_to_rgb(float4 h, float4 s, float4 v, float4 *r, float4 *g, float4 *b)
{
  float8 f, p, q, t;

  if (s == 0.0)
    {
      *r = v;
      *g = v;
      *b = v;
    }
  else
    {
      h *= 6.0f;
      if (h == 6.0f) h = 0.0;

      f = h - (int)h;
      p = v * (1.0 - s);
      q = v * (1.0 - s*f);
      t = v * (1.0 - s*(1.0 - f));

      switch ((int)h)
      {
        case 0:
          *r = (float4)v;
          *g = (float4)t;
          *b = (float4)p;
          break;

        case 1:
          *r = (float4)q;
          *g = (float4)v;
          *b = (float4)p;
          break;

        case 2:
          *r = (float4)p;
          *g = (float4)v;
          *b = (float4)t;
          break;

        case 3:
          *r = (float4)p;
          *g = (float4)q;
          *b = (float4)v;
          break;
        case 4:
          *r = (float4)t;
          *g = (float4)p;
          *b = (float4)v;
          break;

        case 5:
          *r = (float4)v;
          *g = (float4)p;
          *b = (float4)q;
          break;
        }
    }
}

// -------------------------------------------------------------------

float4 my_atof(char8 *s)
{
  // converte numeri decimali che usino sia il "." che la "," per
  // la separazione degli interi dai decimali
  int32 k, l;
  
  l=strlen(s);
  for (k=0; k<l; k++)
  {
	if (s[k]==',') s[k]='.';
  }
  return(float4(atof(s)));
}

// -------------------------------------------------------------------

char8 esiste_file (char8 *nome)
{
   FILE *f;

   f=fopen(nome, "rb");
   if (f==(FILE *)NULL) return(0);
   else {
          fclose(f);
          return(1);
        }
}

// -------------------------------------------------------------------

void write_string0 (FILE *f, char8 *st)
{
  int32 k;

  for (k=0; st[k]!='\0'; k++)
    fwrite(&st[k], sizeof(char), 1, f);
  
  // scrivo anche il \0
  fwrite(&st[k], sizeof(char), 1, f);
}

// -------------------------------------------------------------------

void read_string0 (FILE *f, char8 *st)
{
  int32 k;
  char8 ch;

  ch=' ';
  k=0;
  while (ch != '\0')
  {
     fread(&ch, sizeof(char), 1, f);
     st[k]=ch;
     k++;
  }
}

// -------------------------------------------------------------------

float4 map_intervals (float4 Interval1Start, float4 Interval1End, 
                      float4 Interval2Start, float4 Interval2End,
					  float4 Interval1Position)
{
  float4 t, k;
  
  if (Interval1Position<=Interval1Start) return(Interval2Start);
  else
  if (Interval1Position>=Interval1End) return(Interval2End);
  else
  {
	 t=(Interval1Position-Interval1Start)/(Interval1End-Interval1Start);
	 k=t*(Interval2End-Interval2Start)+Interval2Start;
	 return(k);
  }
}

// -------------------------------------------------------------------
#ifdef  _MSC_VER
int32 cpu_has3DNow (void) { return GetCPUCaps(HAS_3DNOW) != 0; }
int32 cpu_hasMMX (void) { return GetCPUCaps(HAS_MMX) != 0; }
int32 cpu_hasSSE (void) { return GetCPUCaps(HAS_SSE) != 0; }
int32 cpu_isAthlon (void) { return GetCPUCaps(CPU_TYPE)==AMD_ATHLON; }
int32 cpu_isK6III (void) { return GetCPUCaps (CPU_TYPE) == AMD_K6_3; }
#else
int32 cpu_has3DNow (void) { return 0; }
int32 cpu_hasMMX (void) { return 0; }
int32 cpu_hasSSE (void) { return 0; }
int32 cpu_isAthlon (void) { return 0; }
int32 cpu_isK6III (void) { return 0; }
#endif