	.MODEL	TINY	
	.STACK	100	
	.DATA		
TAB_1:		    	DB	0H,5H,6H,3H,8H,92H,04H,57H,46H,01H,0FFH,0A0H,45H,99H,55H,66H
	.CODE		
START:	MOV	AX,@DATA	
	MOV	DS,AX	
	NOP		
	MOV	CX,16 - 1	;存放比较次数 = 数据个数 - 1
STAR2:	PUSH		CX	
	LEA	SI,TAB_1	
	MOV	DL,0		;0->交换过数据标志
STAR3:	LODSB		
	MOV		AH,[SI]	
	CMP	AH,AL	
	JNB		STAR5	
	MOV		[SI],AL	
	MOV	[SI - 1],AH		
	MOV		DL,1		;1->交换过数据标志
STAR5:	LOOP		STAR3	
	POP		CX	
	CMP	DL,0	
	JZ	Exit	
	LOOP	STAR2	
Exit:	JMP		$	
			
	END	START	

