#include <windows.h>
#include <stdio.h>

void debug_error(HWND hWnd, char *messaggio)
{
	MessageBox(hWnd, messaggio,"AD-Debug", NULL); 
	exit(0);
}


void memsetd(void *dest, unsigned int val, int dwords) 
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


/*
void memsetd(void *dest, unsigned int val, int dwords) 
{
	_asm
	{

			mov edi, dest
			push ecx

			push eax
			mov eax,val

			mov ecx, dwords
            rep stosd

			pop eax
			pop ecx
		}
}
*/


void memcpyd(void *dest, void *sorg, int dwords) 
{
	_asm{
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


void rgb_to_hsv(float r, float g, float b, float *h, float *s, float *v)
{
  float hh, ss, vv;
  float min, max;
  float delta;

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


void hsv_to_rgb(float h, float s, float v, float *r, float *g, float *b)
{
  double f, p, q, t;

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
          *r = (float)v;
          *g = (float)t;
          *b = (float)p;
          break;

        case 1:
          *r = (float)q;
          *g = (float)v;
          *b = (float)p;
          break;

        case 2:
          *r = (float)p;
          *g = (float)v;
          *b = (float)t;
          break;

        case 3:
          *r = (float)p;
          *g = (float)q;
          *b = (float)v;
          break;
        case 4:
          *r = (float)t;
          *g = (float)p;
          *b = (float)v;
          break;

        case 5:
          *r = (float)v;
          *g = (float)p;
          *b = (float)q;
          break;
        }
    }
}


float my_atof(char *s)
{
  int k, l;
  
  l=strlen(s);
  for (k=0; k<l; k++)
  {
	if (s[k]==',') s[k]='.';
  }
  return(float(atof(s)));
}


char esiste_file (char *nome)
{
   FILE *f;

   f=fopen(nome, "rb");
   if (f==(FILE *)NULL) return(0);
   else {
          fclose(f);
          return(1);
        }
}


void write_string0 (FILE *f, char *st)
{
  int k;

  for (k=0; st[k]!='\0'; k++)
    fwrite(&st[k], sizeof(char), 1, f);
  fwrite(&st[k], sizeof(char), 1, f);  // scrivo anche il \0
}