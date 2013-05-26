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
	?debug	S "D:\redshift\Demo_sw\Math\fastmath.cpp"
	?debug	T "D:\redshift\Demo_sw\Math\fastmath.cpp"
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
_fast_sqrt_table	label	dword
	db	1024	dup(?)
	align	4
_fast_rcpsqrt_table	label	dword
	db	1024	dup(?)
_BSS	ends
_DATA	segment dword public use32 'DATA'
	align	4
_Scale16	label	dword
	db        0,0,128,71
	align	4
_Scale5	label	dword
	db        0,0,0,66
	align	4
_half	label	dword
	db        0,0,0,63
_DATA	ends
_BSS	segment dword public use32 'BSS'
	align	2
_OldFPUCW	label	word
	db	2	dup(?)
	align	2
_FPUCW	label	word
	db	2	dup(?)
_BSS	ends
_TEXT	segment dword public use32 'CODE'
@build_sqrt_tables$qv	segment virtual
@@build_sqrt_tables$qv	proc	near
?live16385@0:
	?debug L 17
	push      ebp
	mov       ebp,esp
	add       esp,-12
	?debug L 21
@1:
	lea       eax,dword ptr [ebp-8]
	mov       dword ptr [ebp-12],eax
	?debug L 23
	mov       word ptr [ebp-2],0
	?debug L 26
@2:
	mov       edx,dword ptr [ebp-12]
	xor       ecx,ecx
	mov       dword ptr [edx],ecx
	?debug L 29
	movzx     eax,word ptr [ebp-2]
	shl       eax,16
	or        eax,1065353216
	mov       edx,dword ptr [ebp-12]
	mov       dword ptr [edx],eax
	?debug L 30
	fld       dword ptr [ebp-8]
	add       esp,-8
	fstp      qword ptr [esp]
	call      @_sqrt
	add       esp,8
	fstp      dword ptr [ebp-8]
	?debug L 31
	mov       ecx,dword ptr [ebp-12]
	mov       eax,dword ptr [ecx]
	and       eax,8388607
	movzx     edx,word ptr [ebp-2]
	mov       dword ptr [_fast_sqrt_table+4*edx],eax
	?debug L 33
	movzx     ecx,word ptr [ebp-2]
	shl       ecx,16
	or        ecx,1065353216
	mov       eax,dword ptr [ebp-12]
	mov       dword ptr [eax],ecx
	?debug L 34
	fld       dword ptr [ebp-8]
	add       esp,-8
	fstp      qword ptr [esp]
	call      @_sqrt
	add       esp,8
	fdivr     dword ptr [@4]
	fstp      dword ptr [ebp-8]
	?debug L 35
	mov       edx,dword ptr [ebp-12]
	mov       ecx,dword ptr [edx]
	and       ecx,8388607
	movzx     eax,word ptr [ebp-2]
	mov       dword ptr [_fast_rcpsqrt_table+4*eax],ecx
	?debug L 39
	mov       edx,dword ptr [ebp-12]
	xor       ecx,ecx
	mov       dword ptr [edx],ecx
	?debug L 40
	movzx     eax,word ptr [ebp-2]
	shl       eax,16
	or        eax,1073741824
	mov       edx,dword ptr [ebp-12]
	mov       dword ptr [edx],eax
	?debug L 41
	fld       dword ptr [ebp-8]
	add       esp,-8
	fstp      qword ptr [esp]
	call      @_sqrt
	add       esp,8
	fstp      dword ptr [ebp-8]
	?debug L 42
	mov       ecx,dword ptr [ebp-12]
	mov       eax,dword ptr [ecx]
	and       eax,8388607
	movzx     edx,word ptr [ebp-2]
	mov       dword ptr [_fast_sqrt_table+512+4*edx],eax
	?debug L 44
	movzx     ecx,word ptr [ebp-2]
	shl       ecx,16
	or        ecx,1073741824
	mov       eax,dword ptr [ebp-12]
	mov       dword ptr [eax],ecx
	?debug L 45
	fld       dword ptr [ebp-8]
	add       esp,-8
	fstp      qword ptr [esp]
	call      @_sqrt
	add       esp,8
	fdivr     dword ptr [@4]
	fstp      dword ptr [ebp-8]
	?debug L 46
	mov       edx,dword ptr [ebp-12]
	mov       ecx,dword ptr [edx]
	and       ecx,8388607
	movzx     eax,word ptr [ebp-2]
	mov       dword ptr [_fast_rcpsqrt_table+512+4*eax],ecx
	inc       word ptr [ebp-2]
	cmp       word ptr [ebp-2],127
	jbe       @2
	?debug L 48
@6:
	mov       esp,ebp
	pop       ebp
	ret 
	align 4        
@4:
	db        0,0,128,63
	?debug L 0
@@build_sqrt_tables$qv	endp
@build_sqrt_tables$qv	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	db	2
	db	0
	db	0
	db	0
	dw	68
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
	df	@@build_sqrt_tables$qv
	dw	0
	dw	4096
	dw	0
	dw	1
	dw	0
	dw	0
	dw	0
	db	21
	db	64
	db	98
	db	117
	db	105
	db	108
	db	100
	db	95
	db	115
	db	113
	db	114
	db	116
	db	95
	db	116
	db	97
	db	98
	db	108
	db	101
	db	115
	db	36
	db	113
	db	118
	dw	18
	dw	512
	dw	65524
	dw	65535
	dw	1141
	dw	0
	dw	2
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65528
	dw	65535
	dw	64
	dw	0
	dw	3
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65534
	dw	65535
	dw	33
	dw	0
	dw	4
	dw	0
	dw	0
	dw	0
?patch1	equ	@6-@@build_sqrt_tables$qv+9
?patch2	equ	0
?patch3	equ	@6-@@build_sqrt_tables$qv
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@init_fast_math_library$qv	segment virtual
@@init_fast_math_library$qv	proc	near
?live16386@0:
	?debug L 51
	push      ebp
	mov       ebp,esp
	add       esp,-12
	?debug L 105
@7:
	mov       dword ptr [_vect_normalize],offset @@vect_normalize_C$qp9AD_Vect3Dt1
	?debug L 106
	mov       dword ptr [_vect_auto_normalize],offset @@vect_auto_normalize_C$qp9AD_Vect3D
	?debug L 107
	mov       dword ptr [_vect_length],offset @@vect_length_C$qp9AD_Vect3D
	?debug L 108
	mov       dword ptr [_vect_cross],offset @@vect_cross_C$qp9AD_Vect3Dt1t1
	?debug L 109
	mov       dword ptr [_vect_scale],offset @@vect_scale_C$qp9AD_Vect3Dft1
	?debug L 110
	mov       dword ptr [_vect_auto_scale],offset @@vect_auto_scale_C$qp9AD_Vect3Df
	?debug L 111
	mov       dword ptr [_mat_mulvect],offset @@mat_mulvect_C$qp9AD_Matrixp9AD_Vect3Dt2
	?debug L 112
	mov       dword ptr [_mat_mul],offset @@mat_mul_C$qp9AD_Matrixt1t1
	?debug L 113
	mov       dword ptr [_vect_add],offset @@vect_add_C$qp9AD_Vect3Dt1t1
	?debug L 114
	mov       dword ptr [_vect_auto_add],offset @@vect_auto_add_C$qp9AD_Vect3Dt1
	?debug L 115
	mov       dword ptr [_vect_sub],offset @@vect_sub_C$qp9AD_Vect3Dt1t1
	?debug L 116
	mov       dword ptr [_vect_auto_sub],offset @@vect_auto_sub_C$qp9AD_Vect3Dt1
	?debug L 121
	mov       dword ptr [_vect_auto_neg],offset @@vect_auto_neg_C$qp9AD_Vect3D
	?debug L 122
	mov       dword ptr [_vect_neg],offset @@vect_neg_C$qp9AD_Vect3Dt1
	?debug L 123
	mov       dword ptr [_vect_length2],offset @@vect_length2_C$qp9AD_Vect3D
	?debug L 124
	mov       dword ptr [_vect_dot],offset @@vect_dot_C$qp9AD_Vect3Dt1
	?debug L 127
	call      @@build_sqrt_tables$qv
	?debug L 138
@8:
	mov       esp,ebp
	pop       ebp
	ret 
	?debug L 0
@@init_fast_math_library$qv	endp
@init_fast_math_library$qv	ends
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
	dd	?patch4
	dd	?patch5
	dd	?patch6
	df	@@init_fast_math_library$qv
	dw	0
	dw	4098
	dw	0
	dw	5
	dw	0
	dw	0
	dw	0
	db	26
	db	64
	db	105
	db	110
	db	105
	db	116
	db	95
	db	102
	db	97
	db	115
	db	116
	db	95
	db	109
	db	97
	db	116
	db	104
	db	95
	db	108
	db	105
	db	98
	db	114
	db	97
	db	114
	db	121
	db	36
	db	113
	db	118
	dw	18
	dw	512
	dw	65524
	dw	65535
	dw	116
	dw	0
	dw	6
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65528
	dw	65535
	dw	116
	dw	0
	dw	7
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	116
	dw	0
	dw	8
	dw	0
	dw	0
	dw	0
?patch4	equ	@8-@@init_fast_math_library$qv+4
?patch5	equ	0
?patch6	equ	@8-@@init_fast_math_library$qv
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@my2fist16$qpfpi	segment virtual
@@my2fist16$qpfpi	proc	near
?live16387@0:
	?debug L 142
	push      ebp
	mov       ebp,esp
	?debug L 145
@9:
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax]
	fmul      dword ptr [_Scale16]
	call      __ftol
	mov       edx,dword ptr [ebp+12]
	mov       dword ptr [edx],eax
	?debug L 146
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+4]
	fmul      dword ptr [_Scale16]
	call      __ftol
	mov       edx,dword ptr [ebp+12]
	mov       dword ptr [edx+4],eax
	?debug L 147
@10:
	pop       ebp
	ret 
	?debug L 0
@@my2fist16$qpfpi	endp
@my2fist16$qpfpi	ends
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
	dd	?patch7
	dd	?patch8
	dd	?patch9
	df	@@my2fist16$qpfpi
	dw	0
	dw	4100
	dw	0
	dw	9
	dw	0
	dw	0
	dw	0
	db	16
	db	64
	db	109
	db	121
	db	50
	db	102
	db	105
	db	115
	db	116
	db	49
	db	54
	db	36
	db	113
	db	112
	db	102
	db	112
	db	105
	dw	18
	dw	512
	dw	8
	dw	0
	dw	1088
	dw	0
	dw	10
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	1140
	dw	0
	dw	11
	dw	0
	dw	0
	dw	0
?patch7	equ	@10-@@my2fist16$qpfpi+2
?patch8	equ	0
?patch9	equ	@10-@@my2fist16$qpfpi
	dw	2
	dw	6
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@imul16$qii	segment virtual
@@imul16$qii	proc	near
?live16388@0:
	?debug L 150
	push      ebp
	mov       ebp,esp
	push      ecx
	?debug L 155
@11:
	push	 eax
	?debug L 156
	push	 edx
	?debug L 157
	mov	 eax, dword ptr [ebp+8]
	?debug L 158
	imul	 dword ptr [ebp+12]
	?debug L 159
	shrd	 eax, edx, 16
	?debug L 160
	mov	 dword ptr [ebp-4], eax
	?debug L 161
	pop	 edx
	?debug L 162
	pop	 eax
	?debug L 164
	mov       eax,dword ptr [ebp-4]
	?debug L 165
@13:
@12:
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@imul16$qii	endp
@imul16$qii	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	58
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
	df	@@imul16$qii
	dw	0
	dw	4102
	dw	0
	dw	12
	dw	0
	dw	0
	dw	0
	db	11
	db	64
	db	105
	db	109
	db	117
	db	108
	db	49
	db	54
	db	36
	db	113
	db	105
	db	105
	dw	18
	dw	512
	dw	8
	dw	0
	dw	116
	dw	0
	dw	13
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	116
	dw	0
	dw	14
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	116
	dw	0
	dw	15
	dw	0
	dw	0
	dw	0
?patch10	equ	@13-@@imul16$qii+3
?patch11	equ	0
?patch12	equ	@13-@@imul16$qii
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@imul14$qii	segment virtual
@@imul14$qii	proc	near
?live16389@0:
	?debug L 168
	push      ebp
	mov       ebp,esp
	push      ecx
	?debug L 173
@14:
	push	 eax
	?debug L 174
	push	 edx
	?debug L 175
	mov	 eax, dword ptr [ebp+8]
	?debug L 176
	imul	 dword ptr [ebp+12]
	?debug L 177
	shrd	 eax, edx, 14
	?debug L 178
	mov	 dword ptr [ebp-4], eax
	?debug L 179
	pop	 edx
	?debug L 180
	pop	 eax
	?debug L 182
	mov       eax,dword ptr [ebp-4]
	?debug L 183
@16:
@15:
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@imul14$qii	endp
@imul14$qii	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	58
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
	df	@@imul14$qii
	dw	0
	dw	4104
	dw	0
	dw	16
	dw	0
	dw	0
	dw	0
	db	11
	db	64
	db	105
	db	109
	db	117
	db	108
	db	49
	db	52
	db	36
	db	113
	db	105
	db	105
	dw	18
	dw	512
	dw	8
	dw	0
	dw	116
	dw	0
	dw	17
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	116
	dw	0
	dw	18
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	116
	dw	0
	dw	19
	dw	0
	dw	0
	dw	0
?patch13	equ	@16-@@imul14$qii+3
?patch14	equ	0
?patch15	equ	@16-@@imul14$qii
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@idiv16$qii	segment virtual
@@idiv16$qii	proc	near
?live16390@0:
	?debug L 186
	push      ebp
	mov       ebp,esp
	push      ecx
	?debug L 191
@17:
	push	 eax
	?debug L 192
	push	 edx
	?debug L 193
	mov	 eax, dword ptr [ebp+8]
	?debug L 194
	mov	 edx, eax
	?debug L 195
	sar	 edx, 16
	?debug L 196
	shl	 eax, 16
	?debug L 197
	idiv	  dword ptr [ebp+12]
	?debug L 198
	mov	 dword ptr [ebp-4], eax
	?debug L 199
	pop	 edx
	?debug L 200
	pop	 eax
	?debug L 202
	mov       eax,dword ptr [ebp-4]
	?debug L 203
@19:
@18:
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@idiv16$qii	endp
@idiv16$qii	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	58
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
	df	@@idiv16$qii
	dw	0
	dw	4106
	dw	0
	dw	20
	dw	0
	dw	0
	dw	0
	db	11
	db	64
	db	105
	db	100
	db	105
	db	118
	db	49
	db	54
	db	36
	db	113
	db	105
	db	105
	dw	18
	dw	512
	dw	8
	dw	0
	dw	116
	dw	0
	dw	21
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	116
	dw	0
	dw	22
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	116
	dw	0
	dw	23
	dw	0
	dw	0
	dw	0
?patch16	equ	@19-@@idiv16$qii+3
?patch17	equ	0
?patch18	equ	@19-@@idiv16$qii
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@fast_rcpsqrt$qf	segment virtual
@@fast_rcpsqrt$qf	proc	near
?live16391@0:
	?debug L 230
	push      ebp
	mov       ebp,esp
	add       esp,-8
	?debug L 232
@20:
	lea       eax,dword ptr [ebp+8]
	mov       dword ptr [ebp-4],eax
	?debug L 236
	mov       edx,dword ptr [ebp-4]
	mov       ecx,dword ptr [edx]
	shr       ecx,23
	sub       ecx,127
	mov       dword ptr [ebp-8],ecx
	?debug L 238
	mov       eax,dword ptr [ebp-4]
	and       dword ptr [eax],8388607
	?debug L 239
	test      byte ptr [ebp-8],1
	je        short @21
	mov       edx,dword ptr [ebp-4]
	or        dword ptr [edx],8388608
	?debug L 241
@21:
	sar       dword ptr [ebp-8],1
	?debug L 242
	mov       ecx,dword ptr [ebp-4]
	mov       eax,dword ptr [ecx]
	test      eax,eax
	je        short @22
	mov       edx,dword ptr [ebp-8]
	inc       edx
	neg       edx
	mov       dword ptr [ebp-8],edx
	jmp       short @23
	?debug L 243
@22:
	neg       dword ptr [ebp-8]
	?debug L 245
@23:
	mov       ecx,dword ptr [ebp-4]
	mov       eax,dword ptr [ecx]
	shr       eax,16
	mov       edx,dword ptr [_fast_rcpsqrt_table+4*eax]
	mov       ecx,dword ptr [ebp-8]
	add       ecx,127
	shl       ecx,23
	add       edx,ecx
	mov       eax,dword ptr [ebp-4]
	mov       dword ptr [eax],edx
	?debug L 246
	fld       dword ptr [ebp+8]
	?debug L 247
@25:
@24:
	pop       ecx
	pop       ecx
	pop       ebp
	ret 
	?debug L 0
@@fast_rcpsqrt$qf	endp
@fast_rcpsqrt$qf	ends
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
	df	@@fast_rcpsqrt$qf
	dw	0
	dw	4108
	dw	0
	dw	24
	dw	0
	dw	0
	dw	0
	db	16
	db	64
	db	102
	db	97
	db	115
	db	116
	db	95
	db	114
	db	99
	db	112
	db	115
	db	113
	db	114
	db	116
	db	36
	db	113
	db	102
	dw	18
	dw	512
	dw	8
	dw	0
	dw	64
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
	dw	1141
	dw	0
	dw	27
	dw	0
	dw	0
	dw	0
?patch19	equ	@25-@@fast_rcpsqrt$qf+4
?patch20	equ	0
?patch21	equ	@25-@@fast_rcpsqrt$qf
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@vect_table_auto_normalize$qp9AD_Vect3D	segment virtual
@@vect_table_auto_normalize$qp9AD_Vect3D	proc	near
?live16392@0:
	?debug L 250
	push      ebp
	mov       ebp,esp
	push      ecx
	?debug L 254
@26:
	push      dword ptr [ebp+8]
	push      dword ptr [ebp+8]
	call      dword ptr [_vect_dot]
	add       esp,8
	add       esp,-4
	fstp      dword ptr [esp]
	call      @@fastsqrt$qf
	pop       ecx
	fdivr     dword ptr [@27]
	fstp      dword ptr [ebp-4]
	?debug L 255
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax]
	fmul      dword ptr [ebp-4]
	mov       edx,dword ptr [ebp+8]
	fstp      dword ptr [edx]
	?debug L 256
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+4]
	fmul      dword ptr [ebp-4]
	mov       eax,dword ptr [ebp+8]
	fstp      dword ptr [eax+4]
	?debug L 257
	mov       edx,dword ptr [ebp+8]
	fld       dword ptr [edx+8]
	fmul      dword ptr [ebp-4]
	mov       ecx,dword ptr [ebp+8]
	fstp      dword ptr [ecx+8]
	?debug L 258
@28:
	pop       ecx
	pop       ebp
	ret 
	align 4        
@27:
	db        0,0,128,63
	?debug L 0
@@vect_table_auto_normalize$qp9AD_Vect3D	endp
@vect_table_auto_normalize$qp9AD_Vect3D	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	86
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
	df	@@vect_table_auto_normalize$qp9AD_Vect3D
	dw	0
	dw	4110
	dw	0
	dw	28
	dw	0
	dw	0
	dw	0
	db	39
	db	64
	db	118
	db	101
	db	99
	db	116
	db	95
	db	116
	db	97
	db	98
	db	108
	db	101
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
	dw	29
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	64
	dw	0
	dw	30
	dw	0
	dw	0
	dw	0
?patch22	equ	@28-@@vect_table_auto_normalize$qp9AD_Vect3D+9
?patch23	equ	0
?patch24	equ	@28-@@vect_table_auto_normalize$qp9AD_Vect3D
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@vect_table_normalize$qp9AD_Vect3Dt1	segment virtual
@@vect_table_normalize$qp9AD_Vect3Dt1	proc	near
?live16394@0:
	?debug L 261
	push      ebp
	mov       ebp,esp
	push      ecx
	?debug L 265
@29:
	push      dword ptr [ebp+8]
	push      dword ptr [ebp+8]
	call      dword ptr [_vect_dot]
	add       esp,8
	add       esp,-4
	fstp      dword ptr [esp]
	call      @@fastsqrt$qf
	pop       ecx
	fdivr     dword ptr [@30]
	fstp      dword ptr [ebp-4]
	?debug L 266
	mov       eax,dword ptr [ebp+8]
	fld       dword ptr [eax]
	fmul      dword ptr [ebp-4]
	mov       edx,dword ptr [ebp+12]
	fstp      dword ptr [edx]
	?debug L 267
	mov       ecx,dword ptr [ebp+8]
	fld       dword ptr [ecx+4]
	fmul      dword ptr [ebp-4]
	mov       eax,dword ptr [ebp+12]
	fstp      dword ptr [eax+4]
	?debug L 268
	mov       edx,dword ptr [ebp+8]
	fld       dword ptr [edx+8]
	fmul      dword ptr [ebp-4]
	mov       ecx,dword ptr [ebp+12]
	fstp      dword ptr [ecx+8]
	?debug L 269
@31:
	pop       ecx
	pop       ebp
	ret 
	align 4        
@30:
	db        0,0,128,63
	?debug L 0
@@vect_table_normalize$qp9AD_Vect3Dt1	endp
@vect_table_normalize$qp9AD_Vect3Dt1	ends
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
	dd	?patch25
	dd	?patch26
	dd	?patch27
	df	@@vect_table_normalize$qp9AD_Vect3Dt1
	dw	0
	dw	4112
	dw	0
	dw	31
	dw	0
	dw	0
	dw	0
	db	36
	db	64
	db	118
	db	101
	db	99
	db	116
	db	95
	db	116
	db	97
	db	98
	db	108
	db	101
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
	dw	32
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	65535
	dw	65535
	dw	33
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	64
	dw	0
	dw	34
	dw	0
	dw	0
	dw	0
?patch25	equ	@31-@@vect_table_normalize$qp9AD_Vect3Dt1+9
?patch26	equ	0
?patch27	equ	@31-@@vect_table_normalize$qp9AD_Vect3Dt1
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@fastsqrt$qf	segment virtual
@@fastsqrt$qf	proc	near
?live16393@0:
	?debug L 206
	push      ebp
	mov       ebp,esp
	add       esp,-12
	?debug L 211
@32:
	mov       eax,dword ptr [ebp+8]
	mov       dword ptr [ebp-4],eax
	?debug L 212
	mov       edx,dword ptr [ebp-4]
	mov       dword ptr [ebp-12],edx
	?debug L 213
	and       dword ptr [ebp-4],8388607
	?debug L 214
	add       dword ptr [ebp-12],-1065353216
	?debug L 215
	mov       ecx,dword ptr [ebp-12]
	mov       dword ptr [ebp-8],ecx
	?debug L 216
	mov       eax,dword ptr [ebp-12]
	and       eax,-16777216
	shr       eax,1
	mov       dword ptr [ebp-12],eax
	?debug L 217
	and       dword ptr [ebp-8],8388608
	?debug L 218
	add       dword ptr [ebp-12],1065353216
	?debug L 219
	mov       edx,dword ptr [ebp-8]
	or        dword ptr [ebp-4],edx
	?debug L 220
	shr       dword ptr [ebp-4],16
	?debug L 221
	mov       ecx,dword ptr [ebp-4]
	mov       eax,dword ptr [_fast_sqrt_table+4*ecx]
	mov       dword ptr [ebp-4],eax
	?debug L 222
	mov       edx,dword ptr [ebp-12]
	add       dword ptr [ebp-4],edx
	?debug L 223
	mov       ecx,dword ptr [ebp-4]
	mov       dword ptr [ebp+8],ecx
	?debug L 225
	fld       dword ptr [ebp+8]
	?debug L 226
@34:
@33:
	mov       esp,ebp
	pop       ebp
	ret 
	?debug L 0
@@fastsqrt$qf	endp
@fastsqrt$qf	ends
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
	dd	?patch28
	dd	?patch29
	dd	?patch30
	df	@@fastsqrt$qf
	dw	0
	dw	4114
	dw	0
	dw	35
	dw	0
	dw	0
	dw	0
	db	12
	db	64
	db	102
	db	97
	db	115
	db	116
	db	115
	db	113
	db	114
	db	116
	db	36
	db	113
	db	102
	dw	18
	dw	512
	dw	8
	dw	0
	dw	64
	dw	0
	dw	36
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65524
	dw	65535
	dw	116
	dw	0
	dw	37
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65528
	dw	65535
	dw	117
	dw	0
	dw	38
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65532
	dw	65535
	dw	117
	dw	0
	dw	39
	dw	0
	dw	0
	dw	0
?patch28	equ	@34-@@fastsqrt$qf+4
?patch29	equ	0
?patch30	equ	@34-@@fastsqrt$qf
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
_TEXT	ends
	public	_fast_sqrt_table
	public	_fast_rcpsqrt_table
	public	_Scale16
	public	_Scale5
	public	_half
	public	_OldFPUCW
	public	_FPUCW
@_sqrt equ _sqrt
 extrn   _sqrt:near
 extrn   _vect_normalize:dword
@@vect_normalize_C$qp9AD_Vect3Dt1 equ @vect_normalize_C$qp9AD_Vect3Dt1
 extrn   @vect_normalize_C$qp9AD_Vect3Dt1:near
 extrn   _vect_auto_normalize:dword
@@vect_auto_normalize_C$qp9AD_Vect3D equ @vect_auto_normalize_C$qp9AD_Vect3D
 extrn   @vect_auto_normalize_C$qp9AD_Vect3D:near
 extrn   _vect_length:dword
@@vect_length_C$qp9AD_Vect3D equ @vect_length_C$qp9AD_Vect3D
 extrn   @vect_length_C$qp9AD_Vect3D:near
 extrn   _vect_cross:dword
@@vect_cross_C$qp9AD_Vect3Dt1t1 equ @vect_cross_C$qp9AD_Vect3Dt1t1
 extrn   @vect_cross_C$qp9AD_Vect3Dt1t1:near
 extrn   _vect_scale:dword
@@vect_scale_C$qp9AD_Vect3Dft1 equ @vect_scale_C$qp9AD_Vect3Dft1
 extrn   @vect_scale_C$qp9AD_Vect3Dft1:near
 extrn   _vect_auto_scale:dword
@@vect_auto_scale_C$qp9AD_Vect3Df equ @vect_auto_scale_C$qp9AD_Vect3Df
 extrn   @vect_auto_scale_C$qp9AD_Vect3Df:near
 extrn   _mat_mulvect:dword
@@mat_mulvect_C$qp9AD_Matrixp9AD_Vect3Dt2 equ @mat_mulvect_C$qp9AD_Matrixp9AD_Vect3Dt2
 extrn   @mat_mulvect_C$qp9AD_Matrixp9AD_Vect3Dt2:near
 extrn   _mat_mul:dword
@@mat_mul_C$qp9AD_Matrixt1t1 equ @mat_mul_C$qp9AD_Matrixt1t1
 extrn   @mat_mul_C$qp9AD_Matrixt1t1:near
 extrn   _vect_add:dword
@@vect_add_C$qp9AD_Vect3Dt1t1 equ @vect_add_C$qp9AD_Vect3Dt1t1
 extrn   @vect_add_C$qp9AD_Vect3Dt1t1:near
 extrn   _vect_auto_add:dword
@@vect_auto_add_C$qp9AD_Vect3Dt1 equ @vect_auto_add_C$qp9AD_Vect3Dt1
 extrn   @vect_auto_add_C$qp9AD_Vect3Dt1:near
 extrn   _vect_sub:dword
@@vect_sub_C$qp9AD_Vect3Dt1t1 equ @vect_sub_C$qp9AD_Vect3Dt1t1
 extrn   @vect_sub_C$qp9AD_Vect3Dt1t1:near
 extrn   _vect_auto_sub:dword
@@vect_auto_sub_C$qp9AD_Vect3Dt1 equ @vect_auto_sub_C$qp9AD_Vect3Dt1
 extrn   @vect_auto_sub_C$qp9AD_Vect3Dt1:near
 extrn   _vect_auto_neg:dword
@@vect_auto_neg_C$qp9AD_Vect3D equ @vect_auto_neg_C$qp9AD_Vect3D
 extrn   @vect_auto_neg_C$qp9AD_Vect3D:near
 extrn   _vect_neg:dword
@@vect_neg_C$qp9AD_Vect3Dt1 equ @vect_neg_C$qp9AD_Vect3Dt1
 extrn   @vect_neg_C$qp9AD_Vect3Dt1:near
 extrn   _vect_length2:dword
@@vect_length2_C$qp9AD_Vect3D equ @vect_length2_C$qp9AD_Vect3D
 extrn   @vect_length2_C$qp9AD_Vect3D:near
 extrn   _vect_dot:dword
@@vect_dot_C$qp9AD_Vect3Dt1 equ @vect_dot_C$qp9AD_Vect3Dt1
 extrn   @vect_dot_C$qp9AD_Vect3Dt1:near
 extrn   __ftol:near
	extrn	__turboFloat:word
 ?debug  C 9F757569642E6C6962
 ?debug  C 9F757569642E6C6962
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	22
	dw	514
	df	_fast_sqrt_table
	dw	0
	dw	4116
	dw	0
	dw	40
	dw	0
	dw	0
	dw	0
	dw	22
	dw	514
	df	_fast_rcpsqrt_table
	dw	0
	dw	4117
	dw	0
	dw	41
	dw	0
	dw	0
	dw	0
	dw	22
	dw	514
	df	_Scale16
	dw	0
	dw	64
	dw	0
	dw	42
	dw	0
	dw	0
	dw	0
	dw	22
	dw	514
	df	_Scale5
	dw	0
	dw	64
	dw	0
	dw	43
	dw	0
	dw	0
	dw	0
	dw	22
	dw	514
	df	_half
	dw	0
	dw	64
	dw	0
	dw	44
	dw	0
	dw	0
	dw	0
	dw	22
	dw	514
	df	_OldFPUCW
	dw	0
	dw	17
	dw	0
	dw	45
	dw	0
	dw	0
	dw	0
	dw	22
	dw	514
	df	_FPUCW
	dw	0
	dw	17
	dw	0
	dw	46
	dw	0
	dw	0
	dw	0
	dw	?patch31
	dw	37
	dw	47
	dw	0
	dw	0
	dw	0
	dw	0
?patch31	equ	12
	dw	?patch32
	dw	38
	dw	415
	dw	48
	dw	0
	dw	49
	dw	0
	dw	50
	dw	0
	dw	51
	dw	0
	dw	52
	dw	0
	dw	53
	dw	0
	dw	54
	dw	0
	dw	55
	dw	0
	dw	56
	dw	0
	dw	57
	dw	0
	dw	58
	dw	0
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
	dw	436
	dw	0
	dw	437
	dw	0
	dw	438
	dw	0
	dw	439
	dw	0
	dw	440
	dw	0
	dw	441
	dw	0
	dw	442
	dw	0
	dw	443
	dw	0
	dw	444
	dw	0
	dw	445
	dw	0
	dw	446
	dw	0
	dw	447
	dw	0
	dw	448
	dw	0
	dw	449
	dw	0
	dw	450
	dw	0
	dw	451
	dw	0
	dw	452
	dw	0
	dw	453
	dw	0
	dw	454
	dw	0
	dw	455
	dw	0
	dw	456
	dw	0
	dw	457
	dw	0
	dw	458
	dw	0
	dw	459
	dw	0
	dw	460
	dw	0
	dw	461
	dw	0
	dw	462
	dw	0
?patch32	equ	1664
	dw	?patch33
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
?patch33	equ	18
$$BSYMS	ends
$$BTYPES	segment byte public use32 'DEBTYP'
	db        2,0,0,0,14,0,8,0,3,0,0,0,0,0,0,0
	db        1,16,0,0,4,0,1,2,0,0,14,0,8,0,3,0
	db        0,0,0,0,0,0,3,16,0,0,4,0,1,2,0,0
	db        14,0,8,0,3,0,0,0,0,0,2,0,5,16,0,0
	db        12,0,1,2,2,0,64,4,0,0,116,4,0,0,14,0
	db        8,0,116,0,0,0,0,0,2,0,7,16,0,0,12,0
	db        1,2,2,0,116,0,0,0,116,0,0,0,14,0,8,0
	db        116,0,0,0,0,0,2,0,9,16,0,0,12,0,1,2
	db        2,0,116,0,0,0,116,0,0,0,14,0,8,0,116,0
	db        0,0,0,0,2,0,11,16,0,0,12,0,1,2,2,0
	db        116,0,0,0,116,0,0,0,14,0,8,0,64,0,0,0
	db        0,0,1,0,13,16,0,0,8,0,1,2,1,0,64,0
	db        0,0,14,0,8,0,3,0,0,0,0,0,1,0,15,16
	db        0,0,8,0,1,2,1,0,255,255,255,255,14,0,8,0
	db        3,0,0,0,0,0,2,0,17,16,0,0,12,0,1,2
	db        2,0,255,255,255,255,255,255,255,255,14,0,8,0,64,0
	db        0,0,0,0,1,0,19,16,0,0,8,0,1,2,1,0
	db        64,0,0,0,18,0,3,0,117,0,0,0,17,0,0,0
	db        0,0,0,0,0,4,0,1,18,0,3,0,117,0,0,0
	db        17,0,0,0,0,0,0,0,0,4,0,1,14,0,8,0
	db        65,0,0,0,0,0,1,0,23,16,0,0,8,0,1,2
	db        1,0,65,0,0,0,8,0,2,0,10,0,25,16,0,0
	db        14,0,8,0,3,0,0,0,0,0,2,0,26,16,0,0
	db        12,0,1,2,2,0,255,255,255,255,255,255,255,255,14,0
	db        8,0,3,0,0,0,0,0,2,0,28,16,0,0,12,0
	db        1,2,2,0,255,255,255,255,255,255,255,255,8,0,2,0
	db        10,0,30,16,0,0,14,0,8,0,3,0,0,0,0,0
	db        1,0,31,16,0,0,8,0,1,2,1,0,255,255,255,255
	db        14,0,8,0,3,0,0,0,0,0,1,0,33,16,0,0
	db        8,0,1,2,1,0,255,255,255,255,8,0,2,0,10,0
	db        35,16,0,0,14,0,8,0,64,0,0,0,0,0,1,0
	db        36,16,0,0,8,0,1,2,1,0,255,255,255,255,14,0
	db        8,0,64,0,0,0,0,0,1,0,38,16,0,0,8,0
	db        1,2,1,0,255,255,255,255,8,0,2,0,10,0,40,16
	db        0,0,14,0,8,0,3,0,0,0,0,0,3,0,41,16
	db        0,0,16,0,1,2,3,0,255,255,255,255,255,255,255,255
	db        255,255,255,255,14,0,8,0,3,0,0,0,0,0,3,0
	db        43,16,0,0,16,0,1,2,3,0,255,255,255,255,255,255
	db        255,255,255,255,255,255,8,0,2,0,10,0,45,16,0,0
	db        14,0,8,0,3,0,0,0,0,0,3,0,46,16,0,0
	db        16,0,1,2,3,0,255,255,255,255,64,0,0,0,255,255
	db        255,255,14,0,8,0,3,0,0,0,0,0,3,0,48,16
	db        0,0,16,0,1,2,3,0,255,255,255,255,64,0,0,0
	db        255,255,255,255,8,0,2,0,10,0,50,16,0,0,14,0
	db        8,0,3,0,0,0,0,0,2,0,51,16,0,0,12,0
	db        1,2,2,0,255,255,255,255,64,0,0,0,14,0,8,0
	db        3,0,0,0,0,0,2,0,53,16,0,0,12,0,1,2
	db        2,0,255,255,255,255,64,0,0,0,8,0,2,0,10,0
	db        55,16,0,0,14,0,8,0,3,0,0,0,0,0,3,0
	db        56,16,0,0,16,0,1,2,3,0,254,255,255,255,255,255
	db        255,255,255,255,255,255,14,0,8,0,3,0,0,0,0,0
	db        3,0,58,16,0,0,16,0,1,2,3,0,254,255,255,255
	db        255,255,255,255,255,255,255,255,8,0,2,0,10,0,60,16
	db        0,0,14,0,8,0,3,0,0,0,0,0,3,0,61,16
	db        0,0,16,0,1,2,3,0,254,255,255,255,254,255,255,255
	db        254,255,255,255,14,0,8,0,3,0,0,0,0,0,3,0
	db        63,16,0,0,16,0,1,2,3,0,254,255,255,255,254,255
	db        255,255,254,255,255,255,14,0,8,0,3,0,0,0,0,0
	db        3,0,65,16,0,0,16,0,1,2,3,0,255,255,255,255
	db        255,255,255,255,255,255,255,255,14,0,8,0,3,0,0,0
	db        0,0,2,0,67,16,0,0,12,0,1,2,2,0,255,255
	db        255,255,255,255,255,255,14,0,8,0,3,0,0,0,0,0
	db        3,0,69,16,0,0,16,0,1,2,3,0,255,255,255,255
	db        255,255,255,255,255,255,255,255,14,0,8,0,3,0,0,0
	db        0,0,2,0,71,16,0,0,12,0,1,2,2,0,255,255
	db        255,255,255,255,255,255,14,0,8,0,3,0,0,0,0,0
	db        1,0,73,16,0,0,8,0,1,2,1,0,255,255,255,255
	db        14,0,8,0,3,0,0,0,0,0,2,0,75,16,0,0
	db        12,0,1,2,2,0,255,255,255,255,255,255,255,255,14,0
	db        8,0,64,0,0,0,0,0,1,0,77,16,0,0,8,0
	db        1,2,1,0,255,255,255,255,8,0,2,0,10,0,79,16
	db        0,0,14,0,8,0,64,0,0,0,0,0,2,0,80,16
	db        0,0,12,0,1,2,2,0,255,255,255,255,255,255,255,255
	db        14,0,8,0,64,0,0,0,0,0,2,0,82,16,0,0
	db        12,0,1,2,2,0,255,255,255,255,255,255,255,255,14,0
	db        8,0,116,0,0,0,2,0,0,0,84,16,0,0,4,0
	db        1,2,0,0
$$BTYPES	ends
$$BNAMES	segment byte public use32 'DEBNAM'
	db	17,'build_sqrt_tables'
	db	2,'fi'
	db	1,'f'
	db	1,'i'
	db	22,'init_fast_math_library'
	db	8,'Has3DNow'
	db	6,'Athlon'
	db	5,'K6III'
	db	9,'my2fist16'
	db	7,'fvalues'
	db	7,'ivalues'
	db	6,'imul16'
	db	1,'x'
	db	1,'y'
	db	6,'result'
	db	6,'imul14'
	db	1,'x'
	db	1,'y'
	db	6,'result'
	db	6,'idiv16'
	db	1,'x'
	db	1,'y'
	db	6,'result'
	db	12,'fast_rcpsqrt'
	db	1,'n'
	db	1,'e'
	db	3,'num'
	db	25,'vect_table_auto_normalize'
	db	1,'a'
	db	3,'aux'
	db	20,'vect_table_normalize'
	db	1,'a'
	db	3,'out'
	db	3,'aux'
	db	8,'fastsqrt'
	db	1,'n'
	db	1,'d'
	db	1,'b'
	db	1,'a'
	db	15,'fast_sqrt_table'
	db	18,'fast_rcpsqrt_table'
	db	7,'Scale16'
	db	6,'Scale5'
	db	4,'half'
	db	8,'OldFPUCW'
	db	5,'FPUCW'
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
	db	4,'itoa'
	db	6,'getenv'
	db	4,'free'
	db	4,'exit'
	db	3,'div'
	db	6,'calloc'
	db	4,'atol'
	db	4,'atoi'
	db	6,'atexit'
	db	5,'abort'
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
	db	5,'_ltow'
	db	5,'_ltoa'
	db	6,'_lrand'
	db	5,'_itow'
	db	6,'_ultow'
	db	5,'_gcvt'
	db	5,'_fcvt'
	db	9,'_fullpath'
	db	5,'_exit'
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
	?debug	D "D:\redshift\Demo_sw\Math\vect3D.h" 11379 36509
	?debug	D "D:\redshift\Demo_sw\Math\Matrix.h" 11349 29653
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
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\mem.h" 10503 10272
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\windows.h" 10303 10240
	?debug	D "..\..\DEMOSYSTEM\Utility.h" 11321 36359
	?debug	D "D:\redshift\Demo_sw\Math\FASTMATH.H" 11319 18629
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\_null.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\_defs.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\_stddef.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\MATH.H" 10503 10272
	?debug	D "D:\redshift\Demo_sw\Math\fastmath.cpp" 11319 19894
 ?debug  C FB062A633A5C50524F4752417E315C626F726C616E645C434255494C447E315C6C69625C76636C35302E233048C8071000F87A872C
 ?debug  C FB050B232A41445F566563743344
 ?debug  C FB050B232A41445F4D6174726978
	end
