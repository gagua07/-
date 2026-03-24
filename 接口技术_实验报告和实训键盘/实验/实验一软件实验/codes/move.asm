	.MODEL	TINY	
			
	.STACK	100	
	.DATA		
			
	.CODE		
START:	MOV	AX,@DATA	
	MOV	DS,AX	
	MOV	ES,AX	
	NOP		
	MOV		CX,100H	
	MOV	SI,3000H	
	MOV	DI,6000H	
	CALL	Move	
	MOV	CX,100H	
	MOV	SI,3000H	
	MOV	DI,6000H	
	CLD		
	REPE	CMPSB	
	JNE	ERROR	
TRUE:	JMP	$		
ERROR:	JMP	$		
Move		PROC		NEAR	
	CLD		
	CMP	SI,DI	
	JZ	Return	
	JNB	Move1	
	ADD	SI,CX	
	DEC	SI	
	ADD	DI,CX	
	DEC	DI	
	STD		
Move1:	REP	MOVSB	
Return:		RET		
Move		ENDP		
	END	START	

