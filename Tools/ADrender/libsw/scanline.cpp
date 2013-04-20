//  Disegno delle scanline, by [AD]Turbo (zio animals, ma i
//  lavoracci sporchi li devo sempre fare io ?)
//  ME COJONI! 109 procedure di disegno!!!!
//  69 procedure di 109 CODATE E FUNZIONANTI !
//  mancano le altre 50 che contemplano lo schema di mixing
//  tex+env+luci+trasparenza

#include <windows.h>
#include "scanline.h"
#include "..\fastmath.h"
#include "extrend.h"


// ####################################################################
// ############## PROCEDURE DI DISEGNO DELLE SCAN LINE ################
// ####################################################################

void scanline_flat (void)
{
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;
  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);

  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      *intzb=*ddd;;
      uvideo32[offset]=color_flat;
    }
    offset++;   intzb+=1;
    d+=dz;
  }
}


/*
void scanline_RGB (void)
{
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
	if (*ddd > *intzb)
    {
      *intzb=*ddd;
      uvideo32[offset]=( (RR1 & 0x00FF0000)|
		                 ((GG1 & 0x00FF0000)>>8)|
						 (BB1>>16) );
    }
    offset++;
	intzb+=1;
    d+=dz;
    RR1+=dR;
	GG1+=dG;
	BB1+=dB;
  }
}
*/
void __declspec(naked) scanline_RGB (void)
{
  _asm
  {
	 push ebp

	 mov ebp, [left_R]
	 mov edx, [left_G]
	 mov esi, [left_B]

	 mov eax, [left_x]
	 mov ebx, [right_x]

	 fld [left_z]
	 add eax, 0xFFFF

	 add ebx, 0xFFFF
	 sar eax, 16

	 sar ebx, 16
	 test eax, eax

	 jge noclip2Dleft

// clip2Dleft:

	 mov  ecx, eax
	 fstp  dword ptr [fi2]   // tolgo il lef_z che ho messo poco fa
	 
	 neg  ecx
	 mov  [fi], ecx

     mov edi, eax      // edi=xx1
	 imul edi, [dR]    // edi=xx1*du

     fild  dword ptr [fi]
	 fmul  dword ptr [dz]

	 sub  ebp, edi     // ebp = u+=(-xx1)*du

     mov edi, eax
	 imul edi, [dG]
	 fadd  dword ptr [left_z]    // sullo stack c'e' d
	 sub  edx, edi   // edi = v+=(-xx1)*dv

     mov edi, eax
	 imul edi, [dB]
	 sub esi, edi

     xor eax, eax    // xx1 = 0


noclip2Dleft:

	 mov [BB1], esi

	 mov esi, [scrX]
	 cmp ebx, esi
	 jl noclip2Dright
     mov ebx, esi

noclip2Dright:

     sub ebx, eax        // ebx = xx2-xx1
	 mov esi, [offset0]
	 add esi, eax       // esi = offset = offset0+xx1
	 shl esi, 2

	 // a questo punto i registri sono cosi' messi
	 //
	 // ebx = xx2-xx1
     // ebp = RR1
	 // edx = GG1
	 // esi = offset
     // sullo stack FPU = d
	 // rimangono liberi  eax, edi, ecx

	 test ebx, ebx
	 jle fine

ciclo:

	 fld st(0)
     fadd dword ptr [trick]

	 mov edi, dword ptr [zbuf]
	 mov [l], ebx

	 lea ecx, [edi + esi]

	 fstp dword ptr [fi]      // fi = d
     mov eax, [fi]

	 cmp eax, [ecx]
	 jle next_pixel

// write_pixel:

	 mov [ecx], eax     // scrivo nello zbuffer
     mov edi, ebp       // edi = RR1

	 and edi, 0x00FF0000  // Rosso OK!
	 mov ecx, edx         // ecx=GG1

	 and ecx, 0x00FF0000
	 mov eax, [BB1]

	 shr ecx, 8    // Verde OK!
     or edi, ecx  // rosso + verde

	 shr eax, 16
	 or eax, edi

     mov ecx, [uvideo32]
	 mov [ecx+esi], eax

next_pixel:

	 add ebp, [dR]           //  RR1+=dR;
	 mov eax, [BB1]

	 add edx, [dG]           //  GG1+=dG;
	 add eax, [dB]

	 add esi, 4            // offset++
	 mov ebx, [l]

	 mov [BB1], eax
	 dec ebx

	 fadd dword ptr [dz]
	 jnz ciclo

fine:

	 pop ebp
	 fstp dword ptr [fi]
	 ret
  }
}


void scanline_RGB_tBLEND50 (void)
{
  unsigned int a, b;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
	  a=(uvideo32[offset] & 0x00fefefe) >> 1 ;
      b=((RR1 & 0x00FF0000) | ((GG1 & 0x00FF0000) >> 8) | ((BB1>>16)));
      b=(b & 0x00fefefe)>>1;
      uvideo32[offset]=a+b;
    }
    offset++;   intzb+=1;
    d+=dz;
    RR1+=dR;
	GG1+=dG;
	BB1+=dB;
  }
}


void scanline_RGB_tBLENDalpha (void)
{
  unsigned char a1, a2, a3, b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
	  a1=RR1>>16; a2=GG1>>16; a3=BB1>>16;
      b1=uvideo8[offset*4]; b2=uvideo8[offset*4+1]; b3=uvideo8[offset*4+2];

	  b1=b1+ttable[a1]-ttable[b1];
      b2=b2+ttable[a2]-ttable[b2];
	  b3=b3+ttable[a3]-ttable[b3];
      uvideo32[offset]=b1 | (b2<<8) | (b3<<16);     
    }
    offset++;   intzb+=1;
    d+=dz;
    RR1+=dR;
	GG1+=dG;
	BB1+=dB;
  }
}


// ###########################################################
// #####################  SEZIONE TEXTURE  ###################
// ###########################################################
/*
void scanline_texture (void)
{
  float newz;
  int *intzb, *ddd=(((int *)&d));

  d=left_z;
  u=left_u;
  v=left_v;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
    if (*ddd > *intzb)
    {
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
      *intzb=*ddd;
	  uvideo32[offset]=texmap32[tex_offset];
	}
    offset++;   intzb+=1;
    d+=dz;
    u+=du;
    v+=dv;
  }
}
*/

void __declspec(naked) scanline_texture (void)
{
  _asm
  {
	 push ebp

	 mov ebp, [left_u]
	 mov edx, [left_v]

	 mov eax, [left_x]
	 mov ebx, [right_x]

	 fld [left_z]
	 add eax, 0xFFFF

	 add ebx, 0xFFFF
	 sar eax, 16

	 sar ebx, 16
	 test eax, eax

	 jge noclip2Dleft

// clip2Dleft:

	 mov  esi, eax
	 fstp  dword ptr [fi2]   // tolgo il lef_z che ho messo poco fa
	 
	 neg  esi
	 mov  [fi], esi

     mov edi, eax      // edi=xx1
	 imul edi, [du]    // edi=xx1*du

     fild  dword ptr [fi]
	 fmul  dword ptr [dz]

	 sub  ebp, edi     // ebp = u+=(-xx1)*du

     mov edi, eax
	 imul edi, [dv]
	 fadd  dword ptr [left_z]    // sullo stack c'e' d
	 sub  edx, edi   // edi = v+=(-xx1)*dv

     xor eax, eax    // xx1 = 0


noclip2Dleft:

	 mov esi, [scrX]
	 cmp ebx, esi
	 jl noclip2Dright
     mov ebx, esi

noclip2Dright:

     sub ebx, eax        // ebx = xx2-xx1
	 mov esi, [offset0]
	 add esi, eax       // esi = offset = offset0+xx1
	 shl esi, 2

	 // a questo punto i registri sono cosi' messi
	 //
	 // ebx = xx2-xx1
     // ebp = u
	 // edx = v
	 // esi = offset
     // sullo stack FPU = d
	 // rimangono liberi  eax, edi, ecx

	 test ebx, ebx
	 jle fine

ciclo:

	 fld st(0)
     fadd dword ptr [trick]

	 mov edi, dword ptr [zbuf]
	 mov [l], ebx

	 lea ecx, [edi + esi]
     mov edi, ebp             // salvo ecx (che e' u)

	 fstp dword ptr [fi]      // fi = d
     mov eax, [fi]

	 cmp eax, [ecx]
	 jle next_pixel

// write_pixel:

	 mov [ecx], eax
	 mov eax, edx  // eax = v

     sar eax, 16
	 mov ecx, [vshift]

	 sar edi, 16
	 and eax, [vmask]

	 shl eax, cl     // eax = ((v>>16) & vmask)<<vshift
	 and edi, [_umask_]  // edi = (u >> 16) & _umask_

	 add eax, edi  // eax = tex_offset
	 mov edi, dword ptr [texmap32]

     mov eax, [edi + eax*4]
     mov ecx, [uvideo32]

	 mov [ecx+esi], eax

next_pixel:

	 add ebp, [du]           //  u+=du;
	 add edx, [dv]           //  v+=dv;

	 add esi, 4            // offset++
	 mov ebx, [l]

	 dec ebx
	 fadd dword ptr [dz]

	 jnz ciclo

fine:

	 pop ebp
	 fstp dword ptr [fi]
	 ret
  }
}


/*
void scanline_texture_tADD(void)
{
  unsigned int r, g, b;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  u=left_u;
  v=left_v;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
//      *intzb=*ddd;

	  b=texmap8[tex_offset*4];
	  g=texmap8[tex_offset*4+1];
	  r=texmap8[tex_offset*4+2];

      b=add_table[b+uvideo8[offset*4]];
      g=add_table[g+uvideo8[offset*4+1]];
      r=add_table[r+uvideo8[offset*4+2]];

	  uvideo32[offset]=(b | (g<<8) | (r<<16));
	}
    offset++;
	intzb+=1;
    d+=dz;
    u+=du;
    v+=dv;
  }
}
*/


void __declspec(naked) scanline_texture_tADD(void)
{
  _asm
  {
     push ebp

	 mov ebp, [left_u]
	 mov edx, [left_v]

	 fld [left_z]
	 mov eax, [left_x]

	 add eax, 0xFFFF
	 sar eax, 16

	 test eax, eax
	 jge noclip2Dleft

// clip2Dleft:

	 mov   ecx, eax
	 
	 neg  ecx
	 mov  [fi], ecx

     mov   ecx, eax         // ecx=xx1
	 imul  ecx, [du]     // ecx=xx1*du
	 sub  ebp, ecx   // ecx = envu+=(-xx1)*envdu

     mov   ecx, eax
	 imul  ecx, [dv]

	 sub  edx, ecx   // edi = envv+=(-xx1)*envdv
     xor eax, eax    // xx1 = 0

noclip2Dleft:

	 mov ebx, [right_x]
	 add ebx, 0xFFFF
	 sar ebx, 16

	 mov esi, [scrX]
	 cmp ebx, esi
	 jl noclip2Dright
     mov ebx, esi

noclip2Dright:

     sub ebx, eax        // ebx = xx2-xx1
	 mov esi, [offset0]
	 add esi, eax       // esi = offset = offset0+xx1
	 shl esi, 2

	 // a questo punto i registri sono cosi' messi
	 //
	 // ebx = xx2-xx1 (ma tanto poi metto in [l])
     // ebp = u
	 // edx = v
	 // esi = offset
     // sullo stack FPU = d
	 // sono liberi  eax, ecx, edi, ebx

	 test ebx, ebx
	 jle fine
	 mov [l], ebx

ciclo:

	 fld st(0)
	 fadd dword ptr [trick]

	 mov edi, dword ptr [zbuf]
     fstp dword ptr [fi]

     mov eax, [fi]
	 mov ecx, [edi + esi]

	 cmp eax, ecx
	 jle next_pixel

// write_pixel:

	 mov eax, edx  // eax = envv
     mov edi, ebp  // salvo ebp (che e' envu)

     sar eax, 16
	 mov ecx, [vshift]

	 and eax, [vmask]
	 shl eax, cl         // eax = (envv>>16)<<envvshift

	 sar edi, 16 
	 and edi, [_umask_]

     mov ebx, [texmap32]
	 add eax, edi        // eax = tex_offset

     mov edi, [uvideo32]
	 mov eax, [ebx + eax*4]   // eax = texel
	                          // rimangono liberi ecx, edi, ebx

	 mov ecx, [edi + esi]   // lettura pixel di fondo

	 and eax, 0x00fefeff
	 and ecx, 0x00fefeff
	 add ecx, eax     // ecx = (b & 0xfefeff)+(a & 0xfefeff)
	 mov ebx, ecx
	 and ebx, 0x1010100
	 mov eax, ebx
	 shr eax, 8
	 sub ebx, eax
	 or ecx, ebx

/*
	 add  al, cl
     sbb  ebx, ebx
     or   al, bl

	 add  ah, ch
     sbb  ebx, ebx
     or   ah, bl

     ror eax, 16
	 mov  edi, [uvideo32]

	 ror ecx, 16
	 add  edi, esi

	 add  al, cl
     sbb  ebx, ebx
     or   al, bl

     ror  eax, 16
*/
     mov [edi+esi], ecx
	 

next_pixel:

	 add ebp, [du]           //  u+=du;
	 add edx, [dv]           //  v+=dv;

	 add esi,4               // offset++
	 mov ebx, [l]

	 dec ebx
	 mov [l], ebx
	 jnz ciclo

fine:
	 fstp dword ptr [fi]
	 pop ebp
	 ret
  }
}



/*
void scanline_texture_tBLEND50 (void)
{
  unsigned int a, b;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  u=left_u;
  v=left_v;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
	  a=(texmap32[tex_offset] & 0x00fefefe) >> 1; 
	  b=(uvideo32[offset] & 0x00fefefe) >> 1; 
	  uvideo32[offset]=a+b;
	}
    offset++;   intzb+=1;
    d+=dz;
    u+=du;
    v+=dv;
  }
}
*/

void __declspec(naked) scanline_texture_tBLEND50 (void)
{
  _asm
  {
	 push ebp
	 mov ebp, [left_u]

	 mov edx, [left_v]
	 mov eax, [left_x]

	 mov ebx, [right_x]
	 add eax, 0xFFFF

	 add ebx, 0xFFFF
	 fld [left_z]

	 sar eax, 16
	 test eax, eax

	 jge noclip2Dleft

// clip2Dleft:

	 mov  esi, eax
	 fstp  dword ptr [fi2]   // tolgo il lef_z che ho messo poco fa
	 
	 neg  esi
	 mov  [fi], esi

     mov edi, eax    // edi=xx1
	 imul edi, [du]    // edi=xx1*du
     fild  dword ptr [fi]
	 fmul  dword ptr [dz]

	 sub  ebp, edi   // ebp = u+=(-xx1)*du

     mov edi, eax
	 imul edi, [dv]
	 fadd  dword ptr [left_z]    // sullo stack c'e' d

	 sub  edx, edi   // edi = v+=(-xx1)*dv
     xor eax, eax    // xx1 = 0


noclip2Dleft:

	 mov esi, [scrX]
	 sar ebx, 16

	 cmp ebx, esi
	 jl noclip2Dright

     mov ebx, esi

noclip2Dright:

     sub ebx, eax        // ebx = xx2-xx1
	 mov esi, [offset0]
	 add esi, eax       // esi = offset = offset0+xx1
	 shl esi, 2

	 // a questo punto i registri sono cosi' messi
	 //
	 // ebx = xx2-xx1
     // ebp = u
	 // edx = v
	 // esi = offset
     // sullo stack FPU = d
	 // sono liberi ecx, eax, edi

	 test ebx, ebx
	 jle fine

ciclo:

     fld st(0)
     fadd dword ptr [trick]

	 mov [l], ebx
	 mov edi, dword ptr [zbuf]

	 fstp dword ptr [fi]
     mov ebx, [vshift]

     mov eax, [fi]
	 mov ecx, [edi + esi]

	 cmp eax, ecx
	 jle next_pixel

// write_pixel:

	 mov [edi + esi], eax
	 mov eax, edx  // eax = v

     sar eax, 16
	 mov ecx, ebx

	 and eax, [vmask]
     mov edi, ebp  // salvo ebp (che e' u)

	 shl eax, cl     // eax = ((v>>16) & vmask)<<vshift
	 mov ecx, [texmap32]

	 sar edi, 16
	 and edi, [_umask_]  // edi = (u >> 16) & _umask_

	 add eax, edi  // eax = tex_offset
     mov eax, [ecx + eax*4]      // lettura pixel texture

	 mov ecx, [uvideo32]
	 and eax, 0x00fefefe

	 shr eax, 1
	 mov edi, [ecx + esi]        // lettura pixel di sfondo

	 and edi, 0x00fefefe
	 shr edi, 1

	 add eax, edi
     mov [ecx + esi], eax   // scrittura pixel

next_pixel:

	 add ebp, [du]           //  u+=du;
	 add edx, [dv]           //  v+=dv;

	 add esi, 4            // offset++
	 mov ebx, [l]
	 
	 fadd dword ptr [dz]
	 dec ebx

	 jnz ciclo

fine:
	 pop ebp
	 fstp dword ptr [fi]
	 ret
  }
}


void scanline_texture_tBLENDalpha (void)
{
  unsigned char a1, a2, a3, b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  u=left_u;
  v=left_v;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
	  tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
      a1=texmap8[tex_offset*4];
	  a2=texmap8[tex_offset*4+1];
	  a3=texmap8[tex_offset*4+2];
      b1=uvideo8[offset*4]; b2=uvideo8[offset*4+1]; b3=uvideo8[offset*4+2];

	  b1=b1+ttable[a1]-ttable[b1];
      b2=b2+ttable[a2]-ttable[b2];
	  b3=b3+ttable[a3]-ttable[b3];
      uvideo32[offset]=b1 | (b2<<8) | (b3<<16);     
	}
    offset++;   intzb+=1;
    d+=dz;
    u+=du;
    v+=dv;
  }
}


// ***********************************************************
/*
void scanline_texture_lADD(void)
{
  unsigned char a1,a2,a3;
  unsigned int b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  u=left_u;
  v=left_v;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
     RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  else
  {
	unsigned int subtexel=(xx1<<16)-left_x;
	u=u+((du*subtexel) >> 16);
	v=v+((dv*subtexel) >> 16);
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
      *intzb=*ddd;;
      a1=texmap8[tex_offset*4];
	  a2=texmap8[tex_offset*4+1];
	  a3=texmap8[tex_offset*4+2];
	  b1=BB1>>16;   // blu
      b2=GG1>>16;   // verde
	  b3=RR1>>16;   // rosso

      b1=add_table[a1+b1];
	  b2=add_table[a2+b2];
      b3=add_table[a3+b3];
      uvideo32[offset]=b1 | (b2<<8) | (b3<<16);
    }
    offset+=1; intzb+=1;
    d+=dz;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
    u+=du;
    v+=dv;
  }
}
*/

void __declspec(naked) scanline_texture_lADD(void)
{
  _asm
  {
     push ebp

	 mov ebp, [left_u]
	 mov edx, [left_v]

	 mov esi, [left_R]
	 mov edi, [left_G]
	 mov ebx, [left_B]

	 fld [left_z]
	 mov eax, [left_x]

	 add eax, 0xFFFF
	 sar eax, 16

	 test eax, eax
	 jge subtexel

// clip2Dleft:

	 mov   ecx, eax
	 fstp  dword ptr [fi2]   // tolgo il lef_z che ho messo poco fa
	 
	 neg  ecx
	 mov  [fi], ecx

     mov   ecx, eax         // ecx=xx1
	 imul  ecx, [du]     // ecx=xx1*du
     fild  dword ptr [fi]
	 fmul  dword ptr [dz]

	 sub  ebp, ecx   // ecx = envu+=(-xx1)*envdu

     mov   ecx, eax
	 imul  ecx, [dv]
	 fadd  dword ptr [left_z]    // sullo stack c'e' d

	 sub  edx, ecx   // edi = envv+=(-xx1)*envdv

	 mov ecx, eax
	 imul ecx, [dR]
     sub esi, ecx

	 mov ecx, eax
	 imul ecx, [dG]
     sub edi, ecx

	 mov ecx, eax
	 imul ecx, [dB]
     sub ebx, ecx
     
     xor eax, eax    // xx1 = 0
//	 jmp near noclip2Dleft

subtexel:
/*
     mov ecx, eax
	 shl ecx, 16
	 sub ecx, [left_x]
	 push ecx
     imul ecx, [du]
	 shr  ecx, 16
	 add  ebp, ecx
     pop ecx
     imul ecx, [dv]
	 shr  ecx, 16
	 add  edx, ecx
*/
noclip2Dleft:

	 mov [BB1], ebx      // RR1, GG1, BB1 sistemati in memoria
     mov [RR1], esi
     mov [GG1], edi

	 mov ebx, [right_x]
	 add ebx, 0xFFFF
	 sar ebx, 16

	 mov esi, [scrX]
	 cmp ebx, esi
	 jl noclip2Dright
     mov ebx, esi

noclip2Dright:

     sub ebx, eax        // ebx = xx2-xx1
	 mov esi, [offset0]
	 add esi, eax       // esi = offset = offset0+xx1
	 shl esi, 2

	 // a questo punto i registri sono cosi' messi
	 //
	 // ebx = xx2-xx1 (ma tanto poi metto in [l])
     // ebp = u
	 // edx = v
	 // esi = offset
     // sullo stack FPU = d
	 // sono liberi  eax, ecx, edi, ebx

	 test ebx, ebx
	 jle fine
	 mov [l], ebx

ciclo:

	 fld st(0)
	 fadd dword ptr [trick]

	 mov edi, dword ptr [zbuf]
     fstp dword ptr [fi]

     mov eax, [fi]
	 mov ecx, [edi + esi]

	 cmp eax, ecx
	 jle next_pixel

// write_pixel:

	 mov [edi + esi], eax
	 mov eax, edx  // eax = envv

     mov edi, ebp  // salvo ebp (che e' envu)
     sar eax, 16

	 mov ecx, [vshift]
	 and eax, [vmask]

	 shl eax, cl         // eax = (envv>>16)<<envvshift
     mov ecx, [BB1]

	 sar edi, 16 
	 mov ebx, [_umask_]

	 and edi, ebx
     mov ebx, [texmap32]

	 add eax, edi        // eax = tex_offset
     mov eax, [ebx + eax*4]   // eax = texel
	                          // rimangono liberi ecx, edi, ebx

	 mov  edi, [GG1]
	 shr  ecx, 16      // estraggo BB1

	 add  al, cl
     sbb  ebx, ebx

     or   al, bl
     mov  ecx, edi     // ecx = GG1
	 
	 shr  ecx, 16
	 mov  edi, [RR1]

	 add  ah, cl
     sbb  ebx, ebx

     or   ah, bl
     mov  ecx, edi     // ecx = RR1

	 shr  ecx, 16
     mov edi, dword ptr [uvideo32]	 

	 ror  eax, 16
	 add  al, cl

     sbb  ebx, ebx
     or   al, bl

     ror  eax, 16
     mov [edi + esi], eax
	 

next_pixel:

	 mov ecx, [dR]
	 add ebp, [du]           //  u+=du;

	 add edx, [dv]           //  v+=dv;
	 mov edi, [dG]

     add [RR1], ecx
	 mov eax, [dB]

	 add [GG1], edi
	 add [BB1], eax

	 add esi,4               // offset++
	 mov ebx, [l]

	 fadd dword ptr [dz]

	 dec ebx
	 mov [l], ebx
	 jnz ciclo

fine:

	 fstp dword ptr [fi]
	 pop ebp
	 ret
  }
}


void scanline_texture_lSUB(void)
{
  unsigned char a1,a2,a3;
  unsigned int b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  u=left_u;
  v=left_v;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
     RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
	if (*ddd > *intzb)
    {
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
      *intzb=*ddd;;
      a1=texmap8[tex_offset*4];
	  a2=texmap8[tex_offset*4+1];
	  a3=texmap8[tex_offset*4+2];
	  b1=BB1>>16;   // blu
      b2=GG1>>16;   // verde
	  b3=RR1>>16;   // rosso

      b1=sub_table[a1-b1+255];
	  b2=sub_table[a2-b2+255];
      b3=sub_table[a3-b3+255];
      uvideo32[offset]=b1 | (b2<<8) | (b3<<16);
    }
    offset+=1; intzb+=1;
    d+=dz;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
    u+=du;
    v+=dv;
  }
}

/*
void scanline_texture_lBLEND50(void)
{
  unsigned int a, b;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  u=left_u;
  v=left_v;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
     RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
	  tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
      *intzb=*ddd;;
	  a=(texmap32[tex_offset] & 0x00fefefe) >> 1;
	  b=((RR1 & 0x00FF0000) | ((GG1 & 0x00FF0000)>>8) | ((BB1>>16)));
      b=(b & 0x00fefefe) >> 1;
	  uvideo32[offset]=(a+b);
    }
	offset++;
	intzb+=1;
    d+=dz;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
    u+=du;
    v+=dv;
  }
}
*/

//void __cdecl scanline_texture_lBLEND50(void)
void __declspec(naked) scanline_texture_lBLEND50(void)
{
  _asm
  {
     push ebp

	 mov ebp, [left_u]
	 mov edx, [left_v]

	 mov esi, [left_R]
	 mov edi, [left_G]
	 mov ebx, [left_B]

	 fld [left_z]
	 mov eax, [left_x]

	 add eax, 0xFFFF
	 sar eax, 16

	 test eax, eax
	 jge noclip2Dleft

// clip2Dleft:

	 mov   ecx, eax
	 fstp  dword ptr [fi2]   // tolgo il lef_z che ho messo poco fa
	 
	 neg  ecx
	 mov  [fi], ecx

     mov   ecx, eax         // ecx=xx1
	 imul  ecx, [du]     // ecx=xx1*du
     fild  dword ptr [fi]
	 fmul  dword ptr [dz]

	 sub  ebp, ecx   // ecx = envu+=(-xx1)*envdu

     mov   ecx, eax
	 imul  ecx, [dv]
	 fadd  dword ptr [left_z]    // sullo stack c'e' d

	 sub  edx, ecx   // edi = envv+=(-xx1)*envdv

	 mov ecx, eax
	 imul ecx, [dR]
     sub esi, ecx

	 mov ecx, eax
	 imul ecx, [dG]
     sub edi, ecx

	 mov ecx, eax
	 imul ecx, [dB]
     sub ebx, ecx
     
     xor eax, eax    // xx1 = 0

noclip2Dleft:

	 mov [BB1], ebx      // RR1, GG1, BB1 sistemati in memoria
     mov [RR1], esi
     mov [GG1], edi

	 mov ebx, [right_x]
	 add ebx, 0xFFFF
	 sar ebx, 16

	 mov esi, [scrX]
	 cmp ebx, esi
	 jl noclip2Dright
     mov ebx, esi

noclip2Dright:

     sub ebx, eax        // ebx = xx2-xx1
	 mov esi, [offset0]
	 add esi, eax       // esi = offset = offset0+xx1
	 shl esi, 2

	 // a questo punto i registri sono cosi' messi
	 //
	 // ebx = xx2-xx1 (ma tanto poi metto in [l])
     // ebp = u
	 // edx = v
	 // esi = offset
     // sullo stack FPU = d
	 // sono liberi  eax, ecx, edi, ebx

	 test ebx, ebx
	 jle fine
	 mov [l], ebx

ciclo:

	 fld st(0)
	 fadd dword ptr [trick]

	 mov edi, dword ptr [zbuf]
	 mov [l], ebx

     fstp dword ptr [fi]
     mov eax, [fi]

	 mov ecx, [edi + esi]
	 cmp eax, ecx

	 jle next_pixel

// write_pixel:

	 mov [edi + esi], eax
	 mov eax, edx  // eax = v

     sar eax, 16   // v >> 16
     mov edi, ebp  // salvo ebp (che e' u)

	 mov ecx, [vshift]
	 and eax, [vmask]  // (v >> 16) & vmask

	 shl eax, cl         // eax = (v >> 16) << vshift
     mov ebx, [texmap32]

	 sar edi, 16        // u >> 16
     mov ecx, [BB1]

	 and edi, [_umask_]
	 add eax, edi        // eax = tex_offset

     mov eax, [ebx + eax*4]   // eax = texel
	 mov ebx, [GG1]

	 shr ecx, 16      //  BB1 >> 16
     and eax, 0x00fefefe

	 shr eax, 1         // texel pronto per essere sommato
     mov edi, dword ptr [uvideo32]

	 mov [color_flat], eax
     mov eax, [RR1]

	 shr ebx, 16       // GG1 >> 16
	 lea edi, [edi+esi]

	 shr eax, 16       // RR1 >> 16
     mov ch, bl        // ecx = | ? | ? | G | B |

	 ror ecx, 16
	 mov ebx, 0xfefe00fe

	 mov cl, al            // ecx = | G | B | ? | R |
	 mov eax, [color_flat]

     and ecx, ebx
	 ror ecx, 17           // ecx = | ? | R | G | B |

     add eax, ecx
     mov [edi], eax
	 

next_pixel:

	 mov ecx, [RR1]
	 add ebp, [du]           //  u+=du;

	 add edx, [dv]           //  v+=dv;
	 mov edi, [GG1]

     add ecx, [dR]
	 add edi, [dG]

	 mov eax, [BB1]
	 mov [RR1], ecx

     add eax, [dB]
     mov [GG1], edi

     mov [BB1], eax
	 add esi, 4               // offset++

	 mov ebx, [l]
	 fadd dword ptr [dz]

	 dec ebx
	 jnz ciclo

fine:

	 fstp dword ptr [fi]
	 pop ebp
	 ret
  }
}


void scanline_texture_lBLEND3DS (void)
{
  unsigned char a1,a2,a3;
  unsigned int b1, b2, b3;
  unsigned int bb;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  u=left_u;
  v=left_v;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
     RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
      *intzb=*ddd;;
      a1=texmap8[tex_offset*4] >> 1;
	  a2=texmap8[tex_offset*4+1] >> 1;
	  a3=texmap8[tex_offset*4+2] >> 1;
	  b1=BB1 >> (16+1);   // blu
      b2=GG1 >> (16+1);   // verde
	  b3=RR1 >> (16+1);   // rosso
	  bb=(a1*b1)>>6;  a1=bb;
      bb=(a2*b2)>>6;  a2=bb;
      bb=(a3*b3)>>6;  a3=bb;
	  uvideo32[offset]=a1 | (a2<<8) | (a3<<16);
    }
    offset+=1;
	intzb+=1;
    d+=dz;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
    u+=du;
    v+=dv;
  }
}

// ***********************************************************

void scanline_texture_tBLEND50_lADD(void)
{
  unsigned char a1, a2, a3, b1, b2, b3;
  unsigned int a, b;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  u=left_u;
  v=left_v;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
     RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
	  a1=texmap8[tex_offset*4];
	  a2=texmap8[tex_offset*4+1];
	  a3=texmap8[tex_offset*4+2];
	  b1=BB1>>16; b2=GG1>>16; b3=RR1>>16;

      b1=add_table[a1+b1];
	  b2=add_table[a2+b2];
	  b3=add_table[a3+b3];
	  
	  a=(uvideo32[offset] & 0xfffefefe) >> 1;
	  b=((b1 | (b2<<8) | (b3<<16)) & 0xfffefefe) >> 1;
	  uvideo32[offset]=a+b;
	}
    offset++;   intzb+=1;
    d+=dz;
    u+=du;
    v+=dv;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
  }
}


void scanline_texture_tBLEND50_lSUB(void)
{
  unsigned char a1, a2, a3, b1, b2, b3;
  unsigned int a, b;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  u=left_u;
  v=left_v;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
     RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
	  a1=texmap8[tex_offset*4];
	  a2=texmap8[tex_offset*4+1];
	  a3=texmap8[tex_offset*4+2];
	  b1=BB1>>16; b2=GG1>>16; b3=RR1>>16;

      b1=sub_table[a1-b1+255];
	  b2=sub_table[a2-b2+255];
	  b3=sub_table[a3-b3+255];
	  
	  a=(uvideo32[offset] & 0x00fefefe) >> 1;
	  b=((b1 | (b2<<8) | (b3<<16)) & 0x00fefefe) >> 1;
	  uvideo32[offset]=a+b;
	}
    offset++;   intzb+=1;
    d+=dz;
    u+=du;
    v+=dv;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
  }
}


void scanline_texture_tBLEND50_lBLEND50(void)
{
  unsigned int a, b, c;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  u=left_u;
  v=left_v;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
     RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
	  a=(texmap32[tex_offset] & 0x00fefefe) >> 1;
	  b=((RR1 & 0x00FF0000) | ((GG1 & 0x00FF0000) >> 8) | ((BB1>>16)));
      b=(b & 0x00fefefe) >> 1;
	  b=((a+b) & 0x00fefefe) >> 1;
      c=(uvideo32[offset] & 0x00fefefe) >> 1;
	  uvideo32[offset]=b+c;
    }
    offset+=1; intzb+=1;
    d+=dz;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
    u+=du;
    v+=dv;
  }
}


void scanline_texture_tBLEND50_lBLEND3DS(void)
{
  unsigned char a1,a2,a3;
  unsigned int b1, b2, b3;
  unsigned int bb, cc;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  u=left_u;
  v=left_v;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
     RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
      a1=texmap8[tex_offset*4] >> 1;
	  a2=texmap8[tex_offset*4+1] >> 1;
	  a3=texmap8[tex_offset*4+2] >> 1;
	  b1=BB1 >> (16+1);   // blu
      b2=GG1 >> (16+1);   // verde
	  b3=RR1 >> (16+1);   // rosso
	  bb=(a1*b1)>>6; a1=bb;
      bb=(a2*b2)>>6; a2=bb;
      bb=(a3*b3)>>6; a3=bb;
	  cc=(uvideo32[offset] & 0x00fefefe) >> 1;
      bb=((a1 | (a2<<8) | (a3<<16)) & 0x00fefefe) >> 1;
	  uvideo32[offset]=cc+bb;
    }
    offset+=1; intzb+=1;
    d+=dz;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
    u+=du;
    v+=dv;
  }
}


// ***********************************************************

void scanline_texture_tBLENDalpha_lADD(void)
{
  unsigned char a1, a2, a3, b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  u=left_u;
  v=left_v;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
     RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
	  a1=texmap8[tex_offset*4];
	  a2=texmap8[tex_offset*4+1];
	  a3=texmap8[tex_offset*4+2];
	  b1=BB1>>16; b2=GG1>>16; b3=RR1>>16;

      b1=add_table[a1+b1];
	  b2=add_table[a2+b2];
	  b3=add_table[a3+b3];
	  
	  b1=b1+ttable[uvideo8[offset*4  ]]-ttable[b1];
	  b2=b2+ttable[uvideo8[offset*4+1]]-ttable[b2];
	  b3=b3+ttable[uvideo8[offset*4+2]]-ttable[b3];
	  uvideo32[offset]=b1 | (b2<<8) | (b3<<16);
	}
    offset++;   intzb+=1;
    d+=dz;
    u+=du;
    v+=dv;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
  }
}


void scanline_texture_tBLENDalpha_lSUB(void)
{
  unsigned char a1, a2, a3, b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  u=left_u;
  v=left_v;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
     RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
	  a1=texmap8[tex_offset*4  ];
	  a2=texmap8[tex_offset*4+1];
	  a3=texmap8[tex_offset*4+2];
	  b1=BB1>>16; b2=GG1>>16; b3=RR1>>16;

      b1=sub_table[a1-b1+255];
	  b2=sub_table[a2-b2+255];
	  b3=sub_table[a3-b3+255];

	  b1=b1+ttable[uvideo8[offset*4  ]]-ttable[b1];
	  b2=b2+ttable[uvideo8[offset*4+1]]-ttable[b2];
	  b3=b3+ttable[uvideo8[offset*4+2]]-ttable[b3];
	  uvideo32[offset]=b1 | (b2<<8) | (b3<<16);
	}
    offset++;   intzb+=1;
    d+=dz;
    u+=du;
    v+=dv;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
  }
}


void scanline_texture_tBLENDalpha_lBLEND50(void)
{
  unsigned char a1, a2, a3, b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  u=left_u;
  v=left_v;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
     RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
	  a1=texmap8[tex_offset*4];
      a2=texmap8[tex_offset*4+1];
      a3=texmap8[tex_offset*4+2];
	  b1=BB1>>16; b2=GG1>>16; b3=RR1>>16;
      b1=(a1+b1)/2; b2=(a2+b2)/2; b3=(a3+b3)/2;

	  b1=b1+ttable[uvideo8[offset*4  ]]-ttable[b1];
	  b2=b2+ttable[uvideo8[offset*4+1]]-ttable[b2];
	  b3=b3+ttable[uvideo8[offset*4+2]]-ttable[b3];
	  uvideo32[offset]=b1 | (b2<<8) | (b3<<16);
    }
    offset+=1; intzb+=1;
    d+=dz;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
    u+=du;
    v+=dv;
  }
}


void scanline_texture_tBLENDalpha_lBLEND3DS(void)
{
  unsigned char a1,a2,a3;
  unsigned int b1, b2, b3;
  unsigned int bb;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  u=left_u;
  v=left_v;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
     RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
      a1=texmap8[tex_offset*4] >> 1;
	  a2=texmap8[tex_offset*4+1] >> 1;
	  a3=texmap8[tex_offset*4+2] >> 1;
	  b1=BB1 >> (16+1);   // blu
      b2=GG1 >> (16+1);   // verde
	  b3=RR1 >> (16+1);   // rosso
	  bb=(a1*b1)>>6; b1=bb;
      bb=(a2*b2)>>6; b2=bb;
      bb=(a3*b3)>>6; b3=bb;

	  b1=b1+ttable[uvideo8[offset*4  ]]-ttable[b1];
	  b2=b2+ttable[uvideo8[offset*4+1]]-ttable[b2];
	  b3=b3+ttable[uvideo8[offset*4+2]]-ttable[b3];
	  uvideo32[offset]=b1 | (b2<<8) | (b3<<16);
    }
    offset+=1; intzb+=1;
    d+=dz;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
    u+=du;
    v+=dv;
  }
}



// ###########################################################
// #####################  SEZIONE ENVMAP  ####################
// ###########################################################
/*
void scanline_envmap (void)
{
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  xx2=ceil16(right_x);
  if (xx2>=scrX) xx2=scrX;
  xx1=ceil16(left_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     xx1=0;
  }


  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv >>16)<<envvshift) + (envu>>16));
      *intzb=*ddd;;
      uvideo32[offset]=envmap32[env_offset];
    }
    offset++;
	intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
  }
}
*/


void __declspec(naked) scanline_envmap (void)
{
  _asm
  {
	 push ebp
	 mov ebp, [left_envu]

	 mov edx, [left_envv]
	 mov eax, [left_x]

	 fld [left_z]
	 add eax, 0xFFFF
	 
	 mov ebx, [right_x]
     sar eax, 16

	 add ebx, 0xFFFF
	 sar ebx, 16

	 test eax, eax
	 jge noclip2Dleft

// clip2Dleft:

	 mov  esi, eax
	 fstp  dword ptr [fi2]   // tolgo il lef_z che ho messo poco fa
	 
	 neg  esi
	 mov  [fi], esi

     mov  edi, eax         // edi=xx1
	 imul edi, [envdu]     // edi=xx1*du
     fild  dword ptr [fi]
	 fmul  dword ptr [dz]

	 sub  ebp, edi               // ebp = u+=(-xx1)*du

     mov  edi, eax
	 imul edi, [envdv]
	 fadd  dword ptr [left_z]    // sullo stack c'e' d
	 sub  edx, edi   // edi = v+=(-xx1)*dv
     xor eax, eax    // xx1 = 0


noclip2Dleft:

	 mov esi, [scrX]
	 cmp ebx, esi
	 jl noclip2Dright
     mov ebx, esi

noclip2Dright:

     sub ebx, eax        // ebx = xx2-xx1
	 mov esi, [offset0]
	 add esi, eax       // esi = offset = offset0+xx1
	 shl esi, 2

	 // a questo punto i registri sono cosi' messi
	 //
	 // ebx = xx2-xx1
     // ebp = envu
	 // edx = envv
	 // esi = offset
     // sullo stack FPU = d
	 // sono liberi edi, eax, ecx

	 test ebx, ebx
	 jle fine

ciclo:

	 fld st(0)
	 fadd dword ptr [trick]

	 mov edi, dword ptr [zbuf]
	 mov [l], ebx

     fstp dword ptr [fi]
	 mov ecx, [envvshift]

     mov ebx, [fi]
	 mov eax, [edi + esi]

	 cmp ebx, eax
	 jle next_pixel

// write_pixel:

	 mov [edi + esi], ebx
	 mov eax, edx  // eax = envv

     sar eax, 16
     mov edi, ebp  // salvo ecx (che e' envu) perche' mi serve shiftare

	 shl eax, cl     // eax = (envv>>16)<<envvshift
	 mov ecx, [uvideo32]

	 sar edi, 16
	 lea ecx, [ecx+esi]

	 add eax, edi  // eax = tex_offset
	 mov ebx, [envmap32]

     mov eax, [ebx + eax*4]
     mov [ecx], eax

next_pixel:

	 add ebp, [envdu]           //  u+=du;
	 add edx, [envdv]           //  v+=dv;

	 add esi, 4            // offset++
	 mov ebx, [l]

	 fadd dword ptr [dz]
	 dec ebx

	 jnz ciclo

fine:

	 pop ebp
	 fstp dword ptr [fi]
	 ret
  }
}


void scanline_envmap_tADD(void)
{
  return;
}

/*
void scanline_envmap_tBLEND50(void)
{
  unsigned int a, b;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
	  env_offset=(((envv >>16)<<envvshift) + (envu>>16));
	  a=(uvideo32[offset] & 0x00fefefe) >> 1;
	  b=(envmap32[env_offset] & 0x00fefefe) >> 1;
	  uvideo32[offset]=a+b;
	}
    offset++;   intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
  }
}
*/

void __declspec(naked) scanline_envmap_tBLEND50(void)
{
  _asm
  {
	 push ebp
	 mov ebp, [left_envu]

	 mov edx, [left_envv]
	 mov eax, [left_x]
	 mov ebx, [right_x]

	 fld [left_z]
	 add eax, 0xFFFF

	 sar eax, 16
	 add ebx, 0xFFFF

	 sar ebx, 16
	 test eax, eax

	 jge noclip2Dleft

// clip2Dleft:

	 mov  esi, eax
	 fstp  dword ptr [fi2]   // tolgo il lef_z che ho messo poco fa
	 
	 neg  esi
	 mov  [fi], esi

     mov edi, eax         // edi=xx1
	 imul edi, [envdu]    // edi=xx1*envdu
     fild  dword ptr [fi]
	 fmul  dword ptr [dz]

	 sub  ebp, edi        // ebp = envu+=(-xx1)*envdu

     mov edi, eax
	 imul edi, [envdv]
	 fadd  dword ptr [left_z]    // sullo stack c'e' d
	 sub  edx, edi   // edi = envv+=(-xx1)*envdv
     xor eax, eax    // xx1 = 0


noclip2Dleft:

	 mov esi, [scrX]
	 cmp ebx, esi
	 jl noclip2Dright
     mov ebx, esi

noclip2Dright:

     sub ebx, eax        // ebx = xx2-xx1
	 mov esi, [offset0]
	 add esi, eax       // esi = offset = offset0+xx1
	 shl esi, 2

	 // a questo punto i registri sono cosi' messi
	 //
	 // ebx = xx2-xx1
     // ebp = envu
	 // edx = envv
	 // esi = offset
     // sullo stack FPU = d

	 test ebx, ebx
	 jle fine

ciclo:

	 fld st(0)
     fadd dword ptr [trick]

	 mov edi, dword ptr [zbuf]
	 mov [l], ebx

	 mov ecx, [vshift]
     fstp dword ptr [fi]

     mov eax, [fi]
	 mov ebx, [edi + esi]

	 cmp eax, ebx
	 jle next_pixel

// write_pixel:

	 mov [edi + esi], eax
	 mov eax, edx  // eax = envv

     sar eax, 16
     mov edi, ebp  // salvo ebp (che e' envu)

	 shl eax, cl     // eax = (envv>>16)<<vshift
	 mov ecx, edi

	 sar edi, 16    // edi = (envu >> 16)
	 mov ebx, [uvideo32]

	 add eax, edi  // eax = tex_offset
	 mov edi, dword ptr [envmap32]

     mov eax, [edi + eax*4]      // lettura pixel texture
	 add ebx, esi

	 and eax, 0x00fefefe
	 mov edi, [ebx + esi]        // lettura pixel di sfondo

	 shr eax, 1
	 and edi, 0x00fefefe

	 shr edi, 1
	 add eax, edi

     mov [ebx], eax   // scrittura pixel

next_pixel:

	 add ecx, [envdu]           //  envu+=envdu;
	 add edx, [envdv]           //  envv+=envdv;

	 add esi, 4            // offset++
	 mov ebx, [l]

	 fadd dword ptr [dz]
	 dec ebx

	 jnz ciclo

fine:
	 pop ebp
	 fstp dword ptr [fi]
	 ret
  }
}


void scanline_envmap_tBLENDalpha (void)
{
  unsigned char a1, a2, a3, b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
	  env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      a1=envmap8[env_offset*4];
	  a2=envmap8[env_offset*4+1];
	  a3=envmap8[env_offset*4+2];
      b1=uvideo8[offset*4]; b2=uvideo8[offset*4+1]; b3=uvideo8[offset*4+2];

	  b1=b1+ttable[a1]-ttable[b1];
      b2=b2+ttable[a2]-ttable[b2];
	  b3=b3+ttable[a3]-ttable[b3];
      uvideo32[offset]=b1 | (b2<<8) | (b3<<16);     
 	}
    offset++;   intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
  }
}


// ***********************************************************
/*
void scanline_envmap_lADD(void)
{
  unsigned int b1, b2, b3, b;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  envu=left_envu;
  envv=left_envv;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      *intzb=*ddd;;

      b=envmap32[env_offset];
      b1=add_table[(unsigned char)(b) + (BB1>>16)];
	  b2=add_table[(unsigned char)(b>>8) + (GG1>>16)];
      b3=add_table[(unsigned char)(b >> 16) + (RR1>>16)];
      uvideo32[offset]=b1 | (b2<<8) | (b3<<16);
    }
    offset+=1;
	intzb+=1;
    d+=dz;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
    envu+=envdu;
    envv+=envdv;
  }
}
*/

void __declspec(naked) scanline_envmap_lADD(void)
{
  _asm
  {
     push ebp
	 mov ebp, [left_envu]

	 mov edx, [left_envv]
	 mov edi, [left_G]

	 fld [left_z]
	 mov eax, [left_x]

	 mov ebx, [left_B]
	 add eax, 0xFFFF

	 mov esi, [left_R]
	 sar eax, 16

	 test eax, eax
	 jge noclip2Dleft

// clip2Dleft:

	 mov   ecx, eax
	 fstp  dword ptr [fi2]   // tolgo il lef_z che ho messo poco fa
	 
	 neg  ecx
	 mov  [fi], ecx

     mov   ecx, eax         // ecx=xx1
	 imul  ecx, [envdu]     // ecx=xx1*du
     fild  dword ptr [fi]
	 fmul  dword ptr [dz]

	 sub  ebp, ecx   // ecx = envu+=(-xx1)*envdu

     mov   ecx, eax
	 imul  ecx, [envdv]
	 fadd  dword ptr [left_z]    // sullo stack c'e' d

	 sub  edx, ecx   // edi = envv+=(-xx1)*envdv

	 mov ecx, eax
	 imul ecx, [dR]
     sub esi, ecx

	 mov ecx, eax
	 imul ecx, [dG]
     sub edi, ecx

	 mov ecx, eax
	 imul ecx, [dB]
     sub ebx, ecx
     
     xor eax, eax    // xx1 = 0

noclip2Dleft:

	 mov [BB1], ebx      // RR1, GG1, BB1 sistemati in memoria
     mov [RR1], esi
     mov [GG1], edi

	 mov ebx, [right_x]
	 add ebx, 0xFFFF
	 sar ebx, 16

	 mov esi, [scrX]
	 cmp ebx, esi
	 jl noclip2Dright
     mov ebx, esi

noclip2Dright:

     sub ebx, eax        // ebx = xx2-xx1
	 mov esi, [offset0]
	 add esi, eax       // esi = offset = offset0+xx1
	 shl esi, 2

	 // a questo punto i registri sono cosi' messi
	 //
	 // ebx = xx2-xx1 (ma tanto poi metto in [l])
     // ebp = envu
	 // edx = envv
	 // esi = offset
     // sullo stack FPU = d
	 // sono liberi  eax, ecx, edi, ebx

	 test ebx, ebx
	 jle fine

ciclo:

	 fld st(0)
     fadd dword ptr [trick]

	 mov [l], ebx
	 mov edi, dword ptr [zbuf]

     mov ebx, [envvshift]
	 fstp dword ptr [fi]

     mov eax, [fi]
	 mov ecx, [edi + esi]

	 cmp eax, ecx
	 jle next_pixel

// write_pixel:

	 mov [edi + esi], eax
	 mov eax, edx  // eax = envv

     sar eax, 16
     mov edi, ebp  // salvo ebp (che e' envu)

	 sar edi, 16        
	 mov ecx, ebx

	 shl  eax, cl         // eax = (envv>>16)<<envvshift
     mov  ecx, [BB1]

	 add eax, edi        // eax = tex_offset
	 mov edi, dword ptr [envmap32]

	 shr  ecx, 16      // estraggo BB1
     mov  eax, [edi + eax*4]   // eax = envtexel

	 mov  edi, [GG1]          // rimangono liberi ecx, edi, ebx
	 add  al, cl

     sbb  ebx, ebx
     mov  [color_flat], edx

     mov  ecx, edi     // ecx = GG1
	 or   al, bl

	 shr  ecx, 16
	 mov  edx, [RR1]

	 add  ah, cl
     mov  edi, [uvideo32]

     sbb  ebx, ebx
	 lea  edi, [edi+esi]

	 shr  edx, 16      // RR1 >> 16
     or   ah, bl

	 ror  eax, 16
	 mov  ecx, edi

	 add  al, dl
	 mov  edx, [color_flat]

     sbb  ebx, ebx
	 mov  edx, [color_flat]

     or   al, bl
	 mov  edx, [color_flat]

     ror  eax, 16
	 mov  edx, [color_flat]

     mov [edi], eax
	 

next_pixel:


	 add ebp, [envdu]           //  envu+=envdu;
	 mov ecx, [RR1]

	 add edx, [envdv]           //  envv+=envdv;
	 mov edi, [GG1]

     add ecx, [dR]
	 mov eax, [BB1]

	 add edi, [dG]
	 mov [RR1], ecx

	 add eax, [dB]
	 mov [GG1], edi

     mov [BB1], eax
	 add esi,4               // offset++

	 mov ebx, [l]
	 fadd dword ptr [dz]

	 dec ebx
	 jnz ciclo

fine:

	 fstp dword ptr [fi]
	 pop ebp
	 ret
  }
}



void scanline_envmap_lSUB(void)
{
  unsigned char a1,a2,a3;
  unsigned int b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  envu=left_envu;
  envv=left_envv;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      *intzb=*ddd;;
      a1=envmap8[env_offset*4];
	  a2=envmap8[env_offset*4+1];
	  a3=envmap8[env_offset*4+2];
	  b1=BB1>>16;   // blu
      b2=GG1>>16;   // verde
	  b3=RR1>>16;   // rosso

      b1=sub_table[a1-b1+255];
	  b2=sub_table[a2-b2+255];
      b3=sub_table[a3-b3+255];
      uvideo32[offset]=b1 | (b2<<8) | (b3<<16);
    }
    offset+=1; intzb+=1;
    d+=dz;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
    envu+=envdu;
    envv+=envdv;
  }
}

/*
void scanline_envmap_lBLEND50(void)
{
  unsigned int a, b;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  envu=left_envu;
  envv=left_envv;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;

  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv >>16)<<envvshift) + (envu>>16));
      *intzb=*ddd;;
	  a=(envmap32[env_offset] & 0x00fefefe) >> 1;
	  b=((RR1 & 0x00FF0000) | ((GG1 & 0x00FF0000) >> 8) | ((BB1>>16)));
      b=(b & 0x00fefefe) >> 1;
	  uvideo32[offset]=a+b;
    }
    offset++;   intzb+=1;
    d+=dz;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
    envu+=envdu;
    envv+=envdv;
  }
}
*/
void __declspec(naked) scanline_envmap_lBLEND50(void)
{
  _asm
  {
     push ebp

	 mov ebp, [left_envu]
	 mov edx, [left_envv]

	 mov esi, [left_R]
	 mov edi, [left_G]
	 mov ebx, [left_B]

	 fld [left_z]
	 mov eax, [left_x]

	 add eax, 0xFFFF
	 sar eax, 16

	 test eax, eax
	 jge noclip2Dleft

// clip2Dleft:

	 mov   ecx, eax
	 fstp  dword ptr [fi2]   // tolgo il lef_z che ho messo poco fa
	 
	 neg  ecx
	 mov  [fi], ecx

     mov   ecx, eax         // ecx=xx1
	 imul  ecx, [envdu]     // ecx=xx1*envdu
     fild  dword ptr [fi]
	 fmul  dword ptr [dz]

	 sub  ebp, ecx   // ecx = envu+=(-xx1)*envdu

     mov   ecx, eax
	 imul  ecx, [envdv]
	 fadd  dword ptr [left_z]    // sullo stack c'e' d

	 sub  edx, ecx   // edi = envv+=(-xx1)*envdv

	 mov ecx, eax
	 imul ecx, [dR]
     sub esi, ecx

	 mov ecx, eax
	 imul ecx, [dG]
     sub edi, ecx

	 mov ecx, eax
	 imul ecx, [dB]
     sub ebx, ecx
     
     xor eax, eax    // xx1 = 0

noclip2Dleft:

	 mov [BB1], ebx      // RR1, GG1, BB1 sistemati in memoria
     mov [RR1], esi
     mov [GG1], edi

	 mov ebx, [right_x]
	 add ebx, 0xFFFF
	 sar ebx, 16

	 mov esi, [scrX]
	 cmp ebx, esi
	 jl noclip2Dright
     mov ebx, esi

noclip2Dright:

     sub ebx, eax        // ebx = xx2-xx1
	 mov esi, [offset0]
	 add esi, eax       // esi = offset = offset0+xx1
	 shl esi, 2

	 // a questo punto i registri sono cosi' messi
	 //
	 // ebx = xx2-xx1 (ma tanto poi metto in [l])
     // ebp = envu
	 // edx = envv
	 // esi = offset
     // sullo stack FPU = d
	 // sono liberi  eax, ecx, edi, ebx

	 test ebx, ebx
	 jle fine
	 mov [l], ebx

ciclo:

	 fld st(0)
	 fadd dword ptr [trick]

	 mov edi, dword ptr [zbuf]
	 mov [l], ebx

     fstp dword ptr [fi]
     mov eax, [fi]

	 mov ecx, [edi + esi]
	 cmp eax, ecx

	 jle next_pixel

// write_pixel:

	 mov [edi + esi], eax
	 mov eax, edx  // eax = envv

     sar eax, 16   // envv >> 16
     mov edi, ebp  // salvo ebp (che e' envu)

	 mov ecx, [envvshift]
     mov ebx, [envmap32]

	 shl eax, cl         // eax = (envv >> 16) << envvshift
     mov ecx, [BB1]

	 sar edi, 16        // envu >> 16
	 add eax, edi        // eax = tex_offset

     mov eax, [ebx + eax*4]   // eax = envtexel
	 mov ebx, [GG1]

	 shr ecx, 16      //  BB1 >> 16
     and eax, 0x00fefefe

	 shr eax, 1         // texel pronto per essere sommato
     mov edi, dword ptr [uvideo32]

	 mov [color_flat], eax
     mov eax, [RR1]

	 shr ebx, 16       // GG1 >> 16
	 lea edi, [edi+esi]

	 shr eax, 16       // RR1 >> 16
     mov ch, bl        // ecx = | ? | ? | G | B |

	 ror ecx, 16
	 mov ebx, 0xfefe00fe

	 mov cl, al            // ecx = | G | B | ? | R |
	 mov eax, [color_flat]

     and ecx, ebx
	 ror ecx, 17           // ecx = | ? | R | G | B |

     add eax, ecx
     mov [edi], eax
	 

next_pixel:

	 mov ecx, [RR1]
	 add ebp, [envdu]           //  envu+=envdu;

	 add edx, [envdv]           //  envv+=envdv;
	 mov edi, [GG1]

     add ecx, [dR]
	 add edi, [dG]

	 mov eax, [BB1]
	 mov [RR1], ecx

     add eax, [dB]
     mov [GG1], edi

     mov [BB1], eax
	 add esi, 4               // offset++

	 fadd dword ptr [dz]
	 mov ebx, [l]

	 dec ebx
	 jnz ciclo

fine:

	 fstp dword ptr [fi]
	 pop ebp
	 ret
  }
}


void scanline_envmap_lBLEND3DS (void)
{
  unsigned char a1, a2, a3;
  unsigned int b1, b2, b3;
  unsigned int aa, bb;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  envu=left_envu;
  envv=left_envv;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv >>16)<<envvshift) + (envu>>16));
      *intzb=*ddd;;

      a1=envmap8[env_offset*4] >> 1;
	  a2=envmap8[env_offset*4+1] >> 1;
	  a3=envmap8[env_offset*4+2] >> 1;

	  b1=BB1 >> (16+1);   // blu
      b2=GG1 >> (16+1);   // verde
	  b3=RR1 >> (16+1);   // rosso

	  bb=(a1*b1)>>6; a1=bb;
      bb=(a2*b2)>>6; a2=bb;
	  bb=(a3*b3)>>6; a3=bb;

	  aa=a1 | (a2<<8) | (a3<<16);
      uvideo32[offset]=aa;
    }
    offset+=1; intzb+=1;
    d+=dz;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
    envu+=envdu;
    envv+=envdv;
  }
}


// ***********************************************************

void scanline_envmap_tBLEND50_lADD(void)
{
  unsigned char a1, a2, a3, b1, b2, b3;
  unsigned int a, b;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
	  a1=envmap8[env_offset*4];
	  a2=envmap8[env_offset*4+1];
	  a3=envmap8[env_offset*4+2];
	  b1=BB1>>16; b2=GG1>>16; b3=RR1>>16;

      b1=add_table[a1+b1];
	  b2=add_table[a2+b2];
	  b3=add_table[a3+b3];
	  
	  a=(uvideo32[offset] & 0x00fefefe) >> 1;
	  b=((b1 | (b2<<8) | (b3<<16)) & 0x00fefefe) >> 1;
	  uvideo32[offset]=a+b;
	}
    offset++;   intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
  }
}


void scanline_envmap_tBLEND50_lSUB(void)
{
  unsigned char a1, a2, a3, b1, b2, b3;
  unsigned int a, b;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
	  a1=envmap8[env_offset*4];
	  a2=envmap8[env_offset*4+1];
	  a3=envmap8[env_offset*4+2];
	  b1=BB1>>16; b2=GG1>>16; b3=RR1>>16;

      b1=sub_table[a1-b1+255];
	  b2=sub_table[a2-b2+255];
	  b3=sub_table[a3-b3+255];
	  
	  a=(uvideo32[offset] & 0x00fefefe) >> 1;
	  b=((b1 | (b2<<8) | (b3<<16)) & 0x00fefefe) >> 1;
	  uvideo32[offset]=a+b;
	}
    offset++;   intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
  }
}


void scanline_envmap_tBLEND50_lBLEND50(void)
{
  unsigned int a, b, c;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  envu=left_envu;
  envv=left_envv;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
	  env_offset=(((envv >>16)<<envvshift) + (envu>>16));
	  a=(envmap32[env_offset] & 0x00fefefe) >> 1;
	  b=((RR1 & 0x00FF0000) | ((GG1 & 0x00FF0000) >> 8) | ((BB1>>16)));
      b=(b & 0x00fefefe) >> 1;
	  b=((a+b) & 0x00fefefe) >> 1;
      c=(uvideo32[offset] & 0x00fefefe) >> 1;
	  uvideo32[offset]=b+c;
    }
    offset+=1; intzb+=1;
    d+=dz;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
    envu+=envdu;
    envv+=envdv;
  }
}


void scanline_envmap_tBLEND50_lBLEND3DS(void)
{
  unsigned char a1,a2,a3;
  unsigned int b1, b2, b3;
  unsigned int bb, cc;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  envu=left_envu;
  envv=left_envv;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
	  env_offset=(((envv >>16)<<envvshift) + (envu>>16));
      a1=envmap8[env_offset*4] >> 1;
	  a2=envmap8[env_offset*4+1] >> 1;
	  a3=envmap8[env_offset*4+2] >> 1;
	  b1=BB1 >> (16+1);   // blu
      b2=GG1 >> (16+1);   // verde
	  b3=RR1 >> (16+1);   // rosso
	  bb=(a1*b1)>>6; a1=bb;
      bb=(a2*b2)>>6; a2=bb;
      bb=(a3*b3)>>6; a3=bb;
	  cc=(uvideo32[offset] & 0x00fefefe) >> 1;
      bb=((a1 | (a2<<8) | (a3<<16)) & 0x00fefefe) >> 1;
	  uvideo32[offset]=cc+bb;
    }
    offset+=1; intzb+=1;
    d+=dz;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
    envu+=envdu;
    envv+=envdv;
  }
}


//************************************************************

void scanline_envmap_tBLENDalpha_lADD(void)
{
  unsigned char a1, a2, a3, b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
	  a1=envmap8[env_offset*4];
	  a2=envmap8[env_offset*4+1];
	  a3=envmap8[env_offset*4+2];
	  b1=BB1>>16; b2=GG1>>16; b3=RR1>>16;

      b1=add_table[a1+b1];
	  b2=add_table[a2+b2];
	  b3=add_table[a3+b3];
	  
	  b1=b1+ttable[uvideo8[offset*4  ]]-ttable[b1];
	  b2=b2+ttable[uvideo8[offset*4+1]]-ttable[b2];
	  b3=b3+ttable[uvideo8[offset*4+2]]-ttable[b3];
	  uvideo32[offset]=b1 | (b2<<8) | (b3<<16);
	}
    offset++;   intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
  }
}


void scanline_envmap_tBLENDalpha_lSUB(void)
{
  unsigned char a1, a2, a3, b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
	  a1=envmap8[env_offset*4];
	  a2=envmap8[env_offset*4+1];
	  a3=envmap8[env_offset*4+2];
	  b1=BB1>>16; b2=GG1>>16; b3=RR1>>16;

      b1=sub_table[a1-b1+255];
	  b2=sub_table[a2-b2+255];
	  b3=sub_table[a3-b3+255];
	  
	  b1=b1+ttable[uvideo8[offset*4  ]]-ttable[b1];
	  b2=b2+ttable[uvideo8[offset*4+1]]-ttable[b2];
	  b3=b3+ttable[uvideo8[offset*4+2]]-ttable[b3];
	  uvideo32[offset]=b1 | (b2<<8) | (b3<<16);
	}
    offset++;   intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
  }
}


void scanline_envmap_tBLENDalpha_lBLEND50(void)
{
  unsigned char a1, a2, a3, b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  envu=left_envu;
  envv=left_envv;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<vshift) + (envu>>16));
	  a1=envmap8[env_offset];
      a2=envmap8[env_offset+1];
      a3=envmap8[env_offset+2];
	  b1=BB1>>16; b2=GG1>>16; b3=RR1>>16;
      b1=(a1+b1)/2; b2=(a2+b2)/2; b3=(a3+b3)/2;

	  b1=b1+ttable[uvideo8[offset*4  ]]-ttable[b1];
	  b2=b2+ttable[uvideo8[offset*4+1]]-ttable[b2];
	  b3=b3+ttable[uvideo8[offset*4+2]]-ttable[b3];
	  uvideo32[offset]=b1 | (b2<<8) | (b3<<16);
    }
    offset+=1; intzb+=1;
    d+=dz;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
    envu+=envdu;
    envv+=envdv;
  }
}


void scanline_envmap_tBLENDalpha_lBLEND3DS(void)
{
  unsigned char a1,a2,a3;
  unsigned int b1, b2, b3;
  unsigned int bb;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  envu=left_envu;
  envv=left_envv;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<vshift) + (envu>>16));
      a1=envmap8[env_offset*4] >> 1;
	  a2=envmap8[env_offset*4+1] >> 1;
	  a3=envmap8[env_offset*4+2] >> 1;
	  b1=BB1 >> (16+1);   // blu
      b2=GG1 >> (16+1);   // verde
	  b3=RR1 >> (16+1);   // rosso
	  bb=(a1*b1)>>6; b1=bb;
      bb=(a2*b2)>>6; b2=bb;
      bb=(a3*b3)>>6; b3=bb;

	  b1=b1+ttable[uvideo8[offset*4  ]]-ttable[b1];
	  b2=b2+ttable[uvideo8[offset*4+1]]-ttable[b2];
	  b3=b3+ttable[uvideo8[offset*4+2]]-ttable[b3];
	  uvideo32[offset]=b1 | (b2<<8) | (b3<<16);
    }
    offset+=1; intzb+=1;
    d+=dz;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
    envu+=envdu;
    envv+=envdv;
  }
}





// ###########################
// SEZIONE MISTA 
// ###########################

void scanline_texenv_mADD(void)
{
  unsigned char a1, a2, a3, b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  u=left_u;
  v=left_v;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
	  a1=envmap8[env_offset*4  ];  b1=texmap8[tex_offset*4  ];
      a2=envmap8[env_offset*4+1];  b2=texmap8[tex_offset*4+1];
      a3=envmap8[env_offset*4+2];  b3=texmap8[tex_offset*4+2];

	  b1=add_table[a1+b1];
	  b2=add_table[a2+b2];
	  b3=add_table[a3+b3];

      *intzb=*ddd;
      uvideo32[offset]=b1 | (b2<<8) | (b3<<16);
    }
    offset+=1; intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    u+=du;
    v+=dv;
  }
}


void scanline_texenv_mTEXSUBENV(void)
{
  unsigned char a1, a2, a3, b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  u=left_u;
  v=left_v;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
	  a1=envmap8[env_offset*4  ];  b1=texmap8[tex_offset*4  ];
      a2=envmap8[env_offset*4+1];  b2=texmap8[tex_offset*4+1];
      a3=envmap8[env_offset*4+2];  b3=texmap8[tex_offset*4+2];

	  b1=sub_table[b1-a1+255];
	  b2=sub_table[b2-a2+255];
	  b3=sub_table[b3-a3+255];

      *intzb=*ddd;
      uvideo32[offset]=b1 | (b2<<8) | (b3<<16);
    }
    offset+=1; intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    u+=du;
    v+=dv;
  }
}


void scanline_texenv_mENVSUBTEX(void)
{
  unsigned char a1, a2, a3, b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  u=left_u;
  v=left_v;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
	  a1=envmap8[env_offset*4  ];  b1=texmap8[tex_offset*4  ];
      a2=envmap8[env_offset*4+1];  b2=texmap8[tex_offset*4+1];
      a3=envmap8[env_offset*4+2];  b3=texmap8[tex_offset*4+2];

	  b1=sub_table[a1-b1+255];
	  b2=sub_table[a2-b2+255];
	  b3=sub_table[a3-b3+255];

      *intzb=*ddd;;
      uvideo32[offset]=b1 | (b2<<8) | (b3<<16);
    }
    offset+=1; intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    u+=du;
    v+=dv;
  }
}


void scanline_texenv_mBLENDalpha(void)
{
  unsigned char a1, a2, a3, b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  u=left_u;
  v=left_v;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
	  a1=envmap8[env_offset*4  ];  b1=texmap8[tex_offset*4  ];
      a2=envmap8[env_offset*4+1];  b2=texmap8[tex_offset*4+1];
      a3=envmap8[env_offset*4+2];  b3=texmap8[tex_offset*4+2];

	  b1=b1 + atable[a1] - atable[b1];
	  b2=b2 + atable[a2] - atable[b2];
	  b3=b3 + atable[a3] - atable[b3];

      *intzb=*ddd;;
      uvideo32[offset]=b1 | (b2<<8) | (b3<<16);
    }
    offset+=1; intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    u+=du;
    v+=dv;
  }
}

/*
void scanline_texenv_mBLEND50(void)
{
  unsigned int a, b;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  u=left_u;
  v=left_v;
  xx2=ceil16(right_x);
  if (xx2>=scrX) xx2=scrX;
  xx1=ceil16(left_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
     xx1=0;
  }

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
      a=(envmap32[env_offset] & 0x00fefefe) >>1;
	  b=(texmap32[tex_offset] & 0x00fefefe) >>1;
      *intzb=*ddd;;
      uvideo32[offset]=a+b;
    }
    offset+=1; intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    u+=du;
    v+=dv;
  }
}
*/
void __declspec(naked) scanline_texenv_mBLEND50(void)
{
  _asm
  {
	 push ebp

     mov edi, [left_envu]
	 mov esi, [left_envv]

	 mov ebp, [left_u]
	 mov edx, [left_v]

	 mov eax, [left_x]

	 fld [left_z]
	 add eax, 0xFFFF

	 sar eax, 16
	 test eax, eax

	 jge noclip2Dleft

// clip2Dleft:

	 mov   ebx, eax
	 fstp  dword ptr [fi2]   // tolgo il lef_z che ho messo poco fa
	 
	 neg  ebx
	 mov  [fi], ebx

     mov ebx, eax      // ebx=xx1
	 imul ebx, [du]    // ebx=xx1*du
     fild  dword ptr [fi]
	 fmul  dword ptr [dz]
     sub  ebp, ebx     // ebp = u+=(-xx1)*du
	 
     mov ebx, eax
	 imul ebx, [dv]
	 fadd  dword ptr [left_z]    // sullo stack c'e' d
	 sub  edx, ebx   // edx = v+=(-xx1)*dv

     mov ebx, eax
	 imul ebx, [envdu]
	 sub  edi, ebx     // edi = envu+=(-xx1)*envdu

	 mov ebx, eax
	 imul ebx, [envdv]
	 sub  esi, ebx     // esi = envv+=(-xx1)*envdv

     xor eax, eax    // xx1 = 0

noclip2Dleft:

     mov [envv], esi
	 mov ebx, [right_x]

	 mov [envu], edi
	 add ebx, 0xFFFF

	 mov esi, [scrX]
	 sar ebx, 16

	 cmp ebx, esi
	 jl noclip2Dright
     mov ebx, esi

noclip2Dright:

     sub ebx, eax        // ebx = xx2-xx1
	 mov esi, [offset0]

	 add esi, eax       // esi = offset = offset0+xx1
	 mov [l], ebx

	 shl esi, 2

	 // a questo punto i registri sono cosi' messi
	 //
     // ebp = u
	 // edx = v
	 // envu in memoria
	 // envv in memoria
	 // esi = offset
     // sullo stack FPU = d
	 // sono liberi durante il ciclo eax, edi, ecx, ebx

	 test ebx, ebx
	 jle fine

ciclo:

	 fld st(0)
     fadd dword ptr [trick]

	 mov edi, dword ptr [zbuf]
	 mov [l], ebx

     fstp dword ptr [fi]
     mov eax, [fi]

	 mov ecx, [edi+esi]
	 cmp eax, ecx

	 jle next_pixel

// write_pixel:

	 mov [edi+esi], eax    // scrivo lo zbuffer
	 mov eax, edx     // eax = v

     sar eax, 16
     mov edi, ebp  // salvo edi=ebp (che e' u)

	 mov ecx, [vshift]
	 and eax, [vmask]

	 sar edi, 16     // edi = u>>16
	 mov ebx, [texmap32]

	 shl eax, cl     // eax = ((v>>16) & vmask)<<vshift
	 and edi, [_umask_]  // edi = (u >> 16) & _umask_

	 add eax, edi        // eax = tex_offset
	 lea ebx, [ebx+eax*4]

     mov eax, [ebx]   // eax = texel; rimangono liberi edi, ecx, ebx
     mov edi, [envv]

	 sar edi, 16
	 mov ecx, [envvshift]

	 shl edi, cl           // edi = (envv>>16) << envvshift
	 and eax, 0x00fefefe   // intanto lavoro il texel

	 mov ecx, [envu]
	 mov ebx, [envmap32]

	 sar ecx, 16
	 add edi, ecx         // edi = env_offset
	                      // rimane libero solo ecx

     shr eax, 1           // texel pronto per essere sommato
	 mov ecx, [ebx+edi*4]  // ecx = pixel envmap

	 and ecx, 0x00fefefe
	 mov ebx, [uvideo32]

     shr ecx, 1            // texel env pronto per essere sommato
	 add eax, ecx

	 mov [ebx+esi], eax

next_pixel:

	 mov ecx, [envu]
	 add ebp, [du]           //  u+=du;

	 add edx, [dv]           //  v+=dv;
     mov edi, [envv]

	 add ecx, [envdu]
	 add edi, [envdv]

	 mov [envu], ecx
	 mov [envv], edi

	 add esi, 4             // offset++
	 mov ebx, [l]

	 fadd dword ptr [dz]
	 dec ebx

	 jnz ciclo

fine:

	 pop ebp
	 fstp dword ptr [fi]
	 ret
  }
}


//************************************************************

void scanline_texenv_mADD_tBLEND50(void)
{
  unsigned int a, b;
  unsigned char a1, a2, a3, b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  u=left_u;
  v=left_v;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
	  a1=envmap8[env_offset*4  ];  b1=texmap8[tex_offset*4  ];
      a2=envmap8[env_offset*4+1];  b2=texmap8[tex_offset*4+1];
      a3=envmap8[env_offset*4+2];  b3=texmap8[tex_offset*4+2];
	  b1=add_table[a1+b1];
	  b2=add_table[a2+b2];
	  b3=add_table[a3+b3];
      
	  a=b1 | (b2<<8) | (b3<<16);
	  a=(a & 0x00fefefe) >> 1;
	  b=(uvideo32[offset] & 0x00fefefe) >> 1;
      uvideo32[offset]=a+b;
    }
    offset+=1; intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    u+=du;
    v+=dv;
  }
}


void scanline_texenv_mTEXSUBENV_tBLEND50(void)
{
  unsigned int a, b;
  unsigned char a1, a2, a3, b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  u=left_u;
  v=left_v;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
	  a1=envmap8[env_offset*4  ];  b1=texmap8[tex_offset*4  ];
      a2=envmap8[env_offset*4+1];  b2=texmap8[tex_offset*4+1];
      a3=envmap8[env_offset*4+2];  b3=texmap8[tex_offset*4+2];
	  b1=sub_table[b1-a1+255];
	  b2=sub_table[b2-a2+255];
	  b3=sub_table[b3-a3+255];
      
	  a=b1 | (b2<<8) | (b3<<16);
	  a=(a & 0x00fefefe) >> 1;
	  b=(uvideo32[offset] & 0x00fefefe) >> 1;
      uvideo32[offset]=a+b;
    }
    offset+=1; intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    u+=du;
    v+=dv;
  }
}


void scanline_texenv_mENVSUBTEX_tBLEND50(void)
{
  unsigned int a, b;
  unsigned char a1, a2, a3, b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  u=left_u;
  v=left_v;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
	  a1=envmap8[env_offset*4  ];  b1=texmap8[tex_offset*4  ];
      a2=envmap8[env_offset*4+1];  b2=texmap8[tex_offset*4+1];
      a3=envmap8[env_offset*4+2];  b3=texmap8[tex_offset*4+2];
	  b1=sub_table[a1-b1+255];
	  b2=sub_table[a2-b2+255];
	  b3=sub_table[a3-b3+255];
      
	  a=b1 | (b2<<8) | (b3<<16);
	  a=(a & 0x00fefefe) >> 1;
	  b=(uvideo32[offset] & 0x00fefefe) >> 1;
      uvideo32[offset]=a+b;
    }
    offset+=1; intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    u+=du;
    v+=dv;
  }
}


void scanline_texenv_mBLEND50_tBLEND50(void)
{
  unsigned int a, b, c;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  u=left_u;
  v=left_v;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
	  a=(envmap32[env_offset] & 0x00fefefe) >> 1;
	  b=(texmap32[tex_offset] & 0x00fefefe) >> 1;
	  b=((b+a) & 0x00fefefe) >> 1;
	  c=(uvideo32[offset] & 0x00fefefe) >> 1;
      uvideo32[offset]=b+c;
    }
    offset+=1; intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    u+=du;
    v+=dv;
  }
}


void scanline_texenv_mBLENDalpha_tBLEND50(void)
{
  unsigned int a, b;
  unsigned char a1, a2, a3, b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  u=left_u;
  v=left_v;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
	  a1=envmap8[env_offset*4  ];  b1=texmap8[tex_offset*4  ];
      a2=envmap8[env_offset*4+1];  b2=texmap8[tex_offset*4+1];
      a3=envmap8[env_offset*4+2];  b3=texmap8[tex_offset*4+2];
	  b1=b1 + atable[a1] - atable[b1];
	  b2=b2 + atable[a2] - atable[b2];
	  b3=b3 + atable[a2] - atable[b3];
      
	  a=b1 | (b2<<8) | (b3<<16);
	  a=(a & 0x00fefefe) >> 1;
	  b=(uvideo32[offset] & 0x00fefefe) >> 1;
      uvideo32[offset]=a+b;
    }
    offset+=1; intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    u+=du;
    v+=dv;
  }
}

//************************************************************

void scanline_texenv_mADD_tBLENDalpha(void)
{
  unsigned int a;
  unsigned char a1, a2, a3, b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  u=left_u;
  v=left_v;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
	  a1=envmap8[env_offset*4  ];  b1=texmap8[tex_offset*4  ];
      a2=envmap8[env_offset*4+1];  b2=texmap8[tex_offset*4+1];
      a3=envmap8[env_offset*4+2];  b3=texmap8[tex_offset*4+2];
	  b1=add_table[a1+b1];
	  b2=add_table[a2+b2];
	  b3=add_table[a3+b3];
      
      b1=b1 + ttable[uvideo8[offset*4  ]] - ttable[b1];
	  b2=b2 + ttable[uvideo8[offset*4+1]] - ttable[b2];
	  b3=b3 + ttable[uvideo8[offset*4+2]] - ttable[b3];

	  a=b1 | (b2<<8) | (b3<<16);
      uvideo32[offset]=a;
    }
    offset+=1; intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    u+=du;
    v+=dv;
  }
}


void scanline_texenv_mTEXSUBENV_tBLENDalpha(void)
{
  unsigned int a;
  unsigned char a1, a2, a3, b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  u=left_u;
  v=left_v;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
	  a1=envmap8[env_offset*4  ];  b1=texmap8[tex_offset*4  ];
      a2=envmap8[env_offset*4+1];  b2=texmap8[tex_offset*4+1];
      a3=envmap8[env_offset*4+2];  b3=texmap8[tex_offset*4+2];
	  b1=sub_table[b1-a1+255];
	  b2=sub_table[b2-a2+255];
	  b3=sub_table[b3-a3+255];

      b1=b1 + ttable[uvideo8[offset*4  ]] - ttable[b1];
	  b2=b2 + ttable[uvideo8[offset*4+1]] - ttable[b2];
	  b3=b3 + ttable[uvideo8[offset*4+2]] - ttable[b3];

	  a=b1 | (b2<<8) | (b3<<16);
      uvideo32[offset]=a;
    }
    offset+=1; intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    u+=du;
    v+=dv;
  }
}


void scanline_texenv_mENVSUBTEX_tBLENDalpha(void)
{
  unsigned int a;
  unsigned char a1, a2, a3, b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  u=left_u;
  v=left_v;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
	  a1=envmap8[env_offset*4  ];  b1=texmap8[tex_offset*4  ];
      a2=envmap8[env_offset*4+1];  b2=texmap8[tex_offset*4+1];
      a3=envmap8[env_offset*4+2];  b3=texmap8[tex_offset*4+2];
	  b1=sub_table[a1-b1+255];
	  b2=sub_table[a2-b2+255];
	  b3=sub_table[a3-b3+255];

      b1=b1 + ttable[uvideo8[offset*4  ]] - ttable[b1];
	  b2=b2 + ttable[uvideo8[offset*4+1]] - ttable[b2];
	  b3=b3 + ttable[uvideo8[offset*4+2]] - ttable[b3];

	  a=b1 | (b2<<8) | (b3<<16);
      uvideo32[offset]=a;
    }
    offset+=1; intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    u+=du;
    v+=dv;
  }
}


void scanline_texenv_mBLEND50_tBLENDalpha(void)
{
  unsigned int a;
  unsigned char a1, a2, a3, b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  u=left_u;
  v=left_v;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));

	  a1=envmap8[env_offset*4  ];  b1=texmap8[tex_offset*4  ];
      a2=envmap8[env_offset*4+1];  b2=texmap8[tex_offset*4+1];
      a3=envmap8[env_offset*4+2];  b3=texmap8[tex_offset*4+2];

	  b1=(b1+a1)/2; b2=(b2+a2)/2; b3=(b3+a3)/2;
      b1=b1 + ttable[uvideo8[offset*4  ]] - ttable[b1];
	  b2=b2 + ttable[uvideo8[offset*4+1]] - ttable[b2];
	  b3=b3 + ttable[uvideo8[offset*4+2]] - ttable[b3];
	  
	  a=b1 | (b2<<8) | (b3<<16);
      uvideo32[offset]=a;
    }
    offset+=1; intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    u+=du;
    v+=dv;
  }
}


void scanline_texenv_mBLENDalpha_tBLENDalpha(void)
{
  unsigned int a;
  unsigned char a1, a2, a3, b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  u=left_u;
  v=left_v;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
	  a1=envmap8[env_offset*4  ];  b1=texmap8[tex_offset*4  ];
      a2=envmap8[env_offset*4+1];  b2=texmap8[tex_offset*4+1];
      a3=envmap8[env_offset*4+2];  b3=texmap8[tex_offset*4+2];
	  b1=b1 + atable[a1] - atable[b1];
	  b2=b2 + atable[a2] - atable[b2];
	  b3=b3 + atable[a2] - atable[b3];

      b1=b1 + ttable[uvideo8[offset*4  ]] - ttable[b1];
	  b2=b2 + ttable[uvideo8[offset*4+1]] - ttable[b2];
	  b3=b3 + ttable[uvideo8[offset*4+2]] - ttable[b3];

	  a=b1 | (b2<<8) | (b3<<16);
      uvideo32[offset]=a;
    }
    offset+=1; intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    u+=du;
    v+=dv;
  }
}


//************************************************************

void scanline_texenv_mADD_lADD(void)
{
  unsigned char a1, a2, a3, b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  u=left_u;
  v=left_v;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
	 RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
	  a1=envmap8[env_offset*4  ];  b1=texmap8[tex_offset*4  ];
      a2=envmap8[env_offset*4+1];  b2=texmap8[tex_offset*4+1];
      a3=envmap8[env_offset*4+2];  b3=texmap8[tex_offset*4+2];

	  b1=add_table[a1+b1];
	  b2=add_table[a2+b2];
	  b3=add_table[a3+b3];

	  b1=add_table[(BB1>>16)+b1];
	  b2=add_table[(GG1>>16)+b2];
	  b3=add_table[(RR1>>16)+b3];

      *intzb=*ddd;;
      uvideo32[offset]=b1 | (b2<<8) | (b3<<16);
    }
    offset+=1; intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    u+=du;
    v+=dv;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
  }
}


void scanline_texenv_mTEXSUBENV_lADD(void)
{
  unsigned char a1, a2, a3, b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  u=left_u;
  v=left_v;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
	 RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
	  a1=envmap8[env_offset*4  ];  b1=texmap8[tex_offset*4  ];
      a2=envmap8[env_offset*4+1];  b2=texmap8[tex_offset*4+1];
      a3=envmap8[env_offset*4+2];  b3=texmap8[tex_offset*4+2];

	  b1=sub_table[b1-a1+255];
	  b2=sub_table[b2-a2+255];
	  b3=sub_table[b3-a3+255];

	  b1=add_table[(BB1>>16)+b1];
	  b2=add_table[(GG1>>16)+b2];
	  b3=add_table[(RR1>>16)+b3];

      *intzb=*ddd;;
      uvideo32[offset]=b1 | (b2<<8) | (b3<<16);
    }
    offset+=1; intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    u+=du;
    v+=dv;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
  }
}


void scanline_texenv_mENVSUBTEX_lADD(void)
{
  unsigned char a1, a2, a3, b1, b2, b3;
  unsigned int c1, c2, c3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  u=left_u;
  v=left_v;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
	 RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
	  a1=envmap8[env_offset*4  ];  b1=texmap8[tex_offset*4  ];
      a2=envmap8[env_offset*4+1];  b2=texmap8[tex_offset*4+1];
      a3=envmap8[env_offset*4+2];  b3=texmap8[tex_offset*4+2];

	  b1=sub_table[a1-b1+255];
	  b2=sub_table[a2-b2+255];
	  b3=sub_table[a3-b3+255];

	  c1=BB1>>16; c2=GG1>>16; c3=RR1>>16;

	  a1=add_table[b1+c1];
	  a2=add_table[b2+c2];
	  a3=add_table[b3+c3];

      *intzb=*ddd;;
      uvideo32[offset]=a1 | (a2<<8) | (a3<<16);
    }
    offset+=1; intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    u+=du;
    v+=dv;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
  }
}


void scanline_texenv_mBLENDalpha_lADD(void)
{
  unsigned char a1, a2, a3, b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  u=left_u;
  v=left_v;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
	 RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
	  a1=envmap8[env_offset*4  ];  b1=texmap8[tex_offset*4  ];
      a2=envmap8[env_offset*4+1];  b2=texmap8[tex_offset*4+1];
      a3=envmap8[env_offset*4+2];  b3=texmap8[tex_offset*4+2];

	  b1=b1 + atable[a1] - atable[b1];
	  b2=b2 + atable[a2] - atable[b2];
	  b3=b3 + atable[a3] - atable[b3];

	  b1=add_table[(BB1>>16)+b1];
	  b2=add_table[(GG1>>16)+b2];
	  b3=add_table[(RR1>>16)+b3];

      *intzb=*ddd;;
      uvideo32[offset]=b1 | (b2<<8) | (b3<<16);
    }
    offset+=1; intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    u+=du;
    v+=dv;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
  }
}


/*
void scanline_texenv_mBLEND50_lADD(void)
{
  unsigned int a, b;
  unsigned char b1, b2, b3;
  int R1, G1, B1, ll, x1, x2, enu, env, uu, vv, off;
  float newz, dd;
  int *intzb, *ddd=(((int *)&newz));


  dd=left_z;
  enu=left_envu;
  env=left_envv;
  uu=left_u;
  vv=left_v;
  R1=left_R;
  G1=left_G;
  B1=left_B;
  x1=ceil16(left_x);
  x2=ceil16(right_x);
  if (x1<0)
  {
     dd+=(dz*(-x1));
     enu+=(envdu*(-x1));
     env+=(envdv*(-x1));
     uu+=(du*(-x1));
     vv+=(dv*(-x1));
	 R1+=(dR*(-x1));
     G1+=(dG*(-x1));
     B1+=(dB*(-x1));
     x1=0;
  }
  if (x2>=scrX) x2=scrX;

  off=offset0+x1;
  intzb=((int *)&zbuf[off]);

  for (ll=x2-x1; ll>0; ll--)
  {
	newz=dd+trick;
	if (*ddd > *intzb)
    {
      env_offset=(((env>>16)<<envvshift) + (enu>>16));
      tex_offset=((((vv>>16) & vmask)<<vshift) + ((uu>>16) & _umask_));

	  a=(envmap32[env_offset] & 0x00fefefe) >> 1;
	  b=(texmap32[tex_offset] & 0x00fefefe) >> 1;
	  b=b+a;

	  b1=add_table[(B1>>16)+(unsigned char)(b)];
	  b2=add_table[(G1>>16)+(unsigned char)(b >> 8)];
	  b3=add_table[(R1>>16)+(unsigned char)(b >> 16)];

	  *intzb=*ddd;;
	  uvideo32[off]=b1 | (b2<<8) | (b3<<16);
    }
	intzb+=1;
    off+=1;
    dd+=dz;
    enu+=envdu;
    env+=envdv;
    uu+=du;
    vv+=dv;
    R1+=dR;
    G1+=dG;
    B1+=dB;
  }
}
*/


void __declspec(naked) scanline_texenv_mBLEND50_lADD(void)
{
  _asm
  {
	 push ebp
     mov edi, [left_envu]

	 mov esi, [left_envv]
	 mov ebp, [left_u]

	 mov edx, [left_v]
	 mov ebx, [left_R]

     mov eax, [left_G]
	 mov ecx, [left_B]

	 mov [GG1], eax
	 mov eax, [left_x]

	 fld [left_z]
	 add eax, 0xFFFF

	 sar eax, 16
	 mov [BB1], ecx


	 mov [RR1], ebx
	 test eax, eax

	 jge noclip2Dleft

// clip2Dleft:

	 mov   ebx, eax
	 fstp  dword ptr [fi2]   // tolgo il lef_z che ho messo poco fa
	 
	 neg  ebx
	 mov  [fi], ebx

     mov ebx, eax      // ebx=xx1
	 imul ebx, [du]    // ebx=xx1*du
     fild  dword ptr [fi]
	 fmul  dword ptr [dz]
     sub  ebp, ebx     // ebp = u+=(-xx1)*du
	 
     mov ebx, eax
	 imul ebx, [dv]
	 fadd  dword ptr [left_z]    // sullo stack c'e' d
	 sub  edx, ebx   // edx = v+=(-xx1)*dv

     mov ebx, eax
	 imul ebx, [envdu]
	 sub  edi, ebx     // edi = envu+=(-xx1)*envdu

	 mov ebx, eax
	 imul ebx, [envdv]
	 sub  esi, ebx     // esi = envv+=(-xx1)*envdv

     mov ebx, eax
	 imul ebx, [dR]
     sub [RR1], ebx
     mov ebx, eax
	 imul ebx, [dG]
     sub [GG1], ebx
     mov ebx, eax
	 imul ebx, [dB]
     sub [BB1], ebx

	 xor eax, eax    // xx1 = 0

noclip2Dleft:

     mov [envv], esi
	 mov ebx, [right_x]

	 mov [envu], edi
	 add ebx, 0xFFFF

	 sar ebx, 16
	 mov esi, [scrX]

	 cmp ebx, esi
	 jl noclip2Dright
     mov ebx, esi

noclip2Dright:

     sub ebx, eax        // ebx = xx2-xx1
	 mov esi, [offset0]

	 add esi, eax       // esi = offset = offset0+xx1
	 mov edi, ebx

	 shl esi, 2

	 // a questo punto i registri sono cosi' messi
	 //
     // ebp = u
	 // edx = v
	 // envu in memoria
	 // envv in memoria
	 // RR1, GG1, BB1 in memoria
	 // esi = offset
     // sullo stack FPU = d
	 // sono liberi durante il ciclo eax, edi, ecx, ebx

	 test ebx, ebx
	 jle fine

ciclo:

     fld st(0)
	 fadd dword ptr [trick]

     mov [l], ebx
	 mov edi, dword ptr [zbuf]

	 mov ebx, [vshift]
	 mov ecx, [edi + esi]
     
	 fstp dword ptr [fi]
	 mov eax, [fi]

	 cmp eax, ecx
	 jle next_pixel

// write_pixel:

	 mov [edi+esi], eax    // scrivo lo zbuffer
	 mov eax, edx     // eax = v

     sar eax, 16
     mov edi, ebp  // salvo edi=ebp (che e' u)

	 mov ecx, ebx
	 and eax, [vmask]

	 sar edi, 16     // edi = u>>16
	 mov ebx, [texmap32]

	 shl eax, cl     // eax = ((v>>16) & vmask)<<vshift
	 and edi, [_umask_]  // edi = (u >> 16) & _umask_

	 add eax, edi        // eax = tex_offset
     mov edi, [envv]

	 sar edi, 16
     mov eax, [ebx+eax*4]   // eax = texel; rimangono liberi edi, ecx, ebx

	 and eax, 0x00fefefe   // intanto lavoro il texel
	 mov ecx, [envvshift]

	 shl edi, cl           // edi = (envv>>16) << envvshift
	 mov ecx, [envu]

	 sar ecx, 16
	 mov ebx, [envmap32]

     shr eax, 1           // texel pronto per essere sommato
	 add edi, ecx         // edi = env_offset

	 mov ecx, [ebx+edi*4]  // ecx = pixel envmap
     mov ebx, [BB1]

	 and ecx, 0x00fefefe
     mov edi, [GG1]

     shr ecx, 1            // texel env pronto per essere sommato
	 add eax, ecx          // eax = pixel (tex+env)/2
                           // liberi edi, ecx, ebx

	 shr edi, 16           // edi = GG1 >> 16
     mov ecx, ebx          // ecx = BB1

	 shr ecx, 16           // ecx = BB1 >> 16
	 add al, cl

	 sbb ebx, ebx
	 mov ecx, edi          // ecx = GG1 >> 16

	 or al, bl             // componente B ok!
	 mov edi, [RR1]

	 add ah, cl
	 mov [color_flat], edx

	 sbb ebx, ebx
     mov edx, [uvideo32]

	 shr edi, 16           // edi = RR1 >> 16
	 or ah, bl             // componente G ok!

     mov ecx, edi
	 mov edi, edx

	 ror eax, 16
     lea edi, [edi+esi]

	 add al, cl
	 mov edx, [color_flat]

	 sbb ebx, ebx
	 mov edx, [color_flat]

	 or al, bl             // componente R ok!
	 mov edx, [color_flat]

	 ror eax, 16
	 mov edx, [color_flat]

	 mov [edi], eax

next_pixel:

	 mov ecx, [envu]
	 add ebp, [du]           //  u+=du;

	 add edx, [dv]           //  v+=dv;
     mov edi, [envv]

	 add ecx, [envdu]
	 add edi, [envdv]

	 mov [envu], ecx
	 mov [envv], edi

	 mov eax, [RR1]
	 mov ebx, [GG1]

	 mov edi, [BB1]
	 add eax, [dR]

	 add ebx, [dG]
	 add edi, [dB]

	 mov [RR1], eax
	 mov [BB1], edi

	 fadd dword ptr [dz]
	 mov [GG1], ebx

	 mov ebx, [l]
	 add esi, 4             // offset++

	 dec ebx
	 jnz ciclo

fine:

	 pop ebp
	 fstp dword ptr [fi]
	 ret
  }
}


//************************************************************

void scanline_texenv_mADD_lSUB(void)
{
  unsigned char a1, a2, a3, b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  u=left_u;
  v=left_v;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
	 RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
	  a1=envmap8[env_offset*4  ];  b1=texmap8[tex_offset*4  ];
      a2=envmap8[env_offset*4+1];  b2=texmap8[tex_offset*4+1];
      a3=envmap8[env_offset*4+2];  b3=texmap8[tex_offset*4+2];

	  b1=add_table[a1+b1];
	  b2=add_table[a2+b2];
	  b3=add_table[a3+b3];

	  b1=sub_table[b1-(BB1>>16)+255];
	  b2=sub_table[b2-(GG1>>16)+255];
	  b3=sub_table[b3-(RR1>>16)+255];

      *intzb=*ddd;;
      uvideo32[offset]=b1 | (b2<<8) | (b3<<16);
    }
    offset+=1; intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    u+=du;
    v+=dv;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
  }
}


void scanline_texenv_mTEXSUBENV_lSUB(void)
{
  unsigned char a1, a2, a3, b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  u=left_u;
  v=left_v;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
	 RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
	  a1=envmap8[env_offset*4  ];  b1=texmap8[tex_offset*4  ];
      a2=envmap8[env_offset*4+1];  b2=texmap8[tex_offset*4+1];
      a3=envmap8[env_offset*4+2];  b3=texmap8[tex_offset*4+2];

	  b1=sub_table[b1-a1+255];
	  b2=sub_table[b2-a2+255];
	  b3=sub_table[b3-a3+255];

	  b1=sub_table[b1-(BB1>>16)+255];
	  b2=sub_table[b2-(GG1>>16)+255];
	  b3=sub_table[b3-(RR1>>16)+255];

      *intzb=*ddd;;
      uvideo32[offset]=b1 | (b2<<8) | (b3<<16);
    }
    offset+=1; intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    u+=du;
    v+=dv;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
  }
}


void scanline_texenv_mENVSUBTEX_lSUB(void)
{
  unsigned char a1, a2, a3, b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  u=left_u;
  v=left_v;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
	 RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
	  a1=envmap8[env_offset*4  ];  b1=texmap8[tex_offset*4  ];
      a2=envmap8[env_offset*4+1];  b2=texmap8[tex_offset*4+1];
      a3=envmap8[env_offset*4+2];  b3=texmap8[tex_offset*4+2];

	  b1=sub_table[a1-b1+255];
	  b2=sub_table[a2-b2+255];
	  b3=sub_table[a3-b3+255];

	  b1=sub_table[b1-(BB1>>16)+255];
	  b2=sub_table[b2-(GG1>>16)+255];
	  b3=sub_table[b3-(RR1>>16)+255];

      *intzb=*ddd;;
      uvideo32[offset]=b1 | (b2<<8) | (b3<<16);
    }
    offset+=1; intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    u+=du;
    v+=dv;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
  }
}


void scanline_texenv_mBLENDalpha_lSUB(void)
{
  unsigned char a1, a2, a3, b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  u=left_u;
  v=left_v;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
	 RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
	  a1=envmap8[env_offset*4  ];  b1=texmap8[tex_offset*4  ];
      a2=envmap8[env_offset*4+1];  b2=texmap8[tex_offset*4+1];
      a3=envmap8[env_offset*4+2];  b3=texmap8[tex_offset*4+2];

	  b1=b1 + atable[a1] - atable[b1];
	  b2=b2 + atable[a2] - atable[b2];
	  b3=b3 + atable[a3] - atable[b3];

	  b1=sub_table[b1-(BB1>>16)+255];
	  b2=sub_table[b2-(GG1>>16)+255];
	  b3=sub_table[b3-(RR1>>16)+255];

      *intzb=*ddd;;
      uvideo32[offset]=b1 | (b2<<8) | (b3<<16);
    }
    offset+=1; intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    u+=du;
    v+=dv;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
  }
}


void scanline_texenv_mBLEND50_lSUB(void)
{
  unsigned char a1, a2, a3, b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  u=left_u;
  v=left_v;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
	 RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));

	  a1=envmap8[env_offset*4  ];  b1=texmap8[tex_offset*4  ];
      a2=envmap8[env_offset*4+1];  b2=texmap8[tex_offset*4+1];
      a3=envmap8[env_offset*4+2];  b3=texmap8[tex_offset*4+2];
	  b1=(b1+a1)>>1; b2=(b2+a2)>>1; b3=(b3+a3)>>1;

	  b1=sub_table[b1-(BB1>>16)+255];
	  b2=sub_table[b2-(GG1>>16)+255];
	  b3=sub_table[b3-(RR1>>16)+255];

      *intzb=*ddd;;
	  uvideo32[offset]=b1 | (b2<<8) | (b3<<16);
    }
    offset+=1; intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    u+=du;
    v+=dv;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
  }
}


//************************************************************

void scanline_texenv_mADD_lBLEND50(void)
{
  unsigned char a1, a2, a3, b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  u=left_u;
  v=left_v;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
	 RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
	  a1=envmap8[env_offset*4  ];  b1=texmap8[tex_offset*4  ];
      a2=envmap8[env_offset*4+1];  b2=texmap8[tex_offset*4+1];
      a3=envmap8[env_offset*4+2];  b3=texmap8[tex_offset*4+2];

	  b1=add_table[a1+b1];
	  b2=add_table[a2+b2];
	  b3=add_table[a3+b3];

	  b1=(b1+(BB1>>16)) >> 1;
	  b2=(b2+(GG1>>16)) >> 1;
	  b3=(b3+(RR1>>16)) >> 1;

      *intzb=*ddd;;
      uvideo32[offset]=b1 | (b2<<8) | (b3<<16);
    }
    offset+=1; intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    u+=du;
    v+=dv;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
  }
}


void scanline_texenv_mTEXSUBENV_lBLEND50(void)
{
  unsigned char a1, a2, a3, b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  u=left_u;
  v=left_v;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
	 RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
	  a1=envmap8[env_offset*4  ];  b1=texmap8[tex_offset*4  ];
      a2=envmap8[env_offset*4+1];  b2=texmap8[tex_offset*4+1];
      a3=envmap8[env_offset*4+2];  b3=texmap8[tex_offset*4+2];

	  b1=sub_table[b1-a1+255];
	  b2=sub_table[b2-a2+255];
	  b3=sub_table[b3-a3+255];

	  b1=(b1+(BB1>>16)) >> 1;
	  b2=(b2+(GG1>>16)) >> 1;
	  b3=(b3+(RR1>>16)) >> 1;
	  
      *intzb=*ddd;;
      uvideo32[offset]=b1 | (b2<<8) | (b3<<16);
    }
    offset+=1; intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    u+=du;
    v+=dv;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
  }
}


void scanline_texenv_mENVSUBTEX_lBLEND50(void)
{
  unsigned char a1, a2, a3, b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  u=left_u;
  v=left_v;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
	 RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
	  a1=envmap8[env_offset*4  ];  b1=texmap8[tex_offset*4  ];
      a2=envmap8[env_offset*4+1];  b2=texmap8[tex_offset*4+1];
      a3=envmap8[env_offset*4+2];  b3=texmap8[tex_offset*4+2];

	  b1=sub_table[a1-b1+255];
	  b2=sub_table[a2-b2+255];
	  b3=sub_table[a3-b3+255];

	  b1=(b1+(BB1>>16)) >> 1;
	  b2=(b2+(GG1>>16)) >> 1;
	  b3=(b3+(RR1>>16)) >> 1;

      *intzb=*ddd;;
      uvideo32[offset]=b1 | (b2<<8) | (b3<<16);
    }
    offset+=1; intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    u+=du;
    v+=dv;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
  }
}


void scanline_texenv_mBLENDalpha_lBLEND50(void)
{
  unsigned char a1, a2, a3, b1, b2, b3;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  u=left_u;
  v=left_v;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
	 RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
	  a1=envmap8[env_offset*4  ];  b1=texmap8[tex_offset*4  ];
      a2=envmap8[env_offset*4+1];  b2=texmap8[tex_offset*4+1];
      a3=envmap8[env_offset*4+2];  b3=texmap8[tex_offset*4+2];

	  b1=b1 + atable[a1] - atable[b1];
	  b2=b2 + atable[a2] - atable[b2];
	  b3=b3 + atable[a3] - atable[b3];

	  b1=(b1+(BB1>>16)) >> 1;
	  b2=(b2+(GG1>>16)) >> 1;
	  b3=(b3+(RR1>>16)) >> 1;

      *intzb=*ddd;;
      uvideo32[offset]=b1 | (b2<<8) | (b3<<16);
    }
    offset+=1; intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    u+=du;
    v+=dv;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
  }
}


void scanline_texenv_mBLEND50_lBLEND50(void)
{
  unsigned int a, b, c;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  u=left_u;
  v=left_v;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
	 RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));

	  a=(envmap32[env_offset] & 0x00fefefe) >>1;
	  b=(texmap32[tex_offset] & 0x00fefefe) >>1;
	  b=((a+b) & 0x00fefefe) >> 1;
	  c=((RR1 & 0x00FF0000) | ((GG1 & 0x00FF0000) >> 8) | ((BB1>>16)));
      c=(c & 0x00fefefe) >> 1;

      *intzb=*ddd;;
	  uvideo32[offset]=b+c;
    }
    offset+=1; intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    u+=du;
    v+=dv;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
  }
}


//************************************************************

void scanline_texenv_mADD_lBLEND3DS(void)
{
  unsigned char a1, a2, a3, b1, b2, b3, c1, c2, c3;
  unsigned int bb;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  u=left_u;
  v=left_v;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
	 RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
	  a1=envmap8[env_offset*4  ];  b1=texmap8[tex_offset*4  ];
      a2=envmap8[env_offset*4+1];  b2=texmap8[tex_offset*4+1];
      a3=envmap8[env_offset*4+2];  b3=texmap8[tex_offset*4+2];

	  b1=add_table[a1+b1] >> 1;
	  b2=add_table[a2+b2]  >> 1;
	  b3=add_table[a3+b3]  >> 1;

	  c1=BB1 >> (16+1);   // blu
      c2=GG1 >> (16+1);   // verde
	  c3=RR1 >> (16+1);   // rosso
	  bb=(c1*b1)>>6; c1=bb;
      bb=(c2*b2)>>6; c2=bb;
      bb=(c3*b3)>>6; c3=bb;

	  *intzb=*ddd;
	  uvideo32[offset]=c1 | (c2<<8) | (c3<<16);
    }
    offset+=1; intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    u+=du;
    v+=dv;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
  }
}


void scanline_texenv_mTEXSUBENV_lBLEND3DS(void)
{
  unsigned char a1, a2, a3, b1, b2, b3, c1, c2, c3;
  unsigned int bb;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  u=left_u;
  v=left_v;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
	 RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
	  a1=envmap8[env_offset*4  ];  b1=texmap8[tex_offset*4  ];
      a2=envmap8[env_offset*4+1];  b2=texmap8[tex_offset*4+1];
      a3=envmap8[env_offset*4+2];  b3=texmap8[tex_offset*4+2];

	  b1=sub_table[b1-a1+255] >>1;
	  b2=sub_table[b2-a2+255] >>1;
	  b3=sub_table[b3-a3+255] >>1;

	  c1=BB1 >> (16+1);   // blu
      c2=GG1 >> (16+1);   // verde
	  c3=RR1 >> (16+1);   // rosso
	  bb=(c1*b1)>>6; c1=bb;
      bb=(c2*b2)>>6; c2=bb;
      bb=(c3*b3)>>6; c3=bb;
	  
      *intzb=*ddd;;
      uvideo32[offset]=c1 | (c2<<8) | (c3<<16);
    }
    offset+=1; intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    u+=du;
    v+=dv;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
  }
}


void scanline_texenv_mENVSUBTEX_lBLEND3DS(void)
{
  unsigned char a1, a2, a3, b1, b2, b3, c1, c2, c3;
  unsigned int bb;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  u=left_u;
  v=left_v;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
	 RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
	  a1=envmap8[env_offset*4  ];  b1=texmap8[tex_offset*4  ];
      a2=envmap8[env_offset*4+1];  b2=texmap8[tex_offset*4+1];
      a3=envmap8[env_offset*4+2];  b3=texmap8[tex_offset*4+2];

	  b1=sub_table[a1-b1+255] >>1;
	  b2=sub_table[a2-b2+255] >>1;
	  b3=sub_table[a3-b3+255] >>1;

	  c1=BB1 >> (16+1);   // blu
      c2=GG1 >> (16+1);   // verde
	  c3=RR1 >> (16+1);   // rosso
	  bb=(c1*b1)>>6; c1=bb;
      bb=(c2*b2)>>6; c2=bb;
      bb=(c3*b3)>>6; c3=bb;

      *intzb=*ddd;;
      uvideo32[offset]=c1 | (c2<<8) | (c3<<16);
    }
    offset+=1; intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    u+=du;
    v+=dv;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
  }
}


void scanline_texenv_mBLENDalpha_lBLEND3DS(void)
{
  unsigned char a1, a2, a3, b1, b2, b3, c1, c2, c3;
  unsigned int bb;
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  u=left_u;
  v=left_v;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
	 RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));
	  a1=envmap8[env_offset*4  ];  b1=texmap8[tex_offset*4  ];
      a2=envmap8[env_offset*4+1];  b2=texmap8[tex_offset*4+1];
      a3=envmap8[env_offset*4+2];  b3=texmap8[tex_offset*4+2];

	  b1=(b1 + atable[a1] - atable[b1]) >>1;
	  b2=(b2 + atable[a2] - atable[b2]) >>1;
	  b3=(b3 + atable[a3] - atable[b3]) >>1;

	  c1=BB1 >> (16+1);   // blu
      c2=GG1 >> (16+1);   // verde
	  c3=RR1 >> (16+1);   // rosso
	  bb=(c1*b1)>>6; c1=bb;
      bb=(c2*b2)>>6; c2=bb;
      bb=(c3*b3)>>6; c3=bb;

      *intzb=*ddd;;
      uvideo32[offset]=c1 | (c2<<8) | (c3<<16);
    }
    offset+=1; intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    u+=du;
    v+=dv;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
  }
}


void scanline_texenv_mBLEND50_lBLEND3DS(void)
{
  unsigned char a1, a2, a3, b1, b2, b3, c1, c2, c3;
  unsigned int bb;  
  float newz;
  int *intzb, *ddd=(((int *)&newz));

  d=left_z;
  envu=left_envu;
  envv=left_envv;
  u=left_u;
  v=left_v;
  RR1=left_R;
  GG1=left_G;
  BB1=left_B;
  xx1=ceil16(left_x);
  xx2=ceil16(right_x);
  if (xx1<0)
  {
     d+=(dz*(-xx1));
     envu+=(envdu*(-xx1));
     envv+=(envdv*(-xx1));
     u+=(du*(-xx1));
     v+=(dv*(-xx1));
	 RR1+=(dR*(-xx1));
     GG1+=(dG*(-xx1));
     BB1+=(dB*(-xx1));
     xx1=0;
  }
  if (xx2>=scrX) xx2=scrX;

  offset=offset0+xx1;
  intzb=((int *)&zbuf[offset]);
  for (l=xx2-xx1; l>0; l--)
  {
	newz=d+trick;
    if (*ddd > *intzb)
    {
      env_offset=(((envv>>16)<<envvshift) + (envu>>16));
      tex_offset=((((v>>16) & vmask)<<vshift) + ((u>>16) & _umask_));

	  a1=envmap8[env_offset*4  ];  b1=texmap8[tex_offset*4  ];
      a2=envmap8[env_offset*4+1];  b2=texmap8[tex_offset*4+1];
      a3=envmap8[env_offset*4+2];  b3=texmap8[tex_offset*4+2];
      
	  b1=(a1+b1) >> 1;
	  b2=(a2+b2) >> 1;
	  b3=(a3+b3) >> 1;

	  c1=BB1 >> (16+1);   // blu
      c2=GG1 >> (16+1);   // verde
	  c3=RR1 >> (16+1);   // rosso
	  bb=(c1*b1)>>7; c1=bb;
      bb=(c2*b2)>>7; c2=bb;
      bb=(c3*b3)>>7; c3=bb;

      *intzb=*ddd;;
	  uvideo32[offset]=c1 | (c2<<8) | (c3<<16);
    }
    offset+=1; intzb+=1;
    d+=dz;
    envu+=envdu;
    envv+=envdv;
    u+=du;
    v+=dv;
    RR1+=dR;
    GG1+=dG;
    BB1+=dB;
  }
}