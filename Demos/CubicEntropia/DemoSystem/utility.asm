	.586p
	ifdef ??version
	if    ??version GT 500H
	.mmx
	endif
	endif
	model flat
	ifndef	??version
	?debug	macro
	endm
	endif
	?debug	S "D:\redshift\Demo_sw\DemoSystem\utility.cpp"
	?debug	T "D:\redshift\Demo_sw\DemoSystem\utility.cpp"
_TEXT	segment dword public use32 'CODE'
_TEXT	ends
_DATA	segment dword public use32 'DATA'
_DATA	ends
_BSS	segment dword public use32 'BSS'
_BSS	ends
$$BSYMS	segment byte public use32 'DEBSYM'
$$BSYMS	ends
$$BTYPES	segment byte public use32 'DEBTYP'
$$BTYPES	ends
$$BNAMES	segment byte public use32 'DEBNAM'
$$BNAMES	ends
$$BROWSE	segment byte public use32 'DEBSYM'
$$BROWSE	ends
$$BROWFILE	segment byte public use32 'DEBSYM'
$$BROWFILE	ends
DGROUP	group	_BSS,_DATA
_DATA	segment dword public use32 'DATA'
	align	4
_Pi__	label	dword
	db        219,15,73,64
_DATA	ends
_TEXT	segment dword public use32 'CODE'
@debug_error$qpvpc	segment virtual
@@debug_error$qpvpc	proc	near
?live16385@0:
	?debug L 5
	push      ebp
	mov       ebp,esp
	?debug L 7
@1:
	push      0
	push      offset s@
	push      dword ptr [ebp+12]
	push      dword ptr [ebp+8]
	call      @MessageBoxA
	?debug L 8
	push      0
	call      @_exit
	pop       ecx
	?debug L 9
@2:
	pop       ebp
	ret 
	?debug L 0
@@debug_error$qpvpc	endp
@debug_error$qpvpc	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	db	2
	db	0
	db	0
	db	0
	dw	65
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch1
	dd	?patch2
	dd	?patch3
	df	@@debug_error$qpvpc
	dw	0
	dw	4096
	dw	0
	dw	1
	dw	0
	dw	0
	dw	0
	db	18
	db	64
	db	100
	db	101
	db	98
	db	117
	db	103
	db	95
	db	101
	db	114
	db	114
	db	111
	db	114
	db	36
	db	113
	db	112
	db	118
	db	112
	db	99
	dw	18
	dw	512
	dw	8
	dw	0
	dw	1027
	dw	0
	dw	2
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	1136
	dw	0
	dw	3
	dw	0
	dw	0
	dw	0
?patch1	equ	@2-@@debug_error$qpvpc+2
?patch2	equ	0
?patch3	equ	@2-@@debug_error$qpvpc
	dw	2
	dw	6
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@memsetd$qpvuiui	segment virtual
@@memsetd$qpvuiui	proc	near
?live16386@0:
	?debug L 13
	push      ebp
	mov       ebp,esp
	push      esi
	push      edi
	?debug L 18
@3:
	mov	 edi, [dword ptr [ebp+8]]
	?debug L 19
	mov	 ecx, [dword ptr [ebp+16]]
	?debug L 21
	shr	 ecx, 1
	?debug L 22
	mov	 eax, [dword ptr [ebp+12]]
	?debug L 24
	lea	 esi, [edi+ecx*8]
	?debug L 25
	neg	 ecx
	?debug L 27
@4:
ciclo:
	?debug L 28
	mov	 [esi+ecx*8], eax
	?debug L 29
	mov	 [esi+ecx*8+4], eax
	?debug L 31
	inc	 ecx
	?debug L 32
	jne       @4
	?debug L 34
@5:
	pop       edi
	pop       esi
	pop       ebp
	ret 
	?debug L 0
@@memsetd$qpvuiui	endp
@memsetd$qpvuiui	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	63
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch4
	dd	?patch5
	dd	?patch6
	df	@@memsetd$qpvuiui
	dw	0
	dw	4098
	dw	0
	dw	4
	dw	0
	dw	0
	dw	0
	db	16
	db	64
	db	109
	db	101
	db	109
	db	115
	db	101
	db	116
	db	100
	db	36
	db	113
	db	112
	db	118
	db	117
	db	105
	db	117
	db	105
	dw	18
	dw	512
	dw	8
	dw	0
	dw	1027
	dw	0
	dw	5
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	117
	dw	0
	dw	6
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	117
	dw	0
	dw	7
	dw	0
	dw	0
	dw	0
?patch4	equ	@5-@@memsetd$qpvuiui+4
?patch5	equ	0
?patch6	equ	@5-@@memsetd$qpvuiui
	dw	2
	dw	6
	dw	8
	dw	531
	dw	6
	dw	65528
	dw	65535
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@memcpyd$qpvt1i	segment virtual
@@memcpyd$qpvt1i	proc	near
?live16387@0:
	?debug L 38
	push      ebp
	mov       ebp,esp
	push      esi
	push      edi
	?debug L 41
@6:
	push	 edi
	?debug L 42
	push	 ecx
	?debug L 43
	push	 esi
	?debug L 44
	mov	 edi,dword ptr [ebp+8]
	?debug L 45
	mov	 esi,dword ptr [ebp+12]
	?debug L 46
	mov	 ecx,dword ptr [ebp+16]
	?debug L 47
	rep movsd	
	?debug L 48
	pop	 esi
	?debug L 49
	pop	 ecx
	?debug L 50
	pop	 edi
	?debug L 52
@7:
	pop       edi
	pop       esi
	pop       ebp
	ret 
	?debug L 0
@@memcpyd$qpvt1i	endp
@memcpyd$qpvt1i	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	62
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch7
	dd	?patch8
	dd	?patch9
	df	@@memcpyd$qpvt1i
	dw	0
	dw	4100
	dw	0
	dw	8
	dw	0
	dw	0
	dw	0
	db	15
	db	64
	db	109
	db	101
	db	109
	db	99
	db	112
	db	121
	db	100
	db	36
	db	113
	db	112
	db	118
	db	116
	db	49
	db	105
	dw	18
	dw	512
	dw	8
	dw	0
	dw	1027
	dw	0
	dw	9
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	1027
	dw	0
	dw	10
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	116
	dw	0
	dw	11
	dw	0
	dw	0
	dw	0
?patch7	equ	@7-@@memcpyd$qpvt1i+4
?patch8	equ	0
?patch9	equ	@7-@@memcpyd$qpvt1i
	dw	2
	dw	6
	dw	8
	dw	531
	dw	6
	dw	65528
	dw	65535
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@rgb_to_hsv$qfffpft4t4	segment virtual
@@rgb_to_hsv$qfffpft4t4	proc	near
?live16388@0:
	?debug L 56
	push      ebp
	mov       ebp,esp
	add       esp,-24
	?debug L 62
@8:
	xor       eax,eax
	mov       dword ptr [ebp-4],eax
	?debug L 63
	fld       dword ptr [ebp+8]
	fcomp     dword ptr [ebp+12]
	fnstsw ax
	sahf
	jbe       short @10
	?debug L 65
	fld       dword ptr [ebp+8]
	fcomp     dword ptr [ebp+16]
	fnstsw ax
	sahf
	jbe       short @11
	?debug L 66
	mov       edx,dword ptr [ebp+8]
	mov       dword ptr [ebp-20],edx
	jmp       short @12
	?debug L 68
@11:
	mov       ecx,dword ptr [ebp+16]
	mov       dword ptr [ebp-20],ecx
	?debug L 70
@12:
	fld       dword ptr [ebp+12]
	fcomp     dword ptr [ebp+16]
	fnstsw ax
	sahf
	jae       short @13
	?debug L 71
	mov       edx,dword ptr [ebp+12]
	mov       dword ptr [ebp-16],edx
	jmp       short @14
	?debug L 73
@13:
	mov       ecx,dword ptr [ebp+16]
	mov       dword ptr [ebp-16],ecx
	?debug L 74
	jmp       short @14
	?debug L 77
@10:
	fld       dword ptr [ebp+12]
	fcomp     dword ptr [ebp+16]
	fnstsw ax
	sahf
	jbe       short @15
	?debug L 78
	mov       edx,dword ptr [ebp+12]
	mov       dword ptr [ebp-20],edx
	jmp       short @16
	?debug L 80
@15:
	mov       ecx,dword ptr [ebp+16]
	mov       dword ptr [ebp-20],ecx
	?debug L 82
@16:
	fld       dword ptr [ebp+8]
	fcomp     dword ptr [ebp+16]
	fnstsw ax
	sahf
	jae       short @17
	?debug L 83
	mov       edx,dword ptr [ebp+8]
	mov       dword ptr [ebp-16],edx
	jmp       short @18
	?debug L 85
@17:
	mov       ecx,dword ptr [ebp+16]
	mov       dword ptr [ebp-16],ecx
	?debug L 88
@18:
@14:
	mov       eax,dword ptr [ebp-20]
	mov       dword ptr [ebp-12],eax
	?debug L 90
	fld       dword ptr [ebp-20]
	fcomp     dword ptr [@20]
	fnstsw ax
	sahf
	je        short @19
	?debug L 91
	fld       dword ptr [ebp-20]
	fsub      dword ptr [ebp-16]
	fdiv      dword ptr [ebp-20]
	fstp      dword ptr [ebp-8]
	jmp       short @21
	?debug L 93
@19:
	xor       edx,edx
	mov       dword ptr [ebp-8],edx
	?debug L 95
@21:
	fld       dword ptr [ebp-8]
	fcomp     dword ptr [@20]
	fnstsw ax
	sahf
	jne       short @22
	?debug L 97
	xor       edx,edx
	mov       dword ptr [ebp-4],edx
	?debug L 98
	jmp       @23
	?debug L 101
@22:
	fld       dword ptr [ebp-20]
	fsub      dword ptr [ebp-16]
	fstp      dword ptr [ebp-24]
	?debug L 103
	fld       dword ptr [ebp+8]
	fcomp     dword ptr [ebp-20]
	fnstsw ax
	sahf
	jne       short @24
	?debug L 104
	fld       dword ptr [ebp+12]
	fsub      dword ptr [ebp+16]
	fdiv      dword ptr [ebp-24]
	fstp      dword ptr [ebp-4]
	jmp       short @25
	?debug L 105
@24:
	fld       dword ptr [ebp+12]
	fcomp     dword ptr [ebp-20]
	fnstsw ax
	sahf
	jne       short @26
	?debug L 106
	fld       dword ptr [ebp+16]
	fsub      dword ptr [ebp+8]
	fdiv      dword ptr [ebp-24]
	fadd      dword ptr [@20+4]
	fstp      dword ptr [ebp-4]
	jmp       short @25
	?debug L 107
@26:
	fld       dword ptr [ebp+16]
	fcomp     dword ptr [ebp-20]
	fnstsw ax
	sahf
	jne       short @27
	?debug L 108
	fld       dword ptr [ebp+8]
	fsub      dword ptr [ebp+12]
	fdiv      dword ptr [ebp-24]
	fadd      dword ptr [@20+8]
	fstp      dword ptr [ebp-4]
	?debug L 110
@27:
@25:
	fld       tbyte ptr [@20+12]
	fmul      dword ptr [ebp-4]
	fstp      dword ptr [ebp-4]
	?debug L 112
	fld       dword ptr [ebp-4]
	fcomp     dword ptr [@20]
	fnstsw ax
	sahf
	jae       short @28
	?debug L 113
	fld       dword ptr [ebp-4]
	fadd      dword ptr [@20+24]
	fstp      dword ptr [ebp-4]
	jmp       short @29
	?debug L 114
@28:
	fld       dword ptr [ebp-4]
	fcomp     dword ptr [@20+24]
	fnstsw ax
	sahf
	jb        short @30
	?debug L 115
	fld       dword ptr [ebp-4]
	fsub      dword ptr [@20+24]
	fstp      dword ptr [ebp-4]
	?debug L 118
@30:
@29:
@23:
	mov       edx,dword ptr [ebp+20]
	mov       ecx,dword ptr [ebp-4]
	mov       dword ptr [edx],ecx
	?debug L 119
	mov       eax,dword ptr [ebp+24]
	mov       edx,dword ptr [ebp-8]
	mov       dword ptr [eax],edx
	?debug L 120
	mov       ecx,dword ptr [ebp+28]
	mov       eax,dword ptr [ebp-12]
	mov       dword ptr [ecx],eax
	?debug L 121
@31:
	mov       esp,ebp
	pop       ebp
	ret 
	align 4        
@20:
	db        0,0,0,0,0,0,0,64,0,0,128,64,171,170,170,170
	db        170,170,170,170,252,63,0,0,0,0,128,63
	?debug L 0
@@rgb_to_hsv$qfffpft4t4	endp
@rgb_to_hsv$qfffpft4t4	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	69
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch10
	dd	?patch11
	dd	?patch12
	df	@@rgb_to_hsv$qfffpft4t4
	dw	0
	dw	4102
	dw	0
	dw	12
	dw	0
	dw	0
	dw	0
	db	22
	db	64
	db	114
	db	103
	db	98
	db	95
	db	116
	db	111
	db	95
	db	104
	db	115
	db	118
	db	36
	db	113
	db	102
	db	102
	db	102
	db	112
	db	102
	db	116
	db	52
	db	116
	db	52
	dw	18
	dw	512
	dw	8
	dw	0
	dw	64
	dw	0
	dw	13
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	64
	dw	0
	dw	14
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	64
	dw	0
	dw	15
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	20
	dw	0
	dw	1088
	dw	0
	dw	16
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	24
	dw	0
	dw	1088
	dw	0
	dw	17
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	28
	dw	0
	dw	1088
	dw	0
	dw	18
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65512
	dw	65535
	dw	64
	dw	0
	dw	19
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65516
	dw	65535
	dw	64
	dw	0
	dw	20
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65520
	dw	65535
	dw	64
	dw	0
	dw	21
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65524
	dw	65535
	dw	64
	dw	0
	dw	22
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65528
	dw	65535
	dw	64
	dw	0
	dw	23
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	64
	dw	0
	dw	24
	dw	0
	dw	0
	dw	0
?patch10	equ	@31-@@rgb_to_hsv$qfffpft4t4+33
?patch11	equ	0
?patch12	equ	@31-@@rgb_to_hsv$qfffpft4t4
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@hsv_to_rgb$qfffpft4t4	segment virtual
@@hsv_to_rgb$qfffpft4t4	proc	near
?live16389@0:
	?debug L 125
	push      ebp
	mov       ebp,esp
	add       esp,-36
	?debug L 129
@32:
	fld       dword ptr [ebp+12]
	fcomp     dword ptr [@34]
	fnstsw ax
	sahf
	jne       short @33
	?debug L 131
	mov       edx,dword ptr [ebp+20]
	mov       ecx,dword ptr [ebp+16]
	mov       dword ptr [edx],ecx
	?debug L 132
	mov       eax,dword ptr [ebp+24]
	mov       edx,dword ptr [ebp+16]
	mov       dword ptr [eax],edx
	?debug L 133
	mov       ecx,dword ptr [ebp+28]
	mov       eax,dword ptr [ebp+16]
	mov       dword ptr [ecx],eax
	?debug L 134
	jmp       @35
	?debug L 137
@33:
	fld       dword ptr [@34+4]
	fmul      dword ptr [ebp+8]
	fstp      dword ptr [ebp+8]
	?debug L 138
	fld       dword ptr [ebp+8]
	fcomp     dword ptr [@34+4]
	fnstsw ax
	sahf
	jne       short @36
	xor       edx,edx
	mov       dword ptr [ebp+8],edx
	?debug L 140
@36:
	fld       dword ptr [ebp+8]
	call      __ftol
	mov       dword ptr [ebp-36],eax
	fild      dword ptr [ebp-36]
	fsubr     dword ptr [ebp+8]
	fstp      qword ptr [ebp-8]
	?debug L 141
	fld       dword ptr [@34+8]
	fsub      dword ptr [ebp+12]
	fmul      dword ptr [ebp+16]
	fstp      qword ptr [ebp-16]
	?debug L 142
	fld       dword ptr [ebp+12]
	fmul      qword ptr [ebp-8]
	fsubr     dword ptr [@34+8]
	fmul      dword ptr [ebp+16]
	fstp      qword ptr [ebp-24]
	?debug L 143
	fld       dword ptr [@34+8]
	fsub      qword ptr [ebp-8]
	fmul      dword ptr [ebp+12]
	fsubr     dword ptr [@34+8]
	fmul      dword ptr [ebp+16]
	fstp      qword ptr [ebp-32]
	?debug L 145
	fld       dword ptr [ebp+8]
	call      __ftol
	cmp       eax,5
	ja        @37
	jmp       dword ptr [@45+4*eax]
@45:
	dd        @44
	dd        @43
	dd        @42
	dd        @41
	dd        @40
	dd        @39
	?debug L 148
@44:
	mov       edx,dword ptr [ebp+20]
	mov       ecx,dword ptr [ebp+16]
	mov       dword ptr [edx],ecx
	?debug L 149
	fld       qword ptr [ebp-32]
	mov       eax,dword ptr [ebp+24]
	fstp      dword ptr [eax]
	?debug L 150
	fld       qword ptr [ebp-16]
	mov       edx,dword ptr [ebp+28]
	fstp      dword ptr [edx]
	?debug L 151
	jmp       @46
	?debug L 154
@43:
	fld       qword ptr [ebp-24]
	mov       ecx,dword ptr [ebp+20]
	fstp      dword ptr [ecx]
	?debug L 155
	mov       eax,dword ptr [ebp+24]
	mov       edx,dword ptr [ebp+16]
	mov       dword ptr [eax],edx
	?debug L 156
	fld       qword ptr [ebp-16]
	mov       ecx,dword ptr [ebp+28]
	fstp      dword ptr [ecx]
	?debug L 157
	jmp       short @46
	?debug L 160
@42:
	fld       qword ptr [ebp-16]
	mov       eax,dword ptr [ebp+20]
	fstp      dword ptr [eax]
	?debug L 161
	mov       edx,dword ptr [ebp+24]
	mov       ecx,dword ptr [ebp+16]
	mov       dword ptr [edx],ecx
	?debug L 162
	fld       qword ptr [ebp-32]
	mov       eax,dword ptr [ebp+28]
	fstp      dword ptr [eax]
	?debug L 163
	jmp       short @46
	?debug L 166
@41:
	fld       qword ptr [ebp-16]
	mov       edx,dword ptr [ebp+20]
	fstp      dword ptr [edx]
	?debug L 167
	fld       qword ptr [ebp-24]
	mov       ecx,dword ptr [ebp+24]
	fstp      dword ptr [ecx]
	?debug L 168
	mov       eax,dword ptr [ebp+28]
	mov       edx,dword ptr [ebp+16]
	mov       dword ptr [eax],edx
	?debug L 169
	jmp       short @46
	?debug L 171
@40:
	fld       qword ptr [ebp-32]
	mov       ecx,dword ptr [ebp+20]
	fstp      dword ptr [ecx]
	?debug L 172
	fld       qword ptr [ebp-16]
	mov       eax,dword ptr [ebp+24]
	fstp      dword ptr [eax]
	?debug L 173
	mov       edx,dword ptr [ebp+28]
	mov       ecx,dword ptr [ebp+16]
	mov       dword ptr [edx],ecx
	?debug L 174
	jmp       short @46
	?debug L 177
@39:
	mov       eax,dword ptr [ebp+20]
	mov       edx,dword ptr [ebp+16]
	mov       dword ptr [eax],edx
	?debug L 178
	fld       qword ptr [ebp-16]
	mov       ecx,dword ptr [ebp+24]
	fstp      dword ptr [ecx]
	?debug L 179
	fld       qword ptr [ebp-24]
	mov       eax,dword ptr [ebp+28]
	fstp      dword ptr [eax]
	?debug L 180
	?debug L 183
@37:
@46:
@35:
@47:
	mov       esp,ebp
	pop       ebp
	ret 
	align 4        
@34:
	db        0,0,0,0,0,0,192,64,0,0,128,63
	?debug L 0
@@hsv_to_rgb$qfffpft4t4	endp
@hsv_to_rgb$qfffpft4t4	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	69
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch13
	dd	?patch14
	dd	?patch15
	df	@@hsv_to_rgb$qfffpft4t4
	dw	0
	dw	4104
	dw	0
	dw	25
	dw	0
	dw	0
	dw	0
	db	22
	db	64
	db	104
	db	115
	db	118
	db	95
	db	116
	db	111
	db	95
	db	114
	db	103
	db	98
	db	36
	db	113
	db	102
	db	102
	db	102
	db	112
	db	102
	db	116
	db	52
	db	116
	db	52
	dw	18
	dw	512
	dw	8
	dw	0
	dw	64
	dw	0
	dw	26
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	64
	dw	0
	dw	27
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	64
	dw	0
	dw	28
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	20
	dw	0
	dw	1088
	dw	0
	dw	29
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	24
	dw	0
	dw	1088
	dw	0
	dw	30
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	28
	dw	0
	dw	1088
	dw	0
	dw	31
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65504
	dw	65535
	dw	65
	dw	0
	dw	32
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65512
	dw	65535
	dw	65
	dw	0
	dw	33
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65520
	dw	65535
	dw	65
	dw	0
	dw	34
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65528
	dw	65535
	dw	65
	dw	0
	dw	35
	dw	0
	dw	0
	dw	0
?patch13	equ	@47-@@hsv_to_rgb$qfffpft4t4+18
?patch14	equ	0
?patch15	equ	@47-@@hsv_to_rgb$qfffpft4t4
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@my_atof$qpc	segment virtual
@@my_atof$qpc	proc	near
?live16390@0:
	?debug L 187
	push      ebp
	mov       ebp,esp
	add       esp,-8
	?debug L 193
@48:
	push      dword ptr [ebp+8]
	call      @_strlen
	pop       ecx
	mov       dword ptr [ebp-8],eax
	?debug L 194
	xor       eax,eax
	mov       dword ptr [ebp-4],eax
	mov       edx,dword ptr [ebp-4]
	mov       ecx,dword ptr [ebp-8]
	cmp       edx,ecx
	jge       short @50
	?debug L 196
@49:
	mov       eax,dword ptr [ebp-4]
	mov       edx,dword ptr [ebp+8]
	movsx     ecx,byte ptr [edx+eax]
	cmp       ecx,44
	jne       short @51
	mov       eax,dword ptr [ebp-4]
	mov       edx,dword ptr [ebp+8]
	mov       byte ptr [edx+eax],46
@51:
	inc       dword ptr [ebp-4]
	mov       ecx,dword ptr [ebp-4]
	mov       eax,dword ptr [ebp-8]
	cmp       ecx,eax
	jl        short @49
	?debug L 198
@50:
	push      dword ptr [ebp+8]
	call      @_atof
	pop       ecx
	?debug L 199
@54:
@53:
	pop       ecx
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@my_atof$qpc	endp
@my_atof$qpc	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	59
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch16
	dd	?patch17
	dd	?patch18
	df	@@my_atof$qpc
	dw	0
	dw	4106
	dw	0
	dw	36
	dw	0
	dw	0
	dw	0
	db	12
	db	64
	db	109
	db	121
	db	95
	db	97
	db	116
	db	111
	db	102
	db	36
	db	113
	db	112
	db	99
	dw	18
	dw	512
	dw	8
	dw	0
	dw	1136
	dw	0
	dw	37
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65528
	dw	65535
	dw	116
	dw	0
	dw	38
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	116
	dw	0
	dw	39
	dw	0
	dw	0
	dw	0
?patch16	equ	@54-@@my_atof$qpc+4
?patch17	equ	0
?patch18	equ	@54-@@my_atof$qpc
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@esiste_file$qpc	segment virtual
@@esiste_file$qpc	proc	near
?live16391@0:
	?debug L 203
	push      ebp
	mov       ebp,esp
	push      ecx
	?debug L 207
@55:
	push      offset s@+9
	push      dword ptr [ebp+8]
	call      @_fopen
	add       esp,8
	mov       dword ptr [ebp-4],eax
	?debug L 208
	mov       eax,dword ptr [ebp-4]
	test      eax,eax
	jne       short @56
	xor       eax,eax
@60:
	pop       ecx
	pop       ebp
	ret 
	?debug L 210
@56:
	push      dword ptr [ebp-4]
	call      @_fclose
	pop       ecx
	?debug L 211
	mov       al,1
	?debug L 213
@59:
@57:
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@esiste_file$qpc	endp
@esiste_file$qpc	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	63
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch19
	dd	?patch20
	dd	?patch21
	df	@@esiste_file$qpc
	dw	0
	dw	4108
	dw	0
	dw	40
	dw	0
	dw	0
	dw	0
	db	16
	db	64
	db	101
	db	115
	db	105
	db	115
	db	116
	db	101
	db	95
	db	102
	db	105
	db	108
	db	101
	db	36
	db	113
	db	112
	db	99
	dw	18
	dw	512
	dw	8
	dw	0
	dw	1136
	dw	0
	dw	41
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	65535
	dw	65535
	dw	42
	dw	0
	dw	0
	dw	0
	dw	8
	dw	530
	dd	@60-@@esiste_file$qpc
	dw	3
?patch19	equ	@59-@@esiste_file$qpc+3
?patch20	equ	0
?patch21	equ	@59-@@esiste_file$qpc
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@write_string0$qp8std@FILEpc	segment virtual
@@write_string0$qp8std@FILEpc	proc	near
?live16392@0:
	?debug L 217
	push      ebp
	mov       ebp,esp
	push      ecx
	?debug L 221
@61:
	xor       eax,eax
	mov       dword ptr [ebp-4],eax
	jmp       short @63
	?debug L 222
@62:
	push      dword ptr [ebp+8]
	push      1
	push      1
	mov       edx,dword ptr [ebp+12]
	mov       ecx,dword ptr [ebp-4]
	add       edx,ecx
	push      edx
	call      @_fwrite
	add       esp,16
	inc       dword ptr [ebp-4]
@63:
	mov       eax,dword ptr [ebp-4]
	mov       edx,dword ptr [ebp+12]
	mov       cl,byte ptr [edx+eax]
	test      cl,cl
	jne       short @62
	?debug L 225
	push      dword ptr [ebp+8]
	push      1
	push      1
	mov       eax,dword ptr [ebp+12]
	mov       edx,dword ptr [ebp-4]
	add       eax,edx
	push      eax
	call      @_fwrite
	add       esp,16
	?debug L 226
@65:
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@write_string0$qp8std@FILEpc	endp
@write_string0$qp8std@FILEpc	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	75
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch22
	dd	?patch23
	dd	?patch24
	df	@@write_string0$qp8std@FILEpc
	dw	0
	dw	4110
	dw	0
	dw	43
	dw	0
	dw	0
	dw	0
	db	28
	db	64
	db	119
	db	114
	db	105
	db	116
	db	101
	db	95
	db	115
	db	116
	db	114
	db	105
	db	110
	db	103
	db	48
	db	36
	db	113
	db	112
	db	56
	db	115
	db	116
	db	100
	db	64
	db	70
	db	73
	db	76
	db	69
	db	112
	db	99
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	44
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	1136
	dw	0
	dw	45
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	116
	dw	0
	dw	46
	dw	0
	dw	0
	dw	0
?patch22	equ	@65-@@write_string0$qp8std@FILEpc+3
?patch23	equ	0
?patch24	equ	@65-@@write_string0$qp8std@FILEpc
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@read_string0$qp8std@FILEpc	segment virtual
@@read_string0$qp8std@FILEpc	proc	near
?live16393@0:
	?debug L 230
	push      ebp
	mov       ebp,esp
	add       esp,-8
	?debug L 235
@66:
	mov       byte ptr [ebp-5],32
	?debug L 236
	xor       eax,eax
	mov       dword ptr [ebp-4],eax
	jmp       short @68
	?debug L 239
@67:
	push      dword ptr [ebp+8]
	push      1
	push      1
	lea       edx,dword ptr [ebp-5]
	push      edx
	call      @_fread
	add       esp,16
	?debug L 240
	mov       ecx,dword ptr [ebp+12]
	mov       eax,dword ptr [ebp-4]
	mov       dl,byte ptr [ebp-5]
	mov       byte ptr [ecx+eax],dl
	?debug L 241
	inc       dword ptr [ebp-4]
	?debug L 237
@68:
	mov       cl,byte ptr [ebp-5]
	test      cl,cl
	jne       short @67
	?debug L 243
@69:
	pop       ecx
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@read_string0$qp8std@FILEpc	endp
@read_string0$qp8std@FILEpc	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	74
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch25
	dd	?patch26
	dd	?patch27
	df	@@read_string0$qp8std@FILEpc
	dw	0
	dw	4112
	dw	0
	dw	47
	dw	0
	dw	0
	dw	0
	db	27
	db	64
	db	114
	db	101
	db	97
	db	100
	db	95
	db	115
	db	116
	db	114
	db	105
	db	110
	db	103
	db	48
	db	36
	db	113
	db	112
	db	56
	db	115
	db	116
	db	100
	db	64
	db	70
	db	73
	db	76
	db	69
	db	112
	db	99
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	48
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	1136
	dw	0
	dw	49
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65531
	dw	65535
	dw	112
	dw	0
	dw	50
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	116
	dw	0
	dw	51
	dw	0
	dw	0
	dw	0
?patch25	equ	@69-@@read_string0$qp8std@FILEpc+4
?patch26	equ	0
?patch27	equ	@69-@@read_string0$qp8std@FILEpc
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@cpu_has3DNow$qv	segment virtual
@@cpu_has3DNow$qv	proc	near
?live16394@0:
	?debug L 253
	push      ebp
	mov       ebp,esp
@70:
	xor       eax,eax
	?debug L 253
@72:
@71:
	pop       ebp
	ret 
	?debug L 0
@@cpu_has3DNow$qv	endp
@cpu_has3DNow$qv	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	63
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch28
	dd	?patch29
	dd	?patch30
	df	@@cpu_has3DNow$qv
	dw	0
	dw	4114
	dw	0
	dw	52
	dw	0
	dw	0
	dw	0
	db	16
	db	64
	db	99
	db	112
	db	117
	db	95
	db	104
	db	97
	db	115
	db	51
	db	68
	db	78
	db	111
	db	119
	db	36
	db	113
	db	118
?patch28	equ	@72-@@cpu_has3DNow$qv+2
?patch29	equ	0
?patch30	equ	@72-@@cpu_has3DNow$qv
	dw	2
	dw	6
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@cpu_hasMMX$qv	segment virtual
@@cpu_hasMMX$qv	proc	near
?live16395@0:
	?debug L 254
	push      ebp
	mov       ebp,esp
@73:
	xor       eax,eax
	?debug L 254
@75:
@74:
	pop       ebp
	ret 
	?debug L 0
@@cpu_hasMMX$qv	endp
@cpu_hasMMX$qv	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	61
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch31
	dd	?patch32
	dd	?patch33
	df	@@cpu_hasMMX$qv
	dw	0
	dw	4116
	dw	0
	dw	53
	dw	0
	dw	0
	dw	0
	db	14
	db	64
	db	99
	db	112
	db	117
	db	95
	db	104
	db	97
	db	115
	db	77
	db	77
	db	88
	db	36
	db	113
	db	118
?patch31	equ	@75-@@cpu_hasMMX$qv+2
?patch32	equ	0
?patch33	equ	@75-@@cpu_hasMMX$qv
	dw	2
	dw	6
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@cpu_hasSSE$qv	segment virtual
@@cpu_hasSSE$qv	proc	near
?live16396@0:
	?debug L 255
	push      ebp
	mov       ebp,esp
@76:
	xor       eax,eax
	?debug L 255
@78:
@77:
	pop       ebp
	ret 
	?debug L 0
@@cpu_hasSSE$qv	endp
@cpu_hasSSE$qv	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	61
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch34
	dd	?patch35
	dd	?patch36
	df	@@cpu_hasSSE$qv
	dw	0
	dw	4118
	dw	0
	dw	54
	dw	0
	dw	0
	dw	0
	db	14
	db	64
	db	99
	db	112
	db	117
	db	95
	db	104
	db	97
	db	115
	db	83
	db	83
	db	69
	db	36
	db	113
	db	118
?patch34	equ	@78-@@cpu_hasSSE$qv+2
?patch35	equ	0
?patch36	equ	@78-@@cpu_hasSSE$qv
	dw	2
	dw	6
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@cpu_isAthlon$qv	segment virtual
@@cpu_isAthlon$qv	proc	near
?live16397@0:
	?debug L 256
	push      ebp
	mov       ebp,esp
@79:
	xor       eax,eax
	?debug L 256
@81:
@80:
	pop       ebp
	ret 
	?debug L 0
@@cpu_isAthlon$qv	endp
@cpu_isAthlon$qv	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	63
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch37
	dd	?patch38
	dd	?patch39
	df	@@cpu_isAthlon$qv
	dw	0
	dw	4120
	dw	0
	dw	55
	dw	0
	dw	0
	dw	0
	db	16
	db	64
	db	99
	db	112
	db	117
	db	95
	db	105
	db	115
	db	65
	db	116
	db	104
	db	108
	db	111
	db	110
	db	36
	db	113
	db	118
?patch37	equ	@81-@@cpu_isAthlon$qv+2
?patch38	equ	0
?patch39	equ	@81-@@cpu_isAthlon$qv
	dw	2
	dw	6
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@cpu_isK6III$qv	segment virtual
@@cpu_isK6III$qv	proc	near
?live16398@0:
	?debug L 257
	push      ebp
	mov       ebp,esp
@82:
	xor       eax,eax
	?debug L 257
@84:
@83:
	pop       ebp
	ret 
	?debug L 0
@@cpu_isK6III$qv	endp
@cpu_isK6III$qv	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	62
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch40
	dd	?patch41
	dd	?patch42
	df	@@cpu_isK6III$qv
	dw	0
	dw	4122
	dw	0
	dw	56
	dw	0
	dw	0
	dw	0
	db	15
	db	64
	db	99
	db	112
	db	117
	db	95
	db	105
	db	115
	db	75
	db	54
	db	73
	db	73
	db	73
	db	36
	db	113
	db	118
?patch40	equ	@84-@@cpu_isK6III$qv+2
?patch41	equ	0
?patch42	equ	@84-@@cpu_isK6III$qv
	dw	2
	dw	6
$$BSYMS	ends
_DATA	segment dword public use32 'DATA'
s@	label	byte
	;	s@+0:
	db	"AD-Debug",0
	;	s@+9:
	db	"rb",0
	align	4
_DATA	ends
_TEXT	segment dword public use32 'CODE'
_TEXT	ends
	public	_Pi__
@MessageBoxA equ MessageBoxA
 extrn   MessageBoxA:near
@_exit equ _exit
 extrn   _exit:near
 extrn   __ftol:near
@_strlen equ _strlen
 extrn   _strlen:near
@_atof equ _atof
 extrn   _atof:near
@_fopen equ _fopen
 extrn   _fopen:near
@_fclose equ _fclose
 extrn   _fclose:near
@_fwrite equ _fwrite
 extrn   _fwrite:near
@_fread equ _fread
 extrn   _fread:near
 ?debug  C 9F757569642E6C6962
 ?debug  C 9F757569642E6C6962
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	22
	dw	514
	df	_Pi__
	dw	0
	dw	4124
	dw	0
	dw	57
	dw	0
	dw	0
	dw	0
	dw	?patch43
	dw	37
	dw	58
	dw	0
	dw	0
	dw	0
	dw	0
?patch43	equ	12
	dw	?patch44
	dw	38
	dw	363
	dw	59
	dw	0
	dw	60
	dw	0
	dw	61
	dw	0
	dw	62
	dw	0
	dw	63
	dw	0
	dw	64
	dw	0
	dw	65
	dw	0
	dw	66
	dw	0
	dw	67
	dw	0
	dw	68
	dw	0
	dw	69
	dw	0
	dw	70
	dw	0
	dw	71
	dw	0
	dw	72
	dw	0
	dw	73
	dw	0
	dw	74
	dw	0
	dw	75
	dw	0
	dw	76
	dw	0
	dw	77
	dw	0
	dw	78
	dw	0
	dw	79
	dw	0
	dw	80
	dw	0
	dw	81
	dw	0
	dw	82
	dw	0
	dw	83
	dw	0
	dw	84
	dw	0
	dw	85
	dw	0
	dw	86
	dw	0
	dw	87
	dw	0
	dw	88
	dw	0
	dw	89
	dw	0
	dw	90
	dw	0
	dw	91
	dw	0
	dw	92
	dw	0
	dw	93
	dw	0
	dw	94
	dw	0
	dw	95
	dw	0
	dw	96
	dw	0
	dw	97
	dw	0
	dw	98
	dw	0
	dw	99
	dw	0
	dw	100
	dw	0
	dw	101
	dw	0
	dw	102
	dw	0
	dw	103
	dw	0
	dw	104
	dw	0
	dw	105
	dw	0
	dw	106
	dw	0
	dw	107
	dw	0
	dw	108
	dw	0
	dw	109
	dw	0
	dw	110
	dw	0
	dw	111
	dw	0
	dw	112
	dw	0
	dw	113
	dw	0
	dw	114
	dw	0
	dw	115
	dw	0
	dw	116
	dw	0
	dw	117
	dw	0
	dw	118
	dw	0
	dw	119
	dw	0
	dw	120
	dw	0
	dw	121
	dw	0
	dw	122
	dw	0
	dw	123
	dw	0
	dw	124
	dw	0
	dw	125
	dw	0
	dw	126
	dw	0
	dw	127
	dw	0
	dw	128
	dw	0
	dw	129
	dw	0
	dw	130
	dw	0
	dw	131
	dw	0
	dw	132
	dw	0
	dw	133
	dw	0
	dw	134
	dw	0
	dw	135
	dw	0
	dw	136
	dw	0
	dw	137
	dw	0
	dw	138
	dw	0
	dw	139
	dw	0
	dw	140
	dw	0
	dw	141
	dw	0
	dw	142
	dw	0
	dw	143
	dw	0
	dw	144
	dw	0
	dw	145
	dw	0
	dw	146
	dw	0
	dw	147
	dw	0
	dw	148
	dw	0
	dw	149
	dw	0
	dw	150
	dw	0
	dw	151
	dw	0
	dw	152
	dw	0
	dw	153
	dw	0
	dw	154
	dw	0
	dw	155
	dw	0
	dw	156
	dw	0
	dw	157
	dw	0
	dw	158
	dw	0
	dw	159
	dw	0
	dw	160
	dw	0
	dw	161
	dw	0
	dw	162
	dw	0
	dw	163
	dw	0
	dw	164
	dw	0
	dw	165
	dw	0
	dw	166
	dw	0
	dw	167
	dw	0
	dw	168
	dw	0
	dw	169
	dw	0
	dw	170
	dw	0
	dw	171
	dw	0
	dw	172
	dw	0
	dw	173
	dw	0
	dw	174
	dw	0
	dw	175
	dw	0
	dw	176
	dw	0
	dw	177
	dw	0
	dw	178
	dw	0
	dw	179
	dw	0
	dw	180
	dw	0
	dw	181
	dw	0
	dw	182
	dw	0
	dw	183
	dw	0
	dw	184
	dw	0
	dw	185
	dw	0
	dw	186
	dw	0
	dw	187
	dw	0
	dw	188
	dw	0
	dw	189
	dw	0
	dw	190
	dw	0
	dw	191
	dw	0
	dw	192
	dw	0
	dw	193
	dw	0
	dw	194
	dw	0
	dw	195
	dw	0
	dw	196
	dw	0
	dw	197
	dw	0
	dw	198
	dw	0
	dw	199
	dw	0
	dw	200
	dw	0
	dw	201
	dw	0
	dw	202
	dw	0
	dw	203
	dw	0
	dw	204
	dw	0
	dw	205
	dw	0
	dw	206
	dw	0
	dw	207
	dw	0
	dw	208
	dw	0
	dw	209
	dw	0
	dw	210
	dw	0
	dw	211
	dw	0
	dw	212
	dw	0
	dw	213
	dw	0
	dw	214
	dw	0
	dw	215
	dw	0
	dw	216
	dw	0
	dw	217
	dw	0
	dw	218
	dw	0
	dw	219
	dw	0
	dw	220
	dw	0
	dw	221
	dw	0
	dw	222
	dw	0
	dw	223
	dw	0
	dw	224
	dw	0
	dw	225
	dw	0
	dw	226
	dw	0
	dw	227
	dw	0
	dw	228
	dw	0
	dw	229
	dw	0
	dw	230
	dw	0
	dw	231
	dw	0
	dw	232
	dw	0
	dw	233
	dw	0
	dw	234
	dw	0
	dw	235
	dw	0
	dw	236
	dw	0
	dw	237
	dw	0
	dw	238
	dw	0
	dw	239
	dw	0
	dw	240
	dw	0
	dw	241
	dw	0
	dw	242
	dw	0
	dw	243
	dw	0
	dw	244
	dw	0
	dw	245
	dw	0
	dw	246
	dw	0
	dw	247
	dw	0
	dw	248
	dw	0
	dw	249
	dw	0
	dw	250
	dw	0
	dw	251
	dw	0
	dw	252
	dw	0
	dw	253
	dw	0
	dw	254
	dw	0
	dw	255
	dw	0
	dw	256
	dw	0
	dw	257
	dw	0
	dw	258
	dw	0
	dw	259
	dw	0
	dw	260
	dw	0
	dw	261
	dw	0
	dw	262
	dw	0
	dw	263
	dw	0
	dw	264
	dw	0
	dw	265
	dw	0
	dw	266
	dw	0
	dw	267
	dw	0
	dw	268
	dw	0
	dw	269
	dw	0
	dw	270
	dw	0
	dw	271
	dw	0
	dw	272
	dw	0
	dw	273
	dw	0
	dw	274
	dw	0
	dw	275
	dw	0
	dw	276
	dw	0
	dw	277
	dw	0
	dw	278
	dw	0
	dw	279
	dw	0
	dw	280
	dw	0
	dw	281
	dw	0
	dw	282
	dw	0
	dw	283
	dw	0
	dw	284
	dw	0
	dw	285
	dw	0
	dw	286
	dw	0
	dw	287
	dw	0
	dw	288
	dw	0
	dw	289
	dw	0
	dw	290
	dw	0
	dw	291
	dw	0
	dw	292
	dw	0
	dw	293
	dw	0
	dw	294
	dw	0
	dw	295
	dw	0
	dw	296
	dw	0
	dw	297
	dw	0
	dw	298
	dw	0
	dw	299
	dw	0
	dw	300
	dw	0
	dw	301
	dw	0
	dw	302
	dw	0
	dw	303
	dw	0
	dw	304
	dw	0
	dw	305
	dw	0
	dw	306
	dw	0
	dw	307
	dw	0
	dw	308
	dw	0
	dw	309
	dw	0
	dw	310
	dw	0
	dw	311
	dw	0
	dw	312
	dw	0
	dw	313
	dw	0
	dw	314
	dw	0
	dw	315
	dw	0
	dw	316
	dw	0
	dw	317
	dw	0
	dw	318
	dw	0
	dw	319
	dw	0
	dw	320
	dw	0
	dw	321
	dw	0
	dw	322
	dw	0
	dw	323
	dw	0
	dw	324
	dw	0
	dw	325
	dw	0
	dw	326
	dw	0
	dw	327
	dw	0
	dw	328
	dw	0
	dw	329
	dw	0
	dw	330
	dw	0
	dw	331
	dw	0
	dw	332
	dw	0
	dw	333
	dw	0
	dw	334
	dw	0
	dw	335
	dw	0
	dw	336
	dw	0
	dw	337
	dw	0
	dw	338
	dw	0
	dw	339
	dw	0
	dw	340
	dw	0
	dw	341
	dw	0
	dw	342
	dw	0
	dw	343
	dw	0
	dw	344
	dw	0
	dw	345
	dw	0
	dw	346
	dw	0
	dw	347
	dw	0
	dw	348
	dw	0
	dw	349
	dw	0
	dw	350
	dw	0
	dw	351
	dw	0
	dw	352
	dw	0
	dw	353
	dw	0
	dw	354
	dw	0
	dw	355
	dw	0
	dw	356
	dw	0
	dw	357
	dw	0
	dw	358
	dw	0
	dw	359
	dw	0
	dw	360
	dw	0
	dw	361
	dw	0
	dw	362
	dw	0
	dw	363
	dw	0
	dw	364
	dw	0
	dw	365
	dw	0
	dw	366
	dw	0
	dw	367
	dw	0
	dw	368
	dw	0
	dw	369
	dw	0
	dw	370
	dw	0
	dw	371
	dw	0
	dw	372
	dw	0
	dw	373
	dw	0
	dw	374
	dw	0
	dw	375
	dw	0
	dw	376
	dw	0
	dw	377
	dw	0
	dw	378
	dw	0
	dw	379
	dw	0
	dw	380
	dw	0
	dw	381
	dw	0
	dw	382
	dw	0
	dw	383
	dw	0
	dw	384
	dw	0
	dw	385
	dw	0
	dw	386
	dw	0
	dw	387
	dw	0
	dw	388
	dw	0
	dw	389
	dw	0
	dw	390
	dw	0
	dw	391
	dw	0
	dw	392
	dw	0
	dw	393
	dw	0
	dw	394
	dw	0
	dw	395
	dw	0
	dw	396
	dw	0
	dw	397
	dw	0
	dw	398
	dw	0
	dw	399
	dw	0
	dw	400
	dw	0
	dw	401
	dw	0
	dw	402
	dw	0
	dw	403
	dw	0
	dw	404
	dw	0
	dw	405
	dw	0
	dw	406
	dw	0
	dw	407
	dw	0
	dw	408
	dw	0
	dw	409
	dw	0
	dw	410
	dw	0
	dw	411
	dw	0
	dw	412
	dw	0
	dw	413
	dw	0
	dw	414
	dw	0
	dw	415
	dw	0
	dw	416
	dw	0
	dw	417
	dw	0
	dw	418
	dw	0
	dw	419
	dw	0
	dw	420
	dw	0
	dw	421
	dw	0
?patch44	equ	1456
	dw	?patch45
	dw	1
	db	5
	db	1
	db	0
	db	24
	db	11
	db	66
	db	67
	db	67
	db	51
	db	50
	db	32
	db	53
	db	46
	db	53
	db	46
	db	49
?patch45	equ	18
$$BSYMS	ends
$$BTYPES	segment byte public use32 'DEBTYP'
	db        2,0,0,0,14,0,8,0,3,0,0,0,0,0,2,0
	db        1,16,0,0,12,0,1,2,2,0,3,4,0,0,112,4
	db        0,0,14,0,8,0,3,0,0,0,0,0,3,0,3,16
	db        0,0,16,0,1,2,3,0,3,4,0,0,117,0,0,0
	db        117,0,0,0,14,0,8,0,3,0,0,0,0,0,3,0
	db        5,16,0,0,16,0,1,2,3,0,3,4,0,0,3,4
	db        0,0,116,0,0,0,14,0,8,0,3,0,0,0,0,0
	db        6,0,7,16,0,0,28,0,1,2,6,0,64,0,0,0
	db        64,0,0,0,64,0,0,0,64,4,0,0,64,4,0,0
	db        64,4,0,0,14,0,8,0,3,0,0,0,0,0,6,0
	db        9,16,0,0,28,0,1,2,6,0,64,0,0,0,64,0
	db        0,0,64,0,0,0,64,4,0,0,64,4,0,0,64,4
	db        0,0,14,0,8,0,64,0,0,0,0,0,1,0,11,16
	db        0,0,8,0,1,2,1,0,112,4,0,0,14,0,8,0
	db        112,0,0,0,0,0,1,0,13,16,0,0,8,0,1,2
	db        1,0,112,4,0,0,14,0,8,0,3,0,0,0,0,0
	db        2,0,15,16,0,0,12,0,1,2,2,0,255,255,255,255
	db        112,4,0,0,14,0,8,0,3,0,0,0,0,0,2,0
	db        17,16,0,0,12,0,1,2,2,0,255,255,255,255,112,4
	db        0,0,14,0,8,0,116,0,0,0,0,0,0,0,19,16
	db        0,0,4,0,1,2,0,0,14,0,8,0,116,0,0,0
	db        0,0,0,0,21,16,0,0,4,0,1,2,0,0,14,0
	db        8,0,116,0,0,0,0,0,0,0,23,16,0,0,4,0
	db        1,2,0,0,14,0,8,0,116,0,0,0,0,0,0,0
	db        25,16,0,0,4,0,1,2,0,0,14,0,8,0,116,0
	db        0,0,0,0,0,0,27,16,0,0,4,0,1,2,0,0
	db        8,0,1,0,1,0,64,0,0,0,14,0,8,0,116,0
	db        0,0,7,0,4,0,32,16,0,0,8,0,2,0,10,0
	db        31,16,0,0,8,0,1,0,1,0,112,0,0,0,20,0
	db        1,2,4,0,3,4,0,0,30,16,0,0,30,16,0,0
	db        117,0,0,0,14,0,8,0,3,0,0,0,0,0,1,0
	db        34,16,0,0,8,0,1,2,1,0,116,0,0,0,14,0
	db        8,0,116,0,0,0,2,0,0,0,36,16,0,0,4,0
	db        1,2,0,0,14,0,8,0,117,0,0,0,0,0,1,0
	db        38,16,0,0,8,0,1,2,1,0,30,16,0,0,14,0
	db        8,0,65,0,0,0,0,0,1,0,40,16,0,0,8,0
	db        1,2,1,0,30,16,0,0,14,0,8,0,255,255,255,255
	db        0,0,2,0,42,16,0,0,12,0,1,2,2,0,30,16
	db        0,0,30,16,0,0,14,0,8,0,116,0,0,0,0,0
	db        1,0,44,16,0,0,8,0,1,2,1,0,255,255,255,255
	db        14,0,8,0,117,0,0,0,0,0,4,0,48,16,0,0
	db        8,0,2,0,10,0,47,16,0,0,8,0,1,0,1,0
	db        3,0,0,0,20,0,1,2,4,0,46,16,0,0,117,0
	db        0,0,117,0,0,0,255,255,255,255,14,0,8,0,117,0
	db        0,0,0,0,4,0,50,16,0,0,20,0,1,2,4,0
	db        3,4,0,0,117,0,0,0,117,0,0,0,255,255,255,255
$$BTYPES	ends
$$BNAMES	segment byte public use32 'DEBNAM'
	db	11,'debug_error'
	db	4,'hWnd'
	db	9,'messaggio'
	db	7,'memsetd'
	db	4,'dest'
	db	3,'val'
	db	6,'dwords'
	db	7,'memcpyd'
	db	4,'dest'
	db	4,'sorg'
	db	6,'dwords'
	db	10,'rgb_to_hsv'
	db	1,'r'
	db	1,'g'
	db	1,'b'
	db	1,'h'
	db	1,'s'
	db	1,'v'
	db	5,'delta'
	db	3,'max'
	db	3,'min'
	db	2,'vv'
	db	2,'ss'
	db	2,'hh'
	db	10,'hsv_to_rgb'
	db	1,'h'
	db	1,'s'
	db	1,'v'
	db	1,'r'
	db	1,'g'
	db	1,'b'
	db	1,'t'
	db	1,'q'
	db	1,'p'
	db	1,'f'
	db	7,'my_atof'
	db	1,'s'
	db	1,'l'
	db	1,'k'
	db	11,'esiste_file'
	db	4,'nome'
	db	1,'f'
	db	13,'write_string0'
	db	1,'f'
	db	2,'st'
	db	1,'k'
	db	12,'read_string0'
	db	1,'f'
	db	2,'st'
	db	2,'ch'
	db	1,'k'
	db	12,'cpu_has3DNow'
	db	10,'cpu_hasMMX'
	db	10,'cpu_hasSSE'
	db	12,'cpu_isAthlon'
	db	11,'cpu_isK6III'
	db	4,'Pi__'
	db	3,'std'
	db	6,'unlink'
	db	7,'tempnam'
	db	5,'sopen'
	db	5,'rmtmp'
	db	4,'putw'
	db	4,'getw'
	db	8,'fputchar'
	db	8,'flushall'
	db	6,'fileno'
	db	8,'fgetchar'
	db	6,'fdopen'
	db	9,'fcloseall'
	db	11,'_vsnwprintf'
	db	10,'_vsnprintf'
	db	10,'_snwprintf'
	db	9,'_snprintf'
	db	6,'wscanf'
	db	7,'wprintf'
	db	7,'vwscanf'
	db	8,'vwprintf'
	db	8,'vswscanf'
	db	9,'vswprintf'
	db	7,'vsscanf'
	db	8,'vsprintf'
	db	10,'vsnwprintf'
	db	9,'vsnprintf'
	db	6,'vscanf'
	db	7,'vprintf'
	db	8,'vfwscanf'
	db	9,'vfwprintf'
	db	7,'vfscanf'
	db	8,'vfprintf'
	db	7,'ungetwc'
	db	6,'ungetc'
	db	6,'tmpnam'
	db	7,'tmpfile'
	db	7,'swscanf'
	db	8,'swprintf'
	db	6,'sscanf'
	db	7,'sprintf'
	db	9,'snwprintf'
	db	8,'snprintf'
	db	7,'setvbuf'
	db	6,'setbuf'
	db	5,'scanf'
	db	6,'rewind'
	db	6,'rename'
	db	6,'remove'
	db	8,'putwchar'
	db	5,'putwc'
	db	4,'puts'
	db	7,'putchar'
	db	4,'putc'
	db	6,'printf'
	db	8,'getwchar'
	db	5,'getwc'
	db	4,'gets'
	db	7,'getchar'
	db	4,'getc'
	db	7,'fwscanf'
	db	6,'fwrite'
	db	8,'fwprintf'
	db	5,'ftell'
	db	7,'fsetpos'
	db	5,'fseek'
	db	6,'fscanf'
	db	7,'freopen'
	db	5,'fread'
	db	6,'fputws'
	db	6,'fputwc'
	db	5,'fputs'
	db	5,'fputc'
	db	7,'fprintf'
	db	5,'fopen'
	db	6,'fgetws'
	db	6,'fgetwc'
	db	5,'fgets'
	db	7,'fgetpos'
	db	5,'fgetc'
	db	6,'fflush'
	db	6,'ferror'
	db	4,'feof'
	db	6,'fclose'
	db	8,'clearerr'
	db	8,'_wunlink'
	db	8,'_wtmpnam'
	db	9,'_wtempnam'
	db	7,'_wsopen'
	db	8,'_wrename'
	db	8,'_wremove'
	db	7,'_wpopen'
	db	8,'_wfsopen'
	db	9,'_wfreopen'
	db	7,'_wfopen'
	db	8,'_wfdopen'
	db	7,'_unlink'
	db	8,'_tempnam'
	db	6,'_sopen'
	db	6,'_rmtmp'
	db	6,'_putws'
	db	5,'_putw'
	db	6,'_popen'
	db	7,'_pclose'
	db	6,'_getws'
	db	5,'_getw'
	db	7,'_fsopen'
	db	10,'_fputwchar'
	db	7,'_fputwc'
	db	9,'_fputchar'
	db	6,'_fputc'
	db	9,'_flushall'
	db	7,'_fileno'
	db	10,'_fgetwchar'
	db	7,'_fgetwc'
	db	9,'_fgetchar'
	db	6,'_fgetc'
	db	7,'_fdopen'
	db	10,'_fcloseall'
	db	11,'__getStream'
	db	5,'_rotr'
	db	5,'_rotl'
	db	6,'_crotr'
	db	6,'_lrotr'
	db	6,'_lrotl'
	db	6,'_crotl'
	db	3,'abs'
	db	6,'wctomb'
	db	7,'wcstoul'
	db	8,'wcstombs'
	db	6,'wcstol'
	db	6,'wcstod'
	db	5,'ultoa'
	db	4,'time'
	db	6,'system'
	db	4,'swab'
	db	7,'strtoul'
	db	6,'strtol'
	db	6,'strtod'
	db	5,'srand'
	db	7,'realloc'
	db	4,'rand'
	db	6,'putenv'
	db	6,'perror'
	db	6,'mbtowc'
	db	8,'mbstowcs'
	db	5,'mblen'
	db	13,'_seterrormode'
	db	26,'@std@%max$T1%$qrxT1t1$rxT1'
	db	26,'@std@%min$T1%$qrxT1t1$rxT1'
	db	17,'@std@randomize$qv'
	db	14,'@std@random$qi'
	db	4,'ltoa'
	db	4,'gcvt'
	db	4,'fcvt'
	db	4,'ecvt'
	db	6,'malloc'
	db	4,'ldiv'
	db	4,'labs'
	db	4,'itoa'
	db	6,'getenv'
	db	4,'free'
	db	4,'exit'
	db	3,'div'
	db	6,'calloc'
	db	4,'atol'
	db	4,'atoi'
	db	4,'atof'
	db	6,'atexit'
	db	5,'abort'
	db	6,'_wtold'
	db	5,'_wtol'
	db	5,'_wtoi'
	db	5,'_wtof'
	db	8,'_wsystem'
	db	11,'_wsplitpath'
	db	11,'_wsearchstr'
	db	11,'_wsearchenv'
	db	8,'_wputenv'
	db	8,'_wperror'
	db	10,'_wmakepath'
	db	8,'_wgetenv'
	db	10,'_wfullpath'
	db	8,'_wcstold'
	db	8,'_strtold'
	db	10,'_splitpath'
	db	10,'_searchstr'
	db	10,'_searchenv'
	db	9,'_makepath'
	db	7,'_wtoi64'
	db	8,'_ui64tow'
	db	8,'_ui64toa'
	db	7,'_i64tow'
	db	7,'_i64toa'
	db	7,'_atoi64'
	db	5,'_ltow'
	db	5,'_ltoa'
	db	6,'_lrand'
	db	5,'_itow'
	db	6,'_ultow'
	db	5,'_gcvt'
	db	5,'_fcvt'
	db	9,'_fullpath'
	db	5,'_exit'
	db	6,'_atold'
	db	5,'_ecvt'
	db	7,'__errno'
	db	10,'__doserrno'
	db	5,'qsort'
	db	7,'lsearch'
	db	5,'lfind'
	db	7,'bsearch'
	db	7,'wcsxfrm'
	db	6,'wcstok'
	db	6,'wcsstr'
	db	6,'wcsspn'
	db	7,'wcsrchr'
	db	7,'wcspcpy'
	db	7,'wcspbrk'
	db	7,'wcsncpy'
	db	7,'wcsncmp'
	db	7,'wcsncat'
	db	6,'wcslen'
	db	7,'wcscspn'
	db	6,'wcscpy'
	db	7,'wcscoll'
	db	6,'wcscmp'
	db	6,'wcschr'
	db	6,'wcscat'
	db	6,'strtok'
	db	18,'@std@strstr$qpxct1'
	db	18,'@std@strstr$qpcpxc'
	db	6,'strspn'
	db	19,'@std@strpbrk$qpxct1'
	db	19,'@std@strpbrk$qpcpxc'
	db	8,'strerror'
	db	7,'strcspn'
	db	18,'@std@strrchr$qpxci'
	db	17,'@std@strrchr$qpci'
	db	7,'strncpy'
	db	7,'strncmp'
	db	7,'strncat'
	db	6,'strlen'
	db	6,'strcpy'
	db	6,'strcmp'
	db	17,'@std@strchr$qpxci'
	db	16,'@std@strchr$qpci'
	db	6,'strcat'
	db	6,'strset'
	db	6,'stpcpy'
	db	7,'strnset'
	db	7,'_wcsset'
	db	7,'_wcsrev'
	db	8,'_wcspcpy'
	db	8,'_wcsnset'
	db	10,'_wcsnicoll'
	db	9,'_wcsnicmp'
	db	9,'_wcsncoll'
	db	9,'_wcsicoll'
	db	8,'_wcsicmp'
	db	7,'_wcsdup'
	db	10,'_strnicoll'
	db	9,'_strncoll'
	db	9,'_stricoll'
	db	9,'_strerror'
	db	9,'_lwcsxfrm'
	db	8,'_lwcsupr'
	db	11,'_lwcsnicoll'
	db	10,'_lwcsncoll'
	db	8,'_lwcslwr'
	db	10,'_lwcsicoll'
	db	9,'_lwcscoll'
	db	9,'_lstrxfrm'
	db	11,'_lstrnicoll'
	db	10,'_lstrncoll'
	db	10,'_lstricoll'
	db	9,'_lstrcoll'
	db	6,'strrev'
	db	8,'strnicmp'
	db	7,'stricmp'
	db	6,'strdup'
	db	7,'_stpcpy'
	db	8,'_lstrupr'
	db	8,'_lstrlwr'
	db	12,'_lwsetlocale'
	db	11,'_wsetlocale'
	db	12,'_llocaleconv'
	db	10,'localeconv'
	db	11,'_lsetlocale'
	db	10,'_ltowlower'
	db	10,'_ltowupper'
	db	8,'towupper'
	db	8,'towlower'
	db	9,'_ltoupper'
	db	7,'toupper'
	db	9,'_ltolower'
	db	7,'tolower'
	db	8,'iswascii'
	db	9,'iswxdigit'
	db	8,'iswupper'
	db	8,'iswspace'
	db	8,'iswpunct'
	db	8,'iswprint'
	db	8,'iswlower'
	db	8,'iswgraph'
	db	8,'iswdigit'
	db	8,'iswcntrl'
	db	8,'iswalpha'
	db	8,'iswalnum'
	db	7,'isascii'
	db	8,'isxdigit'
	db	7,'isupper'
	db	7,'isspace'
	db	7,'ispunct'
	db	7,'isprint'
	db	7,'islower'
	db	7,'isgraph'
	db	7,'isdigit'
	db	7,'iscntrl'
	db	7,'isalpha'
	db	7,'isalnum'
	db	6,'memset'
	db	6,'memcpy'
	db	6,'memcmp'
	db	19,'@std@memchr$qpxviui'
	db	18,'@std@memchr$qpviui'
	db	7,'memmove'
	db	7,'memicmp'
	db	7,'memccpy'
	db	8,'_wmemset'
	db	8,'_wmemcpy'
	db	21,'@std@_wmemchr$qpxviui'
	db	20,'@std@_wmemchr$qpviui'
	db	10,'__threadid'
	db	14,'@std@ptrdiff_t'
	db	11,'@std@size_t'
	db	11,'@std@wint_t'
	db	13,'@std@wctype_t'
	db	12,'@std@va_list'
	db	9,'_chartype'
	db	6,'_lower'
	db	6,'_upper'
	db	5,'lconv'
	db	5,'_argc'
	db	5,'_argv'
	db	12,'__mb_cur_max'
	db	16,'_cmdline_escapes'
	db	8,'_environ'
	db	9,'_fileinfo'
	db	6,'_fmode'
	db	8,'_osmajor'
	db	8,'_osminor'
	db	7,'_osmode'
	db	10,'_osversion'
	db	12,'_sys_errlist'
	db	9,'_sys_nerr'
	db	9,'_wenviron'
	db	13,'@std@atexit_t'
	db	10,'@std@div_t'
	db	11,'@std@ldiv_t'
	db	9,'@std@FILE'
	db	18,'@std@_floatconvert'
	db	8,'_streams'
	db	11,'@std@fpos_t'
$$BNAMES	ends
	?debug	D "..\..\..\LIB_AUX\INCLUDE\types.h" 11102 34051
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\_nfile.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\stdio.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\imm.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\mcx.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\winsvc.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\commdlg.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\oleauto.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\propidl.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\oaidl.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\msxml.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\servprov.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\oleidl.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\urlmon.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\cguid.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\objidl.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\unknwn.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\search.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\stdlib.h" 10503 10272
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\objbase.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\ole2.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\prsht.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\winspool.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\winsmcrd.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\winioctl.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\basetsd.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\rpcnsip.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\rpcndr.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\wtypes.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\winscard.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\winefs.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\wincrypt.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\qos.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\winsock2.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\winperf.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\shellapi.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\rpcasync.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\rpcnterr.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\rpcnsi.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\rpcdcep.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\rpcdce.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\rpc.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\nb30.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\mmsystem.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\lzexpand.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\dlgs.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\ddeml.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\dde.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\cderr.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\winnetwk.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\winreg.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\winver.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\wincon.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\winnls.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\tvout.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\winuser.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\pshpack1.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\wingdi.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\winerror.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\winbase.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\pshpack8.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\pshpack2.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\poppack.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\pshpack4.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\_loc.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\locale.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\_str.h" 10503 10272
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\string.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\guiddef.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\DIRECTX8SDK\INCLUDE\basetsd.h" 11080 27560
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\mbctype.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\ctype.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\winnt.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\windef.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\stdarg.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\excpt.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\_null.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\_defs.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\_stddef.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\mem.h" 10503 10272
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\windows.h" 10303 10240
	?debug	D "D:\redshift\Demo_sw\DemoSystem\UTILITY.H" 11321 36359
	?debug	D "D:\redshift\Demo_sw\DemoSystem\utility.cpp" 11321 36353
 ?debug  C FB062A633A5C50524F4752417E315C626F726C616E645C434255494C447E315C6C69625C76636C35302E23304313E90F00F37A872C
 ?debug  C FB0506232A46494C45
	end
