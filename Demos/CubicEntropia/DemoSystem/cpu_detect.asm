	.386p
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
	?debug	S "D:\redshift\Demo_sw\DemoSystem\cpu_detect.cpp"
	?debug	T "D:\redshift\Demo_sw\DemoSystem\cpu_detect.cpp"
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
_features	label	dword
	dd	0
	align	4
_ext_features	label	dword
	dd	0
	align	4
_processor	label	dword
	dd	0
_DATA	ends
_BSS	segment dword public use32 'BSS'
_proc_idstr	label	byte
	db	16	dup(?)
_proc_namestr	label	byte
	db	48	dup(?)
_BSS	ends
_DATA	segment dword public use32 'DATA'
	align	4
_proc_cache_l1	label	dword
	dd	0
	dd	0
	dd	0
	dd	0
	align	4
_proc_cache_l2	label	dword
	dd	0
	dd	0
	dd	0
	dd	0
	align	4
$ildjdaia	label	dword
	dd	0
_DATA	ends
_DATA	segment dword public use32 'DATA'
@_$ECTA$@GetCPUCaps$q7CPUCAPS	segment virtual
	align	2
@@_$ECTA$@GetCPUCaps$q7CPUCAPS	label	dword
	dd	0
	dd	-36
	dw	0
	dw	2
	dd	1
	dd	@1
@_$ECTA$@GetCPUCaps$q7CPUCAPS	ends
_DATA	ends
_TEXT	segment dword public use32 'CODE'
_GetCPUCaps	segment virtual
@_GetCPUCaps	proc	near
?live16385@0:
	?debug L 19
	push      ebp
	mov       ebp,esp
	add       esp,-40
	push      ebx
	push      esi
	push      edi
@2:
	mov       eax,offset @@_$ECTA$@GetCPUCaps$q7CPUCAPS
	call      @__InitExceptBlockLDTC
	?debug L 23
	xor       edx,edx
	mov       dword ptr [ebp-40],edx
	?debug L 26
	cmp       dword ptr [$ildjdaia],0
	jne       short @3
	?debug L 27
	call      @@detect_base$qv
	mov       dword ptr [$ildjdaia],eax
	?debug L 29
@3:
	cmp       dword ptr [$ildjdaia],-1
	jne       short @4
	?debug L 32
	xor       eax,eax
	mov       edx,dword ptr [ebp-36]
	mov       dword ptr fs:[0],edx
	jmp       @5
	?debug L 36
@4:
	movsx     ecx,byte ptr [ebp+8]
	cmp       ecx,29
	ja        @6
	jmp       dword ptr [@38+4*ecx]
@38:
	dd        @37
	dd        @36
	dd        @35
	dd        @34
	dd        @33
	dd        @32
	dd        @31
	dd        @30
	dd        @29
	dd        @28
	dd        @27
	dd        @26
	dd        @25
	dd        @24
	dd        @23
	dd        @22
	dd        @21
	dd        @20
	dd        @19
	dd        @18
	dd        @17
	dd        @16
	dd        @15
	dd        @14
	dd        @13
	dd        @12
	dd        @11
	dd        @10
	dd        @9
	dd        @8
	?debug L 41
@33:
	mov       dword ptr [ebp-40],1
	?debug L 42
	jmp       @39
	?debug L 45
@35:
	mov       eax,offset _proc_idstr
	mov       dword ptr [ebp-40],eax
	?debug L 46
	jmp       @39
	?debug L 49
@34:
	mov       edx,offset _proc_namestr
	mov       dword ptr [ebp-40],edx
	?debug L 50
	jmp       @39
	?debug L 53
@37:
	push      12
	push      offset s@
	push      offset _proc_idstr
	call      @_strncmp
	add       esp,12
	test      eax,eax
	jne       short @40
	?debug L 54
	mov       dword ptr [ebp-40],1
	jmp       @39
	?debug L 55
@40:
	push      12
	push      offset s@+13
	push      offset _proc_idstr
	call      @_strncmp
	add       esp,12
	test      eax,eax
	jne       short @41
	?debug L 56
	mov       dword ptr [ebp-40],2
	jmp       @39
	?debug L 57
@41:
	push      12
	push      offset s@+26
	push      offset _proc_idstr
	call      @_strncmp
	add       esp,12
	test      eax,eax
	jne       short @42
	?debug L 58
	mov       dword ptr [ebp-40],3
	jmp       @39
	?debug L 59
@42:
	push      12
	push      offset s@+39
	push      offset _proc_idstr
	call      @_strncmp
	add       esp,12
	test      eax,eax
	jne       short @43
	?debug L 60
	mov       dword ptr [ebp-40],4
	jmp       @39
	?debug L 62
@43:
	xor       ecx,ecx
	mov       dword ptr [ebp-40],ecx
	?debug L 63
	jmp       @39
	?debug L 71
@36:
	xor       eax,eax
	mov       dword ptr [ebp-40],eax
	?debug L 72
	push      0
	call      @_GetCPUCaps
	pop       ecx
	dec       eax
	je        short @49
	dec       eax
	je        @48
	dec       eax
	je        @47
	dec       eax
	je        @46
	jmp       @39
	?debug L 75
@49:
	mov       edx,dword ptr [_processor]
	shr       edx,8
	and       edx,15
	sub       edx,4
	je        short @54
	dec       edx
	je        short @53
	dec       edx
	je        short @52
	jmp       @39
	?debug L 78
@54:
	mov       dword ptr [ebp-40],1
	?debug L 79
	jmp       @39
	?debug L 82
@53:
	mov       ecx,dword ptr [_processor]
	shr       ecx,4
	and       ecx,15
	sub       ecx,4
	jb        short @60
	sub       ecx,4
	jb        short @59
	je        short @58
	dec       ecx
	sub       ecx,7
	jb        short @57
	jmp       @39
	?debug L 87
@61:
@60:
	mov       dword ptr [ebp-40],2
	jmp       @39
	?debug L 91
@59:
	mov       dword ptr [ebp-40],3
	jmp       @39
	?debug L 92
@58:
	mov       dword ptr [ebp-40],4
	jmp       @39
	?debug L 99
@57:
	mov       dword ptr [ebp-40],5
	jmp       @39
	?debug L 101
	?debug L 105
@55:
@56:
@52:
	mov       dword ptr [ebp-40],6
	?debug L 106
	jmp       @39
	?debug L 108
	?debug L 111
@50:
@51:
@48:
	mov       eax,dword ptr [_processor]
	shr       eax,8
	and       eax,15
	sub       eax,4
	je        short @66
	dec       eax
	je        @65
	dec       eax
	je        @64
	jmp       @39
	?debug L 114
@66:
	mov       edx,dword ptr [_processor]
	shr       edx,4
	and       edx,15
	cmp       edx,8
	ja        @39
	jmp       dword ptr [@77+4*edx]
@77:
	dd        @76
	dd        @75
	dd        @74
	dd        @73
	dd        @72
	dd        @71
	dd        @39
	dd        @70
	dd        @69
	?debug L 116
@76:
	mov       dword ptr [ebp-40],7
	jmp       @39
	?debug L 117
@75:
	mov       dword ptr [ebp-40],7
	jmp       @39
	?debug L 118
@74:
	mov       dword ptr [ebp-40],8
	jmp       @39
	?debug L 119
@73:
	mov       dword ptr [ebp-40],9
	jmp       @39
	?debug L 120
@72:
	mov       dword ptr [ebp-40],10
	jmp       @39
	?debug L 121
@71:
	mov       dword ptr [ebp-40],11
	jmp       @39
	?debug L 122
@70:
	mov       dword ptr [ebp-40],12
	jmp       @39
	?debug L 123
@69:
	mov       dword ptr [ebp-40],13
	jmp       @39
	?debug L 125
	?debug L 128
@67:
@68:
@65:
	mov       ecx,dword ptr [_processor]
	shr       ecx,4
	and       ecx,15
	dec       ecx
	je        short @83
	dec       ecx
	je        short @82
	dec       ecx
	je        short @81
	dec       ecx
	je        short @80
	jmp       @39
	?debug L 130
@83:
	mov       dword ptr [ebp-40],14
	jmp       @39
	?debug L 131
@82:
	mov       dword ptr [ebp-40],14
	jmp       @39
	?debug L 132
@81:
	mov       dword ptr [ebp-40],14
	jmp       @39
	?debug L 133
@80:
	mov       dword ptr [ebp-40],15
	jmp       @39
	?debug L 135
	?debug L 138
@78:
@79:
@64:
	mov       eax,dword ptr [_processor]
	shr       eax,4
	and       eax,15
	cmp       eax,7
	ja        @39
	jmp       dword ptr [@91+4*eax]
@91:
	dd        @39
	dd        @90
	dd        @39
	dd        @89
	dd        @39
	dd        @88
	dd        @87
	dd        @86
	?debug L 140
@90:
	mov       dword ptr [ebp-40],16
	jmp       @39
	?debug L 141
@89:
	mov       dword ptr [ebp-40],17
	jmp       @39
	?debug L 142
@88:
	mov       dword ptr [ebp-40],17
	jmp       @39
	?debug L 143
@87:
	mov       dword ptr [ebp-40],18
	jmp       @39
	?debug L 144
@86:
	mov       dword ptr [ebp-40],19
	jmp       @39
	?debug L 146
	?debug L 148
	?debug L 151
@84:
@85:
@62:
@63:
@47:
	xor       edx,edx
	mov       dword ptr [ebp-40],edx
	?debug L 152
	jmp       @39
	?debug L 155
@46:
	xor       ecx,ecx
	mov       dword ptr [ebp-40],ecx
	?debug L 156
	jmp       @39
	?debug L 158
	?debug L 164
@44:
@45:
@32:
	mov       eax,dword ptr [_features]
	shr       eax,0
	and       eax,1
	mov       dword ptr [ebp-40],eax
	jmp       @39
	?debug L 165
@31:
	mov       edx,dword ptr [_features]
	shr       edx,1
	and       edx,1
	mov       dword ptr [ebp-40],edx
	jmp       @39
	?debug L 166
@30:
	mov       ecx,dword ptr [_features]
	shr       ecx,2
	and       ecx,1
	mov       dword ptr [ebp-40],ecx
	jmp       @39
	?debug L 167
@29:
	mov       eax,dword ptr [_features]
	shr       eax,3
	and       eax,1
	mov       dword ptr [ebp-40],eax
	jmp       @39
	?debug L 168
@28:
	mov       edx,dword ptr [_features]
	shr       edx,4
	and       edx,1
	mov       dword ptr [ebp-40],edx
	jmp       @39
	?debug L 169
@27:
	mov       ecx,dword ptr [_features]
	shr       ecx,5
	and       ecx,1
	mov       dword ptr [ebp-40],ecx
	jmp       @39
	?debug L 170
@26:
	mov       eax,dword ptr [_features]
	shr       eax,6
	and       eax,1
	mov       dword ptr [ebp-40],eax
	jmp       @39
	?debug L 171
@25:
	mov       edx,dword ptr [_features]
	shr       edx,7
	and       edx,1
	mov       dword ptr [ebp-40],edx
	jmp       @39
	?debug L 172
@24:
	mov       ecx,dword ptr [_features]
	shr       ecx,8
	and       ecx,1
	mov       dword ptr [ebp-40],ecx
	jmp       @39
	?debug L 173
@23:
	mov       eax,dword ptr [_features]
	shr       eax,9
	and       eax,1
	mov       dword ptr [ebp-40],eax
	jmp       @39
	?debug L 174
@22:
	mov       edx,dword ptr [_features]
	shr       edx,11
	and       edx,1
	mov       dword ptr [ebp-40],edx
	jmp       @39
	?debug L 175
@21:
	mov       ecx,dword ptr [_features]
	shr       ecx,12
	and       ecx,1
	mov       dword ptr [ebp-40],ecx
	jmp       @39
	?debug L 176
@20:
	mov       eax,dword ptr [_features]
	shr       eax,13
	and       eax,1
	mov       dword ptr [ebp-40],eax
	jmp       @39
	?debug L 177
@19:
	mov       edx,dword ptr [_features]
	shr       edx,14
	and       edx,1
	mov       dword ptr [ebp-40],edx
	jmp       @39
	?debug L 178
@18:
	mov       ecx,dword ptr [_features]
	shr       ecx,15
	and       ecx,1
	mov       dword ptr [ebp-40],ecx
	jmp       @39
	?debug L 179
@17:
	mov       eax,dword ptr [_features]
	shr       eax,16
	and       eax,1
	mov       dword ptr [ebp-40],eax
	jmp       @39
	?debug L 180
@16:
	mov       edx,dword ptr [_features]
	shr       edx,17
	and       edx,1
	mov       dword ptr [ebp-40],edx
	jmp       @39
	?debug L 181
@14:
	mov       ecx,dword ptr [_features]
	shr       ecx,23
	and       ecx,1
	mov       dword ptr [ebp-40],ecx
	jmp       @39
	?debug L 182
@13:
	mov       eax,dword ptr [_features]
	shr       eax,24
	and       eax,1
	mov       dword ptr [ebp-40],eax
	jmp       @39
	?debug L 184
@9:
	mov       edx,dword ptr [_features]
	shr       edx,25
	and       edx,1
	mov       dword ptr [ebp-40],edx
	jmp       @39
	?debug L 188
@8:
	push      28
	call      @_GetCPUCaps
	pop       ecx
	test      eax,eax
	je        @39
	?debug L 190
@92:
	mov       word ptr [ebp-20],8
	?debug L 192
 	_emit	 00fH
	?debug L 193
 	_emit	 056H
	?debug L 194
 	_emit	 0c0H	
	?debug L 195
	mov       dword ptr [ebp-40],1
	?debug L 196
	mov       word ptr [ebp-20],0
	jmp       @39
	?debug L 200
@94:
@95:
@96:
@1:
	xor       eax,eax
	mov       edx,dword ptr [ebp-36]
	mov       dword ptr fs:[0],edx
	jmp       @5
	?debug L 203
@93:
@97:
	jmp       @39
	?debug L 205
@10:
	mov       ecx,dword ptr [_ext_features]
	shr       ecx,22
	and       ecx,1
	mov       dword ptr [ebp-40],ecx
	jmp       @39
	?debug L 207
@15:
	mov       eax,dword ptr [_features]
	shr       eax,25
	and       eax,1
	mov       edx,dword ptr [_ext_features]
	shr       edx,22
	and       edx,1
	or        eax,edx
	mov       dword ptr [ebp-40],eax
	?debug L 208
	jmp       @39
	?debug L 211
@12:
	mov       ecx,dword ptr [_ext_features]
	shr       ecx,30
	and       ecx,1
	mov       dword ptr [ebp-40],ecx
	jmp       @39
	?debug L 212
@11:
	mov       eax,dword ptr [_ext_features]
	shr       eax,31
	and       eax,1
	mov       dword ptr [ebp-40],eax
	jmp       @39
	?debug L 216
@6:
	push      0
	call      @_GetCPUCaps
	pop       ecx
	cmp       eax,1
	jne       @39
	?debug L 219
@98:
	movsx     edx,byte ptr [ebp+8]
	add       edx,-30
	cmp       edx,19
	ja        @99
	jmp       dword ptr [@117+4*edx]
@117:
	dd        @116
	dd        @115
	dd        @114
	dd        @113
	dd        @99
	dd        @99
	dd        @99
	dd        @99
	dd        @112
	dd        @111
	dd        @110
	dd        @109
	dd        @108
	dd        @107
	dd        @106
	dd        @105
	dd        @104
	dd        @103
	dd        @102
	dd        @101
	?debug L 221
@116:
	mov       ecx,dword ptr [_proc_cache_l1+4]
	shr       ecx,24
	and       ecx,255
	mov       dword ptr [ebp-40],ecx
	jmp       @118
	?debug L 222
@115:
	mov       eax,dword ptr [_proc_cache_l1+4]
	shr       eax,16
	and       eax,255
	mov       dword ptr [ebp-40],eax
	jmp       @118
	?debug L 223
@114:
	mov       edx,dword ptr [_proc_cache_l1+4]
	shr       edx,8
	and       edx,255
	mov       dword ptr [ebp-40],edx
	jmp       @118
	?debug L 224
@113:
	mov       ecx,dword ptr [_proc_cache_l1+4]
	shr       ecx,0
	and       ecx,255
	mov       dword ptr [ebp-40],ecx
	jmp       @118
	?debug L 226
@112:
	mov       eax,dword ptr [_proc_cache_l1+8]
	shr       eax,24
	and       eax,255
	mov       dword ptr [ebp-40],eax
	jmp       @118
	?debug L 227
@111:
	mov       edx,dword ptr [_proc_cache_l1+8]
	shr       edx,16
	and       edx,255
	mov       dword ptr [ebp-40],edx
	jmp       @118
	?debug L 228
@110:
	mov       ecx,dword ptr [_proc_cache_l1+8]
	shr       ecx,8
	and       ecx,255
	mov       dword ptr [ebp-40],ecx
	jmp       @118
	?debug L 229
@109:
	mov       eax,dword ptr [_proc_cache_l1+8]
	shr       eax,0
	and       eax,255
	mov       dword ptr [ebp-40],eax
	jmp       @118
	?debug L 231
@108:
	mov       edx,dword ptr [_proc_cache_l1+12]
	shr       edx,24
	and       edx,255
	mov       dword ptr [ebp-40],edx
	jmp       @118
	?debug L 232
@107:
	mov       ecx,dword ptr [_proc_cache_l1+12]
	shr       ecx,16
	and       ecx,255
	mov       dword ptr [ebp-40],ecx
	jmp       short @118
	?debug L 233
@106:
	mov       eax,dword ptr [_proc_cache_l1+12]
	shr       eax,8
	and       eax,255
	mov       dword ptr [ebp-40],eax
	jmp       short @118
	?debug L 234
@105:
	mov       edx,dword ptr [_proc_cache_l1+12]
	shr       edx,0
	and       edx,255
	mov       dword ptr [ebp-40],edx
	jmp       short @118
	?debug L 236
@104:
	mov       ecx,dword ptr [_proc_cache_l2+8]
	shr       ecx,16
	and       ecx,65535
	mov       dword ptr [ebp-40],ecx
	jmp       short @118
	?debug L 237
@103:
	mov       eax,dword ptr [_proc_cache_l2+8]
	shr       eax,12
	and       eax,15
	mov       dword ptr [ebp-40],eax
	jmp       short @118
	?debug L 238
@102:
	mov       edx,dword ptr [_proc_cache_l2+8]
	shr       edx,8
	and       edx,15
	mov       dword ptr [ebp-40],edx
	jmp       short @118
	?debug L 239
@101:
	mov       ecx,dword ptr [_proc_cache_l2+8]
	shr       ecx,0
	and       ecx,255
	mov       dword ptr [ebp-40],ecx
	?debug L 242
@99:
@100:
@118:
	push      1
	call      @_GetCPUCaps
	pop       ecx
	cmp       eax,6
	jne       @39
	?debug L 245
@119:
	movsx     eax,byte ptr [ebp+8]
	add       eax,-34
	cmp       eax,23
	ja        @39
	jmp       dword ptr [@134+4*eax]
@134:
	dd        @133
	dd        @132
	dd        @131
	dd        @130
	dd        @39
	dd        @39
	dd        @39
	dd        @39
	dd        @39
	dd        @39
	dd        @39
	dd        @39
	dd        @39
	dd        @39
	dd        @39
	dd        @39
	dd        @129
	dd        @128
	dd        @127
	dd        @126
	dd        @125
	dd        @124
	dd        @123
	dd        @122
	?debug L 247
@133:
	mov       edx,dword ptr [_proc_cache_l1]
	shr       edx,24
	and       edx,255
	mov       dword ptr [ebp-40],edx
	jmp       @39
	?debug L 248
@132:
	mov       ecx,dword ptr [_proc_cache_l1]
	shr       ecx,16
	and       ecx,255
	mov       dword ptr [ebp-40],ecx
	jmp       @39
	?debug L 249
@131:
	mov       eax,dword ptr [_proc_cache_l1]
	shr       eax,8
	and       eax,255
	mov       dword ptr [ebp-40],eax
	jmp       @39
	?debug L 250
@130:
	mov       edx,dword ptr [_proc_cache_l1]
	shr       edx,0
	and       edx,255
	mov       dword ptr [ebp-40],edx
	jmp       @39
	?debug L 252
@129:
	mov       ecx,dword ptr [_proc_cache_l2]
	shr       ecx,28
	and       ecx,15
	mov       dword ptr [ebp-40],ecx
	jmp       short @39
	?debug L 253
@128:
	mov       eax,dword ptr [_proc_cache_l2]
	shr       eax,16
	and       eax,4095
	mov       dword ptr [ebp-40],eax
	jmp       short @39
	?debug L 254
@127:
	mov       edx,dword ptr [_proc_cache_l2]
	shr       edx,12
	and       edx,15
	mov       dword ptr [ebp-40],edx
	jmp       short @39
	?debug L 255
@126:
	mov       ecx,dword ptr [_proc_cache_l2]
	shr       ecx,0
	and       ecx,4095
	mov       dword ptr [ebp-40],ecx
	jmp       short @39
	?debug L 257
@125:
	mov       eax,dword ptr [_proc_cache_l2+4]
	shr       eax,28
	and       eax,15
	mov       dword ptr [ebp-40],eax
	jmp       short @39
	?debug L 258
@124:
	mov       edx,dword ptr [_proc_cache_l2+4]
	shr       edx,16
	and       edx,4095
	mov       dword ptr [ebp-40],edx
	jmp       short @39
	?debug L 259
@123:
	mov       ecx,dword ptr [_proc_cache_l2+4]
	shr       ecx,12
	and       ecx,15
	mov       dword ptr [ebp-40],ecx
	jmp       short @39
	?debug L 260
@122:
	mov       eax,dword ptr [_proc_cache_l2+4]
	shr       eax,0
	and       eax,4095
	mov       dword ptr [ebp-40],eax
	?debug L 264
	?debug L 267
@120:
@121:
@7:
@39:
	mov       eax,dword ptr [ebp-40]
	mov       edx,dword ptr [ebp-36]
	mov       dword ptr fs:[0],edx
	?debug L 268
@135:
@5:
	pop       edi
	pop       esi
	pop       ebx
	mov       esp,ebp
	pop       ebp
	ret 
	?debug L 0
@_GetCPUCaps	endp
_GetCPUCaps	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	db	2
	db	0
	db	0
	db	0
	dw	57
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
	df	@_GetCPUCaps
	dw	0
	dw	4096
	dw	0
	dw	1
	dw	0
	dw	0
	dw	0
	db	10
	db	71
	db	101
	db	116
	db	67
	db	80
	db	85
	db	67
	db	97
	db	112
	db	115
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
	dw	65496
	dw	65535
	dw	34
	dw	0
	dw	3
	dw	0
	dw	0
	dw	0
	dw	22
	dw	513
	df	$ildjdaia
	dw	0
	dw	116
	dw	0
	dw	4
	dw	0
	dw	0
	dw	0
	dw	24
	dw	519
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch4
	df	@94
	dw	0
	dw	0
?patch4	equ	@95-@94
	dw	2
	dw	6
	dw	24
	dw	519
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch5
	df	@96
	dw	0
	dw	0
?patch5	equ	@97-@96
	dw	2
	dw	6
?patch1	equ	@135-@_GetCPUCaps+7
?patch2	equ	0
?patch3	equ	@135-@_GetCPUCaps
	dw	2
	dw	6
	dw	8
	dw	531
	dw	7
	dw	65484
	dw	65535
$$BSYMS	ends
_DATA	segment dword public use32 'DATA'
	align	4
@_$ECTB$@detect_base$qv	label	dword
	dd	0
	dd	-36
	dw	0
	dw	2
	dd	1
	dd	@136
_DATA	ends
_TEXT	segment dword public use32 'CODE'
@detect_base$qv	proc	near
@@detect_base$qv equ @detect_base$qv
?live1@0:
	?debug L 286
	push      ebp
	mov       ebp,esp
	add       esp,-40
	push      ebx
	push      esi
	push      edi
@137:
	mov       eax,offset @_$ECTB$@detect_base$qv
	call      @__InitExceptBlockLDTC
	?debug L 291
	mov       word ptr [ebp-20],8
	?debug L 295
	mov	     eax,0       
 	function	 0 = manufacturer string
	?debug L 296
	CPUID	
	?debug L 298
	mov       word ptr [ebp-20],0
	jmp       short @138
	?debug L 301
@139:
@140:
@141:
@136:
	or        eax,-1
	mov       edx,dword ptr [ebp-36]
	mov       dword ptr fs:[0],edx
	jmp       @142
	?debug L 332
@138:
@143:
	mov	     DWORD PTR [byte ptr _proc_idstr+0],ebx  
 	Stash	 the manufacturer string for later
	?debug L 333
	mov	     DWORD PTR [byte ptr _proc_idstr+4],edx
	?debug L 334
	mov	     DWORD PTR [byte ptr _proc_idstr+8],ecx
	?debug L 335
	test	    eax,eax             
 	0	 is highest function, then don't query more info
	?debug L 336
	mov	     eax,1               
 	but	 the init DID happen
	?debug L 337
	je        @144
	?debug L 340
	;	 Load up the dword ptr _features and (where appropriate) extended dword ptr _features flags
	?debug L 341
	mov	     eax,1               
 	Check	 for dword ptr _processor dword ptr _features
	?debug L 342
	CPUID	
	?debug L 343
	mov	     [dword ptr _processor],eax   
 	Store	 dword ptr _processor family/model/step
	?debug L 344
	mov	     [dword ptr _features],edx    
 	Store	 dword ptr _features bits
	?debug L 346
	mov	     eax,080000000h      
 	Check	 for support of extended functions.
	?debug L 347
	CPUID	
	?debug L 349
	;	 Check which extended functions can be called
	?debug L 350
	cmp	     eax,080000001h      
 	Extended	 Feature Bits
	?debug L 351
	jb        @144
 	jump	 if not supported
	?debug L 352
	cmp	     eax, 080000004h     
 	CPU	 Name string
	?debug L 353
	jb        @145
 	jump	 if not supported
	?debug L 354
	cmp	     eax, 080000005h     
 	L1	 Cache information
	?debug L 355
	jb        short @146
 	jump	 if not supported
	?debug L 356
	cmp	     eax, 080000006h     
 	L2	 Cache information
	?debug L 357
	jb        short @147
 	jump	 if not supported
	?debug L 359
	;	 Query and save L2 cache information
	?debug L 360
	mov	     eax,080000006h      
 	L2	 Cache Information
	?debug L 361
	CPUID	                       
 	Interpretation	 is CPU specific, but
	?debug L 362
	;	 fetching is not.
	?debug L 363
	mov	     DWORD PTR [dword ptr _proc_cache_l2+0],eax
	?debug L 364
	mov	     DWORD PTR [dword ptr _proc_cache_l2+4],ebx
	?debug L 365
	mov	     DWORD PTR [dword ptr _proc_cache_l2+8],ecx
	?debug L 366
	mov	     DWORD PTR [dword ptr _proc_cache_l2+12],edx
	?debug L 369
@147:
just_L1:
	?debug L 370
	;	 Query and save L1 cache informatin
	?debug L 371
	mov	     eax,080000005h      
 	L1	 Cache Information
	?debug L 372
	CPUID	                       
 	Interpretation	 is CPU specific, but
	?debug L 373
	;	 fetching is not.
	?debug L 374
	mov	     DWORD PTR [dword ptr _proc_cache_l1+0],eax
	?debug L 375
	mov	     DWORD PTR [dword ptr _proc_cache_l1+4],ebx
	?debug L 376
	mov	     DWORD PTR [dword ptr _proc_cache_l1+8],ecx
	?debug L 377
	mov	     DWORD PTR [dword ptr _proc_cache_l1+12],edx
	?debug L 380
@146:
just_name:
	?debug L 381
	;	 Query and save the CPU name string
	?debug L 382
	mov	     eax,080000002h
	?debug L 383
	CPUID	
	?debug L 384
	mov	     DWORD PTR [byte ptr _proc_namestr+0],eax
	?debug L 385
	mov	     DWORD PTR [byte ptr _proc_namestr+4],ebx
	?debug L 386
	mov	     DWORD PTR [byte ptr _proc_namestr+8],ecx
	?debug L 387
	mov	     DWORD PTR [byte ptr _proc_namestr+12],edx
	?debug L 388
	mov	     eax,080000003h
	?debug L 389
	CPUID	
	?debug L 390
	mov	     DWORD PTR [byte ptr _proc_namestr+16],eax
	?debug L 391
	mov	     DWORD PTR [byte ptr _proc_namestr+20],ebx
	?debug L 392
	mov	     DWORD PTR [byte ptr _proc_namestr+24],ecx
	?debug L 393
	mov	     DWORD PTR [byte ptr _proc_namestr+28],edx
	?debug L 394
	mov	     eax,080000004h
	?debug L 395
	CPUID	
	?debug L 396
	mov	     DWORD PTR [byte ptr _proc_namestr+32],eax
	?debug L 397
	mov	     DWORD PTR [byte ptr _proc_namestr+36],ebx
	?debug L 398
	mov	     DWORD PTR [byte ptr _proc_namestr+40],ecx
	?debug L 399
	mov	     DWORD PTR [byte ptr _proc_namestr+44],edx
	?debug L 401
@145:
just_extfeat:
	?debug L 402
	;	 Query and save the extended dword ptr _features bits
	?debug L 403
	mov	     eax,080000001h      
 	Select	 function 080000001H
	?debug L 404
	CPUID	
	?debug L 405
	mov	     [dword ptr _ext_features],edx  
 	Store	 extended dword ptr _features bits
	?debug L 406
	mov	     eax,1               
 	Set	 "Has CPUID" flag to true
	?debug L 408
@144:
no_features:
	?debug L 409
	mov	     dword ptr [ebp-40],eax
	?debug L 412
	mov       eax,dword ptr [ebp-40]
	mov       edx,dword ptr [ebp-36]
	mov       dword ptr fs:[0],edx
	?debug L 413
@148:
@142:
	pop       edi
	pop       esi
	pop       ebx
	mov       esp,ebp
	pop       ebp
	ret 
	?debug L 0
@detect_base$qv	endp
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	46
	dw	516
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch6
	dd	?patch7
	dd	?patch8
	df	@detect_base$qv
	dw	0
	dw	4098
	dw	0
	dw	5
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65496
	dw	65535
	dw	34
	dw	0
	dw	6
	dw	0
	dw	0
	dw	0
	dw	24
	dw	519
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch9
	df	@139
	dw	0
	dw	0
?patch9	equ	@140-@139
	dw	2
	dw	6
	dw	24
	dw	519
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch10
	df	@141
	dw	0
	dw	0
?patch10	equ	@143-@141
	dw	2
	dw	6
?patch6	equ	@148-@detect_base$qv+7
?patch7	equ	0
?patch8	equ	@148-@detect_base$qv
	dw	2
	dw	6
	dw	8
	dw	531
	dw	7
	dw	65484
	dw	65535
$$BSYMS	ends
_DATA	segment dword public use32 'DATA'
s@	label	byte
	;	s@+0:
	db	"AuthenticAMD",0
	;	s@+13:
	db	"GenuineIntel",0
	;	s@+26:
	db	"CyrixInstead",0
	;	s@+39:
	db	"CentaurHauls",0
	align	4
_DATA	ends
_TEXT	segment dword public use32 'CODE'
_TEXT	ends
@_strncmp equ _strncmp
 extrn   _strncmp:near
 extrn   __Exception_list:dword
 extrn   @__InitExceptBlockLDTC:near
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	?patch11
	dw	37
	dw	7
	dw	0
	dw	0
	dw	0
	dw	0
?patch11	equ	12
	dw	22
	dw	513
	df	_features
	dw	0
	dw	34
	dw	0
	dw	8
	dw	0
	dw	0
	dw	0
	dw	22
	dw	513
	df	_ext_features
	dw	0
	dw	34
	dw	0
	dw	9
	dw	0
	dw	0
	dw	0
	dw	22
	dw	513
	df	_processor
	dw	0
	dw	34
	dw	0
	dw	10
	dw	0
	dw	0
	dw	0
	dw	22
	dw	513
	df	_proc_idstr
	dw	0
	dw	4100
	dw	0
	dw	11
	dw	0
	dw	0
	dw	0
	dw	22
	dw	513
	df	_proc_namestr
	dw	0
	dw	4101
	dw	0
	dw	12
	dw	0
	dw	0
	dw	0
	dw	22
	dw	513
	df	_proc_cache_l1
	dw	0
	dw	4102
	dw	0
	dw	13
	dw	0
	dw	0
	dw	0
	dw	22
	dw	513
	df	_proc_cache_l2
	dw	0
	dw	4103
	dw	0
	dw	14
	dw	0
	dw	0
	dw	0
	dw	?patch12
	dw	38
	dw	97
	dw	15
	dw	0
	dw	16
	dw	0
	dw	17
	dw	0
	dw	18
	dw	0
	dw	19
	dw	0
	dw	20
	dw	0
	dw	21
	dw	0
	dw	22
	dw	0
	dw	23
	dw	0
	dw	24
	dw	0
	dw	25
	dw	0
	dw	26
	dw	0
	dw	27
	dw	0
	dw	28
	dw	0
	dw	29
	dw	0
	dw	30
	dw	0
	dw	31
	dw	0
	dw	32
	dw	0
	dw	33
	dw	0
	dw	34
	dw	0
	dw	35
	dw	0
	dw	36
	dw	0
	dw	37
	dw	0
	dw	38
	dw	0
	dw	39
	dw	0
	dw	40
	dw	0
	dw	41
	dw	0
	dw	42
	dw	0
	dw	43
	dw	0
	dw	44
	dw	0
	dw	45
	dw	0
	dw	46
	dw	0
	dw	47
	dw	0
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
?patch12	equ	392
	dw	?patch13
	dw	1
	db	3
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
?patch13	equ	18
$$BSYMS	ends
$$BTYPES	segment byte public use32 'DEBTYP'
	db        2,0,0,0,14,0,8,0,34,0,0,0,0,0,1,0
	db        1,16,0,0,8,0,1,2,1,0,255,255,255,255,14,0
	db        8,0,34,0,0,0,0,0,0,0,3,16,0,0,4,0
	db        1,2,0,0,18,0,3,0,112,0,0,0,17,0,0,0
	db        0,0,0,0,16,0,16,0,18,0,3,0,112,0,0,0
	db        17,0,0,0,0,0,0,0,48,0,48,0,18,0,3,0
	db        34,0,0,0,17,0,0,0,0,0,0,0,16,0,4,0
	db        18,0,3,0,34,0,0,0,17,0,0,0,0,0,0,0
	db        16,0,4,0,14,0,8,0,116,0,0,0,0,0,3,0
	db        11,16,0,0,8,0,2,0,10,0,10,16,0,0,8,0
	db        1,0,1,0,112,0,0,0,16,0,1,2,3,0,9,16
	db        0,0,9,16,0,0,117,0,0,0,8,0,2,0,10,2
	db        3,0,0,0,14,0,8,0,3,0,0,0,4,0,1,0
	db        14,16,0,0,8,0,1,2,1,0,3,4,0,0
$$BTYPES	ends
$$BNAMES	segment byte public use32 'DEBNAM'
	db	10,'GetCPUCaps'
	db	3,'cap'
	db	3,'res'
	db	4,'init'
	db	11,'detect_base'
	db	3,'res'
	db	3,'std'
	db	8,'features'
	db	12,'ext_features'
	db	9,'processor'
	db	10,'proc_idstr'
	db	12,'proc_namestr'
	db	13,'proc_cache_l1'
	db	13,'proc_cache_l2'
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
	db	7,'_wcsupr'
	db	7,'_wcsset'
	db	7,'_wcsrev'
	db	8,'_wcspcpy'
	db	8,'_wcsnset'
	db	10,'_wcsnicoll'
	db	9,'_wcsnicmp'
	db	9,'_wcsncoll'
	db	7,'_wcslwr'
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
	db	12,'_lwsetlocale'
	db	11,'_wsetlocale'
	db	12,'_llocaleconv'
	db	10,'localeconv'
	db	11,'_lsetlocale'
	db	10,'__threadid'
	db	14,'@std@ptrdiff_t'
	db	11,'@std@size_t'
	db	11,'@std@wint_t'
	db	13,'@std@wctype_t'
	db	10,'@std@lconv'
$$BNAMES	ends
	?debug	D "D:\redshift\Demo_sw\DemoSystem\CPU_DETECT.H" 11130 34228
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\mem.h" 10503 10272
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\_loc.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\locale.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\_null.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\_defs.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\_stddef.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\_str.h" 10503 10272
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\STRING.H" 10303 10240
	?debug	D "D:\redshift\Demo_sw\DemoSystem\cpu_detect.cpp" 11130 34228
 ?debug  C FB062A633A5C50524F4752417E315C626F726C616E645C434255494C447E315C6C69625C76636C35302E233035C41F0000F843372C
 ?debug  C FB05082343505543415053
	end
