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
	?debug	S "D:\redshift\Demo_sw\Math\vect3d.cpp"
	?debug	T "D:\redshift\Demo_sw\Math\vect3d.cpp"
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
_vect_auto_neg	label	dword
	db	4	dup(?)
	align	4
_vect_auto_normalize	label	dword
	db	4	dup(?)
	align	4
_vect_neg	label	dword
	db	4	dup(?)
	align	4
_vect_auto_add	label	dword
	db	4	dup(?)
	align	4
_vect_auto_sub	label	dword
	db	4	dup(?)
	align	4
_vect_normalize	label	dword
	db	4	dup(?)
	align	4
_vect_add	label	dword
	db	4	dup(?)
	align	4
_vect_sub	label	dword
	db	4	dup(?)
	align	4
_vect_cross	label	dword
	db	4	dup(?)
	align	4
_vect_scale	label	dword
	db	4	dup(?)
	align	4
_vect_auto_scale	label	dword
	db	4	dup(?)
	align	4
_vect_dot	label	dword
	db	4	dup(?)
	align	4
_vect_length	label	dword
	db	4	dup(?)
	align	4
_vect_length2	label	dword
	db	4	dup(?)
_BSS	ends
_DATA	segment dword public use32 'DATA'
	align	4
_uno	label	dword
	db        0,0,128,63
_DATA	ends
_TEXT	segment dword public use32 'CODE'
@vect_copy$qp9AD_Vect3Dt1	segment virtual
@@vect_copy$qp9AD_Vect3Dt1	proc	near
?live16385@0:
	?debug L 54
	push      ebp
	mov       ebp,esp
	?debug L 56
@1:
	mov       eax,dword ptr [ebp+8]
	mov       edx,dword ptr [eax]
	mov       ecx,dword ptr [ebp+12]
	mov       dword ptr [ecx],edx
	?debug L 57
	mov       eax,dword ptr [ebp+8]
	mov       edx,dword ptr [eax+4]
	mov       ecx,dword ptr [ebp+12]
	mov       dword ptr [ecx+4],edx
	?debug L 58
	mov       eax,dword ptr [ebp+8]
	mov       edx,dword ptr [eax+8]
	mov       ecx,dword ptr [ebp+12]
	mov       dword ptr [ecx+8],edx
	?debug L 59
@2:
	pop       ebp
	ret 
	?debug L 0
@@vect_copy$qp9AD_Vect3Dt1	endp
@vect_copy$qp9AD_Vect3Dt1	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	db	2
	db	0
	db	0
	db	0
	dw	72
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
	df	@@vect_copy$qp9AD_Vect3Dt1
	dw	0
	dw	4096
	dw	0
	dw	1
	dw	0
	dw	0
	dw	0
	db	25
	db	64
	db	118
	db	101
	db	99
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
	db	86
	db	101
	db	99
	db	116
	db	51
	db	68
	db	116
	db	49
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
	dw	65535
	dw	65535
	dw	3
	dw	0
	dw	0
	dw	0
?patch1	equ	@2-@@vect_copy$qp9AD_Vect3Dt1+2
?patch2	equ	0
?patch3	equ	@2-@@vect_copy$qp9AD_Vect3Dt1
	dw	2
	dw	6
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@vect_set$qp9AD_Vect3Dfff	segment virtual
@@vect_set$qp9AD_Vect3Dfff	proc	near
?live16386@0:
	?debug L 62
	push      ebp
	mov       ebp,esp
	?debug L 64
@3:
	mov       eax,dword ptr [ebp+12]
	mov       edx,dword ptr [ebp+8]
	mov       dword ptr [edx],eax
	?debug L 65
	mov       ecx,dword ptr [ebp+16]
	mov       eax,dword ptr [ebp+8]
	mov       dword ptr [eax+4],ecx
	?debug L 66
	mov       edx,dword ptr [ebp+20]
	mov       ecx,dword ptr [ebp+8]
	mov       dword ptr [ecx+8],edx
	?debug L 67
@4:
	pop       ebp
	ret 
	?debug L 0
@@vect_set$qp9AD_Vect3Dfff	endp
@vect_set$qp9AD_Vect3Dfff	ends
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
	dd	?patch4
	dd	?patch5
	dd	?patch6
	df	@@vect_set$qp9AD_Vect3Dfff
	dw	0
	dw	4098
	dw	0
	dw	4
	dw	0
	dw	0
	dw	0
	db	25
	db	64
	db	118
	db	101
	db	99
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
	db	86
	db	101
	db	99
	db	116
	db	51
	db	68
	db	102
	db	102
	db	102
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	5
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	64
	dw	0
	dw	6
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	64
	dw	0
	dw	7
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	20
	dw	0
	dw	64
	dw	0
	dw	8
	dw	0
	dw	0
	dw	0
?patch4	equ	@4-@@vect_set$qp9AD_Vect3Dfff+2
?patch5	equ	0
?patch6	equ	@4-@@vect_set$qp9AD_Vect3Dfff
	dw	2
	dw	6
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@vect_swap_3DS$qp9AD_Vect3D	segment virtual
@@vect_swap_3DS$qp9AD_Vect3D	proc	near
?live16387@0:
	?debug L 70
	push      ebp
	mov       ebp,esp
	push      ecx
	?debug L 74
@5:
	mov       eax,dword ptr [ebp+8]
	mov       edx,dword ptr [eax+4]
	mov       dword ptr [ebp-4],edx
	?debug L 75
	mov       ecx,dword ptr [ebp+8]
	mov       eax,dword ptr [ecx+8]
	mov       edx,dword ptr [ebp+8]
	mov       dword ptr [edx+4],eax
	?debug L 76
	mov       ecx,dword ptr [ebp-4]
	mov       eax,dword ptr [ebp+8]
	mov       dword ptr [eax+8],ecx
	?debug L 77
@6:
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@vect_swap_3DS$qp9AD_Vect3D	endp
@vect_swap_3DS$qp9AD_Vect3D	ends
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
	dd	?patch7
	dd	?patch8
	dd	?patch9
	df	@@vect_swap_3DS$qp9AD_Vect3D
	dw	0
	dw	4100
	dw	0
	dw	9
	dw	0
	dw	0
	dw	0
	db	27
	db	64
	db	118
	db	101
	db	99
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
	dw	10
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	64
	dw	0
	dw	11
	dw	0
	dw	0
	dw	0
?patch7	equ	@6-@@vect_swap_3DS$qp9AD_Vect3D+3
?patch8	equ	0
?patch9	equ	@6-@@vect_swap_3DS$qp9AD_Vect3D
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@print_to_file$qp9AD_Vect3Dp8std@FILE	segment virtual
@@print_to_file$qp9AD_Vect3Dp8std@FILE	proc	near
?live16388@0:
	?debug L 80
	push      ebp
	mov       ebp,esp
	?debug L 82
@7:
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax+8]
	add       esp,-8
	fstp      qword ptr [esp]
	mov       edx,dword ptr [ebp+8]
	fld       dword ptr [edx+4]
	add       esp,-8
	fstp      qword ptr [esp]
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx]
	add       esp,-8
	fstp      qword ptr [esp]
	push      offset s@
	push      dword ptr [ebp+12]
	call      @_fprintf
	add       esp,32
	?debug L 83
@8:
	pop       ebp
	ret 
	?debug L 0
@@print_to_file$qp9AD_Vect3Dp8std@FILE	endp
@print_to_file$qp9AD_Vect3Dp8std@FILE	ends
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
	dd	?patch10
	dd	?patch11
	dd	?patch12
	df	@@print_to_file$qp9AD_Vect3Dp8std@FILE
	dw	0
	dw	4102
	dw	0
	dw	12
	dw	0
	dw	0
	dw	0
	db	37
	db	64
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
	db	86
	db	101
	db	99
	db	116
	db	51
	db	68
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
	dw	13
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65534
	dw	65535
	dw	14
	dw	0
	dw	0
	dw	0
?patch10	equ	@8-@@print_to_file$qp9AD_Vect3Dp8std@FILE+2
?patch11	equ	0
?patch12	equ	@8-@@print_to_file$qp9AD_Vect3Dp8std@FILE
	dw	2
	dw	6
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@vect_add_C$qp9AD_Vect3Dt1t1	segment virtual
@@vect_add_C$qp9AD_Vect3Dt1t1	proc	near
?live16389@0:
	?debug L 91
	push      ebp
	mov       ebp,esp
	?debug L 93
@9:
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax]
	mov       edx,dword ptr [ebp+12]
	fadd      dword ptr [edx]
	mov       ecx,dword ptr [ebp+16]
	fstp      dword ptr [ecx]
	?debug L 94
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax+4]
	mov       edx,dword ptr [ebp+12]
	fadd      dword ptr [edx+4]
	mov       ecx,dword ptr [ebp+16]
	fstp      dword ptr [ecx+4]
	?debug L 95
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax+8]
	mov       edx,dword ptr [ebp+12]
	fadd      dword ptr [edx+8]
	mov       ecx,dword ptr [ebp+16]
	fstp      dword ptr [ecx+8]
	?debug L 96
@10:
	pop       ebp
	ret 
	?debug L 0
@@vect_add_C$qp9AD_Vect3Dt1t1	endp
@vect_add_C$qp9AD_Vect3Dt1t1	ends
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
	dd	?patch13
	dd	?patch14
	dd	?patch15
	df	@@vect_add_C$qp9AD_Vect3Dt1t1
	dw	0
	dw	4104
	dw	0
	dw	15
	dw	0
	dw	0
	dw	0
	db	28
	db	64
	db	118
	db	101
	db	99
	db	116
	db	95
	db	97
	db	100
	db	100
	db	95
	db	67
	db	36
	db	113
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
	db	116
	db	49
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	16
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65535
	dw	65535
	dw	17
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	65535
	dw	65535
	dw	18
	dw	0
	dw	0
	dw	0
?patch13	equ	@10-@@vect_add_C$qp9AD_Vect3Dt1t1+2
?patch14	equ	0
?patch15	equ	@10-@@vect_add_C$qp9AD_Vect3Dt1t1
	dw	2
	dw	6
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@vect_auto_add_C$qp9AD_Vect3Dt1	segment virtual
@@vect_auto_add_C$qp9AD_Vect3Dt1	proc	near
?live16390@0:
	?debug L 99
	push      ebp
	mov       ebp,esp
	?debug L 102
@11:
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax]
	mov       edx,dword ptr [ebp+12]
	fadd      dword ptr [edx]
	mov       ecx,dword ptr [ebp+8]
	fstp      dword ptr [ecx]
	?debug L 103
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax+4]
	mov       edx,dword ptr [ebp+12]
	fadd      dword ptr [edx+4]
	mov       ecx,dword ptr [ebp+8]
	fstp      dword ptr [ecx+4]
	?debug L 104
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax+8]
	mov       edx,dword ptr [ebp+12]
	fadd      dword ptr [edx+8]
	mov       ecx,dword ptr [ebp+8]
	fstp      dword ptr [ecx+8]
	?debug L 123
@12:
	pop       ebp
	ret 
	?debug L 0
@@vect_auto_add_C$qp9AD_Vect3Dt1	endp
@vect_auto_add_C$qp9AD_Vect3Dt1	ends
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
	dd	?patch16
	dd	?patch17
	dd	?patch18
	df	@@vect_auto_add_C$qp9AD_Vect3Dt1
	dw	0
	dw	4106
	dw	0
	dw	19
	dw	0
	dw	0
	dw	0
	db	31
	db	64
	db	118
	db	101
	db	99
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
	db	95
	db	67
	db	36
	db	113
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
?patch16	equ	@12-@@vect_auto_add_C$qp9AD_Vect3Dt1+2
?patch17	equ	0
?patch18	equ	@12-@@vect_auto_add_C$qp9AD_Vect3Dt1
	dw	2
	dw	6
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@vect_sub_C$qp9AD_Vect3Dt1t1	segment virtual
@@vect_sub_C$qp9AD_Vect3Dt1t1	proc	near
?live16391@0:
	?debug L 126
	push      ebp
	mov       ebp,esp
	?debug L 128
@13:
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax]
	mov       edx,dword ptr [ebp+12]
	fsub      dword ptr [edx]
	mov       ecx,dword ptr [ebp+16]
	fstp      dword ptr [ecx]
	?debug L 129
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax+4]
	mov       edx,dword ptr [ebp+12]
	fsub      dword ptr [edx+4]
	mov       ecx,dword ptr [ebp+16]
	fstp      dword ptr [ecx+4]
	?debug L 130
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax+8]
	mov       edx,dword ptr [ebp+12]
	fsub      dword ptr [edx+8]
	mov       ecx,dword ptr [ebp+16]
	fstp      dword ptr [ecx+8]
	?debug L 131
@14:
	pop       ebp
	ret 
	?debug L 0
@@vect_sub_C$qp9AD_Vect3Dt1t1	endp
@vect_sub_C$qp9AD_Vect3Dt1t1	ends
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
	dd	?patch19
	dd	?patch20
	dd	?patch21
	df	@@vect_sub_C$qp9AD_Vect3Dt1t1
	dw	0
	dw	4108
	dw	0
	dw	22
	dw	0
	dw	0
	dw	0
	db	28
	db	64
	db	118
	db	101
	db	99
	db	116
	db	95
	db	115
	db	117
	db	98
	db	95
	db	67
	db	36
	db	113
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
	db	116
	db	49
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	23
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65535
	dw	65535
	dw	24
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	65535
	dw	65535
	dw	25
	dw	0
	dw	0
	dw	0
?patch19	equ	@14-@@vect_sub_C$qp9AD_Vect3Dt1t1+2
?patch20	equ	0
?patch21	equ	@14-@@vect_sub_C$qp9AD_Vect3Dt1t1
	dw	2
	dw	6
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@vect_auto_sub_C$qp9AD_Vect3Dt1	segment virtual
@@vect_auto_sub_C$qp9AD_Vect3Dt1	proc	near
?live16392@0:
	?debug L 134
	push      ebp
	mov       ebp,esp
	?debug L 136
@15:
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax]
	mov       edx,dword ptr [ebp+12]
	fsub      dword ptr [edx]
	mov       ecx,dword ptr [ebp+8]
	fstp      dword ptr [ecx]
	?debug L 137
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax+4]
	mov       edx,dword ptr [ebp+12]
	fsub      dword ptr [edx+4]
	mov       ecx,dword ptr [ebp+8]
	fstp      dword ptr [ecx+4]
	?debug L 138
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax+8]
	mov       edx,dword ptr [ebp+12]
	fsub      dword ptr [edx+8]
	mov       ecx,dword ptr [ebp+8]
	fstp      dword ptr [ecx+8]
	?debug L 157
@16:
	pop       ebp
	ret 
	?debug L 0
@@vect_auto_sub_C$qp9AD_Vect3Dt1	endp
@vect_auto_sub_C$qp9AD_Vect3Dt1	ends
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
	dd	?patch22
	dd	?patch23
	dd	?patch24
	df	@@vect_auto_sub_C$qp9AD_Vect3Dt1
	dw	0
	dw	4110
	dw	0
	dw	26
	dw	0
	dw	0
	dw	0
	db	31
	db	64
	db	118
	db	101
	db	99
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
	db	95
	db	67
	db	36
	db	113
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
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	27
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65535
	dw	65535
	dw	28
	dw	0
	dw	0
	dw	0
?patch22	equ	@16-@@vect_auto_sub_C$qp9AD_Vect3Dt1+2
?patch23	equ	0
?patch24	equ	@16-@@vect_auto_sub_C$qp9AD_Vect3Dt1
	dw	2
	dw	6
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@vect_neg_C$qp9AD_Vect3Dt1	segment virtual
@@vect_neg_C$qp9AD_Vect3Dt1	proc	near
?live16393@0:
	?debug L 160
	push      ebp
	mov       ebp,esp
	?debug L 162
@17:
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax]
	fchs
	mov       edx,dword ptr [ebp+12]
	fstp      dword ptr [edx]
	?debug L 163
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+4]
	fchs
	mov       eax,dword ptr [ebp+12]
	fstp      dword ptr [eax+4]
	?debug L 164
	mov       edx,dword ptr [ebp+8]
	fld       dword ptr [edx+8]
	fchs
	mov       ecx,dword ptr [ebp+12]
	fstp      dword ptr [ecx+8]
	?debug L 165
@18:
	pop       ebp
	ret 
	?debug L 0
@@vect_neg_C$qp9AD_Vect3Dt1	endp
@vect_neg_C$qp9AD_Vect3Dt1	ends
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
	dd	?patch25
	dd	?patch26
	dd	?patch27
	df	@@vect_neg_C$qp9AD_Vect3Dt1
	dw	0
	dw	4112
	dw	0
	dw	29
	dw	0
	dw	0
	dw	0
	db	26
	db	64
	db	118
	db	101
	db	99
	db	116
	db	95
	db	110
	db	101
	db	103
	db	95
	db	67
	db	36
	db	113
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
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	30
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65535
	dw	65535
	dw	31
	dw	0
	dw	0
	dw	0
?patch25	equ	@18-@@vect_neg_C$qp9AD_Vect3Dt1+2
?patch26	equ	0
?patch27	equ	@18-@@vect_neg_C$qp9AD_Vect3Dt1
	dw	2
	dw	6
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@vect_auto_neg_C$qp9AD_Vect3D	segment virtual
@@vect_auto_neg_C$qp9AD_Vect3D	proc	near
?live16394@0:
	?debug L 168
	push      ebp
	mov       ebp,esp
	?debug L 170
@19:
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax]
	fchs
	mov       edx,dword ptr [ebp+8]
	fstp      dword ptr [edx]
	?debug L 171
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+4]
	fchs
	mov       eax,dword ptr [ebp+8]
	fstp      dword ptr [eax+4]
	?debug L 172
	mov       edx,dword ptr [ebp+8]
	fld       dword ptr [edx+8]
	fchs
	mov       ecx,dword ptr [ebp+8]
	fstp      dword ptr [ecx+8]
	?debug L 173
@20:
	pop       ebp
	ret 
	?debug L 0
@@vect_auto_neg_C$qp9AD_Vect3D	endp
@vect_auto_neg_C$qp9AD_Vect3D	ends
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
	dd	?patch28
	dd	?patch29
	dd	?patch30
	df	@@vect_auto_neg_C$qp9AD_Vect3D
	dw	0
	dw	4114
	dw	0
	dw	32
	dw	0
	dw	0
	dw	0
	db	29
	db	64
	db	118
	db	101
	db	99
	db	116
	db	95
	db	97
	db	117
	db	116
	db	111
	db	95
	db	110
	db	101
	db	103
	db	95
	db	67
	db	36
	db	113
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
	dw	33
	dw	0
	dw	0
	dw	0
?patch28	equ	@20-@@vect_auto_neg_C$qp9AD_Vect3D+2
?patch29	equ	0
?patch30	equ	@20-@@vect_auto_neg_C$qp9AD_Vect3D
	dw	2
	dw	6
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@vect_scale_C$qp9AD_Vect3Dft1	segment virtual
@@vect_scale_C$qp9AD_Vect3Dft1	proc	near
?live16395@0:
	?debug L 176
	push      ebp
	mov       ebp,esp
	?debug L 178
@21:
	fld       dword ptr [ebp+12]
	mov       eax,dword ptr [ebp+8]
	fmul      dword ptr [eax]
	mov       edx,dword ptr [ebp+16]
	fstp      dword ptr [edx]
	?debug L 179
	fld       dword ptr [ebp+12]
	mov       ecx,dword ptr [ebp+8]
	fmul      dword ptr [ecx+4]
	mov       eax,dword ptr [ebp+16]
	fstp      dword ptr [eax+4]
	?debug L 180
	fld       dword ptr [ebp+12]
	mov       edx,dword ptr [ebp+8]
	fmul      dword ptr [edx+8]
	mov       ecx,dword ptr [ebp+16]
	fstp      dword ptr [ecx+8]
	?debug L 181
@22:
	pop       ebp
	ret 
	?debug L 0
@@vect_scale_C$qp9AD_Vect3Dft1	endp
@vect_scale_C$qp9AD_Vect3Dft1	ends
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
	dd	?patch31
	dd	?patch32
	dd	?patch33
	df	@@vect_scale_C$qp9AD_Vect3Dft1
	dw	0
	dw	4116
	dw	0
	dw	34
	dw	0
	dw	0
	dw	0
	db	29
	db	64
	db	118
	db	101
	db	99
	db	116
	db	95
	db	115
	db	99
	db	97
	db	108
	db	101
	db	95
	db	67
	db	36
	db	113
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
	db	102
	db	116
	db	49
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	35
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	64
	dw	0
	dw	36
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	65535
	dw	65535
	dw	37
	dw	0
	dw	0
	dw	0
?patch31	equ	@22-@@vect_scale_C$qp9AD_Vect3Dft1+2
?patch32	equ	0
?patch33	equ	@22-@@vect_scale_C$qp9AD_Vect3Dft1
	dw	2
	dw	6
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@vect_auto_scale_C$qp9AD_Vect3Df	segment virtual
@@vect_auto_scale_C$qp9AD_Vect3Df	proc	near
?live16396@0:
	?debug L 184
	push      ebp
	mov       ebp,esp
	?debug L 186
@23:
	fld       dword ptr [ebp+12]
	mov       eax,dword ptr [ebp+8]
	fmul      dword ptr [eax]
	mov       edx,dword ptr [ebp+8]
	fstp      dword ptr [edx]
	?debug L 187
	fld       dword ptr [ebp+12]
	mov       ecx,dword ptr [ebp+8]
	fmul      dword ptr [ecx+4]
	mov       eax,dword ptr [ebp+8]
	fstp      dword ptr [eax+4]
	?debug L 188
	fld       dword ptr [ebp+12]
	mov       edx,dword ptr [ebp+8]
	fmul      dword ptr [edx+8]
	mov       ecx,dword ptr [ebp+8]
	fstp      dword ptr [ecx+8]
	?debug L 189
@24:
	pop       ebp
	ret 
	?debug L 0
@@vect_auto_scale_C$qp9AD_Vect3Df	endp
@vect_auto_scale_C$qp9AD_Vect3Df	ends
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
	dd	?patch34
	dd	?patch35
	dd	?patch36
	df	@@vect_auto_scale_C$qp9AD_Vect3Df
	dw	0
	dw	4118
	dw	0
	dw	38
	dw	0
	dw	0
	dw	0
	db	32
	db	64
	db	118
	db	101
	db	99
	db	116
	db	95
	db	97
	db	117
	db	116
	db	111
	db	95
	db	115
	db	99
	db	97
	db	108
	db	101
	db	95
	db	67
	db	36
	db	113
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
	db	102
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	39
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	64
	dw	0
	dw	40
	dw	0
	dw	0
	dw	0
?patch34	equ	@24-@@vect_auto_scale_C$qp9AD_Vect3Df+2
?patch35	equ	0
?patch36	equ	@24-@@vect_auto_scale_C$qp9AD_Vect3Df
	dw	2
	dw	6
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@vect_dot_C$qp9AD_Vect3Dt1	segment virtual
@@vect_dot_C$qp9AD_Vect3Dt1	proc	near
?live16397@0:
	?debug L 192
	push      ebp
	mov       ebp,esp
	?debug L 194
@25:
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
	?debug L 195
@27:
@26:
	pop       ebp
	ret 
	?debug L 0
@@vect_dot_C$qp9AD_Vect3Dt1	endp
@vect_dot_C$qp9AD_Vect3Dt1	ends
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
	dd	?patch37
	dd	?patch38
	dd	?patch39
	df	@@vect_dot_C$qp9AD_Vect3Dt1
	dw	0
	dw	4120
	dw	0
	dw	41
	dw	0
	dw	0
	dw	0
	db	26
	db	64
	db	118
	db	101
	db	99
	db	116
	db	95
	db	100
	db	111
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
	db	86
	db	101
	db	99
	db	116
	db	51
	db	68
	db	116
	db	49
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
	dw	65535
	dw	65535
	dw	43
	dw	0
	dw	0
	dw	0
?patch37	equ	@27-@@vect_dot_C$qp9AD_Vect3Dt1+2
?patch38	equ	0
?patch39	equ	@27-@@vect_dot_C$qp9AD_Vect3Dt1
	dw	2
	dw	6
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@vect_cross_C$qp9AD_Vect3Dt1t1	segment virtual
@@vect_cross_C$qp9AD_Vect3Dt1t1	proc	near
?live16398@0:
	?debug L 198
	push      ebp
	mov       ebp,esp
	?debug L 200
@28:
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax+4]
	mov       edx,dword ptr [ebp+12]
	fmul      dword ptr [edx+8]
	mov       ecx,dword ptr [ebp+12]
	fld       dword ptr [ecx+4]
	mov       eax,dword ptr [ebp+8]
	fmul      dword ptr [eax+8]
	fsubp      st(1),st
	mov       edx,dword ptr [ebp+16]
	fstp      dword ptr [edx]
	?debug L 201
	mov       ecx,dword ptr [ebp+12]
	fld       dword ptr [ecx]
	mov       eax,dword ptr [ebp+8]
	fmul      dword ptr [eax+8]
	mov       edx,dword ptr [ebp+8]
	fld       dword ptr [edx]
	mov       ecx,dword ptr [ebp+12]
	fmul      dword ptr [ecx+8]
	fsubp      st(1),st
	mov       eax,dword ptr [ebp+16]
	fstp      dword ptr [eax+4]
	?debug L 202
	mov       edx,dword ptr [ebp+8]
	fld       dword ptr [edx]
	mov       ecx,dword ptr [ebp+12]
	fmul      dword ptr [ecx+4]
	mov       eax,dword ptr [ebp+12]
	fld       dword ptr [eax]
	mov       edx,dword ptr [ebp+8]
	fmul      dword ptr [edx+4]
	fsubp      st(1),st
	mov       ecx,dword ptr [ebp+16]
	fstp      dword ptr [ecx+8]
	?debug L 203
@29:
	pop       ebp
	ret 
	?debug L 0
@@vect_cross_C$qp9AD_Vect3Dt1t1	endp
@vect_cross_C$qp9AD_Vect3Dt1t1	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	77
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
	df	@@vect_cross_C$qp9AD_Vect3Dt1t1
	dw	0
	dw	4122
	dw	0
	dw	44
	dw	0
	dw	0
	dw	0
	db	30
	db	64
	db	118
	db	101
	db	99
	db	116
	db	95
	db	99
	db	114
	db	111
	db	115
	db	115
	db	95
	db	67
	db	36
	db	113
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
	db	116
	db	49
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	45
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65535
	dw	65535
	dw	46
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	65535
	dw	65535
	dw	47
	dw	0
	dw	0
	dw	0
?patch40	equ	@29-@@vect_cross_C$qp9AD_Vect3Dt1t1+2
?patch41	equ	0
?patch42	equ	@29-@@vect_cross_C$qp9AD_Vect3Dt1t1
	dw	2
	dw	6
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@vect_length_C$qp9AD_Vect3D	segment virtual
@@vect_length_C$qp9AD_Vect3D	proc	near
?live16399@0:
	?debug L 206
	push      ebp
	mov       ebp,esp
	?debug L 208
@30:
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax]
	mov       edx,dword ptr [ebp+8]
	fmul      dword ptr [edx]
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+4]
	mov       eax,dword ptr [ebp+8]
	fmul      dword ptr [eax+4]
	faddp      st(1),st
	mov       edx,dword ptr [ebp+8]
	fld       dword ptr [edx+8]
	mov       ecx,dword ptr [ebp+8]
	fmul      dword ptr [ecx+8]
	faddp      st(1),st
	add       esp,-4
	fstp      dword ptr [esp]
	call      @@mysqrt_2$qf
	pop       ecx
	?debug L 209
@32:
@31:
	pop       ebp
	ret 
	?debug L 0
@@vect_length_C$qp9AD_Vect3D	endp
@vect_length_C$qp9AD_Vect3D	ends
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
	dd	?patch43
	dd	?patch44
	dd	?patch45
	df	@@vect_length_C$qp9AD_Vect3D
	dw	0
	dw	4124
	dw	0
	dw	48
	dw	0
	dw	0
	dw	0
	db	27
	db	64
	db	118
	db	101
	db	99
	db	116
	db	95
	db	108
	db	101
	db	110
	db	103
	db	116
	db	104
	db	95
	db	67
	db	36
	db	113
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
	dw	49
	dw	0
	dw	0
	dw	0
?patch43	equ	@32-@@vect_length_C$qp9AD_Vect3D+2
?patch44	equ	0
?patch45	equ	@32-@@vect_length_C$qp9AD_Vect3D
	dw	2
	dw	6
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@vect_length2_C$qp9AD_Vect3D	segment virtual
@@vect_length2_C$qp9AD_Vect3D	proc	near
?live16401@0:
	?debug L 212
	push      ebp
	mov       ebp,esp
	?debug L 214
@33:
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax]
	mov       edx,dword ptr [ebp+8]
	fmul      dword ptr [edx]
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+4]
	mov       eax,dword ptr [ebp+8]
	fmul      dword ptr [eax+4]
	faddp      st(1),st
	mov       edx,dword ptr [ebp+8]
	fld       dword ptr [edx+8]
	mov       ecx,dword ptr [ebp+8]
	fmul      dword ptr [ecx+8]
	faddp      st(1),st
	?debug L 215
@35:
@34:
	pop       ebp
	ret 
	?debug L 0
@@vect_length2_C$qp9AD_Vect3D	endp
@vect_length2_C$qp9AD_Vect3D	ends
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
	dd	?patch46
	dd	?patch47
	dd	?patch48
	df	@@vect_length2_C$qp9AD_Vect3D
	dw	0
	dw	4126
	dw	0
	dw	50
	dw	0
	dw	0
	dw	0
	db	28
	db	64
	db	118
	db	101
	db	99
	db	116
	db	95
	db	108
	db	101
	db	110
	db	103
	db	116
	db	104
	db	50
	db	95
	db	67
	db	36
	db	113
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
	dw	51
	dw	0
	dw	0
	dw	0
?patch46	equ	@35-@@vect_length2_C$qp9AD_Vect3D+2
?patch47	equ	0
?patch48	equ	@35-@@vect_length2_C$qp9AD_Vect3D
	dw	2
	dw	6
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@vect_auto_normalize_C$qp9AD_Vect3D	segment virtual
@@vect_auto_normalize_C$qp9AD_Vect3D	proc	near
?live16402@0:
	?debug L 218
	push      ebp
	mov       ebp,esp
	push      ecx
	push      esi
	?debug L 231
@36:
	mov	 esi, [dword ptr [ebp+8]]
	?debug L 233
	fld	 dword ptr [esi]
	?debug L 234
	fmul	 dword ptr [esi]
	?debug L 235
	fld	 dword ptr [esi+4]
	?debug L 236
	fmul	 dword ptr [esi+4]
	?debug L 238
	fxch	
	?debug L 239
	faddp	 st(1), st
	?debug L 241
	fld	 dword ptr [esi+8]
	?debug L 242
	fmul	 dword ptr [esi+8]
	?debug L 244
	fxch	
	?debug L 245
	faddp	 st(1), st
	?debug L 247
	fsqrt	
	?debug L 248
	fdivr	 dword ptr [_uno]
	?debug L 250
	fstp	 dword ptr [dword ptr [ebp-4]]
	?debug L 252
	fld	 dword ptr [esi]
	?debug L 253
	fmul	 dword ptr [dword ptr [ebp-4]]
	?debug L 254
	fld	 dword ptr [esi+4]
	?debug L 255
	fmul	 dword ptr [dword ptr [ebp-4]]
	?debug L 256
	fld	 dword ptr [esi+8]
	?debug L 257
	fmul	 dword ptr [dword ptr [ebp-4]]
	?debug L 258
	fstp	  dword ptr [esi+8]
	?debug L 259
	fstp	  dword ptr [esi+4]
	?debug L 260
	fstp	  dword ptr [esi]
	?debug L 262
@37:
	pop       esi
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@vect_auto_normalize_C$qp9AD_Vect3D	endp
@vect_auto_normalize_C$qp9AD_Vect3D	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	82
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
	df	@@vect_auto_normalize_C$qp9AD_Vect3D
	dw	0
	dw	4128
	dw	0
	dw	52
	dw	0
	dw	0
	dw	0
	db	35
	db	64
	db	118
	db	101
	db	99
	db	116
	db	95
	db	97
	db	117
	db	116
	db	111
	db	95
	db	110
	db	111
	db	114
	db	109
	db	97
	db	108
	db	105
	db	122
	db	101
	db	95
	db	67
	db	36
	db	113
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
	dw	53
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	64
	dw	0
	dw	54
	dw	0
	dw	0
	dw	0
?patch49	equ	@37-@@vect_auto_normalize_C$qp9AD_Vect3D+4
?patch50	equ	0
?patch51	equ	@37-@@vect_auto_normalize_C$qp9AD_Vect3D
	dw	2
	dw	6
	dw	8
	dw	531
	dw	4
	dw	65528
	dw	65535
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@vect_normalize_C$qp9AD_Vect3Dt1	segment virtual
@@vect_normalize_C$qp9AD_Vect3Dt1	proc	near
?live16403@0:
	?debug L 265
	push      ebp
	mov       ebp,esp
	push      ecx
	?debug L 269
@38:
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax]
	mov       edx,dword ptr [ebp+8]
	fmul      dword ptr [edx]
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+4]
	mov       eax,dword ptr [ebp+8]
	fmul      dword ptr [eax+4]
	faddp      st(1),st
	mov       edx,dword ptr [ebp+8]
	fld       dword ptr [edx+8]
	mov       ecx,dword ptr [ebp+8]
	fmul      dword ptr [ecx+8]
	faddp      st(1),st
	add       esp,-4
	fstp      dword ptr [esp]
	call      @@mysqrt_2$qf
	pop       ecx
	fdivr     dword ptr [@39]
	fstp      dword ptr [ebp-4]
	?debug L 270
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax]
	fmul      dword ptr [ebp-4]
	mov       edx,dword ptr [ebp+12]
	fstp      dword ptr [edx]
	?debug L 271
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+4]
	fmul      dword ptr [ebp-4]
	mov       eax,dword ptr [ebp+12]
	fstp      dword ptr [eax+4]
	?debug L 272
	mov       edx,dword ptr [ebp+8]
	fld       dword ptr [edx+8]
	fmul      dword ptr [ebp-4]
	mov       ecx,dword ptr [ebp+12]
	fstp      dword ptr [ecx+8]
	?debug L 273
@40:
	pop       ecx
	pop       ebp
	ret 
	align 4        
@39:
	db        0,0,128,63
	?debug L 0
@@vect_normalize_C$qp9AD_Vect3Dt1	endp
@vect_normalize_C$qp9AD_Vect3Dt1	ends
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
	dd	?patch52
	dd	?patch53
	dd	?patch54
	df	@@vect_normalize_C$qp9AD_Vect3Dt1
	dw	0
	dw	4130
	dw	0
	dw	55
	dw	0
	dw	0
	dw	0
	db	32
	db	64
	db	118
	db	101
	db	99
	db	116
	db	95
	db	110
	db	111
	db	114
	db	109
	db	97
	db	108
	db	105
	db	122
	db	101
	db	95
	db	67
	db	36
	db	113
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
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	56
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65535
	dw	65535
	dw	57
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	64
	dw	0
	dw	58
	dw	0
	dw	0
	dw	0
?patch52	equ	@40-@@vect_normalize_C$qp9AD_Vect3Dt1+10
?patch53	equ	0
?patch54	equ	@40-@@vect_normalize_C$qp9AD_Vect3Dt1
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@vect_add_asm$qp9AD_Vect3Dt1t1	segment virtual
@@vect_add_asm$qp9AD_Vect3Dt1t1	proc	near
?live16404@0:
	?debug L 279
	push      ebp
	mov       ebp,esp
	push      ebx
	push      esi
	push      edi
	?debug L 283
@41:
	mov	 esi, [dword ptr [ebp+8]]
	?debug L 284
	xor	 ebx, ebx
	?debug L 285
	mov	 edi, [dword ptr [ebp+12]]
	?debug L 286
	mov	 ebx, [dword ptr [ebp+16]]
	?debug L 287
	fld	 dword ptr [esi]
	?debug L 288
	fadd	 dword ptr [edi]
	?debug L 289
	fld	 dword ptr [esi+4]
	?debug L 290
	fadd	 dword ptr [edi+4]
	?debug L 291
	fxch	
	?debug L 292
	fld	 dword ptr [esi+8]
	?debug L 293
	fadd	 dword ptr [edi+8]
	?debug L 294
	fxch	
	?debug L 295
	fstp	 dword ptr [ebx]
	?debug L 296
	fstp	 dword ptr [ebx+8]
	?debug L 297
	fstp	 dword ptr [ebx+4]
	?debug L 299
@42:
	pop       edi
	pop       esi
	pop       ebx
	pop       ebp
	ret 
	?debug L 0
@@vect_add_asm$qp9AD_Vect3Dt1t1	endp
@vect_add_asm$qp9AD_Vect3Dt1t1	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	77
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
	df	@@vect_add_asm$qp9AD_Vect3Dt1t1
	dw	0
	dw	4132
	dw	0
	dw	59
	dw	0
	dw	0
	dw	0
	db	30
	db	64
	db	118
	db	101
	db	99
	db	116
	db	95
	db	97
	db	100
	db	100
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
	db	86
	db	101
	db	99
	db	116
	db	51
	db	68
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
	dw	60
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65535
	dw	65535
	dw	61
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	65535
	dw	65535
	dw	62
	dw	0
	dw	0
	dw	0
?patch55	equ	@42-@@vect_add_asm$qp9AD_Vect3Dt1t1+5
?patch56	equ	0
?patch57	equ	@42-@@vect_add_asm$qp9AD_Vect3Dt1t1
	dw	2
	dw	6
	dw	8
	dw	531
	dw	7
	dw	65524
	dw	65535
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@vect_sub_asm$qp9AD_Vect3Dt1t1	segment virtual
@@vect_sub_asm$qp9AD_Vect3Dt1t1	proc	near
?live16405@0:
	?debug L 302
	push      ebp
	mov       ebp,esp
	push      ebx
	push      esi
	push      edi
	?debug L 306
@43:
	mov	 esi, [dword ptr [ebp+8]]
	?debug L 307
	xor	 ebx, ebx
	?debug L 308
	mov	 ebx, [dword ptr [ebp+16]]
	?debug L 309
	mov	 edi, [dword ptr [ebp+12]]
	?debug L 310
	fld	 dword ptr [esi]
	?debug L 311
	fsub	 dword ptr [edi]
	?debug L 312
	fld	 dword ptr [esi+4]
	?debug L 313
	fsub	 dword ptr [edi+4]
	?debug L 314
	fxch	
	?debug L 315
	fld	 dword ptr [esi+8]
	?debug L 316
	fsub	 dword ptr [edi+8]
	?debug L 317
	fxch	
	?debug L 318
	fstp	 dword ptr [ebx]
	?debug L 319
	fstp	 dword ptr [ebx+8]
	?debug L 320
	fstp	 dword ptr [ebx+4]
	?debug L 322
@44:
	pop       edi
	pop       esi
	pop       ebx
	pop       ebp
	ret 
	?debug L 0
@@vect_sub_asm$qp9AD_Vect3Dt1t1	endp
@vect_sub_asm$qp9AD_Vect3Dt1t1	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	77
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
	df	@@vect_sub_asm$qp9AD_Vect3Dt1t1
	dw	0
	dw	4134
	dw	0
	dw	63
	dw	0
	dw	0
	dw	0
	db	30
	db	64
	db	118
	db	101
	db	99
	db	116
	db	95
	db	115
	db	117
	db	98
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
	db	86
	db	101
	db	99
	db	116
	db	51
	db	68
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
	dw	64
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65535
	dw	65535
	dw	65
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	65535
	dw	65535
	dw	66
	dw	0
	dw	0
	dw	0
?patch58	equ	@44-@@vect_sub_asm$qp9AD_Vect3Dt1t1+5
?patch59	equ	0
?patch60	equ	@44-@@vect_sub_asm$qp9AD_Vect3Dt1t1
	dw	2
	dw	6
	dw	8
	dw	531
	dw	7
	dw	65524
	dw	65535
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@vect_dot_asm$qp9AD_Vect3Dt1	segment virtual
@@vect_dot_asm$qp9AD_Vect3Dt1	proc	near
?live16406@0:
	?debug L 325
	push      ebp
	mov       ebp,esp
	push      esi
	push      edi
	?debug L 329
@45:
	mov	 esi, [dword ptr [ebp+8]]
	?debug L 330
	mov	 edi, [dword ptr [ebp+12]]
	?debug L 332
	fldz	
	?debug L 334
	fld	 dword ptr [esi]
	?debug L 335
	fmul	 dword ptr [edi]
	?debug L 336
	fld	 dword ptr [esi+4]
	?debug L 337
	fmul	 dword ptr [edi+4]
	?debug L 339
	fxch	
	?debug L 340
	faddp	 st(2), st
	?debug L 342
	fld	 dword ptr [esi+8]
	?debug L 343
	fmul	 dword ptr [edi+8]
	?debug L 345
	fxch	
	?debug L 346
	faddp	 st(2), st
	?debug L 348
	fadd	
	?debug L 351
@46:
	pop       edi
	pop       esi
	pop       ebp
	ret 
	?debug L 0
@@vect_dot_asm$qp9AD_Vect3Dt1	endp
@vect_dot_asm$qp9AD_Vect3Dt1	ends
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
	dd	?patch61
	dd	?patch62
	dd	?patch63
	df	@@vect_dot_asm$qp9AD_Vect3Dt1
	dw	0
	dw	4136
	dw	0
	dw	67
	dw	0
	dw	0
	dw	0
	db	28
	db	64
	db	118
	db	101
	db	99
	db	116
	db	95
	db	100
	db	111
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
	db	86
	db	101
	db	99
	db	116
	db	51
	db	68
	db	116
	db	49
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	68
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65535
	dw	65535
	dw	69
	dw	0
	dw	0
	dw	0
?patch61	equ	@46-@@vect_dot_asm$qp9AD_Vect3Dt1+4
?patch62	equ	0
?patch63	equ	@46-@@vect_dot_asm$qp9AD_Vect3Dt1
	dw	2
	dw	6
	dw	8
	dw	531
	dw	6
	dw	65528
	dw	65535
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@vect_normalize_asm$qp9AD_Vect3Dt1	segment virtual
@@vect_normalize_asm$qp9AD_Vect3Dt1	proc	near
?live16407@0:
	?debug L 354
	push      ebp
	mov       ebp,esp
	push      ecx
	push      esi
	push      edi
	?debug L 360
@47:
	mov	 esi, [dword ptr [ebp+8]]
	?debug L 361
	mov	 edi, [dword ptr [ebp+12]]
	?debug L 363
	fld	 dword ptr [esi]
	?debug L 364
	fmul	 dword ptr [esi]
	?debug L 365
	fld	 dword ptr [esi+4]
	?debug L 366
	fmul	 dword ptr [esi+4]
	?debug L 368
	fxch	
	?debug L 369
	faddp	 st(1), st
	?debug L 371
	fld	 dword ptr [esi+8]
	?debug L 372
	fmul	 dword ptr [esi+8]
	?debug L 374
	fxch	
	?debug L 375
	faddp	 st(1), st
	?debug L 377
	fsqrt	
	?debug L 378
	fdivr	 dword ptr [_uno]
	?debug L 380
	fstp	 dword ptr [dword ptr [ebp-4]]  
	?debug L 382
	fld	 dword ptr [esi]
	?debug L 383
	fmul	 dword ptr [dword ptr [ebp-4]]
	?debug L 384
	fld	 dword ptr [esi+4]
	?debug L 385
	fmul	 dword ptr [dword ptr [ebp-4]]
	?debug L 386
	fld	 dword ptr [esi+8]
	?debug L 387
	fmul	 dword ptr [dword ptr [ebp-4]]
	?debug L 388
	fstp	  dword ptr [edi+8]
	?debug L 389
	fstp	  dword ptr [edi+4]
	?debug L 390
	fstp	  dword ptr [edi]
	?debug L 392
@48:
	pop       edi
	pop       esi
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@vect_normalize_asm$qp9AD_Vect3Dt1	endp
@vect_normalize_asm$qp9AD_Vect3Dt1	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	81
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
	df	@@vect_normalize_asm$qp9AD_Vect3Dt1
	dw	0
	dw	4138
	dw	0
	dw	70
	dw	0
	dw	0
	dw	0
	db	34
	db	64
	db	118
	db	101
	db	99
	db	116
	db	95
	db	110
	db	111
	db	114
	db	109
	db	97
	db	108
	db	105
	db	122
	db	101
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
	db	86
	db	101
	db	99
	db	116
	db	51
	db	68
	db	116
	db	49
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65535
	dw	65535
	dw	71
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65535
	dw	65535
	dw	72
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	64
	dw	0
	dw	73
	dw	0
	dw	0
	dw	0
?patch64	equ	@48-@@vect_normalize_asm$qp9AD_Vect3Dt1+5
?patch65	equ	0
?patch66	equ	@48-@@vect_normalize_asm$qp9AD_Vect3Dt1
	dw	2
	dw	6
	dw	8
	dw	531
	dw	6
	dw	65524
	dw	65535
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@vect_auto_normalize_asm$qp9AD_Vect3D	segment virtual
@@vect_auto_normalize_asm$qp9AD_Vect3D	proc	near
?live16408@0:
	?debug L 395
	push      ebp
	mov       ebp,esp
	push      ecx
	push      esi
	?debug L 401
@49:
	mov	 esi, [dword ptr [ebp+8]]
	?debug L 403
	fld	 dword ptr [esi]
	?debug L 404
	fmul	 dword ptr [esi]
	?debug L 405
	fld	 dword ptr [esi+4]
	?debug L 406
	fmul	 dword ptr [esi+4]
	?debug L 408
	fxch	
	?debug L 409
	faddp	 st(1), st
	?debug L 411
	fld	 dword ptr [esi+8]
	?debug L 412
	fmul	 dword ptr [esi+8]
	?debug L 414
	fxch	
	?debug L 415
	faddp	 st(1), st
	?debug L 417
	fsqrt	
	?debug L 418
	fdivr	 dword ptr [_uno]
	?debug L 420
	fstp	 dword ptr [dword ptr [ebp-4]]  
	?debug L 422
	fld	 dword ptr [esi]
	?debug L 423
	fmul	 dword ptr [dword ptr [ebp-4]]
	?debug L 424
	fld	 dword ptr [esi+4]
	?debug L 425
	fmul	 dword ptr [dword ptr [ebp-4]]
	?debug L 426
	fld	 dword ptr [esi+8]
	?debug L 427
	fmul	 dword ptr [dword ptr [ebp-4]]
	?debug L 428
	fstp	  dword ptr [esi+8]
	?debug L 429
	fstp	  dword ptr [esi+4]
	?debug L 430
	fstp	  dword ptr [esi]
	?debug L 432
@50:
	pop       esi
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@vect_auto_normalize_asm$qp9AD_Vect3D	endp
@vect_auto_normalize_asm$qp9AD_Vect3D	ends
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
	dd	?patch67
	dd	?patch68
	dd	?patch69
	df	@@vect_auto_normalize_asm$qp9AD_Vect3D
	dw	0
	dw	4140
	dw	0
	dw	74
	dw	0
	dw	0
	dw	0
	db	37
	db	64
	db	118
	db	101
	db	99
	db	116
	db	95
	db	97
	db	117
	db	116
	db	111
	db	95
	db	110
	db	111
	db	114
	db	109
	db	97
	db	108
	db	105
	db	122
	db	101
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
?patch67	equ	@50-@@vect_auto_normalize_asm$qp9AD_Vect3D+4
?patch68	equ	0
?patch69	equ	@50-@@vect_auto_normalize_asm$qp9AD_Vect3D
	dw	2
	dw	6
	dw	8
	dw	531
	dw	4
	dw	65528
	dw	65535
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@nvect_make$qp7nVectori	segment virtual
@@nvect_make$qp7nVectori	proc	near
?live16409@0:
	?debug L 750
	push      ebp
	mov       ebp,esp
	push      ecx
	?debug L 754
@51:
	mov       eax,dword ptr [ebp+12]
	test      eax,eax
	jl        short @53
	?debug L 756
	mov       edx,dword ptr [ebp+8]
	mov       ecx,dword ptr [ebp+12]
	mov       dword ptr [edx],ecx
	?debug L 757
	mov       eax,dword ptr [ebp+12]
	shl       eax,2
	push      eax
	call      @$bnwa$qui
	pop       ecx
	mov       edx,dword ptr [ebp+8]
	mov       dword ptr [edx+4],eax
	?debug L 758
	xor       ecx,ecx
	mov       dword ptr [ebp-4],ecx
	mov       eax,dword ptr [ebp-4]
	mov       edx,dword ptr [ebp+12]
	cmp       eax,edx
	jge       short @55
@54:
	mov       ecx,dword ptr [ebp+8]
	mov       eax,dword ptr [ecx+4]
	mov       edx,dword ptr [ebp-4]
	xor       ecx,ecx
	mov       dword ptr [eax+4*edx],ecx
	inc       dword ptr [ebp-4]
	mov       eax,dword ptr [ebp-4]
	mov       edx,dword ptr [ebp+12]
	cmp       eax,edx
	jl        short @54
	?debug L 759
@55:
@57:
@53:
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@nvect_make$qp7nVectori	endp
@nvect_make$qp7nVectori	ends
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
	dd	?patch70
	dd	?patch71
	dd	?patch72
	df	@@nvect_make$qp7nVectori
	dw	0
	dw	4142
	dw	0
	dw	77
	dw	0
	dw	0
	dw	0
	db	23
	db	64
	db	110
	db	118
	db	101
	db	99
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
	db	86
	db	101
	db	99
	db	116
	db	111
	db	114
	db	105
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65533
	dw	65535
	dw	78
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	116
	dw	0
	dw	79
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	116
	dw	0
	dw	80
	dw	0
	dw	0
	dw	0
?patch70	equ	@57-@@nvect_make$qp7nVectori+3
?patch71	equ	0
?patch72	equ	@57-@@nvect_make$qp7nVectori
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@nvect_free$qp7nVector	segment virtual
@@nvect_free$qp7nVector	proc	near
?live16410@0:
	?debug L 762
	push      ebp
	mov       ebp,esp
	?debug L 764
@58:
	mov       eax,dword ptr [ebp+8]
	push      dword ptr [eax+4]
	call      @$bdla$qpv
	pop       ecx
	?debug L 765
	mov       edx,dword ptr [ebp+8]
	xor       ecx,ecx
	mov       dword ptr [edx],ecx
	?debug L 766
@59:
	pop       ebp
	ret 
	?debug L 0
@@nvect_free$qp7nVector	endp
@nvect_free$qp7nVector	ends
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
	dd	?patch73
	dd	?patch74
	dd	?patch75
	df	@@nvect_free$qp7nVector
	dw	0
	dw	4144
	dw	0
	dw	81
	dw	0
	dw	0
	dw	0
	db	22
	db	64
	db	110
	db	118
	db	101
	db	99
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
	db	86
	db	101
	db	99
	db	116
	db	111
	db	114
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65533
	dw	65535
	dw	82
	dw	0
	dw	0
	dw	0
?patch73	equ	@59-@@nvect_free$qp7nVector+2
?patch74	equ	0
?patch75	equ	@59-@@nvect_free$qp7nVector
	dw	2
	dw	6
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@nvect_copy$qp7nVectort1	segment virtual
@@nvect_copy$qp7nVectort1	proc	near
?live16411@0:
	?debug L 769
	push      ebp
	mov       ebp,esp
	push      ecx
	?debug L 773
@60:
	xor       eax,eax
	mov       dword ptr [ebp-4],eax
	jmp       short @62
@61:
	mov       edx,dword ptr [ebp+8]
	mov       ecx,dword ptr [edx+4]
	mov       eax,dword ptr [ebp-4]
	mov       edx,dword ptr [ecx+4*eax]
	mov       ecx,dword ptr [ebp+12]
	mov       eax,dword ptr [ecx+4]
	mov       ecx,dword ptr [ebp-4]
	mov       dword ptr [eax+4*ecx],edx
	inc       dword ptr [ebp-4]
@62:
	mov       edx,dword ptr [ebp-4]
	mov       eax,dword ptr [ebp+8]
	mov       ecx,dword ptr [eax]
	cmp       edx,ecx
	jl        short @61
	?debug L 774
@64:
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@nvect_copy$qp7nVectort1	endp
@nvect_copy$qp7nVectort1	ends
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
	dd	?patch76
	dd	?patch77
	dd	?patch78
	df	@@nvect_copy$qp7nVectort1
	dw	0
	dw	4146
	dw	0
	dw	83
	dw	0
	dw	0
	dw	0
	db	24
	db	64
	db	110
	db	118
	db	101
	db	99
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
	db	86
	db	101
	db	99
	db	116
	db	111
	db	114
	db	116
	db	49
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65533
	dw	65535
	dw	84
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65533
	dw	65535
	dw	85
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	116
	dw	0
	dw	86
	dw	0
	dw	0
	dw	0
?patch76	equ	@64-@@nvect_copy$qp7nVectort1+3
?patch77	equ	0
?patch78	equ	@64-@@nvect_copy$qp7nVectort1
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@nvect_add$qp7nVectort1t1	segment virtual
@@nvect_add$qp7nVectort1t1	proc	near
?live16412@0:
	?debug L 777
	push      ebp
	mov       ebp,esp
	push      ecx
	?debug L 781
@65:
	xor       eax,eax
	mov       dword ptr [ebp-4],eax
	jmp       short @67
@66:
	mov       edx,dword ptr [ebp+8]
	mov       ecx,dword ptr [edx+4]
	mov       eax,dword ptr [ebp-4]
	fld       dword ptr [ecx+4*eax]
	mov       edx,dword ptr [ebp+12]
	mov       ecx,dword ptr [edx+4]
	mov       eax,dword ptr [ebp-4]
	fadd      dword ptr [ecx+4*eax]
	mov       edx,dword ptr [ebp+16]
	mov       ecx,dword ptr [edx+4]
	mov       eax,dword ptr [ebp-4]
	fstp      dword ptr [ecx+4*eax]
	inc       dword ptr [ebp-4]
@67:
	mov       edx,dword ptr [ebp-4]
	mov       ecx,dword ptr [ebp+8]
	mov       eax,dword ptr [ecx]
	cmp       edx,eax
	jl        short @66
	?debug L 782
@69:
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@nvect_add$qp7nVectort1t1	endp
@nvect_add$qp7nVectort1t1	ends
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
	dd	?patch79
	dd	?patch80
	dd	?patch81
	df	@@nvect_add$qp7nVectort1t1
	dw	0
	dw	4148
	dw	0
	dw	87
	dw	0
	dw	0
	dw	0
	db	25
	db	64
	db	110
	db	118
	db	101
	db	99
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
	db	86
	db	101
	db	99
	db	116
	db	111
	db	114
	db	116
	db	49
	db	116
	db	49
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65533
	dw	65535
	dw	88
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65533
	dw	65535
	dw	89
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	65533
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
?patch79	equ	@69-@@nvect_add$qp7nVectort1t1+3
?patch80	equ	0
?patch81	equ	@69-@@nvect_add$qp7nVectort1t1
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@nvect_sub$qp7nVectort1t1	segment virtual
@@nvect_sub$qp7nVectort1t1	proc	near
?live16413@0:
	?debug L 785
	push      ebp
	mov       ebp,esp
	push      ecx
	?debug L 789
@70:
	xor       eax,eax
	mov       dword ptr [ebp-4],eax
	jmp       short @72
@71:
	mov       edx,dword ptr [ebp+8]
	mov       ecx,dword ptr [edx+4]
	mov       eax,dword ptr [ebp-4]
	fld       dword ptr [ecx+4*eax]
	mov       edx,dword ptr [ebp+12]
	mov       ecx,dword ptr [edx+4]
	mov       eax,dword ptr [ebp-4]
	fsub      dword ptr [ecx+4*eax]
	mov       edx,dword ptr [ebp+16]
	mov       ecx,dword ptr [edx+4]
	mov       eax,dword ptr [ebp-4]
	fstp      dword ptr [ecx+4*eax]
	inc       dword ptr [ebp-4]
@72:
	mov       edx,dword ptr [ebp-4]
	mov       ecx,dword ptr [ebp+8]
	mov       eax,dword ptr [ecx]
	cmp       edx,eax
	jl        short @71
	?debug L 790
@74:
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@nvect_sub$qp7nVectort1t1	endp
@nvect_sub$qp7nVectort1t1	ends
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
	dd	?patch82
	dd	?patch83
	dd	?patch84
	df	@@nvect_sub$qp7nVectort1t1
	dw	0
	dw	4150
	dw	0
	dw	92
	dw	0
	dw	0
	dw	0
	db	25
	db	64
	db	110
	db	118
	db	101
	db	99
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
	db	86
	db	101
	db	99
	db	116
	db	111
	db	114
	db	116
	db	49
	db	116
	db	49
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65533
	dw	65535
	dw	93
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65533
	dw	65535
	dw	94
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	65533
	dw	65535
	dw	95
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	116
	dw	0
	dw	96
	dw	0
	dw	0
	dw	0
?patch82	equ	@74-@@nvect_sub$qp7nVectort1t1+3
?patch83	equ	0
?patch84	equ	@74-@@nvect_sub$qp7nVectort1t1
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@nvect_neg$qp7nVectort1	segment virtual
@@nvect_neg$qp7nVectort1	proc	near
?live16414@0:
	?debug L 793
	push      ebp
	mov       ebp,esp
	push      ecx
	?debug L 797
@75:
	xor       eax,eax
	mov       dword ptr [ebp-4],eax
	jmp       short @77
@76:
	mov       edx,dword ptr [ebp+8]
	mov       ecx,dword ptr [edx+4]
	mov       eax,dword ptr [ebp-4]
	fld       dword ptr [ecx+4*eax]
	fchs
	mov       edx,dword ptr [ebp+12]
	mov       ecx,dword ptr [edx+4]
	mov       eax,dword ptr [ebp-4]
	fstp      dword ptr [ecx+4*eax]
	inc       dword ptr [ebp-4]
@77:
	mov       edx,dword ptr [ebp-4]
	mov       ecx,dword ptr [ebp+8]
	mov       eax,dword ptr [ecx]
	cmp       edx,eax
	jl        short @76
	?debug L 798
@79:
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@nvect_neg$qp7nVectort1	endp
@nvect_neg$qp7nVectort1	ends
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
	dd	?patch85
	dd	?patch86
	dd	?patch87
	df	@@nvect_neg$qp7nVectort1
	dw	0
	dw	4152
	dw	0
	dw	97
	dw	0
	dw	0
	dw	0
	db	23
	db	64
	db	110
	db	118
	db	101
	db	99
	db	116
	db	95
	db	110
	db	101
	db	103
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
	db	116
	db	49
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65533
	dw	65535
	dw	98
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65533
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
?patch85	equ	@79-@@nvect_neg$qp7nVectort1+3
?patch86	equ	0
?patch87	equ	@79-@@nvect_neg$qp7nVectort1
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@nvect_scale$qp7nVectorft1	segment virtual
@@nvect_scale$qp7nVectorft1	proc	near
?live16415@0:
	?debug L 801
	push      ebp
	mov       ebp,esp
	push      ecx
	?debug L 805
@80:
	xor       eax,eax
	mov       dword ptr [ebp-4],eax
	jmp       short @82
@81:
	fld       dword ptr [ebp+12]
	mov       edx,dword ptr [ebp+8]
	mov       ecx,dword ptr [edx+4]
	mov       eax,dword ptr [ebp-4]
	fmul      dword ptr [ecx+4*eax]
	mov       edx,dword ptr [ebp+16]
	mov       ecx,dword ptr [edx+4]
	mov       eax,dword ptr [ebp-4]
	fstp      dword ptr [ecx+4*eax]
	inc       dword ptr [ebp-4]
@82:
	mov       edx,dword ptr [ebp-4]
	mov       ecx,dword ptr [ebp+8]
	mov       eax,dword ptr [ecx]
	cmp       edx,eax
	jl        short @81
	?debug L 806
@84:
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@nvect_scale$qp7nVectorft1	endp
@nvect_scale$qp7nVectorft1	ends
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
	dd	?patch88
	dd	?patch89
	dd	?patch90
	df	@@nvect_scale$qp7nVectorft1
	dw	0
	dw	4154
	dw	0
	dw	101
	dw	0
	dw	0
	dw	0
	db	26
	db	64
	db	110
	db	118
	db	101
	db	99
	db	116
	db	95
	db	115
	db	99
	db	97
	db	108
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
	db	102
	db	116
	db	49
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65533
	dw	65535
	dw	102
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	64
	dw	0
	dw	103
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	65533
	dw	65535
	dw	104
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	116
	dw	0
	dw	105
	dw	0
	dw	0
	dw	0
?patch88	equ	@84-@@nvect_scale$qp7nVectorft1+3
?patch89	equ	0
?patch90	equ	@84-@@nvect_scale$qp7nVectorft1
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@nvect_dot$qp7nVectort1	segment virtual
@@nvect_dot$qp7nVectort1	proc	near
?live16416@0:
	?debug L 809
	push      ebp
	mov       ebp,esp
	add       esp,-8
	?debug L 812
@85:
	xor       eax,eax
	mov       dword ptr [ebp-8],eax
	?debug L 814
	xor       edx,edx
	mov       dword ptr [ebp-4],edx
	jmp       short @87
@86:
	mov       ecx,dword ptr [ebp+12]
	mov       eax,dword ptr [ecx+4]
	mov       edx,dword ptr [ebp-4]
	fld       dword ptr [eax+4*edx]
	mov       ecx,dword ptr [ebp+8]
	mov       eax,dword ptr [ecx+4]
	mov       edx,dword ptr [ebp-4]
	fmul      dword ptr [eax+4*edx]
	fadd      dword ptr [ebp-8]
	fstp      dword ptr [ebp-8]
	inc       dword ptr [ebp-4]
@87:
	mov       ecx,dword ptr [ebp+8]
	mov       eax,dword ptr [ecx]
	mov       edx,dword ptr [ebp-4]
	cmp       eax,edx
	jg        short @86
	?debug L 815
	fld       dword ptr [ebp-8]
	?debug L 816
@90:
@89:
	pop       ecx
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@nvect_dot$qp7nVectort1	endp
@nvect_dot$qp7nVectort1	ends
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
	dd	?patch91
	dd	?patch92
	dd	?patch93
	df	@@nvect_dot$qp7nVectort1
	dw	0
	dw	4156
	dw	0
	dw	106
	dw	0
	dw	0
	dw	0
	db	23
	db	64
	db	110
	db	118
	db	101
	db	99
	db	116
	db	95
	db	100
	db	111
	db	116
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
	db	116
	db	49
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65533
	dw	65535
	dw	107
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65533
	dw	65535
	dw	108
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65528
	dw	65535
	dw	64
	dw	0
	dw	109
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	116
	dw	0
	dw	110
	dw	0
	dw	0
	dw	0
?patch91	equ	@90-@@nvect_dot$qp7nVectort1+4
?patch92	equ	0
?patch93	equ	@90-@@nvect_dot$qp7nVectort1
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@nvect_lenght$qp7nVector	segment virtual
@@nvect_lenght$qp7nVector	proc	near
?live16417@0:
	?debug L 819
	push      ebp
	mov       ebp,esp
	add       esp,-8
	?debug L 822
@91:
	xor       eax,eax
	mov       dword ptr [ebp-8],eax
	?debug L 824
	xor       edx,edx
	mov       dword ptr [ebp-4],edx
	jmp       short @93
@92:
	mov       ecx,dword ptr [ebp+8]
	mov       eax,dword ptr [ecx+4]
	mov       edx,dword ptr [ebp-4]
	fld       dword ptr [eax+4*edx]
	mov       ecx,dword ptr [ebp+8]
	mov       eax,dword ptr [ecx+4]
	mov       edx,dword ptr [ebp-4]
	fmul      dword ptr [eax+4*edx]
	fadd      dword ptr [ebp-8]
	fstp      dword ptr [ebp-8]
	inc       dword ptr [ebp-4]
@93:
	mov       ecx,dword ptr [ebp-4]
	mov       eax,dword ptr [ebp+8]
	mov       edx,dword ptr [eax]
	cmp       ecx,edx
	jl        short @92
	?debug L 825
	push      dword ptr [ebp-8]
	call      @@mysqrt_2$qf
	pop       ecx
	?debug L 826
@96:
@95:
	pop       ecx
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@nvect_lenght$qp7nVector	endp
@nvect_lenght$qp7nVector	ends
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
	dd	?patch94
	dd	?patch95
	dd	?patch96
	df	@@nvect_lenght$qp7nVector
	dw	0
	dw	4158
	dw	0
	dw	111
	dw	0
	dw	0
	dw	0
	db	24
	db	64
	db	110
	db	118
	db	101
	db	99
	db	116
	db	95
	db	108
	db	101
	db	110
	db	103
	db	104
	db	116
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
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65533
	dw	65535
	dw	112
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65528
	dw	65535
	dw	64
	dw	0
	dw	113
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	116
	dw	0
	dw	114
	dw	0
	dw	0
	dw	0
?patch94	equ	@96-@@nvect_lenght$qp7nVector+4
?patch95	equ	0
?patch96	equ	@96-@@nvect_lenght$qp7nVector
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@nvect_normalize$qp7nVector	segment virtual
@@nvect_normalize$qp7nVector	proc	near
?live16418@0:
	?debug L 829
	push      ebp
	mov       ebp,esp
	add       esp,-8
	?debug L 832
@97:
	xor       eax,eax
	mov       dword ptr [ebp-8],eax
	?debug L 834
	xor       edx,edx
	mov       dword ptr [ebp-4],edx
	jmp       short @99
@98:
	mov       ecx,dword ptr [ebp+8]
	mov       eax,dword ptr [ecx+4]
	mov       edx,dword ptr [ebp-4]
	fld       dword ptr [eax+4*edx]
	mov       ecx,dword ptr [ebp+8]
	mov       eax,dword ptr [ecx+4]
	mov       edx,dword ptr [ebp-4]
	fmul      dword ptr [eax+4*edx]
	fadd      dword ptr [ebp-8]
	fstp      dword ptr [ebp-8]
	inc       dword ptr [ebp-4]
@99:
	mov       ecx,dword ptr [ebp-4]
	mov       eax,dword ptr [ebp+8]
	mov       edx,dword ptr [eax]
	cmp       ecx,edx
	jl        short @98
	?debug L 836
	fld       dword ptr [@101]
	fdiv      dword ptr [ebp-8]
	fstp      dword ptr [ebp-8]
	?debug L 837
	xor       ecx,ecx
	mov       dword ptr [ebp-4],ecx
	jmp       short @103
@102:
	mov       eax,dword ptr [ebp+8]
	mov       edx,dword ptr [eax+4]
	mov       ecx,dword ptr [ebp-4]
	fld       dword ptr [edx+4*ecx]
	fmul      dword ptr [ebp-8]
	mov       eax,dword ptr [ebp+8]
	mov       edx,dword ptr [eax+4]
	mov       ecx,dword ptr [ebp-4]
	fstp      dword ptr [edx+4*ecx]
	inc       dword ptr [ebp-4]
@103:
	mov       eax,dword ptr [ebp-4]
	mov       edx,dword ptr [ebp+8]
	mov       ecx,dword ptr [edx]
	cmp       eax,ecx
	jl        short @102
	?debug L 838
@105:
	pop       ecx
	pop       ecx
	pop       ebp
	ret 
	align 4        
@101:
	db        0,0,128,63
	?debug L 0
@@nvect_normalize$qp7nVector	endp
@nvect_normalize$qp7nVector	ends
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
	dd	?patch97
	dd	?patch98
	dd	?patch99
	df	@@nvect_normalize$qp7nVector
	dw	0
	dw	4160
	dw	0
	dw	115
	dw	0
	dw	0
	dw	0
	db	27
	db	64
	db	110
	db	118
	db	101
	db	99
	db	116
	db	95
	db	110
	db	111
	db	114
	db	109
	db	97
	db	108
	db	105
	db	122
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
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65533
	dw	65535
	dw	116
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65528
	dw	65535
	dw	64
	dw	0
	dw	117
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	116
	dw	0
	dw	118
	dw	0
	dw	0
	dw	0
?patch97	equ	@105-@@nvect_normalize$qp7nVector+8
?patch98	equ	0
?patch99	equ	@105-@@nvect_normalize$qp7nVector
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@mysqrt_2$qf	segment virtual
@@mysqrt_2$qf	proc	near
?live16400@0:
	?debug L 42
	push      ebp
	mov       ebp,esp
	?debug L 44
@106:
	fld       dword ptr [ebp+8]
	add       esp,-8
	fstp      qword ptr [esp]
	call      @_sqrt
	add       esp,8
	?debug L 45
@108:
@107:
	pop       ebp
	ret 
	?debug L 0
@@mysqrt_2$qf	endp
@mysqrt_2$qf	ends
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
	dd	?patch100
	dd	?patch101
	dd	?patch102
	df	@@mysqrt_2$qf
	dw	0
	dw	4162
	dw	0
	dw	119
	dw	0
	dw	0
	dw	0
	db	12
	db	64
	db	109
	db	121
	db	115
	db	113
	db	114
	db	116
	db	95
	db	50
	db	36
	db	113
	db	102
	dw	18
	dw	512
	dw	8
	dw	0
	dw	64
	dw	0
	dw	120
	dw	0
	dw	0
	dw	0
?patch100	equ	@108-@@mysqrt_2$qf+2
?patch101	equ	0
?patch102	equ	@108-@@mysqrt_2$qf
	dw	2
	dw	6
$$BSYMS	ends
_DATA	segment dword public use32 'DATA'
s@	label	byte
	;	s@+0:
	db	"x=%f  y=%f  z=%f ",10,0
	align	4
_DATA	ends
_TEXT	segment dword public use32 'CODE'
_TEXT	ends
	public	_vect_auto_neg
	public	_vect_auto_normalize
	public	_vect_neg
	public	_vect_auto_add
	public	_vect_auto_sub
	public	_vect_normalize
	public	_vect_add
	public	_vect_sub
	public	_vect_cross
	public	_vect_scale
	public	_vect_auto_scale
	public	_vect_dot
	public	_vect_length
	public	_vect_length2
	public	_uno
@_fprintf equ _fprintf
 extrn   _fprintf:near
 extrn   @$bnwa$qui:near
 extrn   @$bdla$qpv:near
@_sqrt equ _sqrt
 extrn   _sqrt:near
	extrn	__turboFloat:word
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	22
	dw	514
	df	_vect_auto_neg
	dw	0
	dw	4164
	dw	0
	dw	121
	dw	0
	dw	0
	dw	0
	dw	22
	dw	514
	df	_vect_auto_normalize
	dw	0
	dw	4164
	dw	0
	dw	122
	dw	0
	dw	0
	dw	0
	dw	22
	dw	514
	df	_vect_neg
	dw	0
	dw	4167
	dw	0
	dw	123
	dw	0
	dw	0
	dw	0
	dw	22
	dw	514
	df	_vect_auto_add
	dw	0
	dw	4167
	dw	0
	dw	124
	dw	0
	dw	0
	dw	0
	dw	22
	dw	514
	df	_vect_auto_sub
	dw	0
	dw	4167
	dw	0
	dw	125
	dw	0
	dw	0
	dw	0
	dw	22
	dw	514
	df	_vect_normalize
	dw	0
	dw	4167
	dw	0
	dw	126
	dw	0
	dw	0
	dw	0
	dw	22
	dw	514
	df	_vect_add
	dw	0
	dw	4170
	dw	0
	dw	127
	dw	0
	dw	0
	dw	0
	dw	22
	dw	514
	df	_vect_sub
	dw	0
	dw	4170
	dw	0
	dw	128
	dw	0
	dw	0
	dw	0
	dw	22
	dw	514
	df	_vect_cross
	dw	0
	dw	4170
	dw	0
	dw	129
	dw	0
	dw	0
	dw	0
	dw	22
	dw	514
	df	_vect_scale
	dw	0
	dw	4173
	dw	0
	dw	130
	dw	0
	dw	0
	dw	0
	dw	22
	dw	514
	df	_vect_auto_scale
	dw	0
	dw	4176
	dw	0
	dw	131
	dw	0
	dw	0
	dw	0
	dw	22
	dw	514
	df	_vect_dot
	dw	0
	dw	4179
	dw	0
	dw	132
	dw	0
	dw	0
	dw	0
	dw	22
	dw	514
	df	_vect_length
	dw	0
	dw	4182
	dw	0
	dw	133
	dw	0
	dw	0
	dw	0
	dw	22
	dw	514
	df	_vect_length2
	dw	0
	dw	4182
	dw	0
	dw	134
	dw	0
	dw	0
	dw	0
	dw	22
	dw	514
	df	_uno
	dw	0
	dw	64
	dw	0
	dw	135
	dw	0
	dw	0
	dw	0
	dw	?patch103
	dw	37
	dw	136
	dw	0
	dw	0
	dw	0
	dw	0
?patch103	equ	12
	dw	?patch104
	dw	38
	dw	198
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
?patch104	equ	796
	dw	?patch105
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
?patch105	equ	18
$$BSYMS	ends
$$BTYPES	segment byte public use32 'DEBTYP'
	db        2,0,0,0,14,0,8,0,3,0,0,0,0,0,2,0
	db        1,16,0,0,12,0,1,2,2,0,255,255,255,255,255,255
	db        255,255,14,0,8,0,3,0,0,0,0,0,4,0,3,16
	db        0,0,20,0,1,2,4,0,255,255,255,255,64,0,0,0
	db        64,0,0,0,64,0,0,0,14,0,8,0,3,0,0,0
	db        0,0,1,0,5,16,0,0,8,0,1,2,1,0,255,255
	db        255,255,14,0,8,0,3,0,0,0,0,0,2,0,7,16
	db        0,0,12,0,1,2,2,0,255,255,255,255,254,255,255,255
	db        14,0,8,0,3,0,0,0,0,0,3,0,9,16,0,0
	db        16,0,1,2,3,0,255,255,255,255,255,255,255,255,255,255
	db        255,255,14,0,8,0,3,0,0,0,0,0,2,0,11,16
	db        0,0,12,0,1,2,2,0,255,255,255,255,255,255,255,255
	db        14,0,8,0,3,0,0,0,0,0,3,0,13,16,0,0
	db        16,0,1,2,3,0,255,255,255,255,255,255,255,255,255,255
	db        255,255,14,0,8,0,3,0,0,0,0,0,2,0,15,16
	db        0,0,12,0,1,2,2,0,255,255,255,255,255,255,255,255
	db        14,0,8,0,3,0,0,0,0,0,2,0,17,16,0,0
	db        12,0,1,2,2,0,255,255,255,255,255,255,255,255,14,0
	db        8,0,3,0,0,0,0,0,1,0,19,16,0,0,8,0
	db        1,2,1,0,255,255,255,255,14,0,8,0,3,0,0,0
	db        0,0,3,0,21,16,0,0,16,0,1,2,3,0,255,255
	db        255,255,64,0,0,0,255,255,255,255,14,0,8,0,3,0
	db        0,0,0,0,2,0,23,16,0,0,12,0,1,2,2,0
	db        255,255,255,255,64,0,0,0,14,0,8,0,64,0,0,0
	db        0,0,2,0,25,16,0,0,12,0,1,2,2,0,255,255
	db        255,255,255,255,255,255,14,0,8,0,3,0,0,0,0,0
	db        3,0,27,16,0,0,16,0,1,2,3,0,255,255,255,255
	db        255,255,255,255,255,255,255,255,14,0,8,0,64,0,0,0
	db        0,0,1,0,29,16,0,0,8,0,1,2,1,0,255,255
	db        255,255,14,0,8,0,64,0,0,0,0,0,1,0,31,16
	db        0,0,8,0,1,2,1,0,255,255,255,255,14,0,8,0
	db        3,0,0,0,0,0,1,0,33,16,0,0,8,0,1,2
	db        1,0,255,255,255,255,14,0,8,0,3,0,0,0,0,0
	db        2,0,35,16,0,0,12,0,1,2,2,0,255,255,255,255
	db        255,255,255,255,14,0,8,0,3,0,0,0,0,0,3,0
	db        37,16,0,0,16,0,1,2,3,0,255,255,255,255,255,255
	db        255,255,255,255,255,255,14,0,8,0,3,0,0,0,0,0
	db        3,0,39,16,0,0,16,0,1,2,3,0,255,255,255,255
	db        255,255,255,255,255,255,255,255,14,0,8,0,64,0,0,0
	db        0,0,2,0,41,16,0,0,12,0,1,2,2,0,255,255
	db        255,255,255,255,255,255,14,0,8,0,3,0,0,0,0,0
	db        2,0,43,16,0,0,12,0,1,2,2,0,255,255,255,255
	db        255,255,255,255,14,0,8,0,3,0,0,0,0,0,1,0
	db        45,16,0,0,8,0,1,2,1,0,255,255,255,255,14,0
	db        8,0,3,0,0,0,0,0,2,0,47,16,0,0,12,0
	db        1,2,2,0,253,255,255,255,116,0,0,0,14,0,8,0
	db        3,0,0,0,0,0,1,0,49,16,0,0,8,0,1,2
	db        1,0,253,255,255,255,14,0,8,0,3,0,0,0,0,0
	db        2,0,51,16,0,0,12,0,1,2,2,0,253,255,255,255
	db        253,255,255,255,14,0,8,0,3,0,0,0,0,0,3,0
	db        53,16,0,0,16,0,1,2,3,0,253,255,255,255,253,255
	db        255,255,253,255,255,255,14,0,8,0,3,0,0,0,0,0
	db        3,0,55,16,0,0,16,0,1,2,3,0,253,255,255,255
	db        253,255,255,255,253,255,255,255,14,0,8,0,3,0,0,0
	db        0,0,2,0,57,16,0,0,12,0,1,2,2,0,253,255
	db        255,255,253,255,255,255,14,0,8,0,3,0,0,0,0,0
	db        3,0,59,16,0,0,16,0,1,2,3,0,253,255,255,255
	db        64,0,0,0,253,255,255,255,14,0,8,0,64,0,0,0
	db        0,0,2,0,61,16,0,0,12,0,1,2,2,0,253,255
	db        255,255,253,255,255,255,14,0,8,0,64,0,0,0,0,0
	db        1,0,63,16,0,0,8,0,1,2,1,0,253,255,255,255
	db        14,0,8,0,3,0,0,0,0,0,1,0,65,16,0,0
	db        8,0,1,2,1,0,253,255,255,255,14,0,8,0,64,0
	db        0,0,0,0,1,0,67,16,0,0,8,0,1,2,1,0
	db        64,0,0,0,8,0,2,0,10,0,69,16,0,0,14,0
	db        8,0,3,0,0,0,0,0,1,0,70,16,0,0,8,0
	db        1,2,1,0,255,255,255,255,8,0,2,0,10,0,72,16
	db        0,0,14,0,8,0,3,0,0,0,0,0,2,0,73,16
	db        0,0,12,0,1,2,2,0,255,255,255,255,255,255,255,255
	db        8,0,2,0,10,0,75,16,0,0,14,0,8,0,3,0
	db        0,0,0,0,3,0,76,16,0,0,16,0,1,2,3,0
	db        255,255,255,255,255,255,255,255,255,255,255,255,8,0,2,0
	db        10,0,78,16,0,0,14,0,8,0,3,0,0,0,0,0
	db        3,0,79,16,0,0,16,0,1,2,3,0,255,255,255,255
	db        64,0,0,0,255,255,255,255,8,0,2,0,10,0,81,16
	db        0,0,14,0,8,0,3,0,0,0,0,0,2,0,82,16
	db        0,0,12,0,1,2,2,0,255,255,255,255,64,0,0,0
	db        8,0,2,0,10,0,84,16,0,0,14,0,8,0,64,0
	db        0,0,0,0,2,0,85,16,0,0,12,0,1,2,2,0
	db        255,255,255,255,255,255,255,255,8,0,2,0,10,0,87,16
	db        0,0,14,0,8,0,64,0,0,0,0,0,1,0,88,16
	db        0,0,8,0,1,2,1,0,255,255,255,255,14,0,8,0
	db        116,0,0,0,64,0,3,0,92,16,0,0,8,0,2,0
	db        10,0,91,16,0,0,8,0,1,0,1,0,112,0,0,0
	db        16,0,1,2,3,0,254,255,255,255,90,16,0,0,0,0
	db        0,0,14,0,8,0,3,4,0,0,0,0,1,0,94,16
	db        0,0,8,0,1,2,1,0,117,0,0,0,14,0,8,0
	db        3,0,0,0,0,0,1,0,96,16,0,0,8,0,1,2
	db        1,0,3,4,0,0,14,0,8,0,65,0,0,0,0,0
	db        1,0,98,16,0,0,8,0,1,2,1,0,65,0,0,0
$$BTYPES	ends
$$BNAMES	segment byte public use32 'DEBNAM'
	db	9,'vect_copy'
	db	1,'a'
	db	1,'b'
	db	8,'vect_set'
	db	1,'a'
	db	2,'fx'
	db	2,'fy'
	db	2,'fz'
	db	13,'vect_swap_3DS'
	db	1,'a'
	db	3,'tmp'
	db	13,'print_to_file'
	db	1,'a'
	db	4,'dove'
	db	10,'vect_add_C'
	db	1,'a'
	db	1,'b'
	db	1,'c'
	db	15,'vect_auto_add_C'
	db	1,'a'
	db	1,'b'
	db	10,'vect_sub_C'
	db	1,'a'
	db	1,'b'
	db	1,'c'
	db	15,'vect_auto_sub_C'
	db	1,'a'
	db	1,'b'
	db	10,'vect_neg_C'
	db	1,'a'
	db	1,'b'
	db	15,'vect_auto_neg_C'
	db	1,'a'
	db	12,'vect_scale_C'
	db	1,'a'
	db	1,'s'
	db	1,'b'
	db	17,'vect_auto_scale_C'
	db	1,'a'
	db	1,'s'
	db	10,'vect_dot_C'
	db	1,'a'
	db	1,'b'
	db	12,'vect_cross_C'
	db	1,'a'
	db	1,'b'
	db	1,'c'
	db	13,'vect_length_C'
	db	1,'a'
	db	14,'vect_length2_C'
	db	1,'a'
	db	21,'vect_auto_normalize_C'
	db	1,'a'
	db	3,'aux'
	db	16,'vect_normalize_C'
	db	1,'a'
	db	1,'b'
	db	3,'aux'
	db	12,'vect_add_asm'
	db	1,'a'
	db	1,'b'
	db	1,'c'
	db	12,'vect_sub_asm'
	db	1,'a'
	db	1,'b'
	db	1,'c'
	db	12,'vect_dot_asm'
	db	1,'a'
	db	1,'b'
	db	18,'vect_normalize_asm'
	db	1,'a'
	db	1,'b'
	db	3,'aux'
	db	23,'vect_auto_normalize_asm'
	db	1,'a'
	db	3,'aux'
	db	10,'nvect_make'
	db	1,'a'
	db	3,'dim'
	db	1,'t'
	db	10,'nvect_free'
	db	1,'a'
	db	10,'nvect_copy'
	db	1,'a'
	db	1,'b'
	db	1,'t'
	db	9,'nvect_add'
	db	1,'a'
	db	1,'b'
	db	1,'c'
	db	1,'t'
	db	9,'nvect_sub'
	db	1,'a'
	db	1,'b'
	db	1,'c'
	db	1,'t'
	db	9,'nvect_neg'
	db	1,'a'
	db	1,'b'
	db	1,'t'
	db	11,'nvect_scale'
	db	1,'a'
	db	1,'s'
	db	1,'b'
	db	1,'t'
	db	9,'nvect_dot'
	db	1,'a'
	db	1,'b'
	db	4,'faux'
	db	1,'t'
	db	12,'nvect_lenght'
	db	1,'a'
	db	4,'faux'
	db	1,'t'
	db	15,'nvect_normalize'
	db	1,'a'
	db	4,'faux'
	db	1,'t'
	db	8,'mysqrt_2'
	db	5,'value'
	db	13,'vect_auto_neg'
	db	19,'vect_auto_normalize'
	db	8,'vect_neg'
	db	13,'vect_auto_add'
	db	13,'vect_auto_sub'
	db	14,'vect_normalize'
	db	8,'vect_add'
	db	8,'vect_sub'
	db	10,'vect_cross'
	db	10,'vect_scale'
	db	15,'vect_auto_scale'
	db	8,'vect_dot'
	db	11,'vect_length'
	db	12,'vect_length2'
	db	3,'uno'
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
	?debug	D "D:\redshift\Demo_sw\Math\VECT3D.H" 11379 36509
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\_null.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\_defs.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\_stddef.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\MATH.H" 10503 10272
	?debug	D "D:\redshift\Demo_sw\Math\vect3d.cpp" 11319 20564
 ?debug  C FB062A633A5C50524F4752417E315C626F726C616E645C434255494C447E315C6C69625C76636C35302E233047852B0000F77A872C
 ?debug  C FB050B232A41445F566563743344
 ?debug  C FB0506232A46494C45
 ?debug  C FB0509232A6E566563746F72
	end
