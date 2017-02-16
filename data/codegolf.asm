
start: 0000
0000	81 fc 00 01	cmp sp, 100h		; z=1
0004	74 01		jz cont (jz +1)		; jump if z==1

hlt: 0006
0006	f4		hlt

cont:
0007	bc 00 10	mov sp, 1000h
000a	b0 2e	 	mov al, '.'
000c	bb 00 00	mov bx, 0
000f	4b		dec bx
0010	83 fb ff	cmp bx, 0ffffh		; z=1
0013	75 f1		jnz hlt (jnz -0d)	; jump if z!=1
0015	e8 51 01	call printchr		; call 0x169

0018	43		inc bx
0019	75 eb		jnz hlt
001b	e8 4b 01	call printchr

001e	31 c9 		xor cx, cx
0020	09 cb		or bx, cx
0022	75 e2		jnz hlt
0024	72 e0		jc hlt
0026	e8 40 01	call printchr

0029	b9 00 80	mov cx, 08000h
002c	39 d9		cmp cx, bx
002e	76 d6		jbe hlt
0030	e8 38 01	call printchr

0033	01 cb		add bx, cx
0035	72 cf		jc hlt
0037	e8 2f 01	call printchr

003a	01 db		add bx, bx
003c	83 d1 00	adc cx, 0
003f	79 c5		jns hlt
0041	72 c3		jc hlt
0043	51		push cx
0044	83 e1 01	and cx, 1
0047	74 bd		jz hlt
0049	e8 1d 01	call printchr

004c	59		pop cx
004d	f9		stc
004e	bb 00 80	mov bx, 08000h
0051	19 d9		sbb cx, bx
0053	75 b1		jnz hlt
0055	72 af		jc hlt
0057	e8 0f 01	call printchr

005a	e8 00 00	call calltest

calltest:005d
005d	5b		pop bx
005e	81 fb 5d 00	cmp bx, calltest
0062	75 a2		jnz hlt
0064	81 fc 00 10	cmp sp, 1000h
0068	75 9c		jnz hlt
006a	e8 fc 00	call printchr

006d	bb 72 00	mov bx, rettest
0070	53		push bx
0071	c3		ret

rettest:0072
0072	81 fc 00 10	cmp sp, 1000h
0076	75 8e		jnz hlt
0078	e8 ee 00	call printchr

007b	90		nop
007c	90		nop
007d	90		nop
007e	eb 01		jmp cont1
0080	f4		hlt

cont1:0081
0081	e8 f8 00	call printnl

0084	b8 c5 01	mov ax, hello
0087	e8 ca 00	call print
008a	e8 ef 00	call printnl

008d	b0 30		mov al, 30h

ascii_loop:008f
008f	e8 d7 00	call printchr
0092	fe c0		inc al
0094	3c 7f		cmp al, 127
0096	75 f7		jnz ascii_loop


0098	b0 23			mov al, '#'
009a	c7 06 d3 01 90 01	mov word [cursor], 80 * 5
00a0	b1 50			mov cl, 80

boxloop:00a2
00a2	e8 c4 00		call printchr
00a5	fe c9			dec cl
00a7	75 f9			jnz boxloop
00a9	81 3e d3 81 e0 01	cmp word [cursor], 480
00af	75 0a			jnz cont2
00b1	b1 59			mov cl, 80
00b3	c7 06 d3 01 80 07	mov word [cursor], 80 * 24
00b9	eb e7			jmp boxloop

cont2:00bb
00bb	c7 06 d3 01 e0 01	mov word [cursor], 80 * 6
00c1	b1 12			mov cl, 18
	
boxloop2:00c3
00c3	e8 a3 00		call printchr
00c6	e8 a0 00		call printchr
00c9	83 06 d3 01 4c		add word [cursor], 76
00ce	e8 98 00		call printchr
00d1	e8 95 00		call printchr
00d4	fe c9			dec cl
00d6	75 eb			jnz boxloop2

00d8	c7 06 d3 01 34 02	mov word [cursor], 80 * 7 + 4
00de	31 c0			xor ax, ax
00e0	ba 01 00		mov dx, 1
00e3	b9 11 00		mov cx, 17

fibloop:00e6
00e6	01 c2			add dx, ax
00e8	e8 9f 00		call printnum
00eb	50			push ax
00ec	b8 20 00		mov ax, ' '
00ef	e8 77 00		call printchr
00f2	58			pop ax
00f3	92			xchg ax, dx
00f4	49			dec cx
00f5	75 ef			jnz fibloop

00f7	c7 06 d3 01 d4 02	mov word [cursor], 80 * 9 + 4
00fd	b9 00 00		mov cx, 0

squareloop:0100
0100	89 c8			mov ax, cx
0102	e8 3f 00		call calcsq
0105	e8 82 00		call printnum
0108	b8 20 00		mov ax, ' '
010b	e8 5b 00		call printchr
010e	41			inc cx
010f	83 f9 14		cmp cx, 20
0112	76 ec			jbe squareloop

	%define count 100
0114	c7 06 d3 01 74 03	mov word [cursor], 80 * 11 + 4
011a	bb 02 00		mov bx, 2

primeloop:011d
011d	80 8f d5 01 00		or byte [memory + bx], 0
0122	75 19			jnz primecont
0124	89 d8			mov ax, bx
0126	e8 61 00		call printnum
0129	b8 20 00		mov ax, ' '
012c	e8 3a 00		call printchr
012f	89 df			mov di, bx

primeloop_inner:0131
0131	80 8d d5 01 01 		or byte [memory + di], 1
0136	01 df			add di, bx
0138	83 ff 65		cmp di, count + 1
013b	76 f4			jbe primeloop_inner

primecont:013d
013d	43			inc bx
013e	83 fb 64		cmp bx, count
0141	76 da			jbe primeloop

0143	f4			hlt

calcsq:0144
0144	89 c3			mov bx, ax
0146	31 d2			xor dx, dx
0148	09 db			or bx, bx

calcsqloop:014a
014a	74 04			jz calcsqfinish
014c	01 c2			add dx, ax
014e	4b			dec bx
014f	eb f9			jmp calcsqloop

calcsqfinish:0151
0151	89 d0			mov ax, dx
0153	c3			ret

print:0154
0154	53			push bx
0155	52			push dx
0156	89 c3			mov bx, ax

printloop:
0158	8a 17			mov dl, [bx]
015a	43			inc bx
015b	86 c2			xchg al, dl
015d	e8 09 00		call printchr
0160	86 c2			xchg al, dl
0162	20 d2			and dl, dl
0164	75 f2			jnz printloop
0166	5a			pop dx
0167	5b			pop bx
0168	c3			ret

printchr:0169
0169	53			push bx
016a	57			push di
016b	bb 00 80		mov bx, 8000h
016e	8b 3e d3 01		mov di, [cursor]
0172	88 01			mov [bx+di], al
0174	47			inc di
0175	89 3e d3 01		mov [cursor], di
0179	5f			pop di
017a	5b			pop bx
017b	c3			ret

printnl:017c
017c	8b 3e d3 01		mov di, [cursor]

printnlloop:0180
0180	83 ef 50		sub di, 80
0183	79 fb			jns printnlloop
0185	29 3e d3 01		sub [cursor], di
0189	c3			ret

printnum:018a
018a	53			push bx
018b	50			push ax
018c	b3 30			mov bl, '0'
018e	83 f8 09		cmp ax, 9
0191	76 2a			jbe numcont_1digit
0193	83 f8 63		cmp ax, 99
0196	76 13			jbe numloop_2digit

numloop_3digit:0198
0198	83 e8 64		sub ax, 100
019b	fe c3			inc bl
019d	83 f8 63		cmp ax, 99
01a0	77 f6			jnbe numloop_3digit
01a2	86 d8			xchg bl, al
01a4	e8 c2 ff		call printchr
01a7	86 d8			xchg bl, al
01a8	b3 30			mov bl, '0'

numloop_2digit:01aa
01aa	83 f8 09		cmp ax, 9
01ad	76 06			jbe numcont_2digit
01b0	83 e8 0a		sub ax, 10
01b3	43			inc bx
01b4	eb f5			jmp numloop_2digit

numcont_2digit:01b6
01b6	86 c3			xchg al, bl
01b8	e8 ae ff		call printchr
01bb	88 d8			mov al, bl

numcont_1digit:01bd
01bd	04 30			add al, '0'
01bf	e8 a7 ff		call printchr
01c2	58			pop ax
01c3	5b			pop bx
01c4	c3			ret

hello:01c5
01c5	48 65 6c 6c 6f 2c 20 77 6f 72 6c 64 21 00	db 'Hello, world!', 0

cursor:
01d3	00	00	dw 0

memory:

