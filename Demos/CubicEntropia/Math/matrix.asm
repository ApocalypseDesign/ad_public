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
	?debug	S "D:\redshift\Demo_sw\Math\matrix.cpp"
	?debug	T "D:\redshift\Demo_sw\Math\matrix.cpp"
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
_BSS	segment dword public use32 'BSS'
	align	4
_mat_mul	label	dword
	db	4	dup(?)
	align	4
_mat_mulvect	label	dword
	db	4	dup(?)
_BSS	ends
_TEXT	segment dword public use32 'CODE'
@mat_set$qp9AD_Matrixffffffffffffffff	segment virtual
@@mat_set$qp9AD_Matrixffffffffffffffff	proc	near
?live16385@0:
	?debug L 29
	push      ebp
	mov       ebp,esp
	?debug L 35
@1:
	mov       eax,dword ptr [ebp+12]
	mov       edx,dword ptr [ebp+8]
	mov       dword ptr [edx],eax
	mov       ecx,dword ptr [ebp+16]
	mov       eax,dword ptr [ebp+8]
	mov       dword ptr [eax+4],ecx
	mov       edx,dword ptr [ebp+20]
	mov       ecx,dword ptr [ebp+8]
	mov       dword ptr [ecx+8],edx
	mov       eax,dword ptr [ebp+24]
	mov       edx,dword ptr [ebp+8]
	mov       dword ptr [edx+12],eax
	?debug L 36
	mov       ecx,dword ptr [ebp+28]
	mov       eax,dword ptr [ebp+8]
	mov       dword ptr [eax+16],ecx
	mov       edx,dword ptr [ebp+32]
	mov       ecx,dword ptr [ebp+8]
	mov       dword ptr [ecx+20],edx
	mov       eax,dword ptr [ebp+36]
	mov       edx,dword ptr [ebp+8]
	mov       dword ptr [edx+24],eax
	mov       ecx,dword ptr [ebp+40]
	mov       eax,dword ptr [ebp+8]
	mov       dword ptr [eax+28],ecx
	?debug L 37
	mov       edx,dword ptr [ebp+44]
	mov       ecx,dword ptr [ebp+8]
	mov       dword ptr [ecx+32],edx
	mov       eax,dword ptr [ebp+48]
	mov       edx,dword ptr [ebp+8]
	mov       dword ptr [edx+36],eax
	mov       ecx,dword ptr [ebp+52]
	mov       eax,dword ptr [ebp+8]
	mov       dword ptr [eax+40],ecx
	mov       edx,dword ptr [ebp+56]
	mov       ecx,dword ptr [ebp+8]
	mov       dword ptr [ecx+44],edx
	?debug L 38
	mov       eax,dword ptr [ebp+60]
	mov       edx,dword ptr [ebp+8]
	mov       dword ptr [edx+48],eax
	mov       ecx,dword ptr [ebp+64]
	mov       eax,dword ptr [ebp+8]
	mov       dword ptr [eax+52],ecx
	mov       edx,dword ptr [ebp+68]
	mov       ecx,dword ptr [ebp+8]
	mov       dword ptr [ecx+56],edx
	mov       eax,dword ptr [ebp+72]
	mov       edx,dword ptr [ebp+8]
	mov       dword ptr [edx+60],eax
	?debug L 39
@2:
	pop       ebp
	ret 
	?debug L 0
@@mat_set$qp9AD_Matrixffffffffffffffff	endp
@mat_set$qp9AD_Matrixffffffffffffffff	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	db	2
	db	0
	db	0
	db	0
	dw	84
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
	df	@@mat_set$qp9AD_Matrixffffffffffffffff
	dw	0
	dw	4096
	dw	0
	dw	1
	dw	0
	dw	0
	dw	0
	db	37
	db	64
	db	109
	db	97
	db	116
	db	95
	db	115
	db	101
	db	116
	db	36
	db	113
	db	112
	db	57
	db	65
	db	68
	db	95
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	db	102
	db	102
	db	102
	db	102
	db	102
	db	102
	db	102
	db	102
	db	102
	db	102
	db	102
	db	102
	db	102
	db	102
	db	102
	db	102
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	2
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	64
	dw	0
	dw	3
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	64
	dw	0
	dw	4
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	20
	dw	0
	dw	64
	dw	0
	dw	5
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	24
	dw	0
	dw	64
	dw	0
	dw	6
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	28
	dw	0
	dw	64
	dw	0
	dw	7
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	32
	dw	0
	dw	64
	dw	0
	dw	8
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	36
	dw	0
	dw	64
	dw	0
	dw	9
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	40
	dw	0
	dw	64
	dw	0
	dw	10
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	44
	dw	0
	dw	64
	dw	0
	dw	11
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	48
	dw	0
	dw	64
	dw	0
	dw	12
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	52
	dw	0
	dw	64
	dw	0
	dw	13
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	56
	dw	0
	dw	64
	dw	0
	dw	14
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	60
	dw	0
	dw	64
	dw	0
	dw	15
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	64
	dw	0
	dw	64
	dw	0
	dw	16
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	68
	dw	0
	dw	64
	dw	0
	dw	17
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	72
	dw	0
	dw	64
	dw	0
	dw	18
	dw	0
	dw	0
	dw	0
?patch1	equ	@2-@@mat_set$qp9AD_Matrixffffffffffffffff+2
?patch2	equ	0
?patch3	equ	@2-@@mat_set$qp9AD_Matrixffffffffffffffff
	dw	2
	dw	6
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@mat_copy$qp9AD_Matrixt1	segment virtual
@@mat_copy$qp9AD_Matrixt1	proc	near
?live16386@0:
	?debug L 43
	push      ebp
	mov       ebp,esp
	push      ecx
	push      ebx
	?debug L 47
@3:
	xor       eax,eax
	mov       dword ptr [ebp-4],eax
	?debug L 49
@4:
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+12]
	mov       eax,dword ptr [ebp-4]
	shl       eax,4
	mov       ebx,dword ptr [ebp+8]
	mov       eax,dword ptr [ebx+eax]
	mov       dword ptr [ecx+edx],eax
	?debug L 50
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+12]
	mov       eax,dword ptr [ebp-4]
	shl       eax,4
	mov       ebx,dword ptr [ebp+8]
	mov       eax,dword ptr [ebx+eax+4]
	mov       dword ptr [ecx+edx+4],eax
	?debug L 51
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+12]
	mov       eax,dword ptr [ebp-4]
	shl       eax,4
	mov       ebx,dword ptr [ebp+8]
	mov       eax,dword ptr [ebx+eax+8]
	mov       dword ptr [ecx+edx+8],eax
	?debug L 52
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+12]
	mov       eax,dword ptr [ebp-4]
	shl       eax,4
	mov       ebx,dword ptr [ebp+8]
	mov       eax,dword ptr [ebx+eax+12]
	mov       dword ptr [ecx+edx+12],eax
	inc       dword ptr [ebp-4]
	mov       edx,dword ptr [ebp-4]
	cmp       edx,4
	jl        short @4
	?debug L 54
@7:
	pop       ebx
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@mat_copy$qp9AD_Matrixt1	endp
@mat_copy$qp9AD_Matrixt1	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	71
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
	df	@@mat_copy$qp9AD_Matrixt1
	dw	0
	dw	4098
	dw	0
	dw	19
	dw	0
	dw	0
	dw	0
	db	24
	db	64
	db	109
	db	97
	db	116
	db	95
	db	99
	db	111
	db	112
	db	121
	db	36
	db	113
	db	112
	db	57
	db	65
	db	68
	db	95
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	db	116
	db	49
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	20
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65535
	dw	65535
	dw	21
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	116
	dw	0
	dw	22
	dw	0
	dw	0
	dw	0
?patch4	equ	@7-@@mat_copy$qp9AD_Matrixt1+4
?patch5	equ	0
?patch6	equ	@7-@@mat_copy$qp9AD_Matrixt1
	dw	2
	dw	6
	dw	8
	dw	531
	dw	1
	dw	65528
	dw	65535
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@mat_swap_3DS$qp9AD_Matrix	segment virtual
@@mat_swap_3DS$qp9AD_Matrix	proc	near
?live16387@0:
	?debug L 58
	push      ebp
	mov       ebp,esp
	add       esp,-12
	?debug L 63
@8:
	xor       eax,eax
	mov       dword ptr [ebp-8],eax
	?debug L 65
@9:
	mov       edx,dword ptr [ebp-8]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	mov       eax,dword ptr [ecx+edx+4]
	mov       dword ptr [ebp-4],eax
	?debug L 66
	mov       edx,dword ptr [ebp-8]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	mov       eax,dword ptr [ecx+edx+8]
	mov       edx,dword ptr [ebp-8]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	mov       dword ptr [ecx+edx+4],eax
	?debug L 67
	mov       eax,dword ptr [ebp-8]
	shl       eax,4
	mov       edx,dword ptr [ebp+8]
	mov       ecx,dword ptr [ebp-4]
	mov       dword ptr [edx+eax+8],ecx
	inc       dword ptr [ebp-8]
	mov       eax,dword ptr [ebp-8]
	cmp       eax,3
	jl        short @9
	?debug L 70
	xor       edx,edx
	mov       dword ptr [ebp-12],edx
	?debug L 72
@12:
	mov       ecx,dword ptr [ebp-12]
	mov       eax,dword ptr [ebp+8]
	mov       edx,dword ptr [eax+4*ecx+16]
	mov       dword ptr [ebp-4],edx
	?debug L 73
	mov       ecx,dword ptr [ebp-12]
	mov       eax,dword ptr [ebp+8]
	mov       edx,dword ptr [eax+4*ecx+32]
	mov       ecx,dword ptr [ebp-12]
	mov       eax,dword ptr [ebp+8]
	mov       dword ptr [eax+4*ecx+16],edx
	?debug L 74
	mov       edx,dword ptr [ebp-12]
	mov       ecx,dword ptr [ebp+8]
	mov       eax,dword ptr [ebp-4]
	mov       dword ptr [ecx+4*edx+32],eax
	inc       dword ptr [ebp-12]
	mov       edx,dword ptr [ebp-12]
	cmp       edx,3
	jl        short @12
	?debug L 76
@15:
	mov       esp,ebp
	pop       ebp
	ret 
	?debug L 0
@@mat_swap_3DS$qp9AD_Matrix	endp
@mat_swap_3DS$qp9AD_Matrix	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	73
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
	df	@@mat_swap_3DS$qp9AD_Matrix
	dw	0
	dw	4100
	dw	0
	dw	23
	dw	0
	dw	0
	dw	0
	db	26
	db	64
	db	109
	db	97
	db	116
	db	95
	db	115
	db	119
	db	97
	db	112
	db	95
	db	51
	db	68
	db	83
	db	36
	db	113
	db	112
	db	57
	db	65
	db	68
	db	95
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	24
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65524
	dw	65535
	dw	116
	dw	0
	dw	25
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65528
	dw	65535
	dw	116
	dw	0
	dw	26
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	64
	dw	0
	dw	27
	dw	0
	dw	0
	dw	0
?patch7	equ	@15-@@mat_swap_3DS$qp9AD_Matrix+4
?patch8	equ	0
?patch9	equ	@15-@@mat_swap_3DS$qp9AD_Matrix
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@mat_print_to_file$qp9AD_Matrixp8std@FILE	segment virtual
@@mat_print_to_file$qp9AD_Matrixp8std@FILE	proc	near
?live16388@0:
	?debug L 80
	push      ebp
	mov       ebp,esp
	?debug L 82
@16:
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax+12]
	add       esp,-8
	fstp      qword ptr [esp]
	mov       edx,dword ptr [ebp+8]
	fld       dword ptr [edx+8]
	add       esp,-8
	fstp      qword ptr [esp]
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+4]
	add       esp,-8
	fstp      qword ptr [esp]
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax]
	add       esp,-8
	fstp      qword ptr [esp]
	push      offset s@
	push      dword ptr [ebp+12]
	call      @_fprintf
	add       esp,40
	?debug L 83
	mov       edx,dword ptr [ebp+8]
	fld       dword ptr [edx+28]
	add       esp,-8
	fstp      qword ptr [esp]
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+24]
	add       esp,-8
	fstp      qword ptr [esp]
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax+20]
	add       esp,-8
	fstp      qword ptr [esp]
	mov       edx,dword ptr [ebp+8]
	fld       dword ptr [edx+16]
	add       esp,-8
	fstp      qword ptr [esp]
	push      offset s@+42
	push      dword ptr [ebp+12]
	call      @_fprintf
	add       esp,40
	?debug L 84
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+44]
	add       esp,-8
	fstp      qword ptr [esp]
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax+40]
	add       esp,-8
	fstp      qword ptr [esp]
	mov       edx,dword ptr [ebp+8]
	fld       dword ptr [edx+36]
	add       esp,-8
	fstp      qword ptr [esp]
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+32]
	add       esp,-8
	fstp      qword ptr [esp]
	push      offset s@+84
	push      dword ptr [ebp+12]
	call      @_fprintf
	add       esp,40
	?debug L 85
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax+60]
	add       esp,-8
	fstp      qword ptr [esp]
	mov       edx,dword ptr [ebp+8]
	fld       dword ptr [edx+56]
	add       esp,-8
	fstp      qword ptr [esp]
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+52]
	add       esp,-8
	fstp      qword ptr [esp]
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax+48]
	add       esp,-8
	fstp      qword ptr [esp]
	push      offset s@+126
	push      dword ptr [ebp+12]
	call      @_fprintf
	add       esp,40
	?debug L 86
@17:
	pop       ebp
	ret 
	?debug L 0
@@mat_print_to_file$qp9AD_Matrixp8std@FILE	endp
@mat_print_to_file$qp9AD_Matrixp8std@FILE	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	88
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
	df	@@mat_print_to_file$qp9AD_Matrixp8std@FILE
	dw	0
	dw	4102
	dw	0
	dw	28
	dw	0
	dw	0
	dw	0
	db	41
	db	64
	db	109
	db	97
	db	116
	db	95
	db	112
	db	114
	db	105
	db	110
	db	116
	db	95
	db	116
	db	111
	db	95
	db	102
	db	105
	db	108
	db	101
	db	36
	db	113
	db	112
	db	57
	db	65
	db	68
	db	95
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
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
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	29
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65534
	dw	65535
	dw	30
	dw	0
	dw	0
	dw	0
?patch10	equ	@17-@@mat_print_to_file$qp9AD_Matrixp8std@FILE+2
?patch11	equ	0
?patch12	equ	@17-@@mat_print_to_file$qp9AD_Matrixp8std@FILE
	dw	2
	dw	6
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@mat_identity$qp9AD_Matrix	segment virtual
@@mat_identity$qp9AD_Matrix	proc	near
?live16389@0:
	?debug L 90
	push      ebp
	mov       ebp,esp
	?debug L 92
@18:
	mov       eax,dword ptr [ebp+8]
	mov       dword ptr [eax],1065353216
	mov       ecx,dword ptr [ebp+8]
	xor       eax,eax
	mov       dword ptr [ecx+4],eax
	mov       edx,dword ptr [ebp+8]
	xor       ecx,ecx
	mov       dword ptr [edx+8],ecx
	mov       eax,dword ptr [ebp+8]
	xor       edx,edx
	mov       dword ptr [eax+12],edx
	?debug L 93
	mov       ecx,dword ptr [ebp+8]
	xor       eax,eax
	mov       dword ptr [ecx+16],eax
	mov       edx,dword ptr [ebp+8]
	mov       dword ptr [edx+20],1065353216
	mov       eax,dword ptr [ebp+8]
	xor       edx,edx
	mov       dword ptr [eax+24],edx
	mov       ecx,dword ptr [ebp+8]
	xor       eax,eax
	mov       dword ptr [ecx+28],eax
	?debug L 94
	mov       edx,dword ptr [ebp+8]
	xor       ecx,ecx
	mov       dword ptr [edx+32],ecx
	mov       eax,dword ptr [ebp+8]
	xor       edx,edx
	mov       dword ptr [eax+36],edx
	mov       ecx,dword ptr [ebp+8]
	mov       dword ptr [ecx+40],1065353216
	mov       edx,dword ptr [ebp+8]
	xor       ecx,ecx
	mov       dword ptr [edx+44],ecx
	?debug L 95
	mov       eax,dword ptr [ebp+8]
	xor       edx,edx
	mov       dword ptr [eax+48],edx
	mov       ecx,dword ptr [ebp+8]
	xor       eax,eax
	mov       dword ptr [ecx+52],eax
	mov       edx,dword ptr [ebp+8]
	xor       ecx,ecx
	mov       dword ptr [edx+56],ecx
	mov       eax,dword ptr [ebp+8]
	mov       dword ptr [eax+60],1065353216
	?debug L 96
@19:
	pop       ebp
	ret 
	?debug L 0
@@mat_identity$qp9AD_Matrix	endp
@mat_identity$qp9AD_Matrix	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	73
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
	df	@@mat_identity$qp9AD_Matrix
	dw	0
	dw	4104
	dw	0
	dw	31
	dw	0
	dw	0
	dw	0
	db	26
	db	64
	db	109
	db	97
	db	116
	db	95
	db	105
	db	100
	db	101
	db	110
	db	116
	db	105
	db	116
	db	121
	db	36
	db	113
	db	112
	db	57
	db	65
	db	68
	db	95
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	32
	dw	0
	dw	0
	dw	0
?patch13	equ	@19-@@mat_identity$qp9AD_Matrix+2
?patch14	equ	0
?patch15	equ	@19-@@mat_identity$qp9AD_Matrix
	dw	2
	dw	6
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@mat_transpose$qp9AD_Matrixt1	segment virtual
@@mat_transpose$qp9AD_Matrixt1	proc	near
?live16390@0:
	?debug L 101
	push      ebp
	mov       ebp,esp
	push      ecx
	push      ebx
	?debug L 105
@20:
	xor       eax,eax
	mov       dword ptr [ebp-4],eax
	?debug L 107
@21:
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+12]
	mov       eax,dword ptr [ebp-4]
	mov       ebx,dword ptr [ebp+8]
	mov       eax,dword ptr [ebx+4*eax]
	mov       dword ptr [ecx+edx],eax
	?debug L 108
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+12]
	mov       eax,dword ptr [ebp-4]
	mov       ebx,dword ptr [ebp+8]
	mov       eax,dword ptr [ebx+4*eax+16]
	mov       dword ptr [ecx+edx+4],eax
	?debug L 109
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+12]
	mov       eax,dword ptr [ebp-4]
	mov       ebx,dword ptr [ebp+8]
	mov       eax,dword ptr [ebx+4*eax+32]
	mov       dword ptr [ecx+edx+8],eax
	?debug L 110
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+12]
	mov       eax,dword ptr [ebp-4]
	mov       ebx,dword ptr [ebp+8]
	mov       eax,dword ptr [ebx+4*eax+48]
	mov       dword ptr [ecx+edx+12],eax
	inc       dword ptr [ebp-4]
	mov       edx,dword ptr [ebp-4]
	cmp       edx,4
	jl        short @21
	?debug L 112
@24:
	pop       ebx
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@mat_transpose$qp9AD_Matrixt1	endp
@mat_transpose$qp9AD_Matrixt1	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	76
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
	df	@@mat_transpose$qp9AD_Matrixt1
	dw	0
	dw	4106
	dw	0
	dw	33
	dw	0
	dw	0
	dw	0
	db	29
	db	64
	db	109
	db	97
	db	116
	db	95
	db	116
	db	114
	db	97
	db	110
	db	115
	db	112
	db	111
	db	115
	db	101
	db	36
	db	113
	db	112
	db	57
	db	65
	db	68
	db	95
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	db	116
	db	49
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	34
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65535
	dw	65535
	dw	35
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	116
	dw	0
	dw	36
	dw	0
	dw	0
	dw	0
?patch16	equ	@24-@@mat_transpose$qp9AD_Matrixt1+4
?patch17	equ	0
?patch18	equ	@24-@@mat_transpose$qp9AD_Matrixt1
	dw	2
	dw	6
	dw	8
	dw	531
	dw	1
	dw	65528
	dw	65535
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@mat_insert_row$qp9AD_Matrixip9AD_Vect3D	segment virtual
@@mat_insert_row$qp9AD_Matrixip9AD_Vect3D	proc	near
?live16391@0:
	?debug L 116
	push      ebp
	mov       ebp,esp
	?debug L 119
@25:
	mov       eax,dword ptr [ebp+16]
	mov       edx,dword ptr [eax]
	mov       ecx,dword ptr [ebp+12]
	shl       ecx,4
	mov       eax,dword ptr [ebp+8]
	mov       dword ptr [eax+ecx],edx
	?debug L 120
	mov       edx,dword ptr [ebp+16]
	mov       ecx,dword ptr [edx+4]
	mov       eax,dword ptr [ebp+12]
	shl       eax,4
	mov       edx,dword ptr [ebp+8]
	mov       dword ptr [edx+eax+4],ecx
	?debug L 121
	mov       ecx,dword ptr [ebp+16]
	mov       eax,dword ptr [ecx+8]
	mov       edx,dword ptr [ebp+12]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	mov       dword ptr [ecx+edx+8],eax
	?debug L 122
@26:
	pop       ebp
	ret 
	?debug L 0
@@mat_insert_row$qp9AD_Matrixip9AD_Vect3D	endp
@mat_insert_row$qp9AD_Matrixip9AD_Vect3D	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	87
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
	df	@@mat_insert_row$qp9AD_Matrixip9AD_Vect3D
	dw	0
	dw	4108
	dw	0
	dw	37
	dw	0
	dw	0
	dw	0
	db	40
	db	64
	db	109
	db	97
	db	116
	db	95
	db	105
	db	110
	db	115
	db	101
	db	114
	db	116
	db	95
	db	114
	db	111
	db	119
	db	36
	db	113
	db	112
	db	57
	db	65
	db	68
	db	95
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	db	105
	db	112
	db	57
	db	65
	db	68
	db	95
	db	86
	db	101
	db	99
	db	116
	db	51
	db	68
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	38
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	116
	dw	0
	dw	39
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	65533
	dw	65535
	dw	40
	dw	0
	dw	0
	dw	0
?patch19	equ	@26-@@mat_insert_row$qp9AD_Matrixip9AD_Vect3D+2
?patch20	equ	0
?patch21	equ	@26-@@mat_insert_row$qp9AD_Matrixip9AD_Vect3D
	dw	2
	dw	6
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@mat_insert_column$qp9AD_Matrixip9AD_Vect3D	segment virtual
@@mat_insert_column$qp9AD_Matrixip9AD_Vect3D	proc	near
?live16392@0:
	?debug L 126
	push      ebp
	mov       ebp,esp
	?debug L 129
@27:
	mov       eax,dword ptr [ebp+16]
	mov       edx,dword ptr [eax]
	mov       ecx,dword ptr [ebp+12]
	mov       eax,dword ptr [ebp+8]
	mov       dword ptr [eax+4*ecx],edx
	?debug L 130
	mov       edx,dword ptr [ebp+16]
	mov       ecx,dword ptr [edx+4]
	mov       eax,dword ptr [ebp+12]
	mov       edx,dword ptr [ebp+8]
	mov       dword ptr [edx+4*eax+16],ecx
	?debug L 131
	mov       ecx,dword ptr [ebp+16]
	mov       eax,dword ptr [ecx+8]
	mov       edx,dword ptr [ebp+12]
	mov       ecx,dword ptr [ebp+8]
	mov       dword ptr [ecx+4*edx+32],eax
	?debug L 132
@28:
	pop       ebp
	ret 
	?debug L 0
@@mat_insert_column$qp9AD_Matrixip9AD_Vect3D	endp
@mat_insert_column$qp9AD_Matrixip9AD_Vect3D	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	90
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
	df	@@mat_insert_column$qp9AD_Matrixip9AD_Vect3D
	dw	0
	dw	4110
	dw	0
	dw	41
	dw	0
	dw	0
	dw	0
	db	43
	db	64
	db	109
	db	97
	db	116
	db	95
	db	105
	db	110
	db	115
	db	101
	db	114
	db	116
	db	95
	db	99
	db	111
	db	108
	db	117
	db	109
	db	110
	db	36
	db	113
	db	112
	db	57
	db	65
	db	68
	db	95
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	db	105
	db	112
	db	57
	db	65
	db	68
	db	95
	db	86
	db	101
	db	99
	db	116
	db	51
	db	68
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	42
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	116
	dw	0
	dw	43
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	65533
	dw	65535
	dw	44
	dw	0
	dw	0
	dw	0
?patch22	equ	@28-@@mat_insert_column$qp9AD_Matrixip9AD_Vect3D+2
?patch23	equ	0
?patch24	equ	@28-@@mat_insert_column$qp9AD_Matrixip9AD_Vect3D
	dw	2
	dw	6
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@mat_get_row$qp9AD_Matrixip9AD_Vect3D	segment virtual
@@mat_get_row$qp9AD_Matrixip9AD_Vect3D	proc	near
?live16393@0:
	?debug L 136
	push      ebp
	mov       ebp,esp
	?debug L 138
@29:
	mov       eax,dword ptr [ebp+12]
	shl       eax,4
	mov       edx,dword ptr [ebp+8]
	mov       ecx,dword ptr [edx+eax]
	mov       eax,dword ptr [ebp+16]
	mov       dword ptr [eax],ecx
	?debug L 139
	mov       edx,dword ptr [ebp+12]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	mov       eax,dword ptr [ecx+edx+4]
	mov       edx,dword ptr [ebp+16]
	mov       dword ptr [edx+4],eax
	?debug L 140
	mov       ecx,dword ptr [ebp+12]
	shl       ecx,4
	mov       eax,dword ptr [ebp+8]
	mov       edx,dword ptr [eax+ecx+8]
	mov       ecx,dword ptr [ebp+16]
	mov       dword ptr [ecx+8],edx
	?debug L 141
@30:
	pop       ebp
	ret 
	?debug L 0
@@mat_get_row$qp9AD_Matrixip9AD_Vect3D	endp
@mat_get_row$qp9AD_Matrixip9AD_Vect3D	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	84
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
	df	@@mat_get_row$qp9AD_Matrixip9AD_Vect3D
	dw	0
	dw	4112
	dw	0
	dw	45
	dw	0
	dw	0
	dw	0
	db	37
	db	64
	db	109
	db	97
	db	116
	db	95
	db	103
	db	101
	db	116
	db	95
	db	114
	db	111
	db	119
	db	36
	db	113
	db	112
	db	57
	db	65
	db	68
	db	95
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	db	105
	db	112
	db	57
	db	65
	db	68
	db	95
	db	86
	db	101
	db	99
	db	116
	db	51
	db	68
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	46
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	116
	dw	0
	dw	47
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	65533
	dw	65535
	dw	48
	dw	0
	dw	0
	dw	0
?patch25	equ	@30-@@mat_get_row$qp9AD_Matrixip9AD_Vect3D+2
?patch26	equ	0
?patch27	equ	@30-@@mat_get_row$qp9AD_Matrixip9AD_Vect3D
	dw	2
	dw	6
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@mat_get_column$qp9AD_Matrixip9AD_Vect3D	segment virtual
@@mat_get_column$qp9AD_Matrixip9AD_Vect3D	proc	near
?live16394@0:
	?debug L 145
	push      ebp
	mov       ebp,esp
	?debug L 147
@31:
	mov       eax,dword ptr [ebp+16]
	mov       edx,dword ptr [ebp+12]
	mov       ecx,dword ptr [ebp+8]
	mov       edx,dword ptr [ecx+4*edx]
	mov       dword ptr [eax],edx
	?debug L 148
	mov       eax,dword ptr [ebp+16]
	mov       ecx,dword ptr [ebp+12]
	mov       edx,dword ptr [ebp+8]
	mov       ecx,dword ptr [edx+4*ecx+16]
	mov       dword ptr [eax+4],ecx
	?debug L 149
	mov       eax,dword ptr [ebp+16]
	mov       edx,dword ptr [ebp+12]
	mov       ecx,dword ptr [ebp+8]
	mov       edx,dword ptr [ecx+4*edx+32]
	mov       dword ptr [eax+8],edx
	?debug L 150
@32:
	pop       ebp
	ret 
	?debug L 0
@@mat_get_column$qp9AD_Matrixip9AD_Vect3D	endp
@mat_get_column$qp9AD_Matrixip9AD_Vect3D	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	87
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
	df	@@mat_get_column$qp9AD_Matrixip9AD_Vect3D
	dw	0
	dw	4114
	dw	0
	dw	49
	dw	0
	dw	0
	dw	0
	db	40
	db	64
	db	109
	db	97
	db	116
	db	95
	db	103
	db	101
	db	116
	db	95
	db	99
	db	111
	db	108
	db	117
	db	109
	db	110
	db	36
	db	113
	db	112
	db	57
	db	65
	db	68
	db	95
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	db	105
	db	112
	db	57
	db	65
	db	68
	db	95
	db	86
	db	101
	db	99
	db	116
	db	51
	db	68
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	50
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	116
	dw	0
	dw	51
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	65533
	dw	65535
	dw	52
	dw	0
	dw	0
	dw	0
?patch28	equ	@32-@@mat_get_column$qp9AD_Matrixip9AD_Vect3D+2
?patch29	equ	0
?patch30	equ	@32-@@mat_get_column$qp9AD_Matrixip9AD_Vect3D
	dw	2
	dw	6
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@mat_setmatrix_of_scaling$qp9AD_Matrixfff	segment virtual
@@mat_setmatrix_of_scaling$qp9AD_Matrixfff	proc	near
?live16395@0:
	?debug L 155
	push      ebp
	mov       ebp,esp
	?debug L 157
@33:
	push      dword ptr [ebp+8]
	call      @@mat_identity$qp9AD_Matrix
	pop       ecx
	?debug L 158
	mov       eax,dword ptr [ebp+12]
	mov       edx,dword ptr [ebp+8]
	mov       dword ptr [edx],eax
	?debug L 159
	mov       ecx,dword ptr [ebp+16]
	mov       eax,dword ptr [ebp+8]
	mov       dword ptr [eax+20],ecx
	?debug L 160
	mov       edx,dword ptr [ebp+20]
	mov       ecx,dword ptr [ebp+8]
	mov       dword ptr [ecx+40],edx
	?debug L 161
@34:
	pop       ebp
	ret 
	?debug L 0
@@mat_setmatrix_of_scaling$qp9AD_Matrixfff	endp
@mat_setmatrix_of_scaling$qp9AD_Matrixfff	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	88
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
	df	@@mat_setmatrix_of_scaling$qp9AD_Matrixfff
	dw	0
	dw	4116
	dw	0
	dw	53
	dw	0
	dw	0
	dw	0
	db	41
	db	64
	db	109
	db	97
	db	116
	db	95
	db	115
	db	101
	db	116
	db	109
	db	97
	db	116
	db	114
	db	105
	db	120
	db	95
	db	111
	db	102
	db	95
	db	115
	db	99
	db	97
	db	108
	db	105
	db	110
	db	103
	db	36
	db	113
	db	112
	db	57
	db	65
	db	68
	db	95
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	db	102
	db	102
	db	102
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	54
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	64
	dw	0
	dw	55
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	64
	dw	0
	dw	56
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	20
	dw	0
	dw	64
	dw	0
	dw	57
	dw	0
	dw	0
	dw	0
?patch31	equ	@34-@@mat_setmatrix_of_scaling$qp9AD_Matrixfff+2
?patch32	equ	0
?patch33	equ	@34-@@mat_setmatrix_of_scaling$qp9AD_Matrixfff
	dw	2
	dw	6
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@mat_setmatrix_of_pretraslation$qp9AD_Matrixp9AD_Vect3D	segment virtual
@@mat_setmatrix_of_pretraslation$qp9AD_Matrixp9AD_Vect3D	proc	near
?live16396@0:
	?debug L 165
	push      ebp
	mov       ebp,esp
	?debug L 168
@35:
	mov       eax,dword ptr [ebp+8]
	mov       dword ptr [eax],1065353216
	mov       ecx,dword ptr [ebp+8]
	xor       eax,eax
	mov       dword ptr [ecx+4],eax
	mov       edx,dword ptr [ebp+8]
	xor       ecx,ecx
	mov       dword ptr [edx+8],ecx
	mov       eax,dword ptr [ebp+12]
	mov       edx,dword ptr [eax]
	mov       ecx,dword ptr [ebp+8]
	mov       dword ptr [ecx+12],edx
	?debug L 169
	mov       eax,dword ptr [ebp+8]
	xor       edx,edx
	mov       dword ptr [eax+16],edx
	mov       ecx,dword ptr [ebp+8]
	mov       dword ptr [ecx+20],1065353216
	mov       edx,dword ptr [ebp+8]
	xor       ecx,ecx
	mov       dword ptr [edx+24],ecx
	mov       eax,dword ptr [ebp+12]
	mov       edx,dword ptr [eax+4]
	mov       ecx,dword ptr [ebp+8]
	mov       dword ptr [ecx+28],edx
	?debug L 170
	mov       eax,dword ptr [ebp+8]
	xor       edx,edx
	mov       dword ptr [eax+32],edx
	mov       ecx,dword ptr [ebp+8]
	xor       eax,eax
	mov       dword ptr [ecx+36],eax
	mov       edx,dword ptr [ebp+8]
	mov       dword ptr [edx+40],1065353216
	mov       eax,dword ptr [ebp+12]
	mov       edx,dword ptr [eax+8]
	mov       ecx,dword ptr [ebp+8]
	mov       dword ptr [ecx+44],edx
	?debug L 171
	mov       eax,dword ptr [ebp+8]
	xor       edx,edx
	mov       dword ptr [eax+48],edx
	mov       ecx,dword ptr [ebp+8]
	xor       eax,eax
	mov       dword ptr [ecx+52],eax
	mov       edx,dword ptr [ebp+8]
	xor       ecx,ecx
	mov       dword ptr [edx+56],ecx
	mov       eax,dword ptr [ebp+8]
	mov       dword ptr [eax+60],1065353216
	?debug L 172
@36:
	pop       ebp
	ret 
	?debug L 0
@@mat_setmatrix_of_pretraslation$qp9AD_Matrixp9AD_Vect3D	endp
@mat_setmatrix_of_pretraslation$qp9AD_Matrixp9AD_Vect3D	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	102
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
	df	@@mat_setmatrix_of_pretraslation$qp9AD_Matrixp9AD_Vect3D
	dw	0
	dw	4118
	dw	0
	dw	58
	dw	0
	dw	0
	dw	0
	db	55
	db	64
	db	109
	db	97
	db	116
	db	95
	db	115
	db	101
	db	116
	db	109
	db	97
	db	116
	db	114
	db	105
	db	120
	db	95
	db	111
	db	102
	db	95
	db	112
	db	114
	db	101
	db	116
	db	114
	db	97
	db	115
	db	108
	db	97
	db	116
	db	105
	db	111
	db	110
	db	36
	db	113
	db	112
	db	57
	db	65
	db	68
	db	95
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	db	112
	db	57
	db	65
	db	68
	db	95
	db	86
	db	101
	db	99
	db	116
	db	51
	db	68
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	59
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65533
	dw	65535
	dw	60
	dw	0
	dw	0
	dw	0
?patch34	equ	@36-@@mat_setmatrix_of_pretraslation$qp9AD_Matrixp9AD_Vect3D+2
?patch35	equ	0
?patch36	equ	@36-@@mat_setmatrix_of_pretraslation$qp9AD_Matrixp9AD_Vect3D
	dw	2
	dw	6
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@mat_setmatrix_of_eulerrotation$qp9AD_Matrixfff	segment virtual
@@mat_setmatrix_of_eulerrotation$qp9AD_Matrixfff	proc	near
?live16397@0:
	?debug L 176
	push      ebp
	mov       ebp,esp
	add       esp,-24
	?debug L 180
@37:
	fld       dword ptr [ebp+12]
	add       esp,-8
	fstp      qword ptr [esp]
	call      @_cos
	add       esp,8
	fstp      dword ptr [ebp-4]
	fld       dword ptr [ebp+12]
	add       esp,-8
	fstp      qword ptr [esp]
	call      @_sin
	add       esp,8
	fstp      dword ptr [ebp-8]
	?debug L 181
	fld       dword ptr [ebp+16]
	add       esp,-8
	fstp      qword ptr [esp]
	call      @_cos
	add       esp,8
	fstp      dword ptr [ebp-12]
	fld       dword ptr [ebp+16]
	add       esp,-8
	fstp      qword ptr [esp]
	call      @_sin
	add       esp,8
	fstp      dword ptr [ebp-16]
	?debug L 182
	fld       dword ptr [ebp+20]
	add       esp,-8
	fstp      qword ptr [esp]
	call      @_cos
	add       esp,8
	fstp      dword ptr [ebp-20]
	fld       dword ptr [ebp+20]
	add       esp,-8
	fstp      qword ptr [esp]
	call      @_sin
	add       esp,8
	fstp      dword ptr [ebp-24]
	?debug L 184
	push      dword ptr [ebp+8]
	call      @@mat_identity$qp9AD_Matrix
	pop       ecx
	?debug L 185
	fld       dword ptr [ebp-12]
	fmul      dword ptr [ebp-20]
	mov       eax,dword ptr [ebp+8]
	fstp      dword ptr [eax]
	?debug L 186
	fld       dword ptr [ebp-12]
	fchs
	fmul      dword ptr [ebp-24]
	mov       edx,dword ptr [ebp+8]
	fstp      dword ptr [edx+4]
	?debug L 187
	fld       dword ptr [ebp-16]
	fchs
	mov       ecx,dword ptr [ebp+8]
	fstp      dword ptr [ecx+8]
	?debug L 188
	fld       dword ptr [ebp-4]
	fmul      dword ptr [ebp-24]
	fld       dword ptr [ebp-8]
	fmul      dword ptr [ebp-16]
	fmul      dword ptr [ebp-20]
	fsubp      st(1),st
	mov       eax,dword ptr [ebp+8]
	fstp      dword ptr [eax+16]
	?debug L 189
	fld       dword ptr [ebp-4]
	fmul      dword ptr [ebp-20]
	fld       dword ptr [ebp-8]
	fmul      dword ptr [ebp-16]
	fmul      dword ptr [ebp-24]
	faddp      st(1),st
	mov       edx,dword ptr [ebp+8]
	fstp      dword ptr [edx+20]
	?debug L 190
	fld       dword ptr [ebp-8]
	fchs
	fmul      dword ptr [ebp-12]
	mov       ecx,dword ptr [ebp+8]
	fstp      dword ptr [ecx+24]
	?debug L 191
	fld       dword ptr [ebp-4]
	fmul      dword ptr [ebp-16]
	fmul      dword ptr [ebp-20]
	fld       dword ptr [ebp-8]
	fmul      dword ptr [ebp-24]
	faddp      st(1),st
	mov       eax,dword ptr [ebp+8]
	fstp      dword ptr [eax+32]
	?debug L 192
	fld       dword ptr [ebp-8]
	fmul      dword ptr [ebp-20]
	fld       dword ptr [ebp-4]
	fmul      dword ptr [ebp-16]
	fmul      dword ptr [ebp-24]
	fsubp      st(1),st
	mov       edx,dword ptr [ebp+8]
	fstp      dword ptr [edx+36]
	?debug L 193
	fld       dword ptr [ebp-4]
	fmul      dword ptr [ebp-12]
	mov       ecx,dword ptr [ebp+8]
	fstp      dword ptr [ecx+40]
	?debug L 194
@38:
	mov       esp,ebp
	pop       ebp
	ret 
	?debug L 0
@@mat_setmatrix_of_eulerrotation$qp9AD_Matrixfff	endp
@mat_setmatrix_of_eulerrotation$qp9AD_Matrixfff	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	94
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
	df	@@mat_setmatrix_of_eulerrotation$qp9AD_Matrixfff
	dw	0
	dw	4120
	dw	0
	dw	61
	dw	0
	dw	0
	dw	0
	db	47
	db	64
	db	109
	db	97
	db	116
	db	95
	db	115
	db	101
	db	116
	db	109
	db	97
	db	116
	db	114
	db	105
	db	120
	db	95
	db	111
	db	102
	db	95
	db	101
	db	117
	db	108
	db	101
	db	114
	db	114
	db	111
	db	116
	db	97
	db	116
	db	105
	db	111
	db	110
	db	36
	db	113
	db	112
	db	57
	db	65
	db	68
	db	95
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	db	102
	db	102
	db	102
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	62
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	64
	dw	0
	dw	63
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	64
	dw	0
	dw	64
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	20
	dw	0
	dw	64
	dw	0
	dw	65
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65512
	dw	65535
	dw	64
	dw	0
	dw	66
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65516
	dw	65535
	dw	64
	dw	0
	dw	67
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65520
	dw	65535
	dw	64
	dw	0
	dw	68
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65524
	dw	65535
	dw	64
	dw	0
	dw	69
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65528
	dw	65535
	dw	64
	dw	0
	dw	70
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	64
	dw	0
	dw	71
	dw	0
	dw	0
	dw	0
?patch37	equ	@38-@@mat_setmatrix_of_eulerrotation$qp9AD_Matrixfff+4
?patch38	equ	0
?patch39	equ	@38-@@mat_setmatrix_of_eulerrotation$qp9AD_Matrixfff
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@mat_setmatrix_of_eulerrotationX$qp9AD_Matrixf	segment virtual
@@mat_setmatrix_of_eulerrotationX$qp9AD_Matrixf	proc	near
?live16398@0:
	?debug L 198
	push      ebp
	mov       ebp,esp
	add       esp,-8
	?debug L 202
@39:
	fld       dword ptr [ebp+12]
	add       esp,-8
	fstp      qword ptr [esp]
	call      @_cos
	add       esp,8
	fstp      dword ptr [ebp-4]
	?debug L 203
	fld       dword ptr [ebp+12]
	add       esp,-8
	fstp      qword ptr [esp]
	call      @_sin
	add       esp,8
	fstp      dword ptr [ebp-8]
	?debug L 204
	push      dword ptr [ebp+8]
	call      @@mat_identity$qp9AD_Matrix
	pop       ecx
	?debug L 205
	fld       dword ptr [ebp-4]
	mov       eax,dword ptr [ebp+8]
	fst       dword ptr [eax+40]
	mov       edx,dword ptr [ebp+8]
	fstp      dword ptr [edx+20]
	?debug L 206
	mov       ecx,dword ptr [ebp-8]
	mov       eax,dword ptr [ebp+8]
	mov       dword ptr [eax+36],ecx
	?debug L 207
	fld       dword ptr [ebp-8]
	fchs
	mov       edx,dword ptr [ebp+8]
	fstp      dword ptr [edx+24]
	?debug L 208
@40:
	pop       ecx
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@mat_setmatrix_of_eulerrotationX$qp9AD_Matrixf	endp
@mat_setmatrix_of_eulerrotationX$qp9AD_Matrixf	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	93
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
	df	@@mat_setmatrix_of_eulerrotationX$qp9AD_Matrixf
	dw	0
	dw	4122
	dw	0
	dw	72
	dw	0
	dw	0
	dw	0
	db	46
	db	64
	db	109
	db	97
	db	116
	db	95
	db	115
	db	101
	db	116
	db	109
	db	97
	db	116
	db	114
	db	105
	db	120
	db	95
	db	111
	db	102
	db	95
	db	101
	db	117
	db	108
	db	101
	db	114
	db	114
	db	111
	db	116
	db	97
	db	116
	db	105
	db	111
	db	110
	db	88
	db	36
	db	113
	db	112
	db	57
	db	65
	db	68
	db	95
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	db	102
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	73
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	64
	dw	0
	dw	74
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65528
	dw	65535
	dw	64
	dw	0
	dw	75
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	64
	dw	0
	dw	76
	dw	0
	dw	0
	dw	0
?patch40	equ	@40-@@mat_setmatrix_of_eulerrotationX$qp9AD_Matrixf+4
?patch41	equ	0
?patch42	equ	@40-@@mat_setmatrix_of_eulerrotationX$qp9AD_Matrixf
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@mat_setmatrix_of_eulerrotationY$qp9AD_Matrixf	segment virtual
@@mat_setmatrix_of_eulerrotationY$qp9AD_Matrixf	proc	near
?live16399@0:
	?debug L 212
	push      ebp
	mov       ebp,esp
	add       esp,-8
	?debug L 216
@41:
	fld       dword ptr [ebp+12]
	add       esp,-8
	fstp      qword ptr [esp]
	call      @_cos
	add       esp,8
	fstp      dword ptr [ebp-4]
	?debug L 217
	fld       dword ptr [ebp+12]
	add       esp,-8
	fstp      qword ptr [esp]
	call      @_sin
	add       esp,8
	fstp      dword ptr [ebp-8]
	?debug L 218
	push      dword ptr [ebp+8]
	call      @@mat_identity$qp9AD_Matrix
	pop       ecx
	?debug L 219
	fld       dword ptr [ebp-4]
	mov       eax,dword ptr [ebp+8]
	fst       dword ptr [eax+40]
	mov       edx,dword ptr [ebp+8]
	fstp      dword ptr [edx]
	?debug L 220
	mov       ecx,dword ptr [ebp-8]
	mov       eax,dword ptr [ebp+8]
	mov       dword ptr [eax+32],ecx
	?debug L 221
	fld       dword ptr [ebp-8]
	fchs
	mov       edx,dword ptr [ebp+8]
	fstp      dword ptr [edx+8]
	?debug L 222
@42:
	pop       ecx
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@mat_setmatrix_of_eulerrotationY$qp9AD_Matrixf	endp
@mat_setmatrix_of_eulerrotationY$qp9AD_Matrixf	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	93
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch43
	dd	?patch44
	dd	?patch45
	df	@@mat_setmatrix_of_eulerrotationY$qp9AD_Matrixf
	dw	0
	dw	4124
	dw	0
	dw	77
	dw	0
	dw	0
	dw	0
	db	46
	db	64
	db	109
	db	97
	db	116
	db	95
	db	115
	db	101
	db	116
	db	109
	db	97
	db	116
	db	114
	db	105
	db	120
	db	95
	db	111
	db	102
	db	95
	db	101
	db	117
	db	108
	db	101
	db	114
	db	114
	db	111
	db	116
	db	97
	db	116
	db	105
	db	111
	db	110
	db	89
	db	36
	db	113
	db	112
	db	57
	db	65
	db	68
	db	95
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	db	102
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	78
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	64
	dw	0
	dw	79
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65528
	dw	65535
	dw	64
	dw	0
	dw	80
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	64
	dw	0
	dw	81
	dw	0
	dw	0
	dw	0
?patch43	equ	@42-@@mat_setmatrix_of_eulerrotationY$qp9AD_Matrixf+4
?patch44	equ	0
?patch45	equ	@42-@@mat_setmatrix_of_eulerrotationY$qp9AD_Matrixf
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@mat_setmatrix_of_eulerrotationZ$qp9AD_Matrixf	segment virtual
@@mat_setmatrix_of_eulerrotationZ$qp9AD_Matrixf	proc	near
?live16400@0:
	?debug L 226
	push      ebp
	mov       ebp,esp
	add       esp,-8
	?debug L 230
@43:
	fld       dword ptr [ebp+12]
	add       esp,-8
	fstp      qword ptr [esp]
	call      @_cos
	add       esp,8
	fstp      dword ptr [ebp-4]
	?debug L 231
	fld       dword ptr [ebp+12]
	add       esp,-8
	fstp      qword ptr [esp]
	call      @_sin
	add       esp,8
	fstp      dword ptr [ebp-8]
	?debug L 232
	push      dword ptr [ebp+8]
	call      @@mat_identity$qp9AD_Matrix
	pop       ecx
	?debug L 233
	fld       dword ptr [ebp-4]
	mov       eax,dword ptr [ebp+8]
	fst       dword ptr [eax+20]
	mov       edx,dword ptr [ebp+8]
	fstp      dword ptr [edx]
	?debug L 234
	mov       ecx,dword ptr [ebp-8]
	mov       eax,dword ptr [ebp+8]
	mov       dword ptr [eax+4],ecx
	?debug L 235
	fld       dword ptr [ebp-8]
	fchs
	mov       edx,dword ptr [ebp+8]
	fstp      dword ptr [edx+16]
	?debug L 236
@44:
	pop       ecx
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@mat_setmatrix_of_eulerrotationZ$qp9AD_Matrixf	endp
@mat_setmatrix_of_eulerrotationZ$qp9AD_Matrixf	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	93
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch46
	dd	?patch47
	dd	?patch48
	df	@@mat_setmatrix_of_eulerrotationZ$qp9AD_Matrixf
	dw	0
	dw	4126
	dw	0
	dw	82
	dw	0
	dw	0
	dw	0
	db	46
	db	64
	db	109
	db	97
	db	116
	db	95
	db	115
	db	101
	db	116
	db	109
	db	97
	db	116
	db	114
	db	105
	db	120
	db	95
	db	111
	db	102
	db	95
	db	101
	db	117
	db	108
	db	101
	db	114
	db	114
	db	111
	db	116
	db	97
	db	116
	db	105
	db	111
	db	110
	db	90
	db	36
	db	113
	db	112
	db	57
	db	65
	db	68
	db	95
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	db	102
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	83
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	64
	dw	0
	dw	84
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65528
	dw	65535
	dw	64
	dw	0
	dw	85
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	64
	dw	0
	dw	86
	dw	0
	dw	0
	dw	0
?patch46	equ	@44-@@mat_setmatrix_of_eulerrotationZ$qp9AD_Matrixf+4
?patch47	equ	0
?patch48	equ	@44-@@mat_setmatrix_of_eulerrotationZ$qp9AD_Matrixf
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@mat_add$qp9AD_Matrixt1t1	segment virtual
@@mat_add$qp9AD_Matrixt1t1	proc	near
?live16401@0:
	?debug L 240
	push      ebp
	mov       ebp,esp
	push      ecx
	?debug L 244
@45:
	xor       eax,eax
	mov       dword ptr [ebp-4],eax
	?debug L 246
@46:
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+edx]
	mov       eax,dword ptr [ebp-4]
	shl       eax,4
	mov       edx,dword ptr [ebp+12]
	fadd      dword ptr [edx+eax]
	mov       ecx,dword ptr [ebp-4]
	shl       ecx,4
	mov       eax,dword ptr [ebp+16]
	fstp      dword ptr [eax+ecx]
	?debug L 247
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+edx+4]
	mov       eax,dword ptr [ebp-4]
	shl       eax,4
	mov       edx,dword ptr [ebp+12]
	fadd      dword ptr [edx+eax+4]
	mov       ecx,dword ptr [ebp-4]
	shl       ecx,4
	mov       eax,dword ptr [ebp+16]
	fstp      dword ptr [eax+ecx+4]
	?debug L 248
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+edx+8]
	mov       eax,dword ptr [ebp-4]
	shl       eax,4
	mov       edx,dword ptr [ebp+12]
	fadd      dword ptr [edx+eax+8]
	mov       ecx,dword ptr [ebp-4]
	shl       ecx,4
	mov       eax,dword ptr [ebp+16]
	fstp      dword ptr [eax+ecx+8]
	?debug L 249
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+edx+12]
	mov       eax,dword ptr [ebp-4]
	shl       eax,4
	mov       edx,dword ptr [ebp+12]
	fadd      dword ptr [edx+eax+12]
	mov       ecx,dword ptr [ebp-4]
	shl       ecx,4
	mov       eax,dword ptr [ebp+16]
	fstp      dword ptr [eax+ecx+12]
	inc       dword ptr [ebp-4]
	mov       edx,dword ptr [ebp-4]
	cmp       edx,4
	jl        @46
	?debug L 251
@49:
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@mat_add$qp9AD_Matrixt1t1	endp
@mat_add$qp9AD_Matrixt1t1	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	72
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch49
	dd	?patch50
	dd	?patch51
	df	@@mat_add$qp9AD_Matrixt1t1
	dw	0
	dw	4128
	dw	0
	dw	87
	dw	0
	dw	0
	dw	0
	db	25
	db	64
	db	109
	db	97
	db	116
	db	95
	db	97
	db	100
	db	100
	db	36
	db	113
	db	112
	db	57
	db	65
	db	68
	db	95
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	db	116
	db	49
	db	116
	db	49
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	88
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65535
	dw	65535
	dw	89
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	65535
	dw	65535
	dw	90
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	116
	dw	0
	dw	91
	dw	0
	dw	0
	dw	0
?patch49	equ	@49-@@mat_add$qp9AD_Matrixt1t1+3
?patch50	equ	0
?patch51	equ	@49-@@mat_add$qp9AD_Matrixt1t1
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@mat_auto_add$qp9AD_Matrixt1	segment virtual
@@mat_auto_add$qp9AD_Matrixt1	proc	near
?live16402@0:
	?debug L 255
	push      ebp
	mov       ebp,esp
	push      ecx
	?debug L 259
@50:
	xor       eax,eax
	mov       dword ptr [ebp-4],eax
	?debug L 261
@51:
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+edx]
	mov       eax,dword ptr [ebp-4]
	shl       eax,4
	mov       edx,dword ptr [ebp+12]
	fadd      dword ptr [edx+eax]
	mov       ecx,dword ptr [ebp-4]
	shl       ecx,4
	mov       eax,dword ptr [ebp+8]
	fstp      dword ptr [eax+ecx]
	?debug L 262
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+edx+4]
	mov       eax,dword ptr [ebp-4]
	shl       eax,4
	mov       edx,dword ptr [ebp+12]
	fadd      dword ptr [edx+eax+4]
	mov       ecx,dword ptr [ebp-4]
	shl       ecx,4
	mov       eax,dword ptr [ebp+8]
	fstp      dword ptr [eax+ecx+4]
	?debug L 263
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+edx+8]
	mov       eax,dword ptr [ebp-4]
	shl       eax,4
	mov       edx,dword ptr [ebp+12]
	fadd      dword ptr [edx+eax+8]
	mov       ecx,dword ptr [ebp-4]
	shl       ecx,4
	mov       eax,dword ptr [ebp+8]
	fstp      dword ptr [eax+ecx+8]
	?debug L 264
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+edx+12]
	mov       eax,dword ptr [ebp-4]
	shl       eax,4
	mov       edx,dword ptr [ebp+12]
	fadd      dword ptr [edx+eax+12]
	mov       ecx,dword ptr [ebp-4]
	shl       ecx,4
	mov       eax,dword ptr [ebp+8]
	fstp      dword ptr [eax+ecx+12]
	inc       dword ptr [ebp-4]
	mov       edx,dword ptr [ebp-4]
	cmp       edx,4
	jl        @51
	?debug L 266
@54:
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@mat_auto_add$qp9AD_Matrixt1	endp
@mat_auto_add$qp9AD_Matrixt1	ends
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
	dd	?patch52
	dd	?patch53
	dd	?patch54
	df	@@mat_auto_add$qp9AD_Matrixt1
	dw	0
	dw	4130
	dw	0
	dw	92
	dw	0
	dw	0
	dw	0
	db	28
	db	64
	db	109
	db	97
	db	116
	db	95
	db	97
	db	117
	db	116
	db	111
	db	95
	db	97
	db	100
	db	100
	db	36
	db	113
	db	112
	db	57
	db	65
	db	68
	db	95
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	db	116
	db	49
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	93
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65535
	dw	65535
	dw	94
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	116
	dw	0
	dw	95
	dw	0
	dw	0
	dw	0
?patch52	equ	@54-@@mat_auto_add$qp9AD_Matrixt1+3
?patch53	equ	0
?patch54	equ	@54-@@mat_auto_add$qp9AD_Matrixt1
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@mat_sub$qp9AD_Matrixt1t1	segment virtual
@@mat_sub$qp9AD_Matrixt1t1	proc	near
?live16403@0:
	?debug L 270
	push      ebp
	mov       ebp,esp
	push      ecx
	?debug L 274
@55:
	xor       eax,eax
	mov       dword ptr [ebp-4],eax
	?debug L 276
@56:
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+edx]
	mov       eax,dword ptr [ebp-4]
	shl       eax,4
	mov       edx,dword ptr [ebp+12]
	fsub      dword ptr [edx+eax]
	mov       ecx,dword ptr [ebp-4]
	shl       ecx,4
	mov       eax,dword ptr [ebp+16]
	fstp      dword ptr [eax+ecx]
	?debug L 277
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+edx+4]
	mov       eax,dword ptr [ebp-4]
	shl       eax,4
	mov       edx,dword ptr [ebp+12]
	fsub      dword ptr [edx+eax+4]
	mov       ecx,dword ptr [ebp-4]
	shl       ecx,4
	mov       eax,dword ptr [ebp+16]
	fstp      dword ptr [eax+ecx+4]
	?debug L 278
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+edx+8]
	mov       eax,dword ptr [ebp-4]
	shl       eax,4
	mov       edx,dword ptr [ebp+12]
	fsub      dword ptr [edx+eax+8]
	mov       ecx,dword ptr [ebp-4]
	shl       ecx,4
	mov       eax,dword ptr [ebp+16]
	fstp      dword ptr [eax+ecx+8]
	?debug L 279
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+edx+12]
	mov       eax,dword ptr [ebp-4]
	shl       eax,4
	mov       edx,dword ptr [ebp+12]
	fsub      dword ptr [edx+eax+12]
	mov       ecx,dword ptr [ebp-4]
	shl       ecx,4
	mov       eax,dword ptr [ebp+16]
	fstp      dword ptr [eax+ecx+12]
	inc       dword ptr [ebp-4]
	mov       edx,dword ptr [ebp-4]
	cmp       edx,4
	jl        @56
	?debug L 281
@59:
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@mat_sub$qp9AD_Matrixt1t1	endp
@mat_sub$qp9AD_Matrixt1t1	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	72
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch55
	dd	?patch56
	dd	?patch57
	df	@@mat_sub$qp9AD_Matrixt1t1
	dw	0
	dw	4132
	dw	0
	dw	96
	dw	0
	dw	0
	dw	0
	db	25
	db	64
	db	109
	db	97
	db	116
	db	95
	db	115
	db	117
	db	98
	db	36
	db	113
	db	112
	db	57
	db	65
	db	68
	db	95
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	db	116
	db	49
	db	116
	db	49
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	97
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65535
	dw	65535
	dw	98
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	65535
	dw	65535
	dw	99
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	116
	dw	0
	dw	100
	dw	0
	dw	0
	dw	0
?patch55	equ	@59-@@mat_sub$qp9AD_Matrixt1t1+3
?patch56	equ	0
?patch57	equ	@59-@@mat_sub$qp9AD_Matrixt1t1
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@mat_auto_sub$qp9AD_Matrixt1	segment virtual
@@mat_auto_sub$qp9AD_Matrixt1	proc	near
?live16404@0:
	?debug L 285
	push      ebp
	mov       ebp,esp
	push      ecx
	?debug L 289
@60:
	xor       eax,eax
	mov       dword ptr [ebp-4],eax
	?debug L 291
@61:
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+edx]
	mov       eax,dword ptr [ebp-4]
	shl       eax,4
	mov       edx,dword ptr [ebp+12]
	fsub      dword ptr [edx+eax]
	mov       ecx,dword ptr [ebp-4]
	shl       ecx,4
	mov       eax,dword ptr [ebp+8]
	fstp      dword ptr [eax+ecx]
	?debug L 292
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+edx+4]
	mov       eax,dword ptr [ebp-4]
	shl       eax,4
	mov       edx,dword ptr [ebp+12]
	fsub      dword ptr [edx+eax+4]
	mov       ecx,dword ptr [ebp-4]
	shl       ecx,4
	mov       eax,dword ptr [ebp+8]
	fstp      dword ptr [eax+ecx+4]
	?debug L 293
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+edx+8]
	mov       eax,dword ptr [ebp-4]
	shl       eax,4
	mov       edx,dword ptr [ebp+12]
	fsub      dword ptr [edx+eax+8]
	mov       ecx,dword ptr [ebp-4]
	shl       ecx,4
	mov       eax,dword ptr [ebp+8]
	fstp      dword ptr [eax+ecx+8]
	?debug L 294
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+edx+12]
	mov       eax,dword ptr [ebp-4]
	shl       eax,4
	mov       edx,dword ptr [ebp+12]
	fsub      dword ptr [edx+eax+12]
	mov       ecx,dword ptr [ebp-4]
	shl       ecx,4
	mov       eax,dword ptr [ebp+8]
	fstp      dword ptr [eax+ecx+12]
	inc       dword ptr [ebp-4]
	mov       edx,dword ptr [ebp-4]
	cmp       edx,4
	jl        @61
	?debug L 296
@64:
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@mat_auto_sub$qp9AD_Matrixt1	endp
@mat_auto_sub$qp9AD_Matrixt1	ends
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
	dd	?patch58
	dd	?patch59
	dd	?patch60
	df	@@mat_auto_sub$qp9AD_Matrixt1
	dw	0
	dw	4134
	dw	0
	dw	101
	dw	0
	dw	0
	dw	0
	db	28
	db	64
	db	109
	db	97
	db	116
	db	95
	db	97
	db	117
	db	116
	db	111
	db	95
	db	115
	db	117
	db	98
	db	36
	db	113
	db	112
	db	57
	db	65
	db	68
	db	95
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	db	116
	db	49
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	102
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65535
	dw	65535
	dw	103
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	116
	dw	0
	dw	104
	dw	0
	dw	0
	dw	0
?patch58	equ	@64-@@mat_auto_sub$qp9AD_Matrixt1+3
?patch59	equ	0
?patch60	equ	@64-@@mat_auto_sub$qp9AD_Matrixt1
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@mat_mul_C$qp9AD_Matrixt1t1	segment virtual
@@mat_mul_C$qp9AD_Matrixt1t1	proc	near
?live16405@0:
	?debug L 300
	push      ebp
	mov       ebp,esp
	add       esp,-68
	?debug L 305
@65:
	xor       eax,eax
	mov       dword ptr [ebp-4],eax
	?debug L 309
@66:
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+edx]
	mov       eax,dword ptr [ebp+12]
	fmul      dword ptr [eax]
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+edx+4]
	mov       eax,dword ptr [ebp+12]
	fmul      dword ptr [eax+16]
	faddp      st(1),st
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+edx+8]
	mov       eax,dword ptr [ebp+12]
	fmul      dword ptr [eax+32]
	faddp      st(1),st
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+edx+12]
	mov       eax,dword ptr [ebp+12]
	fmul      dword ptr [eax+48]
	faddp      st(1),st
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	fstp      dword ptr [ebp+edx-68]
	?debug L 314
	mov       ecx,dword ptr [ebp-4]
	shl       ecx,4
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax+ecx]
	mov       edx,dword ptr [ebp+12]
	fmul      dword ptr [edx+4]
	mov       ecx,dword ptr [ebp-4]
	shl       ecx,4
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax+ecx+4]
	mov       edx,dword ptr [ebp+12]
	fmul      dword ptr [edx+20]
	faddp      st(1),st
	mov       ecx,dword ptr [ebp-4]
	shl       ecx,4
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax+ecx+8]
	mov       edx,dword ptr [ebp+12]
	fmul      dword ptr [edx+36]
	faddp      st(1),st
	mov       ecx,dword ptr [ebp-4]
	shl       ecx,4
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax+ecx+12]
	mov       edx,dword ptr [ebp+12]
	fmul      dword ptr [edx+52]
	faddp      st(1),st
	mov       ecx,dword ptr [ebp-4]
	shl       ecx,4
	fstp      dword ptr [ebp+ecx-64]
	?debug L 319
	mov       eax,dword ptr [ebp-4]
	shl       eax,4
	mov       edx,dword ptr [ebp+8]
	fld       dword ptr [edx+eax]
	mov       ecx,dword ptr [ebp+12]
	fmul      dword ptr [ecx+8]
	mov       eax,dword ptr [ebp-4]
	shl       eax,4
	mov       edx,dword ptr [ebp+8]
	fld       dword ptr [edx+eax+4]
	mov       ecx,dword ptr [ebp+12]
	fmul      dword ptr [ecx+24]
	faddp      st(1),st
	mov       eax,dword ptr [ebp-4]
	shl       eax,4
	mov       edx,dword ptr [ebp+8]
	fld       dword ptr [edx+eax+8]
	mov       ecx,dword ptr [ebp+12]
	fmul      dword ptr [ecx+40]
	faddp      st(1),st
	mov       eax,dword ptr [ebp-4]
	shl       eax,4
	mov       edx,dword ptr [ebp+8]
	fld       dword ptr [edx+eax+12]
	mov       ecx,dword ptr [ebp+12]
	fmul      dword ptr [ecx+56]
	faddp      st(1),st
	mov       eax,dword ptr [ebp-4]
	shl       eax,4
	fstp      dword ptr [ebp+eax-60]
	?debug L 324
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+edx]
	mov       eax,dword ptr [ebp+12]
	fmul      dword ptr [eax+12]
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+edx+4]
	mov       eax,dword ptr [ebp+12]
	fmul      dword ptr [eax+28]
	faddp      st(1),st
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+edx+8]
	mov       eax,dword ptr [ebp+12]
	fmul      dword ptr [eax+44]
	faddp      st(1),st
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+edx+12]
	mov       eax,dword ptr [ebp+12]
	fmul      dword ptr [eax+60]
	faddp      st(1),st
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	fstp      dword ptr [ebp+edx-56]
	inc       dword ptr [ebp-4]
	mov       ecx,dword ptr [ebp-4]
	cmp       ecx,4
	jl        @66
	?debug L 329
	push      dword ptr [ebp+16]
	lea       eax,dword ptr [ebp-68]
	push      eax
	call      @@mat_copy$qp9AD_Matrixt1
	add       esp,8
	?debug L 330
@69:
	mov       esp,ebp
	pop       ebp
	ret 
	?debug L 0
@@mat_mul_C$qp9AD_Matrixt1t1	endp
@mat_mul_C$qp9AD_Matrixt1t1	ends
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
	dd	?patch61
	dd	?patch62
	dd	?patch63
	df	@@mat_mul_C$qp9AD_Matrixt1t1
	dw	0
	dw	4136
	dw	0
	dw	105
	dw	0
	dw	0
	dw	0
	db	27
	db	64
	db	109
	db	97
	db	116
	db	95
	db	109
	db	117
	db	108
	db	95
	db	67
	db	36
	db	113
	db	112
	db	57
	db	65
	db	68
	db	95
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	db	116
	db	49
	db	116
	db	49
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	106
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65535
	dw	65535
	dw	107
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	65535
	dw	65535
	dw	108
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	116
	dw	0
	dw	109
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65468
	dw	65535
	dw	65532
	dw	65535
	dw	110
	dw	0
	dw	0
	dw	0
?patch61	equ	@69-@@mat_mul_C$qp9AD_Matrixt1t1+4
?patch62	equ	0
?patch63	equ	@69-@@mat_mul_C$qp9AD_Matrixt1t1
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@mat_mulaffine$qp9AD_Matrixt1t1	segment virtual
@@mat_mulaffine$qp9AD_Matrixt1t1	proc	near
?live16406@0:
	?debug L 559
	push      ebp
	mov       ebp,esp
	add       esp,-72
	?debug L 564
@70:
	xor       eax,eax
	mov       dword ptr [ebp-4],eax
	?debug L 565
@71:
	xor       edx,edx
	mov       dword ptr [ebp-8],edx
	?debug L 568
@73:
	mov       ecx,dword ptr [ebp-4]
	shl       ecx,4
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax+ecx]
	mov       edx,dword ptr [ebp+12]
	mov       ecx,dword ptr [ebp-8]
	fmul      dword ptr [edx+4*ecx]
	mov       eax,dword ptr [ebp-4]
	shl       eax,4
	mov       edx,dword ptr [ebp+8]
	fld       dword ptr [edx+eax+4]
	mov       ecx,dword ptr [ebp+12]
	mov       eax,dword ptr [ebp-8]
	fmul      dword ptr [ecx+4*eax+16]
	faddp      st(1),st
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+edx+8]
	mov       eax,dword ptr [ebp+12]
	mov       edx,dword ptr [ebp-8]
	fmul      dword ptr [eax+4*edx+32]
	faddp      st(1),st
	mov       ecx,dword ptr [ebp-4]
	shl       ecx,4
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax+ecx+12]
	mov       edx,dword ptr [ebp+12]
	mov       ecx,dword ptr [ebp-8]
	fmul      dword ptr [edx+4*ecx+48]
	faddp      st(1),st
	mov       eax,dword ptr [ebp-4]
	shl       eax,4
	lea       edx,dword ptr [ebp+eax-72]
	mov       ecx,dword ptr [ebp-8]
	fstp      dword ptr [edx+4*ecx]
	inc       dword ptr [ebp-8]
	mov       eax,dword ptr [ebp-8]
	cmp       eax,3
	jl        short @73
	inc       dword ptr [ebp-4]
	mov       edx,dword ptr [ebp-4]
	cmp       edx,3
	jl        @71
	?debug L 573
	push      dword ptr [ebp+16]
	lea       ecx,dword ptr [ebp-72]
	push      ecx
	call      @@mat_copy$qp9AD_Matrixt1
	add       esp,8
	?debug L 574
@77:
	mov       esp,ebp
	pop       ebp
	ret 
	?debug L 0
@@mat_mulaffine$qp9AD_Matrixt1t1	endp
@mat_mulaffine$qp9AD_Matrixt1t1	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	78
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch64
	dd	?patch65
	dd	?patch66
	df	@@mat_mulaffine$qp9AD_Matrixt1t1
	dw	0
	dw	4138
	dw	0
	dw	111
	dw	0
	dw	0
	dw	0
	db	31
	db	64
	db	109
	db	97
	db	116
	db	95
	db	109
	db	117
	db	108
	db	97
	db	102
	db	102
	db	105
	db	110
	db	101
	db	36
	db	113
	db	112
	db	57
	db	65
	db	68
	db	95
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	db	116
	db	49
	db	116
	db	49
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	112
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65535
	dw	65535
	dw	113
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	65535
	dw	65535
	dw	114
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65528
	dw	65535
	dw	116
	dw	0
	dw	115
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	116
	dw	0
	dw	116
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65464
	dw	65535
	dw	65532
	dw	65535
	dw	117
	dw	0
	dw	0
	dw	0
?patch64	equ	@77-@@mat_mulaffine$qp9AD_Matrixt1t1+4
?patch65	equ	0
?patch66	equ	@77-@@mat_mulaffine$qp9AD_Matrixt1t1
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@mat_mulvect_C$qp9AD_Matrixp9AD_Vect3Dt2	segment virtual
@@mat_mulvect_C$qp9AD_Matrixp9AD_Vect3Dt2	proc	near
?live16407@0:
	?debug L 578
	push      ebp
	mov       ebp,esp
	?debug L 580
@78:
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax]
	mov       edx,dword ptr [ebp+12]
	fmul      dword ptr [edx]
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+4]
	mov       eax,dword ptr [ebp+12]
	fmul      dword ptr [eax+4]
	faddp      st(1),st
	mov       edx,dword ptr [ebp+8]
	fld       dword ptr [edx+8]
	mov       ecx,dword ptr [ebp+12]
	fmul      dword ptr [ecx+8]
	faddp      st(1),st
	mov       eax,dword ptr [ebp+8]
	fadd      dword ptr [eax+12]
	mov       edx,dword ptr [ebp+16]
	fstp      dword ptr [edx]
	?debug L 581
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+16]
	mov       eax,dword ptr [ebp+12]
	fmul      dword ptr [eax]
	mov       edx,dword ptr [ebp+8]
	fld       dword ptr [edx+20]
	mov       ecx,dword ptr [ebp+12]
	fmul      dword ptr [ecx+4]
	faddp      st(1),st
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax+24]
	mov       edx,dword ptr [ebp+12]
	fmul      dword ptr [edx+8]
	faddp      st(1),st
	mov       ecx,dword ptr [ebp+8]
	fadd      dword ptr [ecx+28]
	mov       eax,dword ptr [ebp+16]
	fstp      dword ptr [eax+4]
	?debug L 582
	mov       edx,dword ptr [ebp+8]
	fld       dword ptr [edx+32]
	mov       ecx,dword ptr [ebp+12]
	fmul      dword ptr [ecx]
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax+36]
	mov       edx,dword ptr [ebp+12]
	fmul      dword ptr [edx+4]
	faddp      st(1),st
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+40]
	mov       eax,dword ptr [ebp+12]
	fmul      dword ptr [eax+8]
	faddp      st(1),st
	mov       edx,dword ptr [ebp+8]
	fadd      dword ptr [edx+44]
	mov       ecx,dword ptr [ebp+16]
	fstp      dword ptr [ecx+8]
	?debug L 583
@79:
	pop       ebp
	ret 
	?debug L 0
@@mat_mulvect_C$qp9AD_Matrixp9AD_Vect3Dt2	endp
@mat_mulvect_C$qp9AD_Matrixp9AD_Vect3Dt2	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	87
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch67
	dd	?patch68
	dd	?patch69
	df	@@mat_mulvect_C$qp9AD_Matrixp9AD_Vect3Dt2
	dw	0
	dw	4140
	dw	0
	dw	118
	dw	0
	dw	0
	dw	0
	db	40
	db	64
	db	109
	db	97
	db	116
	db	95
	db	109
	db	117
	db	108
	db	118
	db	101
	db	99
	db	116
	db	95
	db	67
	db	36
	db	113
	db	112
	db	57
	db	65
	db	68
	db	95
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	db	112
	db	57
	db	65
	db	68
	db	95
	db	86
	db	101
	db	99
	db	116
	db	51
	db	68
	db	116
	db	50
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	119
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65533
	dw	65535
	dw	120
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	65533
	dw	65535
	dw	121
	dw	0
	dw	0
	dw	0
?patch67	equ	@79-@@mat_mulvect_C$qp9AD_Matrixp9AD_Vect3Dt2+2
?patch68	equ	0
?patch69	equ	@79-@@mat_mulvect_C$qp9AD_Matrixp9AD_Vect3Dt2
	dw	2
	dw	6
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@mat_mulvect_asm$qp9AD_Matrixp9AD_Vect3Dt2	segment virtual
@@mat_mulvect_asm$qp9AD_Matrixp9AD_Vect3Dt2	proc	near
?live16408@0:
	?debug L 587
	push      ebp
	mov       ebp,esp
	push      esi
	push      edi
	?debug L 591
@80:
	push	 edx
	?debug L 592
	mov	 esi, [dword ptr [ebp+8]]
	?debug L 593
	mov	 edi, [dword ptr [ebp+12]]
	?debug L 594
	mov	 edx, [dword ptr [ebp+16]]
	?debug L 597
	fldz	
	?debug L 599
	fld	 dword ptr [esi]
	?debug L 600
	fmul	 dword ptr [edi]
	?debug L 601
	fld	 dword ptr [esi+4]
	?debug L 602
	fmul	 dword ptr [edi+4]
	?debug L 604
	fxch	
	?debug L 605
	faddp	 st(2), st
	?debug L 607
	fld	 dword ptr [esi+8]
	?debug L 608
	fmul	 dword ptr [edi+8]
	?debug L 610
	fxch	
	?debug L 611
	faddp	 st(2), st
	?debug L 613
	fadd	
	?debug L 614
	fadd	 dword ptr [esi+12]
	?debug L 616
	fstp	 dword ptr [edx]
	?debug L 619
	fldz	
	?debug L 621
	fld	 dword ptr [esi+16]
	?debug L 622
	fmul	 dword ptr [edi]
	?debug L 623
	fld	 dword ptr [esi+20]
	?debug L 624
	fmul	 dword ptr [edi+4]
	?debug L 626
	fxch	
	?debug L 627
	faddp	 st(2), st
	?debug L 629
	fld	 dword ptr [esi+24]
	?debug L 630
	fmul	 dword ptr [edi+8]
	?debug L 632
	fxch	
	?debug L 633
	faddp	 st(2), st
	?debug L 635
	fadd	
	?debug L 636
	fadd	 dword ptr [esi+28]
	?debug L 638
	fstp	 dword ptr [edx+4]
	?debug L 641
	fldz	
	?debug L 643
	fld	 dword ptr [esi+32]
	?debug L 644
	fmul	 dword ptr [edi]
	?debug L 645
	fld	 dword ptr [esi+36]
	?debug L 646
	fmul	 dword ptr [edi+4]
	?debug L 648
	fxch	
	?debug L 649
	faddp	 st(2), st
	?debug L 651
	fld	 dword ptr [esi+40]
	?debug L 652
	fmul	 dword ptr [edi+8]
	?debug L 654
	fxch	
	?debug L 655
	faddp	 st(2), st
	?debug L 657
	fadd	
	?debug L 658
	fadd	 dword ptr [esi+44]
	?debug L 660
	fstp	 dword ptr [edx+8]
	?debug L 661
	pop	 edx
	?debug L 663
@81:
	pop       edi
	pop       esi
	pop       ebp
	ret 
	?debug L 0
@@mat_mulvect_asm$qp9AD_Matrixp9AD_Vect3Dt2	endp
@mat_mulvect_asm$qp9AD_Matrixp9AD_Vect3Dt2	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	89
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch70
	dd	?patch71
	dd	?patch72
	df	@@mat_mulvect_asm$qp9AD_Matrixp9AD_Vect3Dt2
	dw	0
	dw	4142
	dw	0
	dw	122
	dw	0
	dw	0
	dw	0
	db	42
	db	64
	db	109
	db	97
	db	116
	db	95
	db	109
	db	117
	db	108
	db	118
	db	101
	db	99
	db	116
	db	95
	db	97
	db	115
	db	109
	db	36
	db	113
	db	112
	db	57
	db	65
	db	68
	db	95
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	db	112
	db	57
	db	65
	db	68
	db	95
	db	86
	db	101
	db	99
	db	116
	db	51
	db	68
	db	116
	db	50
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	123
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65533
	dw	65535
	dw	124
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	65533
	dw	65535
	dw	125
	dw	0
	dw	0
	dw	0
?patch70	equ	@81-@@mat_mulvect_asm$qp9AD_Matrixp9AD_Vect3Dt2+4
?patch71	equ	0
?patch72	equ	@81-@@mat_mulvect_asm$qp9AD_Matrixp9AD_Vect3Dt2
	dw	2
	dw	6
	dw	8
	dw	531
	dw	6
	dw	65528
	dw	65535
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@mat_mulvectenv$qp9AD_Matrixp9AD_Vect3Dt2	segment virtual
@@mat_mulvectenv$qp9AD_Matrixp9AD_Vect3Dt2	proc	near
?live16409@0:
	?debug L 722
	push      ebp
	mov       ebp,esp
	push      esi
	push      edi
	?debug L 729
@82:
	push	 edx
	?debug L 730
	mov	 esi, [dword ptr [ebp+8]]
	?debug L 731
	mov	 edi, [dword ptr [ebp+12]]
	?debug L 732
	mov	 edx, [dword ptr [ebp+16]]
	?debug L 735
	fldz	
	?debug L 737
	fld	 dword ptr [esi]
	?debug L 738
	fmul	 dword ptr [edi]
	?debug L 739
	fld	 dword ptr [esi+4]
	?debug L 740
	fmul	 dword ptr [edi+4]
	?debug L 742
	fxch	
	?debug L 743
	faddp	 st(2), st
	?debug L 745
	fld	 dword ptr [esi+8]
	?debug L 746
	fmul	 dword ptr [edi+8]
	?debug L 748
	fxch	
	?debug L 749
	faddp	 st(2), st
	?debug L 751
	fadd	
	?debug L 752
	fstp	 dword ptr [edx]
	?debug L 755
	fldz	
	?debug L 757
	fld	 dword ptr [esi+16]
	?debug L 758
	fmul	 dword ptr [edi]
	?debug L 759
	fld	 dword ptr [esi+20]
	?debug L 760
	fmul	 dword ptr [edi+4]
	?debug L 762
	fxch	
	?debug L 763
	faddp	 st(2), st
	?debug L 765
	fld	 dword ptr [esi+24]
	?debug L 766
	fmul	 dword ptr [edi+8]
	?debug L 768
	fxch	
	?debug L 769
	faddp	 st(2), st
	?debug L 771
	fadd	
	?debug L 772
	fstp	 dword ptr [edx+4]
	?debug L 773
	pop	 edx
	?debug L 775
@83:
	pop       edi
	pop       esi
	pop       ebp
	ret 
	?debug L 0
@@mat_mulvectenv$qp9AD_Matrixp9AD_Vect3Dt2	endp
@mat_mulvectenv$qp9AD_Matrixp9AD_Vect3Dt2	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	88
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch73
	dd	?patch74
	dd	?patch75
	df	@@mat_mulvectenv$qp9AD_Matrixp9AD_Vect3Dt2
	dw	0
	dw	4144
	dw	0
	dw	126
	dw	0
	dw	0
	dw	0
	db	41
	db	64
	db	109
	db	97
	db	116
	db	95
	db	109
	db	117
	db	108
	db	118
	db	101
	db	99
	db	116
	db	101
	db	110
	db	118
	db	36
	db	113
	db	112
	db	57
	db	65
	db	68
	db	95
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	db	112
	db	57
	db	65
	db	68
	db	95
	db	86
	db	101
	db	99
	db	116
	db	51
	db	68
	db	116
	db	50
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	127
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65533
	dw	65535
	dw	128
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	65533
	dw	65535
	dw	129
	dw	0
	dw	0
	dw	0
?patch73	equ	@83-@@mat_mulvectenv$qp9AD_Matrixp9AD_Vect3Dt2+4
?patch74	equ	0
?patch75	equ	@83-@@mat_mulvectenv$qp9AD_Matrixp9AD_Vect3Dt2
	dw	2
	dw	6
	dw	8
	dw	531
	dw	6
	dw	65528
	dw	65535
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@mat_mulvectaffine$qp9AD_Matrixp9AD_Vect3Dt2	segment virtual
@@mat_mulvectaffine$qp9AD_Matrixp9AD_Vect3Dt2	proc	near
?live16410@0:
	?debug L 779
	push      ebp
	mov       ebp,esp
	push      esi
	push      edi
	?debug L 788
@84:
	push	 edx
	?debug L 789
	mov	 esi, [dword ptr [ebp+8]]
	?debug L 790
	mov	 edi, [dword ptr [ebp+12]]
	?debug L 791
	mov	 edx, [dword ptr [ebp+16]]
	?debug L 794
	fldz	
	?debug L 796
	fld	 dword ptr [esi]
	?debug L 797
	fmul	 dword ptr [edi]
	?debug L 798
	fld	 dword ptr [esi+4]
	?debug L 799
	fmul	 dword ptr [edi+4]
	?debug L 801
	fxch	
	?debug L 802
	faddp	 st(2), st
	?debug L 804
	fld	 dword ptr [esi+8]
	?debug L 805
	fmul	 dword ptr [edi+8]
	?debug L 807
	fxch	
	?debug L 808
	faddp	 st(2), st
	?debug L 810
	fadd	
	?debug L 811
	fstp	 dword ptr [edx]
	?debug L 814
	fldz	
	?debug L 816
	fld	 dword ptr [esi+16]
	?debug L 817
	fmul	 dword ptr [edi]
	?debug L 818
	fld	 dword ptr [esi+20]
	?debug L 819
	fmul	 dword ptr [edi+4]
	?debug L 821
	fxch	
	?debug L 822
	faddp	 st(2), st
	?debug L 824
	fld	 dword ptr [esi+24]
	?debug L 825
	fmul	 dword ptr [edi+8]
	?debug L 827
	fxch	
	?debug L 828
	faddp	 st(2), st
	?debug L 830
	fadd	
	?debug L 831
	fstp	 dword ptr [edx+4]
	?debug L 834
	fldz	
	?debug L 836
	fld	 dword ptr [esi+32]
	?debug L 837
	fmul	 dword ptr [edi]
	?debug L 838
	fld	 dword ptr [esi+36]
	?debug L 839
	fmul	 dword ptr [edi+4]
	?debug L 841
	fxch	
	?debug L 842
	faddp	 st(2), st
	?debug L 844
	fld	 dword ptr [esi+40]
	?debug L 845
	fmul	 dword ptr [edi+8]
	?debug L 847
	fxch	
	?debug L 848
	faddp	 st(2), st
	?debug L 850
	fadd	
	?debug L 851
	fstp	 dword ptr [edx+8]
	?debug L 852
	pop	 edx
	?debug L 854
@85:
	pop       edi
	pop       esi
	pop       ebp
	ret 
	?debug L 0
@@mat_mulvectaffine$qp9AD_Matrixp9AD_Vect3Dt2	endp
@mat_mulvectaffine$qp9AD_Matrixp9AD_Vect3Dt2	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	91
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch76
	dd	?patch77
	dd	?patch78
	df	@@mat_mulvectaffine$qp9AD_Matrixp9AD_Vect3Dt2
	dw	0
	dw	4146
	dw	0
	dw	130
	dw	0
	dw	0
	dw	0
	db	44
	db	64
	db	109
	db	97
	db	116
	db	95
	db	109
	db	117
	db	108
	db	118
	db	101
	db	99
	db	116
	db	97
	db	102
	db	102
	db	105
	db	110
	db	101
	db	36
	db	113
	db	112
	db	57
	db	65
	db	68
	db	95
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	db	112
	db	57
	db	65
	db	68
	db	95
	db	86
	db	101
	db	99
	db	116
	db	51
	db	68
	db	116
	db	50
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	131
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65533
	dw	65535
	dw	132
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	65533
	dw	65535
	dw	133
	dw	0
	dw	0
	dw	0
?patch76	equ	@85-@@mat_mulvectaffine$qp9AD_Matrixp9AD_Vect3Dt2+4
?patch77	equ	0
?patch78	equ	@85-@@mat_mulvectaffine$qp9AD_Matrixp9AD_Vect3Dt2
	dw	2
	dw	6
	dw	8
	dw	531
	dw	6
	dw	65528
	dw	65535
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@mat_swap_rows$qp9AD_Matrixii	segment virtual
@@mat_swap_rows$qp9AD_Matrixii	proc	near
?live16411@0:
	?debug L 858
	push      ebp
	mov       ebp,esp
	add       esp,-8
	?debug L 863
@86:
	xor       eax,eax
	mov       dword ptr [ebp-4],eax
	?debug L 865
@87:
	mov       edx,dword ptr [ebp+12]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	lea       eax,dword ptr [ecx+edx]
	mov       edx,dword ptr [ebp-4]
	mov       ecx,dword ptr [eax+4*edx]
	mov       dword ptr [ebp-8],ecx
	?debug L 866
	mov       eax,dword ptr [ebp+16]
	shl       eax,4
	mov       edx,dword ptr [ebp+8]
	lea       ecx,dword ptr [edx+eax]
	mov       eax,dword ptr [ebp-4]
	mov       edx,dword ptr [ecx+4*eax]
	mov       ecx,dword ptr [ebp+12]
	shl       ecx,4
	mov       eax,dword ptr [ebp+8]
	lea       ecx,dword ptr [eax+ecx]
	mov       eax,dword ptr [ebp-4]
	mov       dword ptr [ecx+4*eax],edx
	?debug L 867
	mov       edx,dword ptr [ebp+16]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	lea       eax,dword ptr [ecx+edx]
	mov       edx,dword ptr [ebp-4]
	mov       ecx,dword ptr [ebp-8]
	mov       dword ptr [eax+4*edx],ecx
	inc       dword ptr [ebp-4]
	mov       eax,dword ptr [ebp-4]
	cmp       eax,4
	jl        short @87
	?debug L 869
@90:
	pop       ecx
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@mat_swap_rows$qp9AD_Matrixii	endp
@mat_swap_rows$qp9AD_Matrixii	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	76
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch79
	dd	?patch80
	dd	?patch81
	df	@@mat_swap_rows$qp9AD_Matrixii
	dw	0
	dw	4148
	dw	0
	dw	134
	dw	0
	dw	0
	dw	0
	db	29
	db	64
	db	109
	db	97
	db	116
	db	95
	db	115
	db	119
	db	97
	db	112
	db	95
	db	114
	db	111
	db	119
	db	115
	db	36
	db	113
	db	112
	db	57
	db	65
	db	68
	db	95
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	db	105
	db	105
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	135
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	116
	dw	0
	dw	136
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	116
	dw	0
	dw	137
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65528
	dw	65535
	dw	64
	dw	0
	dw	138
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	116
	dw	0
	dw	139
	dw	0
	dw	0
	dw	0
?patch79	equ	@90-@@mat_swap_rows$qp9AD_Matrixii+4
?patch80	equ	0
?patch81	equ	@90-@@mat_swap_rows$qp9AD_Matrixii
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@mat_swap_columns$qp9AD_Matrixii	segment virtual
@@mat_swap_columns$qp9AD_Matrixii	proc	near
?live16412@0:
	?debug L 873
	push      ebp
	mov       ebp,esp
	add       esp,-8
	?debug L 878
@91:
	xor       eax,eax
	mov       dword ptr [ebp-4],eax
	?debug L 880
@92:
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	lea       eax,dword ptr [ecx+edx]
	mov       edx,dword ptr [ebp+12]
	mov       ecx,dword ptr [eax+4*edx]
	mov       dword ptr [ebp-8],ecx
	?debug L 881
	mov       eax,dword ptr [ebp-4]
	shl       eax,4
	mov       edx,dword ptr [ebp+8]
	lea       ecx,dword ptr [edx+eax]
	mov       eax,dword ptr [ebp+16]
	mov       edx,dword ptr [ecx+4*eax]
	mov       ecx,dword ptr [ebp-4]
	shl       ecx,4
	mov       eax,dword ptr [ebp+8]
	lea       ecx,dword ptr [eax+ecx]
	mov       eax,dword ptr [ebp+12]
	mov       dword ptr [ecx+4*eax],edx
	?debug L 882
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	lea       eax,dword ptr [ecx+edx]
	mov       edx,dword ptr [ebp+16]
	mov       ecx,dword ptr [ebp-8]
	mov       dword ptr [eax+4*edx],ecx
	inc       dword ptr [ebp-4]
	mov       eax,dword ptr [ebp-4]
	cmp       eax,4
	jl        short @92
	?debug L 884
@95:
	pop       ecx
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@mat_swap_columns$qp9AD_Matrixii	endp
@mat_swap_columns$qp9AD_Matrixii	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	79
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch82
	dd	?patch83
	dd	?patch84
	df	@@mat_swap_columns$qp9AD_Matrixii
	dw	0
	dw	4150
	dw	0
	dw	140
	dw	0
	dw	0
	dw	0
	db	32
	db	64
	db	109
	db	97
	db	116
	db	95
	db	115
	db	119
	db	97
	db	112
	db	95
	db	99
	db	111
	db	108
	db	117
	db	109
	db	110
	db	115
	db	36
	db	113
	db	112
	db	57
	db	65
	db	68
	db	95
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	db	105
	db	105
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	141
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	116
	dw	0
	dw	142
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	116
	dw	0
	dw	143
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65528
	dw	65535
	dw	64
	dw	0
	dw	144
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	116
	dw	0
	dw	145
	dw	0
	dw	0
	dw	0
?patch82	equ	@95-@@mat_swap_columns$qp9AD_Matrixii+4
?patch83	equ	0
?patch84	equ	@95-@@mat_swap_columns$qp9AD_Matrixii
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@mat_inverse$qp9AD_Matrixt1	segment virtual
@@mat_inverse$qp9AD_Matrixt1	proc	near
?live16413@0:
	?debug L 888
	push      ebp
	mov       ebp,esp
	add       esp,-96
	?debug L 895
@96:
	mov       eax,dword ptr [ebp+8]
	test      eax,eax
	je        short @98
	mov       edx,dword ptr [ebp+12]
	test      edx,edx
	jne       short @97
@98:
	xor       eax,eax
	jmp       @99
	?debug L 897
@97:
	lea       edx,dword ptr [ebp-88]
	push      edx
	push      dword ptr [ebp+8]
	call      @@mat_copy$qp9AD_Matrixt1
	add       esp,8
	?debug L 898
	push      dword ptr [ebp+12]
	call      @@mat_identity$qp9AD_Matrix
	pop       ecx
	?debug L 899
	xor       ecx,ecx
	mov       dword ptr [ebp-16],ecx
	?debug L 900
	xor       eax,eax
	mov       dword ptr [ebp-20],eax
	?debug L 901
	xor       edx,edx
	mov       dword ptr [ebp-4],edx
	?debug L 905
@100:
	mov       ecx,dword ptr [ebp-4]
	shl       ecx,4
	lea       eax,dword ptr [ebp+ecx-88]
	mov       edx,dword ptr [ebp-4]
	mov       ecx,dword ptr [eax+4*edx]
	mov       dword ptr [ebp-20],ecx
	?debug L 906
	mov       eax,dword ptr [ebp-4]
	mov       dword ptr [ebp-16],eax
	?debug L 907
	mov       edx,dword ptr [ebp-4]
	inc       edx
	mov       dword ptr [ebp-8],edx
	mov       ecx,dword ptr [ebp-8]
	cmp       ecx,4
	jge       short @103
	?debug L 909
@102:
	mov       eax,dword ptr [ebp-8]
	shl       eax,4
	lea       edx,dword ptr [ebp+eax-88]
	mov       ecx,dword ptr [ebp-4]
	mov       eax,dword ptr [edx+4*ecx]
	mov       dword ptr [ebp-24],eax
	?debug L 910
	fld       dword ptr [ebp-24]
	add       esp,-8
	fstp      qword ptr [esp]
	call      @_fabs
	add       esp,8
	fstp      qword ptr [ebp-96]
	fld       dword ptr [ebp-20]
	add       esp,-8
	fstp      qword ptr [esp]
	call      @_fabs
	add       esp,8
	fcomp     qword ptr [ebp-96]
	fnstsw ax
	sahf
	jae       short @104
	?debug L 912
	mov       edx,dword ptr [ebp-24]
	mov       dword ptr [ebp-20],edx
	?debug L 913
	mov       ecx,dword ptr [ebp-4]
	mov       dword ptr [ebp-16],ecx
@104:
	inc       dword ptr [ebp-8]
	mov       eax,dword ptr [ebp-8]
	cmp       eax,4
	jl        short @102
	?debug L 919
@103:
	mov       edx,dword ptr [ebp-16]
	mov       ecx,dword ptr [ebp-4]
	cmp       edx,ecx
	je        short @106
	?debug L 921
	push      dword ptr [ebp-16]
	push      dword ptr [ebp-4]
	lea       eax,dword ptr [ebp-88]
	push      eax
	call      @@mat_swap_rows$qp9AD_Matrixii
	add       esp,12
	?debug L 922
	push      dword ptr [ebp-16]
	push      dword ptr [ebp-4]
	push      dword ptr [ebp+12]
	call      @@mat_swap_rows$qp9AD_Matrixii
	add       esp,12
	?debug L 927
@106:
	fld       dword ptr [@107]
	fdiv      dword ptr [ebp-20]
	fstp      dword ptr [ebp-20]
	?debug L 928
	xor       edx,edx
	mov       dword ptr [ebp-12],edx
	?debug L 930
@108:
	fld       dword ptr [ebp-20]
	mov       ecx,dword ptr [ebp-4]
	shl       ecx,4
	lea       eax,dword ptr [ebp+ecx-88]
	mov       edx,dword ptr [ebp-12]
	fmul      dword ptr [eax+4*edx]
	fstp      dword ptr [eax+4*edx]
	?debug L 931
	fld       dword ptr [ebp-20]
	mov       ecx,dword ptr [ebp-4]
	shl       ecx,4
	mov       eax,dword ptr [ebp+12]
	lea       edx,dword ptr [eax+ecx]
	mov       ecx,dword ptr [ebp-12]
	fmul      dword ptr [edx+4*ecx]
	fstp      dword ptr [edx+4*ecx]
	inc       dword ptr [ebp-12]
	mov       eax,dword ptr [ebp-12]
	cmp       eax,4
	jl        short @108
	?debug L 937
	mov       edx,dword ptr [ebp-4]
	inc       edx
	mov       dword ptr [ebp-8],edx
	mov       ecx,dword ptr [ebp-8]
	cmp       ecx,4
	jge       short @112
	?debug L 939
@111:
	mov       eax,dword ptr [ebp-8]
	shl       eax,4
	lea       edx,dword ptr [ebp+eax-88]
	mov       ecx,dword ptr [ebp-4]
	mov       eax,dword ptr [edx+4*ecx]
	mov       dword ptr [ebp-24],eax
	?debug L 940
	xor       edx,edx
	mov       dword ptr [ebp-12],edx
	?debug L 942
@113:
	mov       ecx,dword ptr [ebp-4]
	shl       ecx,4
	lea       eax,dword ptr [ebp+ecx-88]
	mov       edx,dword ptr [ebp-12]
	fld       dword ptr [eax+4*edx]
	fmul      dword ptr [ebp-24]
	mov       ecx,dword ptr [ebp-8]
	shl       ecx,4
	lea       eax,dword ptr [ebp+ecx-88]
	mov       edx,dword ptr [ebp-12]
	fsubr     dword ptr [eax+4*edx]
	fstp      dword ptr [eax+4*edx]
	?debug L 943
	mov       ecx,dword ptr [ebp-4]
	shl       ecx,4
	mov       eax,dword ptr [ebp+12]
	lea       edx,dword ptr [eax+ecx]
	mov       ecx,dword ptr [ebp-12]
	fld       dword ptr [edx+4*ecx]
	fmul      dword ptr [ebp-24]
	mov       eax,dword ptr [ebp-8]
	shl       eax,4
	mov       edx,dword ptr [ebp+12]
	lea       ecx,dword ptr [edx+eax]
	mov       eax,dword ptr [ebp-12]
	fsubr     dword ptr [ecx+4*eax]
	fstp      dword ptr [ecx+4*eax]
	inc       dword ptr [ebp-12]
	mov       edx,dword ptr [ebp-12]
	cmp       edx,4
	jl        short @113
	inc       dword ptr [ebp-8]
	mov       ecx,dword ptr [ebp-8]
	cmp       ecx,4
	jl        short @111
@112:
	inc       dword ptr [ebp-4]
	mov       eax,dword ptr [ebp-4]
	cmp       eax,4
	jl        @100
	?debug L 950
	mov       dword ptr [ebp-4],3
	?debug L 952
@118:
	mov       edx,dword ptr [ebp-4]
	dec       edx
	mov       dword ptr [ebp-8],edx
	mov       ecx,dword ptr [ebp-8]
	test      ecx,ecx
	jl        short @121
	?debug L 954
@120:
	mov       eax,dword ptr [ebp-8]
	shl       eax,4
	lea       edx,dword ptr [ebp+eax-88]
	mov       ecx,dword ptr [ebp-4]
	mov       eax,dword ptr [edx+4*ecx]
	mov       dword ptr [ebp-24],eax
	?debug L 955
	xor       edx,edx
	mov       dword ptr [ebp-12],edx
	?debug L 957
@122:
	mov       ecx,dword ptr [ebp-4]
	shl       ecx,4
	lea       eax,dword ptr [ebp+ecx-88]
	mov       edx,dword ptr [ebp-12]
	fld       dword ptr [eax+4*edx]
	fmul      dword ptr [ebp-24]
	mov       ecx,dword ptr [ebp-8]
	shl       ecx,4
	lea       eax,dword ptr [ebp+ecx-88]
	mov       edx,dword ptr [ebp-12]
	fsubr     dword ptr [eax+4*edx]
	fstp      dword ptr [eax+4*edx]
	?debug L 958
	mov       ecx,dword ptr [ebp-4]
	shl       ecx,4
	mov       eax,dword ptr [ebp+12]
	lea       edx,dword ptr [eax+ecx]
	mov       ecx,dword ptr [ebp-12]
	fld       dword ptr [edx+4*ecx]
	fmul      dword ptr [ebp-24]
	mov       eax,dword ptr [ebp-8]
	shl       eax,4
	mov       edx,dword ptr [ebp+12]
	lea       ecx,dword ptr [edx+eax]
	mov       eax,dword ptr [ebp-12]
	fsubr     dword ptr [ecx+4*eax]
	fstp      dword ptr [ecx+4*eax]
	inc       dword ptr [ebp-12]
	mov       edx,dword ptr [ebp-12]
	cmp       edx,4
	jl        short @122
	dec       dword ptr [ebp-8]
	mov       ecx,dword ptr [ebp-8]
	test      ecx,ecx
	jge       short @120
@121:
	dec       dword ptr [ebp-4]
	mov       eax,dword ptr [ebp-4]
	test      eax,eax
	jge       @118
	?debug L 962
	xor       eax,eax
	?debug L 963
@127:
@99:
	mov       esp,ebp
	pop       ebp
	ret 
	align 4        
@107:
	db        0,0,128,63
	?debug L 0
@@mat_inverse$qp9AD_Matrixt1	endp
@mat_inverse$qp9AD_Matrixt1	ends
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
	dd	?patch85
	dd	?patch86
	dd	?patch87
	df	@@mat_inverse$qp9AD_Matrixt1
	dw	0
	dw	4152
	dw	0
	dw	146
	dw	0
	dw	0
	dw	0
	db	27
	db	64
	db	109
	db	97
	db	116
	db	95
	db	105
	db	110
	db	118
	db	101
	db	114
	db	115
	db	101
	db	36
	db	113
	db	112
	db	57
	db	65
	db	68
	db	95
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	db	116
	db	49
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	147
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65535
	dw	65535
	dw	148
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65448
	dw	65535
	dw	65532
	dw	65535
	dw	149
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65512
	dw	65535
	dw	64
	dw	0
	dw	150
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65516
	dw	65535
	dw	64
	dw	0
	dw	151
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65520
	dw	65535
	dw	116
	dw	0
	dw	152
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65524
	dw	65535
	dw	116
	dw	0
	dw	153
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65528
	dw	65535
	dw	116
	dw	0
	dw	154
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	116
	dw	0
	dw	155
	dw	0
	dw	0
	dw	0
?patch85	equ	@127-@@mat_inverse$qp9AD_Matrixt1+10
?patch86	equ	0
?patch87	equ	@127-@@mat_inverse$qp9AD_Matrixt1
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@mat_eigen$qp9AD_Matrixp9AD_Vect3Dt1i	segment virtual
@@mat_eigen$qp9AD_Matrixp9AD_Vect3Dt1i	proc	near
?live16414@0:
	?debug L 974
	push      ebp
	mov       ebp,esp
	add       esp,-176
	?debug L 982
@128:
	mov       dword ptr [ebp-64],3
	?debug L 984
	lea       eax,dword ptr [ebp-136]
	push      eax
	push      dword ptr [ebp+8]
	call      @@mat_copy$qp9AD_Matrixt1
	add       esp,8
	?debug L 986
	mov       edx,dword ptr [ebp+12]
	mov       dword ptr [ebp-60],edx
	?debug L 987
	push      dword ptr [ebp+16]
	call      @@mat_identity$qp9AD_Matrix
	pop       ecx
	?debug L 988
	fld       dword ptr [@129]
	fst       dword ptr [ebp-152]
	fst       dword ptr [ebp-156]
	fstp      dword ptr [ebp-160]
	?debug L 989
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx]
	mov       eax,dword ptr [ebp-60]
	fst       dword ptr [eax]
	fstp      dword ptr [ebp-148]
	?debug L 990
	mov       edx,dword ptr [ebp+8]
	fld       dword ptr [edx+20]
	mov       ecx,dword ptr [ebp-60]
	fst       dword ptr [ecx+4]
	fstp      dword ptr [ebp-144]
	?debug L 991
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax+40]
	mov       edx,dword ptr [ebp-60]
	fst       dword ptr [edx+8]
	fstp      dword ptr [ebp-140]
	?debug L 992
	xor       ecx,ecx
	mov       dword ptr [ebp-20],ecx
	?debug L 994
	xor       eax,eax
	mov       dword ptr [ebp-16],eax
	?debug L 996
@130:
	xor       edx,edx
	mov       dword ptr [ebp-40],edx
	?debug L 997
	xor       ecx,ecx
	mov       dword ptr [ebp-12],ecx
	jmp       short @133
	?debug L 999
@132:
	mov       eax,dword ptr [ebp-12]
	inc       eax
	mov       dword ptr [ebp-8],eax
	mov       edx,dword ptr [ebp-8]
	mov       ecx,dword ptr [ebp-64]
	cmp       edx,ecx
	jge       short @135
	?debug L 1000
@134:
	mov       eax,dword ptr [ebp-12]
	shl       eax,4
	mov       edx,dword ptr [ebp+8]
	lea       ecx,dword ptr [edx+eax]
	mov       eax,dword ptr [ebp-8]
	fld       dword ptr [ecx+4*eax]
	add       esp,-8
	fstp      qword ptr [esp]
	call      @_fabs
	add       esp,8
	fadd      dword ptr [ebp-40]
	fstp      dword ptr [ebp-40]
	inc       dword ptr [ebp-8]
	mov       edx,dword ptr [ebp-8]
	mov       ecx,dword ptr [ebp-64]
	cmp       edx,ecx
	jl        short @134
@135:
	inc       dword ptr [ebp-12]
@133:
	mov       eax,dword ptr [ebp-64]
	dec       eax
	mov       edx,dword ptr [ebp-12]
	cmp       eax,edx
	jg        short @132
	?debug L 1002
	fld       dword ptr [ebp-40]
	fcomp     dword ptr [@129]
	fnstsw ax
	sahf
	je        @139
	?debug L 1003
	mov       edx,dword ptr [ebp-16]
	cmp       edx,4
	jge       short @140
	fld       dword ptr [@129+4]
	fmul      dword ptr [ebp-40]
	mov       ecx,dword ptr [ebp-64]
	imul      ecx,dword ptr [ebp-64]
	mov       dword ptr [ebp-164],ecx
	fild      dword ptr [ebp-164]
	fdivp      st(1),st
	fstp      dword ptr [ebp-24]
	jmp       short @141
	?debug L 1004
@140:
	xor       eax,eax
	mov       dword ptr [ebp-24],eax
	?debug L 1006
@141:
	xor       edx,edx
	mov       dword ptr [ebp-12],edx
	jmp       @143
	?debug L 1008
@142:
	mov       ecx,dword ptr [ebp-12]
	inc       ecx
	mov       dword ptr [ebp-8],ecx
	mov       eax,dword ptr [ebp-8]
	mov       edx,dword ptr [ebp-64]
	cmp       eax,edx
	jge       @145
	?debug L 1010
@144:
	mov       ecx,dword ptr [ebp-12]
	shl       ecx,4
	mov       eax,dword ptr [ebp+8]
	lea       edx,dword ptr [eax+ecx]
	mov       ecx,dword ptr [ebp-8]
	fld       dword ptr [edx+4*ecx]
	add       esp,-8
	fstp      qword ptr [esp]
	call      @_fabs
	add       esp,8
	fmul      dword ptr [@129+8]
	fstp      dword ptr [ebp-52]
	?debug L 1011
	mov       eax,dword ptr [ebp-16]
	cmp       eax,4
	jle       @146
	mov       edx,dword ptr [ebp-60]
	mov       ecx,dword ptr [ebp-12]
	fld       dword ptr [edx+4*ecx]
	add       esp,-8
	fstp      qword ptr [esp]
	call      @_fabs
	add       esp,8
	fadd      dword ptr [ebp-52]
	fstp      qword ptr [ebp-168]
	mov       eax,dword ptr [ebp-60]
	mov       edx,dword ptr [ebp-12]
	fld       dword ptr [eax+4*edx]
	add       esp,-8
	fstp      qword ptr [esp]
	call      @_fabs
	add       esp,8
	fcomp     qword ptr [ebp-168]
	fnstsw ax
	sahf
	jne       short @146
	mov       edx,dword ptr [ebp-60]
	mov       ecx,dword ptr [ebp-8]
	fld       dword ptr [edx+4*ecx]
	add       esp,-8
	fstp      qword ptr [esp]
	call      @_fabs
	add       esp,8
	fadd      dword ptr [ebp-52]
	fstp      qword ptr [ebp-176]
	mov       eax,dword ptr [ebp-60]
	mov       edx,dword ptr [ebp-8]
	fld       dword ptr [eax+4*edx]
	add       esp,-8
	fstp      qword ptr [esp]
	call      @_fabs
	add       esp,8
	fcomp     qword ptr [ebp-176]
	fnstsw ax
	sahf
	jne       short @146
	?debug L 1015
	mov       edx,dword ptr [ebp-12]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	lea       eax,dword ptr [ecx+edx]
	mov       edx,dword ptr [ebp-8]
	xor       ecx,ecx
	mov       dword ptr [eax+4*edx],ecx
	jmp       @147
	?debug L 1017
@146:
	mov       eax,dword ptr [ebp-12]
	shl       eax,4
	mov       edx,dword ptr [ebp+8]
	lea       ecx,dword ptr [edx+eax]
	mov       eax,dword ptr [ebp-8]
	fld       dword ptr [ecx+4*eax]
	add       esp,-8
	fstp      qword ptr [esp]
	call      @_fabs
	add       esp,8
	fcomp     dword ptr [ebp-24]
	fnstsw ax
	sahf
	jbe       @149
	?debug L 1019
	mov       edx,dword ptr [ebp-60]
	mov       ecx,dword ptr [ebp-8]
	fld       dword ptr [edx+4*ecx]
	mov       eax,dword ptr [ebp-60]
	mov       edx,dword ptr [ebp-12]
	fsub      dword ptr [eax+4*edx]
	fstp      dword ptr [ebp-48]
	?debug L 1020
	fld       dword ptr [ebp-48]
	add       esp,-8
	fstp      qword ptr [esp]
	call      @_fabs
	add       esp,8
	fadd      dword ptr [ebp-52]
	fstp      qword ptr [ebp-168]
	fld       dword ptr [ebp-48]
	add       esp,-8
	fstp      qword ptr [esp]
	call      @_fabs
	add       esp,8
	fcomp     qword ptr [ebp-168]
	fnstsw ax
	sahf
	jne       short @150
	mov       edx,dword ptr [ebp-12]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	lea       eax,dword ptr [ecx+edx]
	mov       edx,dword ptr [ebp-8]
	fld       dword ptr [eax+4*edx]
	fdiv      dword ptr [ebp-48]
	fstp      dword ptr [ebp-36]
	jmp       short @151
	?debug L 1022
@150:
	fld       dword ptr [@129+12]
	fmul      dword ptr [ebp-48]
	mov       ecx,dword ptr [ebp-12]
	shl       ecx,4
	mov       eax,dword ptr [ebp+8]
	lea       edx,dword ptr [eax+ecx]
	mov       ecx,dword ptr [ebp-8]
	fdiv      dword ptr [edx+4*ecx]
	fstp      dword ptr [ebp-28]
	?debug L 1023
	fld       dword ptr [ebp-28]
	add       esp,-8
	fstp      qword ptr [esp]
	call      @_fabs
	add       esp,8
	fstp      qword ptr [ebp-168]
	fld       dword ptr [ebp-28]
	fmul      dword ptr [ebp-28]
	fadd      dword ptr [@129+16]
	add       esp,-8
	fstp      qword ptr [esp]
	call      @_sqrt
	add       esp,8
	fadd      qword ptr [ebp-168]
	fdivr     dword ptr [@129+16]
	fstp      dword ptr [ebp-36]
	?debug L 1024
	fld       dword ptr [ebp-28]
	fcomp     dword ptr [@129]
	fnstsw ax
	sahf
	jae       short @152
	fld       dword ptr [ebp-36]
	fchs
	fstp      dword ptr [ebp-36]
	?debug L 1026
@152:
@151:
	fld       dword ptr [ebp-36]
	fmul      dword ptr [ebp-36]
	fadd      dword ptr [@129+16]
	add       esp,-8
	fstp      qword ptr [esp]
	call      @_sqrt
	add       esp,8
	fdivr     dword ptr [@129+16]
	fstp      dword ptr [ebp-56]
	?debug L 1027
	fld       dword ptr [ebp-36]
	fmul      dword ptr [ebp-56]
	fstp      dword ptr [ebp-44]
	?debug L 1028
	fld       dword ptr [@129+16]
	fadd      dword ptr [ebp-56]
	fdivr     dword ptr [ebp-44]
	fstp      dword ptr [ebp-32]
	?debug L 1029
	fld       dword ptr [ebp-36]
	mov       edx,dword ptr [ebp-12]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	lea       eax,dword ptr [ecx+edx]
	mov       edx,dword ptr [ebp-8]
	fmul      dword ptr [eax+4*edx]
	fstp      dword ptr [ebp-48]
	?debug L 1030
	fld       dword ptr [ebp-48]
	mov       ecx,dword ptr [ebp-12]
	fsubr     dword ptr [ebp+4*ecx-160]
	fstp      dword ptr [ebp+4*ecx-160]
	?debug L 1031
	fld       dword ptr [ebp-48]
	mov       eax,dword ptr [ebp-8]
	fadd      dword ptr [ebp+4*eax-160]
	fstp      dword ptr [ebp+4*eax-160]
	?debug L 1032
	fld       dword ptr [ebp-48]
	mov       edx,dword ptr [ebp-60]
	mov       ecx,dword ptr [ebp-12]
	fsubr     dword ptr [edx+4*ecx]
	fstp      dword ptr [edx+4*ecx]
	?debug L 1033
	fld       dword ptr [ebp-48]
	mov       eax,dword ptr [ebp-60]
	mov       edx,dword ptr [ebp-8]
	fadd      dword ptr [eax+4*edx]
	fstp      dword ptr [eax+4*edx]
	?debug L 1034
	mov       ecx,dword ptr [ebp-12]
	shl       ecx,4
	mov       eax,dword ptr [ebp+8]
	lea       edx,dword ptr [eax+ecx]
	mov       ecx,dword ptr [ebp-8]
	xor       eax,eax
	mov       dword ptr [edx+4*ecx],eax
	?debug L 1035
	xor       edx,edx
	mov       dword ptr [ebp-4],edx
	jmp       short @154
	?debug L 1037
@153:
	mov       ecx,dword ptr [ebp-4]
	shl       ecx,4
	mov       eax,dword ptr [ebp+8]
	lea       edx,dword ptr [eax+ecx]
	mov       ecx,dword ptr [ebp-12]
	mov       eax,dword ptr [edx+4*ecx]
	mov       dword ptr [ebp-52],eax
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	lea       eax,dword ptr [ecx+edx]
	mov       edx,dword ptr [ebp-8]
	mov       ecx,dword ptr [eax+4*edx]
	mov       dword ptr [ebp-48],ecx
	fld       dword ptr [ebp-52]
	fmul      dword ptr [ebp-32]
	fadd      dword ptr [ebp-48]
	fmul      dword ptr [ebp-44]
	fsubr     dword ptr [ebp-52]
	mov       eax,dword ptr [ebp-4]
	shl       eax,4
	mov       edx,dword ptr [ebp+8]
	lea       ecx,dword ptr [edx+eax]
	mov       eax,dword ptr [ebp-12]
	fstp      dword ptr [ecx+4*eax]
	fld       dword ptr [ebp-48]
	fmul      dword ptr [ebp-32]
	fsubr     dword ptr [ebp-52]
	fmul      dword ptr [ebp-44]
	fadd      dword ptr [ebp-48]
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	lea       eax,dword ptr [ecx+edx]
	mov       edx,dword ptr [ebp-8]
	fstp      dword ptr [eax+4*edx]
	inc       dword ptr [ebp-4]
@154:
	mov       ecx,dword ptr [ebp-12]
	dec       ecx
	mov       eax,dword ptr [ebp-4]
	cmp       ecx,eax
	jg        short @153
	?debug L 1039
	mov       edx,dword ptr [ebp-12]
	inc       edx
	mov       dword ptr [ebp-4],edx
	jmp       short @157
	?debug L 1041
@156:
	mov       ecx,dword ptr [ebp-12]
	shl       ecx,4
	mov       eax,dword ptr [ebp+8]
	lea       edx,dword ptr [eax+ecx]
	mov       ecx,dword ptr [ebp-4]
	mov       eax,dword ptr [edx+4*ecx]
	mov       dword ptr [ebp-52],eax
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	lea       eax,dword ptr [ecx+edx]
	mov       edx,dword ptr [ebp-8]
	mov       ecx,dword ptr [eax+4*edx]
	mov       dword ptr [ebp-48],ecx
	fld       dword ptr [ebp-52]
	fmul      dword ptr [ebp-32]
	fadd      dword ptr [ebp-48]
	fmul      dword ptr [ebp-44]
	fsubr     dword ptr [ebp-52]
	mov       eax,dword ptr [ebp-12]
	shl       eax,4
	mov       edx,dword ptr [ebp+8]
	lea       ecx,dword ptr [edx+eax]
	mov       eax,dword ptr [ebp-4]
	fstp      dword ptr [ecx+4*eax]
	fld       dword ptr [ebp-48]
	fmul      dword ptr [ebp-32]
	fsubr     dword ptr [ebp-52]
	fmul      dword ptr [ebp-44]
	fadd      dword ptr [ebp-48]
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	lea       eax,dword ptr [ecx+edx]
	mov       edx,dword ptr [ebp-8]
	fstp      dword ptr [eax+4*edx]
	inc       dword ptr [ebp-4]
@157:
	mov       ecx,dword ptr [ebp-8]
	dec       ecx
	mov       eax,dword ptr [ebp-4]
	cmp       ecx,eax
	jge       short @156
	?debug L 1043
	mov       edx,dword ptr [ebp-8]
	inc       edx
	mov       dword ptr [ebp-4],edx
	mov       ecx,dword ptr [ebp-4]
	mov       eax,dword ptr [ebp-64]
	cmp       ecx,eax
	jge       short @160
	?debug L 1045
@159:
	mov       edx,dword ptr [ebp-12]
	shl       edx,4
	mov       ecx,dword ptr [ebp+8]
	lea       eax,dword ptr [ecx+edx]
	mov       edx,dword ptr [ebp-4]
	mov       ecx,dword ptr [eax+4*edx]
	mov       dword ptr [ebp-52],ecx
	mov       eax,dword ptr [ebp-8]
	shl       eax,4
	mov       edx,dword ptr [ebp+8]
	lea       ecx,dword ptr [edx+eax]
	mov       eax,dword ptr [ebp-4]
	mov       edx,dword ptr [ecx+4*eax]
	mov       dword ptr [ebp-48],edx
	fld       dword ptr [ebp-52]
	fmul      dword ptr [ebp-32]
	fadd      dword ptr [ebp-48]
	fmul      dword ptr [ebp-44]
	fsubr     dword ptr [ebp-52]
	mov       ecx,dword ptr [ebp-12]
	shl       ecx,4
	mov       eax,dword ptr [ebp+8]
	lea       edx,dword ptr [eax+ecx]
	mov       ecx,dword ptr [ebp-4]
	fstp      dword ptr [edx+4*ecx]
	fld       dword ptr [ebp-48]
	fmul      dword ptr [ebp-32]
	fsubr     dword ptr [ebp-52]
	fmul      dword ptr [ebp-44]
	fadd      dword ptr [ebp-48]
	mov       eax,dword ptr [ebp-8]
	shl       eax,4
	mov       edx,dword ptr [ebp+8]
	lea       ecx,dword ptr [edx+eax]
	mov       eax,dword ptr [ebp-4]
	fstp      dword ptr [ecx+4*eax]
	inc       dword ptr [ebp-4]
	mov       edx,dword ptr [ebp-4]
	mov       ecx,dword ptr [ebp-64]
	cmp       edx,ecx
	jl        short @159
	?debug L 1047
@160:
	xor       eax,eax
	mov       dword ptr [ebp-4],eax
	mov       edx,dword ptr [ebp-4]
	mov       ecx,dword ptr [ebp-64]
	cmp       edx,ecx
	jge       short @163
	?debug L 1049
@162:
	mov       eax,dword ptr [ebp-4]
	shl       eax,4
	mov       edx,dword ptr [ebp+16]
	lea       ecx,dword ptr [edx+eax]
	mov       eax,dword ptr [ebp-12]
	mov       edx,dword ptr [ecx+4*eax]
	mov       dword ptr [ebp-52],edx
	mov       ecx,dword ptr [ebp-4]
	shl       ecx,4
	mov       eax,dword ptr [ebp+16]
	lea       edx,dword ptr [eax+ecx]
	mov       ecx,dword ptr [ebp-8]
	mov       eax,dword ptr [edx+4*ecx]
	mov       dword ptr [ebp-48],eax
	fld       dword ptr [ebp-52]
	fmul      dword ptr [ebp-32]
	fadd      dword ptr [ebp-48]
	fmul      dword ptr [ebp-44]
	fsubr     dword ptr [ebp-52]
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+16]
	lea       eax,dword ptr [ecx+edx]
	mov       edx,dword ptr [ebp-12]
	fstp      dword ptr [eax+4*edx]
	fld       dword ptr [ebp-48]
	fmul      dword ptr [ebp-32]
	fsubr     dword ptr [ebp-52]
	fmul      dword ptr [ebp-44]
	fadd      dword ptr [ebp-48]
	mov       ecx,dword ptr [ebp-4]
	shl       ecx,4
	mov       eax,dword ptr [ebp+16]
	lea       edx,dword ptr [eax+ecx]
	mov       ecx,dword ptr [ebp-8]
	fstp      dword ptr [edx+4*ecx]
	inc       dword ptr [ebp-4]
	mov       eax,dword ptr [ebp-4]
	mov       edx,dword ptr [ebp-64]
	cmp       eax,edx
	jl        short @162
	?debug L 1051
@163:
	inc       dword ptr [ebp-20]
@149:
@147:
	inc       dword ptr [ebp-8]
	mov       ecx,dword ptr [ebp-8]
	mov       eax,dword ptr [ebp-64]
	cmp       ecx,eax
	jl        @144
@145:
	inc       dword ptr [ebp-12]
@143:
	mov       edx,dword ptr [ebp-64]
	dec       edx
	mov       ecx,dword ptr [ebp-12]
	cmp       edx,ecx
	jg        @142
	?debug L 1055
	xor       eax,eax
	mov       dword ptr [ebp-12],eax
	mov       edx,dword ptr [ebp-12]
	mov       ecx,dword ptr [ebp-64]
	cmp       edx,ecx
	jge       short @168
	?debug L 1057
@167:
	mov       eax,dword ptr [ebp-12]
	fld       dword ptr [ebp+4*eax-160]
	mov       edx,dword ptr [ebp-12]
	fadd      dword ptr [ebp+4*edx-148]
	fstp      dword ptr [ebp+4*edx-148]
	?debug L 1058
	mov       ecx,dword ptr [ebp-60]
	mov       eax,dword ptr [ebp-12]
	mov       edx,dword ptr [ebp-12]
	mov       edx,dword ptr [ebp+4*edx-148]
	mov       dword ptr [ecx+4*eax],edx
	?debug L 1059
	mov       eax,dword ptr [ebp-12]
	xor       ecx,ecx
	mov       dword ptr [ebp+4*eax-160],ecx
	inc       dword ptr [ebp-12]
	mov       eax,dword ptr [ebp-12]
	mov       edx,dword ptr [ebp-64]
	cmp       eax,edx
	jl        short @167
@168:
	inc       dword ptr [ebp-16]
	mov       ecx,dword ptr [ebp-16]
	cmp       ecx,50
	jl        @130
	?debug L 1065
@139:
	mov       eax,dword ptr [ebp+20]
	test      eax,eax
	jne       short @171
	mov       eax,dword ptr [ebp-20]
	jmp       @172
	?debug L 1071
@171:
@173:
	xor       edx,edx
	mov       dword ptr [ebp-16],edx
	jmp       @175
	?debug L 1073
@174:
	mov       ecx,dword ptr [ebp-16]
	mov       dword ptr [ebp-72],ecx
	mov       eax,dword ptr [ebp-60]
	mov       edx,dword ptr [eax+4*ecx]
	mov       dword ptr [ebp-68],edx
	?debug L 1074
	mov       eax,dword ptr [ebp-16]
	inc       eax
	mov       dword ptr [ebp-4],eax
	mov       edx,dword ptr [ebp-4]
	mov       ecx,dword ptr [ebp-64]
	cmp       edx,ecx
	jge       short @177
@176:
	mov       eax,dword ptr [ebp-60]
	mov       edx,dword ptr [ebp-4]
	fld       dword ptr [eax+4*edx]
	fcomp     dword ptr [ebp-68]
	fnstsw ax
	sahf
	jb        short @178
	mov       edx,dword ptr [ebp-4]
	mov       dword ptr [ebp-72],edx
	mov       eax,dword ptr [ebp-60]
	mov       ecx,dword ptr [eax+4*edx]
	mov       dword ptr [ebp-68],ecx
@178:
	inc       dword ptr [ebp-4]
	mov       eax,dword ptr [ebp-4]
	mov       edx,dword ptr [ebp-64]
	cmp       eax,edx
	jl        short @176
	?debug L 1075
@177:
	mov       ecx,dword ptr [ebp-72]
	mov       eax,dword ptr [ebp-16]
	cmp       ecx,eax
	je        @180
	?debug L 1077
	mov       edx,dword ptr [ebp-60]
	mov       ecx,dword ptr [ebp-16]
	mov       eax,dword ptr [edx+4*ecx]
	mov       edx,dword ptr [ebp-60]
	mov       ecx,dword ptr [ebp-72]
	mov       dword ptr [edx+4*ecx],eax
	?debug L 1078
	mov       eax,dword ptr [ebp-60]
	mov       edx,dword ptr [ebp-16]
	mov       ecx,dword ptr [ebp-68]
	mov       dword ptr [eax+4*edx],ecx
	?debug L 1079
	xor       eax,eax
	mov       dword ptr [ebp-4],eax
	mov       edx,dword ptr [ebp-4]
	mov       ecx,dword ptr [ebp-64]
	cmp       edx,ecx
	jge       short @182
	?debug L 1081
@181:
	mov       eax,dword ptr [ebp-4]
	shl       eax,4
	mov       edx,dword ptr [ebp+16]
	lea       ecx,dword ptr [edx+eax]
	mov       eax,dword ptr [ebp-16]
	mov       edx,dword ptr [ecx+4*eax]
	mov       dword ptr [ebp-68],edx
	?debug L 1082
	mov       ecx,dword ptr [ebp-4]
	shl       ecx,4
	mov       eax,dword ptr [ebp+16]
	lea       edx,dword ptr [eax+ecx]
	mov       ecx,dword ptr [ebp-72]
	mov       eax,dword ptr [edx+4*ecx]
	mov       edx,dword ptr [ebp-4]
	shl       edx,4
	mov       ecx,dword ptr [ebp+16]
	lea       edx,dword ptr [ecx+edx]
	mov       ecx,dword ptr [ebp-16]
	mov       dword ptr [edx+4*ecx],eax
	?debug L 1083
	mov       eax,dword ptr [ebp-4]
	shl       eax,4
	mov       edx,dword ptr [ebp+16]
	lea       eax,dword ptr [edx+eax]
	mov       ecx,dword ptr [ebp-72]
	mov       edx,dword ptr [ebp-68]
	mov       dword ptr [eax+4*ecx],edx
	inc       dword ptr [ebp-4]
	mov       eax,dword ptr [ebp-4]
	mov       ecx,dword ptr [ebp-64]
	cmp       eax,ecx
	jl        short @181
@182:
@180:
	inc       dword ptr [ebp-16]
@175:
	mov       eax,dword ptr [ebp-64]
	dec       eax
	mov       edx,dword ptr [ebp-16]
	cmp       eax,edx
	jg        @174
	?debug L 1088
@185:
	push      dword ptr [ebp+8]
	lea       ecx,dword ptr [ebp-136]
	push      ecx
	call      @@mat_copy$qp9AD_Matrixt1
	add       esp,8
	?debug L 1089
	mov       eax,dword ptr [ebp-20]
	?debug L 1090
@187:
@172:
	mov       esp,ebp
	pop       ebp
	ret 
	align 4        
@129:
	db        0,0,0,0,205,204,76,62,0,0,200,66,0,0,0,63
	db        0,0,128,63
	?debug L 0
@@mat_eigen$qp9AD_Matrixp9AD_Vect3Dt1i	endp
@mat_eigen$qp9AD_Matrixp9AD_Vect3Dt1i	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	84
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch88
	dd	?patch89
	dd	?patch90
	df	@@mat_eigen$qp9AD_Matrixp9AD_Vect3Dt1i
	dw	0
	dw	4154
	dw	0
	dw	156
	dw	0
	dw	0
	dw	0
	db	37
	db	64
	db	109
	db	97
	db	116
	db	95
	db	101
	db	105
	db	103
	db	101
	db	110
	db	36
	db	113
	db	112
	db	57
	db	65
	db	68
	db	95
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	db	112
	db	57
	db	65
	db	68
	db	95
	db	86
	db	101
	db	99
	db	116
	db	51
	db	68
	db	116
	db	49
	db	105
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	157
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65533
	dw	65535
	dw	158
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	65535
	dw	65535
	dw	159
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	20
	dw	0
	dw	116
	dw	0
	dw	160
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65472
	dw	65535
	dw	116
	dw	0
	dw	161
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65476
	dw	65535
	dw	1088
	dw	0
	dw	162
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65376
	dw	65535
	dw	4156
	dw	0
	dw	163
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65388
	dw	65535
	dw	4157
	dw	0
	dw	164
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65480
	dw	65535
	dw	64
	dw	0
	dw	165
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65484
	dw	65535
	dw	64
	dw	0
	dw	166
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65488
	dw	65535
	dw	64
	dw	0
	dw	167
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65492
	dw	65535
	dw	64
	dw	0
	dw	168
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65496
	dw	65535
	dw	64
	dw	0
	dw	169
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65500
	dw	65535
	dw	64
	dw	0
	dw	170
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65504
	dw	65535
	dw	64
	dw	0
	dw	171
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65508
	dw	65535
	dw	64
	dw	0
	dw	172
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65512
	dw	65535
	dw	64
	dw	0
	dw	173
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65516
	dw	65535
	dw	116
	dw	0
	dw	174
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65520
	dw	65535
	dw	116
	dw	0
	dw	175
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65524
	dw	65535
	dw	116
	dw	0
	dw	176
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65528
	dw	65535
	dw	116
	dw	0
	dw	177
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	116
	dw	0
	dw	178
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65400
	dw	65535
	dw	65532
	dw	65535
	dw	179
	dw	0
	dw	0
	dw	0
	dw	24
	dw	519
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch91
	df	@173
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65464
	dw	65535
	dw	116
	dw	0
	dw	180
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65468
	dw	65535
	dw	64
	dw	0
	dw	181
	dw	0
	dw	0
	dw	0
?patch91	equ	@185-@173
	dw	2
	dw	6
?patch88	equ	@187-@@mat_eigen$qp9AD_Matrixp9AD_Vect3Dt1i+26
?patch89	equ	0
?patch90	equ	@187-@@mat_eigen$qp9AD_Matrixp9AD_Vect3Dt1i
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@nmat_make$qp7nMatrixii	segment virtual
@@nmat_make$qp7nMatrixii	proc	near
?live16415@0:
	?debug L 1097
	push      ebp
	mov       ebp,esp
	push      ecx
	?debug L 1101
@188:
	mov       eax,dword ptr [ebp+12]
	test      eax,eax
	jl        short @191
	mov       edx,dword ptr [ebp+16]
	test      edx,edx
	jl        short @191
	?debug L 1103
	mov       ecx,dword ptr [ebp+12]
	mov       eax,dword ptr [ebp+8]
	mov       dword ptr [eax],ecx
	?debug L 1104
	mov       edx,dword ptr [ebp+16]
	mov       ecx,dword ptr [ebp+8]
	mov       dword ptr [ecx+4],edx
	?debug L 1105
	mov       eax,dword ptr [ebp+12]
	imul      dword ptr [ebp+16]
	shl       eax,2
	push      eax
	call      @$bnwa$qui
	pop       ecx
	mov       edx,dword ptr [ebp+8]
	mov       dword ptr [edx+8],eax
	?debug L 1106
	xor       ecx,ecx
	mov       dword ptr [ebp-4],ecx
	jmp       short @193
@192:
	mov       eax,dword ptr [ebp+8]
	mov       edx,dword ptr [eax+8]
	mov       ecx,dword ptr [ebp-4]
	xor       eax,eax
	mov       dword ptr [edx+4*ecx],eax
	inc       dword ptr [ebp-4]
@193:
	mov       edx,dword ptr [ebp+12]
	imul      edx,dword ptr [ebp+16]
	mov       ecx,dword ptr [ebp-4]
	cmp       edx,ecx
	jg        short @192
	?debug L 1107
@195:
@191:
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@nmat_make$qp7nMatrixii	endp
@nmat_make$qp7nMatrixii	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	70
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch92
	dd	?patch93
	dd	?patch94
	df	@@nmat_make$qp7nMatrixii
	dw	0
	dw	4158
	dw	0
	dw	182
	dw	0
	dw	0
	dw	0
	db	23
	db	64
	db	110
	db	109
	db	97
	db	116
	db	95
	db	109
	db	97
	db	107
	db	101
	db	36
	db	113
	db	112
	db	55
	db	110
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	db	105
	db	105
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65531
	dw	65535
	dw	183
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	116
	dw	0
	dw	184
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	116
	dw	0
	dw	185
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	116
	dw	0
	dw	186
	dw	0
	dw	0
	dw	0
?patch92	equ	@195-@@nmat_make$qp7nMatrixii+3
?patch93	equ	0
?patch94	equ	@195-@@nmat_make$qp7nMatrixii
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@nmat_free$qp7nMatrix	segment virtual
@@nmat_free$qp7nMatrix	proc	near
?live16416@0:
	?debug L 1111
	push      ebp
	mov       ebp,esp
	?debug L 1113
@196:
	mov       eax,dword ptr [ebp+8]
	mov       edx,dword ptr [eax+8]
	test      edx,edx
	je        short @197
	mov       ecx,dword ptr [ebp+8]
	push      dword ptr [ecx+8]
	call      @$bdla$qpv
	pop       ecx
	?debug L 1114
@197:
	mov       eax,dword ptr [ebp+8]
	xor       edx,edx
	mov       dword ptr [eax+4],edx
	mov       eax,dword ptr [ebp+8]
	mov       dword ptr [eax],edx
	?debug L 1115
@198:
	pop       ebp
	ret 
	?debug L 0
@@nmat_free$qp7nMatrix	endp
@nmat_free$qp7nMatrix	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	68
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch95
	dd	?patch96
	dd	?patch97
	df	@@nmat_free$qp7nMatrix
	dw	0
	dw	4160
	dw	0
	dw	187
	dw	0
	dw	0
	dw	0
	db	21
	db	64
	db	110
	db	109
	db	97
	db	116
	db	95
	db	102
	db	114
	db	101
	db	101
	db	36
	db	113
	db	112
	db	55
	db	110
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65531
	dw	65535
	dw	188
	dw	0
	dw	0
	dw	0
?patch95	equ	@198-@@nmat_free$qp7nMatrix+2
?patch96	equ	0
?patch97	equ	@198-@@nmat_free$qp7nMatrix
	dw	2
	dw	6
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@nmat_copy$qp7nMatrixt1	segment virtual
@@nmat_copy$qp7nMatrixt1	proc	near
?live16417@0:
	?debug L 1119
	push      ebp
	mov       ebp,esp
	push      ecx
	?debug L 1123
@199:
	xor       eax,eax
	mov       dword ptr [ebp-4],eax
	jmp       short @201
	?debug L 1125
@200:
	mov       edx,dword ptr [ebp+8]
	mov       ecx,dword ptr [edx+8]
	mov       eax,dword ptr [ebp-4]
	mov       edx,dword ptr [ecx+4*eax]
	mov       ecx,dword ptr [ebp+12]
	mov       eax,dword ptr [ecx+8]
	mov       ecx,dword ptr [ebp-4]
	mov       dword ptr [eax+4*ecx],edx
	inc       dword ptr [ebp-4]
@201:
	mov       edx,dword ptr [ebp+8]
	mov       eax,dword ptr [edx+4]
	mov       edx,dword ptr [ebp+8]
	imul      dword ptr [edx]
	mov       ecx,dword ptr [ebp-4]
	cmp       eax,ecx
	jg        short @200
	?debug L 1127
@203:
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@nmat_copy$qp7nMatrixt1	endp
@nmat_copy$qp7nMatrixt1	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	70
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch98
	dd	?patch99
	dd	?patch100
	df	@@nmat_copy$qp7nMatrixt1
	dw	0
	dw	4162
	dw	0
	dw	189
	dw	0
	dw	0
	dw	0
	db	23
	db	64
	db	110
	db	109
	db	97
	db	116
	db	95
	db	99
	db	111
	db	112
	db	121
	db	36
	db	113
	db	112
	db	55
	db	110
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	db	116
	db	49
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65531
	dw	65535
	dw	190
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65531
	dw	65535
	dw	191
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	116
	dw	0
	dw	192
	dw	0
	dw	0
	dw	0
?patch98	equ	@203-@@nmat_copy$qp7nMatrixt1+3
?patch99	equ	0
?patch100	equ	@203-@@nmat_copy$qp7nMatrixt1
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@nmat_identity$qp7nMatrixi	segment virtual
@@nmat_identity$qp7nMatrixi	proc	near
?live16418@0:
	?debug L 1131
	push      ebp
	mov       ebp,esp
	add       esp,-8
	?debug L 1135
@204:
	mov       eax,dword ptr [ebp+8]
	mov       edx,dword ptr [eax+4]
	mov       ecx,dword ptr [ebp+8]
	mov       eax,dword ptr [ecx]
	cmp       edx,eax
	jne       short @206
	?debug L 1137
	xor       edx,edx
	mov       dword ptr [ebp-4],edx
	mov       ecx,dword ptr [ebp-4]
	mov       eax,dword ptr [ebp+12]
	cmp       ecx,eax
	jge       short @208
	?debug L 1139
@207:
	xor       edx,edx
	mov       dword ptr [ebp-8],edx
	mov       ecx,dword ptr [ebp-8]
	mov       eax,dword ptr [ebp+12]
	cmp       ecx,eax
	jge       short @210
	?debug L 1141
@209:
	mov       edx,dword ptr [ebp-8]
	mov       ecx,dword ptr [ebp-4]
	cmp       edx,ecx
	je        short @211
	mov       eax,dword ptr [ebp-4]
	mov       edx,dword ptr [ebp+8]
	imul      dword ptr [edx+4]
	mov       ecx,dword ptr [ebp-8]
	add       eax,ecx
	mov       edx,dword ptr [ebp+8]
	mov       ecx,dword ptr [edx+8]
	xor       edx,edx
	mov       dword ptr [ecx+4*eax],edx
	jmp       short @212
	?debug L 1142
@211:
	mov       eax,dword ptr [ebp-4]
	mov       ecx,dword ptr [ebp+8]
	imul      dword ptr [ecx+4]
	mov       edx,dword ptr [ebp-8]
	add       eax,edx
	mov       ecx,dword ptr [ebp+8]
	mov       edx,dword ptr [ecx+8]
	mov       dword ptr [edx+4*eax],1065353216
@212:
	inc       dword ptr [ebp-8]
	mov       eax,dword ptr [ebp-8]
	mov       edx,dword ptr [ebp+12]
	cmp       eax,edx
	jl        short @209
@210:
	inc       dword ptr [ebp-4]
	mov       eax,dword ptr [ebp-4]
	mov       ecx,dword ptr [ebp+12]
	cmp       eax,ecx
	jl        short @207
	?debug L 1145
@208:
@215:
@206:
	pop       ecx
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@nmat_identity$qp7nMatrixi	endp
@nmat_identity$qp7nMatrixi	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	73
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch101
	dd	?patch102
	dd	?patch103
	df	@@nmat_identity$qp7nMatrixi
	dw	0
	dw	4164
	dw	0
	dw	193
	dw	0
	dw	0
	dw	0
	db	26
	db	64
	db	110
	db	109
	db	97
	db	116
	db	95
	db	105
	db	100
	db	101
	db	110
	db	116
	db	105
	db	116
	db	121
	db	36
	db	113
	db	112
	db	55
	db	110
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	db	105
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65531
	dw	65535
	dw	194
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	116
	dw	0
	dw	195
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65528
	dw	65535
	dw	116
	dw	0
	dw	196
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	116
	dw	0
	dw	197
	dw	0
	dw	0
	dw	0
?patch101	equ	@215-@@nmat_identity$qp7nMatrixi+4
?patch102	equ	0
?patch103	equ	@215-@@nmat_identity$qp7nMatrixi
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@nmat_add$qp7nMatrixt1t1	segment virtual
@@nmat_add$qp7nMatrixt1t1	proc	near
?live16419@0:
	?debug L 1149
	push      ebp
	mov       ebp,esp
	push      ecx
	?debug L 1153
@216:
	xor       eax,eax
	mov       dword ptr [ebp-4],eax
	jmp       short @218
	?debug L 1155
@217:
	mov       edx,dword ptr [ebp+12]
	mov       ecx,dword ptr [edx+8]
	mov       eax,dword ptr [ebp-4]
	fld       dword ptr [ecx+4*eax]
	mov       edx,dword ptr [ebp+8]
	mov       ecx,dword ptr [edx+8]
	mov       eax,dword ptr [ebp-4]
	fadd      dword ptr [ecx+4*eax]
	mov       edx,dword ptr [ebp+16]
	mov       ecx,dword ptr [edx+8]
	mov       eax,dword ptr [ebp-4]
	fstp      dword ptr [ecx+4*eax]
	inc       dword ptr [ebp-4]
@218:
	mov       edx,dword ptr [ebp+8]
	mov       ecx,dword ptr [edx+4]
	mov       eax,dword ptr [ebp+8]
	imul      ecx,dword ptr [eax]
	mov       edx,dword ptr [ebp-4]
	cmp       ecx,edx
	jg        short @217
	?debug L 1157
@220:
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@nmat_add$qp7nMatrixt1t1	endp
@nmat_add$qp7nMatrixt1t1	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	71
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch104
	dd	?patch105
	dd	?patch106
	df	@@nmat_add$qp7nMatrixt1t1
	dw	0
	dw	4166
	dw	0
	dw	198
	dw	0
	dw	0
	dw	0
	db	24
	db	64
	db	110
	db	109
	db	97
	db	116
	db	95
	db	97
	db	100
	db	100
	db	36
	db	113
	db	112
	db	55
	db	110
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	db	116
	db	49
	db	116
	db	49
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65531
	dw	65535
	dw	199
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65531
	dw	65535
	dw	200
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	65531
	dw	65535
	dw	201
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	116
	dw	0
	dw	202
	dw	0
	dw	0
	dw	0
?patch104	equ	@220-@@nmat_add$qp7nMatrixt1t1+3
?patch105	equ	0
?patch106	equ	@220-@@nmat_add$qp7nMatrixt1t1
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@nmat_sub$qp7nMatrixt1t1	segment virtual
@@nmat_sub$qp7nMatrixt1t1	proc	near
?live16420@0:
	?debug L 1161
	push      ebp
	mov       ebp,esp
	push      ecx
	?debug L 1165
@221:
	xor       eax,eax
	mov       dword ptr [ebp-4],eax
	jmp       short @223
	?debug L 1167
@222:
	mov       edx,dword ptr [ebp+8]
	mov       ecx,dword ptr [edx+8]
	mov       eax,dword ptr [ebp-4]
	fld       dword ptr [ecx+4*eax]
	mov       edx,dword ptr [ebp+12]
	mov       ecx,dword ptr [edx+8]
	mov       eax,dword ptr [ebp-4]
	fsub      dword ptr [ecx+4*eax]
	mov       edx,dword ptr [ebp+16]
	mov       ecx,dword ptr [edx+8]
	mov       eax,dword ptr [ebp-4]
	fstp      dword ptr [ecx+4*eax]
	inc       dword ptr [ebp-4]
@223:
	mov       edx,dword ptr [ebp+8]
	mov       ecx,dword ptr [edx+4]
	mov       eax,dword ptr [ebp+8]
	imul      ecx,dword ptr [eax]
	mov       edx,dword ptr [ebp-4]
	cmp       ecx,edx
	jg        short @222
	?debug L 1169
@225:
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@nmat_sub$qp7nMatrixt1t1	endp
@nmat_sub$qp7nMatrixt1t1	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	71
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch107
	dd	?patch108
	dd	?patch109
	df	@@nmat_sub$qp7nMatrixt1t1
	dw	0
	dw	4168
	dw	0
	dw	203
	dw	0
	dw	0
	dw	0
	db	24
	db	64
	db	110
	db	109
	db	97
	db	116
	db	95
	db	115
	db	117
	db	98
	db	36
	db	113
	db	112
	db	55
	db	110
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	db	116
	db	49
	db	116
	db	49
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65531
	dw	65535
	dw	204
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65531
	dw	65535
	dw	205
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	65531
	dw	65535
	dw	206
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	116
	dw	0
	dw	207
	dw	0
	dw	0
	dw	0
?patch107	equ	@225-@@nmat_sub$qp7nMatrixt1t1+3
?patch108	equ	0
?patch109	equ	@225-@@nmat_sub$qp7nMatrixt1t1
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@nmat_swap_rows$qp7nMatrixii	segment virtual
@@nmat_swap_rows$qp7nMatrixii	proc	near
?live16421@0:
	?debug L 1173
	push      ebp
	mov       ebp,esp
	add       esp,-8
	?debug L 1178
@226:
	xor       eax,eax
	mov       dword ptr [ebp-4],eax
	jmp       short @228
	?debug L 1180
@227:
	mov       edx,dword ptr [ebp+12]
	mov       ecx,dword ptr [ebp+8]
	imul      edx,dword ptr [ecx+4]
	mov       eax,dword ptr [ebp-4]
	add       edx,eax
	mov       ecx,dword ptr [ebp+8]
	mov       eax,dword ptr [ecx+8]
	mov       edx,dword ptr [eax+4*edx]
	mov       dword ptr [ebp-8],edx
	?debug L 1181
	mov       ecx,dword ptr [ebp+16]
	mov       eax,dword ptr [ebp+8]
	imul      ecx,dword ptr [eax+4]
	mov       edx,dword ptr [ebp-4]
	add       ecx,edx
	mov       eax,dword ptr [ebp+8]
	mov       edx,dword ptr [eax+8]
	mov       ecx,dword ptr [edx+4*ecx]
	mov       eax,dword ptr [ebp+12]
	mov       edx,dword ptr [ebp+8]
	imul      dword ptr [edx+4]
	mov       edx,dword ptr [ebp-4]
	add       eax,edx
	mov       edx,dword ptr [ebp+8]
	mov       edx,dword ptr [edx+8]
	mov       dword ptr [edx+4*eax],ecx
	?debug L 1182
	mov       eax,dword ptr [ebp+16]
	mov       ecx,dword ptr [ebp+8]
	imul      dword ptr [ecx+4]
	mov       edx,dword ptr [ebp-4]
	add       eax,edx
	mov       ecx,dword ptr [ebp+8]
	mov       edx,dword ptr [ecx+8]
	mov       ecx,dword ptr [ebp-8]
	mov       dword ptr [edx+4*eax],ecx
	inc       dword ptr [ebp-4]
@228:
	mov       eax,dword ptr [ebp-4]
	mov       edx,dword ptr [ebp+8]
	mov       ecx,dword ptr [edx+4]
	cmp       eax,ecx
	jl        short @227
	?debug L 1184
@230:
	pop       ecx
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@nmat_swap_rows$qp7nMatrixii	endp
@nmat_swap_rows$qp7nMatrixii	ends
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
	dd	?patch110
	dd	?patch111
	dd	?patch112
	df	@@nmat_swap_rows$qp7nMatrixii
	dw	0
	dw	4170
	dw	0
	dw	208
	dw	0
	dw	0
	dw	0
	db	28
	db	64
	db	110
	db	109
	db	97
	db	116
	db	95
	db	115
	db	119
	db	97
	db	112
	db	95
	db	114
	db	111
	db	119
	db	115
	db	36
	db	113
	db	112
	db	55
	db	110
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	db	105
	db	105
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65531
	dw	65535
	dw	209
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	116
	dw	0
	dw	210
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	116
	dw	0
	dw	211
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65528
	dw	65535
	dw	64
	dw	0
	dw	212
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	116
	dw	0
	dw	213
	dw	0
	dw	0
	dw	0
?patch110	equ	@230-@@nmat_swap_rows$qp7nMatrixii+4
?patch111	equ	0
?patch112	equ	@230-@@nmat_swap_rows$qp7nMatrixii
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@nmat_inverse$qp7nMatrixt1	segment virtual
@@nmat_inverse$qp7nMatrixt1	proc	near
?live16422@0:
	?debug L 1188
	push      ebp
	mov       ebp,esp
	add       esp,-32
	?debug L 1195
@231:
	mov       eax,dword ptr [ebp+8]
	mov       edx,dword ptr [eax+4]
	mov       ecx,dword ptr [ebp+8]
	mov       eax,dword ptr [ecx]
	cmp       edx,eax
	je        short @232
	or        eax,-1
	jmp       @233
	?debug L 1197
@232:
	mov       edx,dword ptr [ebp+8]
	push      dword ptr [edx]
	push      dword ptr [ebp+12]
	call      @@nmat_identity$qp7nMatrixi
	add       esp,8
	?debug L 1198
	xor       ecx,ecx
	mov       dword ptr [ebp-16],ecx
	?debug L 1199
	xor       eax,eax
	mov       dword ptr [ebp-20],eax
	?debug L 1200
	xor       edx,edx
	mov       dword ptr [ebp-4],edx
	jmp       @235
	?debug L 1204
@234:
	mov       ecx,dword ptr [ebp-4]
	mov       eax,dword ptr [ebp+8]
	imul      ecx,dword ptr [eax+4]
	mov       edx,dword ptr [ebp-4]
	add       ecx,edx
	mov       eax,dword ptr [ebp+8]
	mov       edx,dword ptr [eax+8]
	mov       ecx,dword ptr [edx+4*ecx]
	mov       dword ptr [ebp-20],ecx
	?debug L 1205
	mov       eax,dword ptr [ebp-4]
	mov       dword ptr [ebp-16],eax
	?debug L 1206
	mov       edx,dword ptr [ebp-4]
	inc       edx
	mov       dword ptr [ebp-8],edx
	jmp       short @237
	?debug L 1208
@236:
	mov       ecx,dword ptr [ebp-8]
	mov       eax,dword ptr [ebp+8]
	imul      ecx,dword ptr [eax+4]
	mov       edx,dword ptr [ebp-4]
	add       ecx,edx
	mov       eax,dword ptr [ebp+8]
	mov       edx,dword ptr [eax+8]
	mov       ecx,dword ptr [edx+4*ecx]
	mov       dword ptr [ebp-24],ecx
	?debug L 1209
	fld       dword ptr [ebp-24]
	add       esp,-8
	fstp      qword ptr [esp]
	call      @_fabs
	add       esp,8
	fstp      qword ptr [ebp-32]
	fld       dword ptr [ebp-20]
	add       esp,-8
	fstp      qword ptr [esp]
	call      @_fabs
	add       esp,8
	fcomp     qword ptr [ebp-32]
	fnstsw ax
	sahf
	jae       short @238
	?debug L 1211
	mov       edx,dword ptr [ebp-24]
	mov       dword ptr [ebp-20],edx
	?debug L 1212
	mov       ecx,dword ptr [ebp-4]
	mov       dword ptr [ebp-16],ecx
@238:
	inc       dword ptr [ebp-8]
@237:
	mov       eax,dword ptr [ebp-8]
	mov       edx,dword ptr [ebp+8]
	mov       ecx,dword ptr [edx]
	cmp       eax,ecx
	jl        short @236
	?debug L 1218
	mov       eax,dword ptr [ebp-16]
	mov       edx,dword ptr [ebp-4]
	cmp       eax,edx
	je        short @240
	?debug L 1220
	push      dword ptr [ebp-16]
	push      dword ptr [ebp-4]
	push      dword ptr [ebp+8]
	call      @@nmat_swap_rows$qp7nMatrixii
	add       esp,12
	?debug L 1221
	push      dword ptr [ebp-16]
	push      dword ptr [ebp-4]
	push      dword ptr [ebp+12]
	call      @@nmat_swap_rows$qp7nMatrixii
	add       esp,12
	?debug L 1226
@240:
	fld       dword ptr [@241]
	fdiv      dword ptr [ebp-20]
	fstp      dword ptr [ebp-20]
	?debug L 1227
	xor       ecx,ecx
	mov       dword ptr [ebp-12],ecx
	jmp       short @243
	?debug L 1229
@242:
	fld       dword ptr [ebp-20]
	mov       eax,dword ptr [ebp-4]
	mov       edx,dword ptr [ebp+8]
	imul      dword ptr [edx+4]
	mov       ecx,dword ptr [ebp-12]
	add       eax,ecx
	mov       edx,dword ptr [ebp+8]
	mov       ecx,dword ptr [edx+8]
	fmul      dword ptr [ecx+4*eax]
	fstp      dword ptr [ecx+4*eax]
	?debug L 1230
	fld       dword ptr [ebp-20]
	mov       eax,dword ptr [ebp+12]
	mov       eax,dword ptr [eax+4]
	imul      dword ptr [ebp-4]
	mov       edx,dword ptr [ebp-12]
	add       eax,edx
	mov       ecx,dword ptr [ebp+12]
	mov       edx,dword ptr [ecx+8]
	fmul      dword ptr [edx+4*eax]
	fstp      dword ptr [edx+4*eax]
	inc       dword ptr [ebp-12]
@243:
	mov       eax,dword ptr [ebp-12]
	mov       ecx,dword ptr [ebp+8]
	mov       edx,dword ptr [ecx]
	cmp       eax,edx
	jl        short @242
	?debug L 1236
	mov       eax,dword ptr [ebp-4]
	inc       eax
	mov       dword ptr [ebp-8],eax
	jmp       @246
	?debug L 1238
@245:
	mov       ecx,dword ptr [ebp-8]
	mov       eax,dword ptr [ebp+8]
	imul      ecx,dword ptr [eax+4]
	mov       edx,dword ptr [ebp-4]
	add       ecx,edx
	mov       eax,dword ptr [ebp+8]
	mov       edx,dword ptr [eax+8]
	mov       ecx,dword ptr [edx+4*ecx]
	mov       dword ptr [ebp-24],ecx
	?debug L 1239
	xor       eax,eax
	mov       dword ptr [ebp-12],eax
	jmp       short @248
	?debug L 1241
@247:
	mov       edx,dword ptr [ebp-4]
	mov       ecx,dword ptr [ebp+8]
	imul      edx,dword ptr [ecx+4]
	mov       eax,dword ptr [ebp-12]
	add       edx,eax
	mov       ecx,dword ptr [ebp+8]
	mov       eax,dword ptr [ecx+8]
	fld       dword ptr [eax+4*edx]
	fmul      dword ptr [ebp-24]
	mov       edx,dword ptr [ebp-8]
	mov       ecx,dword ptr [ebp+8]
	imul      edx,dword ptr [ecx+4]
	mov       eax,dword ptr [ebp-12]
	add       edx,eax
	mov       ecx,dword ptr [ebp+8]
	mov       eax,dword ptr [ecx+8]
	fsubr     dword ptr [eax+4*edx]
	fstp      dword ptr [eax+4*edx]
	?debug L 1242
	mov       edx,dword ptr [ebp+12]
	mov       ecx,dword ptr [edx+4]
	imul      ecx,dword ptr [ebp-4]
	mov       eax,dword ptr [ebp-12]
	add       ecx,eax
	mov       edx,dword ptr [ebp+12]
	mov       eax,dword ptr [edx+8]
	fld       dword ptr [eax+4*ecx]
	fmul      dword ptr [ebp-24]
	mov       edx,dword ptr [ebp+12]
	mov       ecx,dword ptr [edx+4]
	imul      ecx,dword ptr [ebp-8]
	mov       eax,dword ptr [ebp-12]
	add       ecx,eax
	mov       edx,dword ptr [ebp+12]
	mov       eax,dword ptr [edx+8]
	fsubr     dword ptr [eax+4*ecx]
	fstp      dword ptr [eax+4*ecx]
	inc       dword ptr [ebp-12]
@248:
	mov       edx,dword ptr [ebp-12]
	mov       ecx,dword ptr [ebp+8]
	mov       eax,dword ptr [ecx]
	cmp       edx,eax
	jl        short @247
	inc       dword ptr [ebp-8]
@246:
	mov       edx,dword ptr [ebp-8]
	mov       ecx,dword ptr [ebp+8]
	mov       eax,dword ptr [ecx]
	cmp       edx,eax
	jl        @245
	inc       dword ptr [ebp-4]
@235:
	mov       edx,dword ptr [ebp-4]
	mov       ecx,dword ptr [ebp+8]
	mov       eax,dword ptr [ecx]
	cmp       edx,eax
	jl        @234
	?debug L 1249
	mov       edx,dword ptr [ebp+8]
	mov       ecx,dword ptr [edx]
	dec       ecx
	mov       dword ptr [ebp-4],ecx
	mov       eax,dword ptr [ebp-4]
	test      eax,eax
	jl        @253
	?debug L 1251
@252:
	mov       edx,dword ptr [ebp-4]
	dec       edx
	mov       dword ptr [ebp-8],edx
	mov       ecx,dword ptr [ebp-8]
	test      ecx,ecx
	jl        @255
	?debug L 1253
@254:
	mov       eax,dword ptr [ebp-8]
	mov       edx,dword ptr [ebp+8]
	imul      dword ptr [edx+4]
	mov       ecx,dword ptr [ebp-4]
	add       eax,ecx
	mov       edx,dword ptr [ebp+8]
	mov       ecx,dword ptr [edx+8]
	mov       eax,dword ptr [ecx+4*eax]
	mov       dword ptr [ebp-24],eax
	?debug L 1254
	xor       eax,eax
	mov       dword ptr [ebp-12],eax
	jmp       short @257
	?debug L 1256
@256:
	mov       edx,dword ptr [ebp-4]
	mov       ecx,dword ptr [ebp+8]
	imul      edx,dword ptr [ecx+4]
	mov       eax,dword ptr [ebp-12]
	add       edx,eax
	mov       ecx,dword ptr [ebp+8]
	mov       eax,dword ptr [ecx+8]
	fld       dword ptr [eax+4*edx]
	fmul      dword ptr [ebp-24]
	mov       edx,dword ptr [ebp-8]
	mov       ecx,dword ptr [ebp+8]
	imul      edx,dword ptr [ecx+4]
	mov       eax,dword ptr [ebp-12]
	add       edx,eax
	mov       ecx,dword ptr [ebp+8]
	mov       eax,dword ptr [ecx+8]
	fsubr     dword ptr [eax+4*edx]
	fstp      dword ptr [eax+4*edx]
	?debug L 1257
	mov       edx,dword ptr [ebp+12]
	mov       ecx,dword ptr [edx+4]
	imul      ecx,dword ptr [ebp-4]
	mov       eax,dword ptr [ebp-12]
	add       ecx,eax
	mov       edx,dword ptr [ebp+12]
	mov       eax,dword ptr [edx+8]
	fld       dword ptr [eax+4*ecx]
	fmul      dword ptr [ebp-24]
	mov       edx,dword ptr [ebp+12]
	mov       ecx,dword ptr [edx+4]
	imul      ecx,dword ptr [ebp-8]
	mov       eax,dword ptr [ebp-12]
	add       ecx,eax
	mov       edx,dword ptr [ebp+12]
	mov       eax,dword ptr [edx+8]
	fsubr     dword ptr [eax+4*ecx]
	fstp      dword ptr [eax+4*ecx]
	inc       dword ptr [ebp-12]
@257:
	mov       edx,dword ptr [ebp-12]
	mov       ecx,dword ptr [ebp+8]
	mov       eax,dword ptr [ecx]
	cmp       edx,eax
	jl        short @256
	dec       dword ptr [ebp-8]
	mov       edx,dword ptr [ebp-8]
	test      edx,edx
	jge       @254
@255:
	dec       dword ptr [ebp-4]
	mov       ecx,dword ptr [ebp-4]
	test      ecx,ecx
	jge       @252
	?debug L 1261
@253:
	xor       eax,eax
	?debug L 1262
@261:
@233:
	mov       esp,ebp
	pop       ebp
	ret 
	align 4        
@241:
	db        0,0,128,63
	?debug L 0
@@nmat_inverse$qp7nMatrixt1	endp
@nmat_inverse$qp7nMatrixt1	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	73
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch113
	dd	?patch114
	dd	?patch115
	df	@@nmat_inverse$qp7nMatrixt1
	dw	0
	dw	4172
	dw	0
	dw	214
	dw	0
	dw	0
	dw	0
	db	26
	db	64
	db	110
	db	109
	db	97
	db	116
	db	95
	db	105
	db	110
	db	118
	db	101
	db	114
	db	115
	db	101
	db	36
	db	113
	db	112
	db	55
	db	110
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	db	116
	db	49
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65531
	dw	65535
	dw	215
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65531
	dw	65535
	dw	216
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65512
	dw	65535
	dw	64
	dw	0
	dw	217
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65516
	dw	65535
	dw	64
	dw	0
	dw	218
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65520
	dw	65535
	dw	116
	dw	0
	dw	219
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65524
	dw	65535
	dw	116
	dw	0
	dw	220
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65528
	dw	65535
	dw	116
	dw	0
	dw	221
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	116
	dw	0
	dw	222
	dw	0
	dw	0
	dw	0
?patch113	equ	@261-@@nmat_inverse$qp7nMatrixt1+11
?patch114	equ	0
?patch115	equ	@261-@@nmat_inverse$qp7nMatrixt1
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@nmat_mulnvect$qp7nMatrixp7nVectort2	segment virtual
@@nmat_mulnvect$qp7nMatrixp7nVectort2	proc	near
?live16423@0:
	?debug L 1266
	push      ebp
	mov       ebp,esp
	add       esp,-12
	?debug L 1271
@262:
	xor       eax,eax
	mov       dword ptr [ebp-4],eax
	jmp       short @264
	?debug L 1273
@263:
	xor       edx,edx
	mov       dword ptr [ebp-12],edx
	?debug L 1274
	xor       ecx,ecx
	mov       dword ptr [ebp-8],ecx
	jmp       short @266
	?debug L 1276
@265:
	mov       eax,dword ptr [ebp-4]
	mov       edx,dword ptr [ebp+8]
	imul      dword ptr [edx+4]
	mov       ecx,dword ptr [ebp-8]
	add       eax,ecx
	mov       edx,dword ptr [ebp+8]
	mov       ecx,dword ptr [edx+8]
	fld       dword ptr [ecx+4*eax]
	mov       eax,dword ptr [ebp+12]
	mov       eax,dword ptr [eax+4]
	mov       edx,dword ptr [ebp-8]
	fmul      dword ptr [eax+4*edx]
	fadd      dword ptr [ebp-12]
	fstp      dword ptr [ebp-12]
	inc       dword ptr [ebp-8]
@266:
	mov       ecx,dword ptr [ebp-8]
	mov       eax,dword ptr [ebp+8]
	mov       edx,dword ptr [eax+4]
	cmp       ecx,edx
	jl        short @265
	?debug L 1279
	mov       ecx,dword ptr [ebp+16]
	mov       eax,dword ptr [ecx+4]
	mov       edx,dword ptr [ebp-4]
	mov       ecx,dword ptr [ebp-12]
	mov       dword ptr [eax+4*edx],ecx
	inc       dword ptr [ebp-4]
@264:
	mov       eax,dword ptr [ebp-4]
	mov       edx,dword ptr [ebp+8]
	mov       ecx,dword ptr [edx]
	cmp       eax,ecx
	jl        short @263
	?debug L 1281
@269:
	mov       esp,ebp
	pop       ebp
	ret 
	?debug L 0
@@nmat_mulnvect$qp7nMatrixp7nVectort2	endp
@nmat_mulnvect$qp7nMatrixp7nVectort2	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	83
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch116
	dd	?patch117
	dd	?patch118
	df	@@nmat_mulnvect$qp7nMatrixp7nVectort2
	dw	0
	dw	4174
	dw	0
	dw	223
	dw	0
	dw	0
	dw	0
	db	36
	db	64
	db	110
	db	109
	db	97
	db	116
	db	95
	db	109
	db	117
	db	108
	db	110
	db	118
	db	101
	db	99
	db	116
	db	36
	db	113
	db	112
	db	55
	db	110
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	db	112
	db	55
	db	110
	db	86
	db	101
	db	99
	db	116
	db	111
	db	114
	db	116
	db	50
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65531
	dw	65535
	dw	224
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65530
	dw	65535
	dw	225
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	65530
	dw	65535
	dw	226
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65524
	dw	65535
	dw	64
	dw	0
	dw	227
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65528
	dw	65535
	dw	116
	dw	0
	dw	228
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	116
	dw	0
	dw	229
	dw	0
	dw	0
	dw	0
?patch116	equ	@269-@@nmat_mulnvect$qp7nMatrixp7nVectort2+4
?patch117	equ	0
?patch118	equ	@269-@@nmat_mulnvect$qp7nMatrixp7nVectort2
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@nmat_diade$qp7nVectorp7nMatrix	segment virtual
@@nmat_diade$qp7nVectorp7nMatrix	proc	near
?live16424@0:
	?debug L 1285
	push      ebp
	mov       ebp,esp
	add       esp,-8
	?debug L 1290
@270:
	xor       eax,eax
	mov       dword ptr [ebp-4],eax
	jmp       short @272
	?debug L 1292
@271:
	xor       edx,edx
	mov       dword ptr [ebp-8],edx
	jmp       short @274
	?debug L 1294
@273:
	mov       ecx,dword ptr [ebp+8]
	mov       eax,dword ptr [ecx+4]
	mov       edx,dword ptr [ebp-4]
	fld       dword ptr [eax+4*edx]
	mov       ecx,dword ptr [ebp+8]
	mov       eax,dword ptr [ecx+4]
	mov       edx,dword ptr [ebp-8]
	fmul      dword ptr [eax+4*edx]
	mov       ecx,dword ptr [ebp+12]
	mov       eax,dword ptr [ecx+4]
	imul      dword ptr [ebp-4]
	mov       edx,dword ptr [ebp-8]
	add       eax,edx
	mov       ecx,dword ptr [ebp+12]
	mov       edx,dword ptr [ecx+8]
	fstp      dword ptr [edx+4*eax]
	inc       dword ptr [ebp-8]
@274:
	mov       eax,dword ptr [ebp-8]
	mov       ecx,dword ptr [ebp+8]
	mov       edx,dword ptr [ecx]
	cmp       eax,edx
	jl        short @273
	inc       dword ptr [ebp-4]
@272:
	mov       eax,dword ptr [ebp-4]
	mov       ecx,dword ptr [ebp+8]
	mov       edx,dword ptr [ecx]
	cmp       eax,edx
	jl        short @271
	?debug L 1297
@277:
	pop       ecx
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@nmat_diade$qp7nVectorp7nMatrix	endp
@nmat_diade$qp7nVectorp7nMatrix	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	78
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch119
	dd	?patch120
	dd	?patch121
	df	@@nmat_diade$qp7nVectorp7nMatrix
	dw	0
	dw	4176
	dw	0
	dw	230
	dw	0
	dw	0
	dw	0
	db	31
	db	64
	db	110
	db	109
	db	97
	db	116
	db	95
	db	100
	db	105
	db	97
	db	100
	db	101
	db	36
	db	113
	db	112
	db	55
	db	110
	db	86
	db	101
	db	99
	db	116
	db	111
	db	114
	db	112
	db	55
	db	110
	db	77
	db	97
	db	116
	db	114
	db	105
	db	120
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65530
	dw	65535
	dw	231
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65531
	dw	65535
	dw	232
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65528
	dw	65535
	dw	116
	dw	0
	dw	233
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	116
	dw	0
	dw	234
	dw	0
	dw	0
	dw	0
?patch119	equ	@277-@@nmat_diade$qp7nVectorp7nMatrix+4
?patch120	equ	0
?patch121	equ	@277-@@nmat_diade$qp7nVectorp7nMatrix
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_DATA	segment dword public use32 'DATA'
s@	label	byte
	;	s@+0:
	db	"[0][0]=%f [0][1]=%f [0][2]=%f [0][3]=%f ",10,0
	;	s@+42:
	db	"[1][0]=%f [1][1]=%f [1][2]=%f [1][3]=%f ",10,0
	;	s@+84:
	db	"[2][0]=%f [2][1]=%f [2][2]=%f [2][3]=%f ",10,0
	;	s@+126:
	db	"[3][0]=%f [3][1]=%f [3][2]=%f [3][3]=%f ",10,0
	align	4
_DATA	ends
_TEXT	segment dword public use32 'CODE'
_TEXT	ends
	public	_mat_mul
	public	_mat_mulvect
@_fprintf equ _fprintf
 extrn   _fprintf:near
@_cos equ _cos
 extrn   _cos:near
@_sin equ _sin
 extrn   _sin:near
@_fabs equ _fabs
 extrn   _fabs:near
@_sqrt equ _sqrt
 extrn   _sqrt:near
 extrn   @$bnwa$qui:near
 extrn   @$bdla$qpv:near
	extrn	__turboFloat:word
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	22
	dw	514
	df	_mat_mul
	dw	0
	dw	4178
	dw	0
	dw	235
	dw	0
	dw	0
	dw	0
	dw	22
	dw	514
	df	_mat_mulvect
	dw	0
	dw	4181
	dw	0
	dw	236
	dw	0
	dw	0
	dw	0
	dw	?patch122
	dw	37
	dw	237
	dw	0
	dw	0
	dw	0
	dw	0
?patch122	equ	12
	dw	?patch123
	dw	38
	dw	198
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
	dw	422
	dw	0
	dw	423
	dw	0
	dw	424
	dw	0
	dw	425
	dw	0
	dw	426
	dw	0
	dw	427
	dw	0
	dw	428
	dw	0
	dw	429
	dw	0
	dw	430
	dw	0
	dw	431
	dw	0
	dw	432
	dw	0
	dw	433
	dw	0
	dw	434
	dw	0
	dw	435
	dw	0
?patch123	equ	796
	dw	?patch124
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
?patch124	equ	18
$$BSYMS	ends
$$BTYPES	segment byte public use32 'DEBTYP'
	db        2,0,0,0,14,0,8,0,3,0,0,0,0,0,17,0
	db        1,16,0,0,72,0,1,2,17,0,255,255,255,255,64,0
	db        0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0
	db        0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0
	db        0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0
	db        0,0,64,0,0,0,64,0,0,0,64,0,0,0,14,0
	db        8,0,3,0,0,0,0,0,2,0,3,16,0,0,12,0
	db        1,2,2,0,255,255,255,255,255,255,255,255,14,0,8,0
	db        3,0,0,0,0,0,1,0,5,16,0,0,8,0,1,2
	db        1,0,255,255,255,255,14,0,8,0,3,0,0,0,0,0
	db        2,0,7,16,0,0,12,0,1,2,2,0,255,255,255,255
	db        254,255,255,255,14,0,8,0,3,0,0,0,0,0,1,0
	db        9,16,0,0,8,0,1,2,1,0,255,255,255,255,14,0
	db        8,0,3,0,0,0,0,0,2,0,11,16,0,0,12,0
	db        1,2,2,0,255,255,255,255,255,255,255,255,14,0,8,0
	db        3,0,0,0,0,0,3,0,13,16,0,0,16,0,1,2
	db        3,0,255,255,255,255,116,0,0,0,253,255,255,255,14,0
	db        8,0,3,0,0,0,0,0,3,0,15,16,0,0,16,0
	db        1,2,3,0,255,255,255,255,116,0,0,0,253,255,255,255
	db        14,0,8,0,3,0,0,0,0,0,3,0,17,16,0,0
	db        16,0,1,2,3,0,255,255,255,255,116,0,0,0,253,255
	db        255,255,14,0,8,0,3,0,0,0,0,0,3,0,19,16
	db        0,0,16,0,1,2,3,0,255,255,255,255,116,0,0,0
	db        253,255,255,255,14,0,8,0,3,0,0,0,0,0,4,0
	db        21,16,0,0,20,0,1,2,4,0,255,255,255,255,64,0
	db        0,0,64,0,0,0,64,0,0,0,14,0,8,0,3,0
	db        0,0,0,0,2,0,23,16,0,0,12,0,1,2,2,0
	db        255,255,255,255,253,255,255,255,14,0,8,0,3,0,0,0
	db        0,0,4,0,25,16,0,0,20,0,1,2,4,0,255,255
	db        255,255,64,0,0,0,64,0,0,0,64,0,0,0,14,0
	db        8,0,3,0,0,0,0,0,2,0,27,16,0,0,12,0
	db        1,2,2,0,255,255,255,255,64,0,0,0,14,0,8,0
	db        3,0,0,0,0,0,2,0,29,16,0,0,12,0,1,2
	db        2,0,255,255,255,255,64,0,0,0,14,0,8,0,3,0
	db        0,0,0,0,2,0,31,16,0,0,12,0,1,2,2,0
	db        255,255,255,255,64,0,0,0,14,0,8,0,3,0,0,0
	db        0,0,3,0,33,16,0,0,16,0,1,2,3,0,255,255
	db        255,255,255,255,255,255,255,255,255,255,14,0,8,0,3,0
	db        0,0,0,0,2,0,35,16,0,0,12,0,1,2,2,0
	db        255,255,255,255,255,255,255,255,14,0,8,0,3,0,0,0
	db        0,0,3,0,37,16,0,0,16,0,1,2,3,0,255,255
	db        255,255,255,255,255,255,255,255,255,255,14,0,8,0,3,0
	db        0,0,0,0,2,0,39,16,0,0,12,0,1,2,2,0
	db        255,255,255,255,255,255,255,255,14,0,8,0,3,0,0,0
	db        0,0,3,0,41,16,0,0,16,0,1,2,3,0,255,255
	db        255,255,255,255,255,255,255,255,255,255,14,0,8,0,3,0
	db        0,0,0,0,3,0,43,16,0,0,16,0,1,2,3,0
	db        255,255,255,255,255,255,255,255,255,255,255,255,14,0,8,0
	db        3,0,0,0,0,0,3,0,45,16,0,0,16,0,1,2
	db        3,0,255,255,255,255,253,255,255,255,253,255,255,255,14,0
	db        8,0,3,0,0,0,0,0,3,0,47,16,0,0,16,0
	db        1,2,3,0,255,255,255,255,253,255,255,255,253,255,255,255
	db        14,0,8,0,3,0,0,0,0,0,3,0,49,16,0,0
	db        16,0,1,2,3,0,255,255,255,255,253,255,255,255,253,255
	db        255,255,14,0,8,0,3,0,0,0,0,0,3,0,51,16
	db        0,0,16,0,1,2,3,0,255,255,255,255,253,255,255,255
	db        253,255,255,255,14,0,8,0,3,0,0,0,0,0,3,0
	db        53,16,0,0,16,0,1,2,3,0,255,255,255,255,116,0
	db        0,0,116,0,0,0,14,0,8,0,3,0,0,0,0,0
	db        3,0,55,16,0,0,16,0,1,2,3,0,255,255,255,255
	db        116,0,0,0,116,0,0,0,14,0,8,0,116,0,0,0
	db        0,0,2,0,57,16,0,0,12,0,1,2,2,0,255,255
	db        255,255,255,255,255,255,14,0,8,0,116,0,0,0,0,0
	db        4,0,59,16,0,0,20,0,1,2,4,0,255,255,255,255
	db        253,255,255,255,255,255,255,255,116,0,0,0,18,0,3,0
	db        64,0,0,0,17,0,0,0,0,0,0,0,12,0,3,0
	db        18,0,3,0,64,0,0,0,17,0,0,0,0,0,0,0
	db        12,0,3,0,14,0,8,0,3,0,0,0,0,0,3,0
	db        63,16,0,0,16,0,1,2,3,0,251,255,255,255,116,0
	db        0,0,116,0,0,0,14,0,8,0,3,0,0,0,0,0
	db        1,0,65,16,0,0,8,0,1,2,1,0,251,255,255,255
	db        14,0,8,0,3,0,0,0,0,0,2,0,67,16,0,0
	db        12,0,1,2,2,0,251,255,255,255,251,255,255,255,14,0
	db        8,0,3,0,0,0,0,0,2,0,69,16,0,0,12,0
	db        1,2,2,0,251,255,255,255,116,0,0,0,14,0,8,0
	db        3,0,0,0,0,0,3,0,71,16,0,0,16,0,1,2
	db        3,0,251,255,255,255,251,255,255,255,251,255,255,255,14,0
	db        8,0,3,0,0,0,0,0,3,0,73,16,0,0,16,0
	db        1,2,3,0,251,255,255,255,251,255,255,255,251,255,255,255
	db        14,0,8,0,3,0,0,0,0,0,3,0,75,16,0,0
	db        16,0,1,2,3,0,251,255,255,255,116,0,0,0,116,0
	db        0,0,14,0,8,0,116,0,0,0,0,0,2,0,77,16
	db        0,0,12,0,1,2,2,0,251,255,255,255,251,255,255,255
	db        14,0,8,0,3,0,0,0,0,0,3,0,79,16,0,0
	db        16,0,1,2,3,0,251,255,255,255,250,255,255,255,250,255
	db        255,255,14,0,8,0,3,0,0,0,0,0,2,0,81,16
	db        0,0,12,0,1,2,2,0,250,255,255,255,251,255,255,255
	db        8,0,2,0,10,0,83,16,0,0,14,0,8,0,3,0
	db        0,0,0,0,3,0,84,16,0,0,16,0,1,2,3,0
	db        255,255,255,255,255,255,255,255,255,255,255,255,8,0,2,0
	db        10,0,86,16,0,0,14,0,8,0,3,0,0,0,0,0
	db        3,0,87,16,0,0,16,0,1,2,3,0,255,255,255,255
	db        253,255,255,255,253,255,255,255,14,0,8,0,116,0,0,0
	db        64,0,3,0,91,16,0,0,8,0,2,0,10,0,90,16
	db        0,0,8,0,1,0,1,0,112,0,0,0,16,0,1,2
	db        3,0,254,255,255,255,89,16,0,0,0,0,0,0,14,0
	db        8,0,65,0,0,0,0,0,1,0,93,16,0,0,8,0
	db        1,2,1,0,65,0,0,0,14,0,8,0,65,0,0,0
	db        0,0,1,0,95,16,0,0,8,0,1,2,1,0,65,0
	db        0,0,14,0,8,0,65,0,0,0,0,0,1,0,97,16
	db        0,0,8,0,1,2,1,0,65,0,0,0,14,0,8,0
	db        65,0,0,0,0,0,1,0,99,16,0,0,8,0,1,2
	db        1,0,65,0,0,0,14,0,8,0,3,4,0,0,0,0
	db        1,0,101,16,0,0,8,0,1,2,1,0,117,0,0,0
	db        14,0,8,0,3,0,0,0,0,0,1,0,103,16,0,0
	db        8,0,1,2,1,0,3,4,0,0
$$BTYPES	ends
$$BNAMES	segment byte public use32 'DEBNAM'
	db	7,'mat_set'
	db	1,'m'
	db	3,'b11'
	db	3,'b12'
	db	3,'b13'
	db	3,'b14'
	db	3,'b21'
	db	3,'b22'
	db	3,'b23'
	db	3,'b24'
	db	3,'b31'
	db	3,'b32'
	db	3,'b33'
	db	3,'b34'
	db	3,'b41'
	db	3,'b42'
	db	3,'b43'
	db	3,'b44'
	db	8,'mat_copy'
	db	2,'m1'
	db	2,'m2'
	db	1,'i'
	db	12,'mat_swap_3DS'
	db	1,'m'
	db	5,'righe'
	db	7,'colonne'
	db	3,'tmp'
	db	17,'mat_print_to_file'
	db	1,'m'
	db	4,'dove'
	db	12,'mat_identity'
	db	1,'m'
	db	13,'mat_transpose'
	db	1,'m'
	db	4,'mout'
	db	1,'i'
	db	14,'mat_insert_row'
	db	1,'m'
	db	1,'r'
	db	1,'v'
	db	17,'mat_insert_column'
	db	1,'m'
	db	1,'c'
	db	1,'v'
	db	11,'mat_get_row'
	db	1,'m'
	db	1,'r'
	db	1,'v'
	db	14,'mat_get_column'
	db	1,'m'
	db	1,'c'
	db	1,'v'
	db	24,'mat_setmatrix_of_scaling'
	db	1,'m'
	db	2,'sx'
	db	2,'sy'
	db	2,'sz'
	db	30,'mat_setmatrix_of_pretraslation'
	db	1,'m'
	db	1,'v'
	db	30,'mat_setmatrix_of_eulerrotation'
	db	1,'m'
	db	4,'angx'
	db	4,'angy'
	db	4,'angz'
	db	2,'s3'
	db	2,'c3'
	db	2,'s2'
	db	2,'c2'
	db	2,'s1'
	db	2,'c1'
	db	31,'mat_setmatrix_of_eulerrotationX'
	db	1,'m'
	db	4,'angx'
	db	1,'s'
	db	1,'c'
	db	31,'mat_setmatrix_of_eulerrotationY'
	db	1,'m'
	db	4,'angy'
	db	1,'s'
	db	1,'c'
	db	31,'mat_setmatrix_of_eulerrotationZ'
	db	1,'m'
	db	4,'angz'
	db	1,'s'
	db	1,'c'
	db	7,'mat_add'
	db	2,'m1'
	db	2,'m2'
	db	2,'m3'
	db	1,'i'
	db	12,'mat_auto_add'
	db	2,'m1'
	db	2,'m2'
	db	1,'i'
	db	7,'mat_sub'
	db	2,'m1'
	db	2,'m2'
	db	2,'m3'
	db	1,'i'
	db	12,'mat_auto_sub'
	db	2,'m1'
	db	2,'m2'
	db	1,'i'
	db	9,'mat_mul_C'
	db	2,'m1'
	db	2,'m2'
	db	2,'m3'
	db	1,'i'
	db	2,'mt'
	db	13,'mat_mulaffine'
	db	2,'m1'
	db	2,'m2'
	db	2,'m3'
	db	1,'j'
	db	1,'i'
	db	2,'mt'
	db	13,'mat_mulvect_C'
	db	1,'m'
	db	1,'v'
	db	4,'vout'
	db	15,'mat_mulvect_asm'
	db	1,'m'
	db	1,'v'
	db	4,'vout'
	db	14,'mat_mulvectenv'
	db	1,'m'
	db	1,'v'
	db	4,'vout'
	db	17,'mat_mulvectaffine'
	db	1,'m'
	db	1,'v'
	db	4,'vout'
	db	13,'mat_swap_rows'
	db	1,'m'
	db	2,'r1'
	db	2,'r2'
	db	1,'f'
	db	1,'g'
	db	16,'mat_swap_columns'
	db	1,'m'
	db	2,'c1'
	db	2,'c2'
	db	1,'f'
	db	1,'g'
	db	11,'mat_inverse'
	db	2,'m1'
	db	3,'out'
	db	1,'M'
	db	3,'val'
	db	6,'maxval'
	db	6,'maxrow'
	db	1,'k'
	db	1,'c'
	db	1,'r'
	db	9,'mat_eigen'
	db	1,'A'
	db	11,'eigenValues'
	db	12,'eigenVectors'
	db	4,'sort'
	db	1,'n'
	db	1,'d'
	db	1,'z'
	db	1,'b'
	db	1,'c'
	db	1,'g'
	db	1,'h'
	db	1,'s'
	db	2,'sm'
	db	1,'t'
	db	3,'tau'
	db	5,'theta'
	db	5,'tresh'
	db	4,'nrot'
	db	1,'i'
	db	2,'ip'
	db	2,'iq'
	db	1,'j'
	db	7,'l_copyA'
	db	1,'k'
	db	1,'p'
	db	9,'nmat_make'
	db	1,'m'
	db	1,'r'
	db	1,'c'
	db	1,'t'
	db	9,'nmat_free'
	db	1,'m'
	db	9,'nmat_copy'
	db	2,'m1'
	db	2,'m2'
	db	1,'t'
	db	13,'nmat_identity'
	db	1,'m'
	db	3,'dim'
	db	1,'c'
	db	1,'r'
	db	8,'nmat_add'
	db	2,'m1'
	db	2,'m2'
	db	2,'m3'
	db	1,'t'
	db	8,'nmat_sub'
	db	2,'m1'
	db	2,'m2'
	db	2,'m3'
	db	1,'t'
	db	14,'nmat_swap_rows'
	db	1,'m'
	db	2,'r1'
	db	2,'r2'
	db	1,'f'
	db	1,'g'
	db	12,'nmat_inverse'
	db	2,'m1'
	db	2,'m2'
	db	3,'val'
	db	6,'maxval'
	db	6,'maxrow'
	db	1,'k'
	db	1,'c'
	db	1,'r'
	db	13,'nmat_mulnvect'
	db	1,'m'
	db	1,'v'
	db	3,'out'
	db	4,'faux'
	db	1,'w'
	db	1,'t'
	db	10,'nmat_diade'
	db	1,'v'
	db	1,'m'
	db	1,'j'
	db	1,'i'
	db	7,'mat_mul'
	db	11,'mat_mulvect'
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
	db	8,'strerror'
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
	db	6,'perror'
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
	db	8,'_wperror'
	db	8,'_wfsopen'
	db	9,'_wfreopen'
	db	7,'_wfopen'
	db	8,'_wfdopen'
	db	7,'_unlink'
	db	8,'_tempnam'
	db	9,'_strerror'
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
	db	6,'_wtold'
	db	5,'_wtol'
	db	5,'_wtoi'
	db	5,'_wtof'
	db	8,'_ui64tow'
	db	7,'_i64tow'
	db	7,'_wtoi64'
	db	8,'_ui64toa'
	db	7,'_i64toa'
	db	7,'_atoi64'
	db	6,'pow10l'
	db	5,'polyl'
	db	6,'hypotl'
	db	4,'poly'
	db	5,'hypot'
	db	6,'_atold'
	db	5,'pow10'
	db	4,'labs'
	db	3,'abs'
	db	4,'cosl'
	db	4,'sinl'
	db	3,'cos'
	db	3,'sin'
	db	4,'atof'
	db	4,'tanl'
	db	5,'tanhl'
	db	5,'sqrtl'
	db	5,'sinhl'
	db	4,'powl'
	db	5,'modfl'
	db	4,'logl'
	db	6,'log10l'
	db	6,'ldexpl'
	db	6,'frexpl'
	db	5,'fmodl'
	db	6,'floorl'
	db	5,'fabsl'
	db	4,'expl'
	db	5,'coshl'
	db	5,'ceill'
	db	5,'atanl'
	db	6,'atan2l'
	db	5,'asinl'
	db	5,'acosl'
	db	4,'tanh'
	db	3,'tan'
	db	4,'sqrt'
	db	4,'sinh'
	db	3,'pow'
	db	4,'modf'
	db	5,'log10'
	db	3,'log'
	db	5,'ldexp'
	db	5,'frexp'
	db	4,'fmod'
	db	5,'floor'
	db	4,'fabs'
	db	3,'exp'
	db	4,'cosh'
	db	4,'ceil'
	db	5,'atan2'
	db	4,'atan'
	db	4,'asin'
	db	4,'acos'
	db	10,'__threadid'
	db	14,'@std@ptrdiff_t'
	db	11,'@std@size_t'
	db	11,'@std@wint_t'
	db	13,'@std@wctype_t'
	db	10,'_huge_dble'
	db	11,'_huge_ldble'
	db	9,'@std@FILE'
	db	18,'@std@_floatconvert'
	db	8,'_streams'
	db	11,'@std@fpos_t'
$$BNAMES	ends
	?debug	D "..\..\..\LIB_AUX\INCLUDE\types.h" 11102 34051
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\_nfile.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\stdio.h" 10303 10240
	?debug	D "D:\redshift\Demo_sw\Math\vect3D.h" 11379 36509
	?debug	D "D:\redshift\Demo_sw\Math\MATRIX.H" 11349 29653
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\_null.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\_defs.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\_stddef.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\MATH.H" 10503 10272
	?debug	D "D:\redshift\Demo_sw\Math\matrix.cpp" 11396 38196
 ?debug  C FB062A633A5C50524F4752417E315C626F726C616E645C434255494C447E315C6C69625C76636C35302E2330464E330000F77A872C
 ?debug  C FB050B232A41445F4D6174726978
 ?debug  C FB0506232A46494C45
 ?debug  C FB050B232A41445F566563743344
 ?debug  C FB050A2341445F4D6174726978
 ?debug  C FB0509232A6E4D6174726978
 ?debug  C FB0509232A6E566563746F72
	end
