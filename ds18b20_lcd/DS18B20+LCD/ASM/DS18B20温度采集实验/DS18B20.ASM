	TEMPER_L 	EQU 	36H	;存放读出温度低位数据
	TEMPER_H 	EQU 	35H	;存放读出温度高位数据
	TEMPER_NUM 	EQU 	60H	;存放转换后的温度值
	FLAG1 		BIT	00H
	DQ 		BIT	 P3.3	;一线总线控制端口

	ORG		0000H
	LJMP	MAIN
	ORG		0100H
MAIN:
	MOV 	SP,#70H
	LCALL 	GET_TEMPER	;从DS18B20读出温度数据
	LCALL	 TEMPER_COV	;转换读出的温度数据并保存
	SJMP 	$						;完成一次数字温度采集


;-------读出转换后的温度值
GET_TEMPER:
	SETB 	DQ 		; 定时入口
BCD:	LCALL 	INIT_1820
	JB 	FLAG1,S22
	LJMP 	BCD 		; 若DS18B20不存在则返回
S22:	LCALL 	DELAY1
	MOV 	A,#0CCH 	; 跳过ROM匹配------0CC
	LCALL 	WRITE_1820
	MOV 	A,#44H 		; 发出温度转换命令
	LCALL 	WRITE_1820
	NOP
	LCALL 	DELAY
	LCALL 	DELAY
CBA:	LCALL 	INIT_1820
	JB 	FLAG1,ABC
	LJMP 	CBA
ABC:	LCALL 	DELAY1
	MOV 	A,#0CCH 	; 跳过ROM匹配
	LCALL 	WRITE_1820
	MOV 	A,#0BEH 	; 发出读温度命令
	LCALL 	WRITE_1820
	LCALL 	READ_18200 	;READ_1820
	RET

;-------读DS18B20的程序,从DS18B20中读出一个字节的数据
READ_1820:
	MOV 	R2,#8
RE1:	CLR 	C
	SETB 	DQ
	NOP
	NOP
	CLR 	DQ
	NOP
	NOP
	NOP
	SETB 	DQ
	MOV 	R3,#7
	DJNZ 	R3,$
	MOV 	C,DQ
	MOV 	R3,#23
	DJNZ 	R3,$
	RRC 	A
	DJNZ 	R2,RE1
	RET

;-------写DS18B20的程序
WRITE_1820:
	MOV 	R2,#8
	CLR 	C
WR1:	CLR 	DQ
	MOV 	R3,#6
	DJNZ 	R3,$
	RRC 	A
	MOV 	DQ,C
	MOV 	R3,#23
	DJNZ 	R3,$
	SETB 	DQ
	NOP
	DJNZ 	R2,WR1
	SETB 	DQ
	RET

;-------读DS18B20的程序,从DS18B20中读出两个字节的温度数据
READ_18200:
	MOV 	R4,#2 	; 将温度高位和低位从DS18B20中读出
	MOV 	R1,#36H ; 低位存入36H(TEMPER_L),高位存入35H(TEMPER_H)
RE00:	MOV 	R2,#8
RE01:	CLR 	C
	SETB 	DQ
	NOP
	NOP
	CLR 	DQ
	NOP
	NOP
	NOP
	SETB 	DQ
	MOV 	R3,#7
	DJNZ 	R3,$
	MOV 	C,DQ
	MOV 	R3,#23
	DJNZ 	R3,$
	RRC 	A
	DJNZ 	R2,RE01
	MOV 	@R1,A
	DEC 	R1
	DJNZ 	R4,RE00
	RET

;-------将从DS18B20中读出的温度数据进行转换
TEMPER_COV:
	MOV 	A,#0F0H
	ANL 	A,TEMPER_L ; 舍去温度低位中小数点后的四位温度数值
	SWAP 	A
	MOV 	TEMPER_NUM,A
	MOV 	A,TEMPER_L
	JNB 	ACC.3,TEMPER_COV1 ; 四舍五入去温度值
	INC 	TEMPER_NUM

TEMPER_COV1:
	MOV 	A,TEMPER_H
	ANL 	A,#07H
	SWAP 	A
	ADD 	A,TEMPER_NUM
	MOV 	TEMPER_NUM,A ; 保存变换后的温度数据
	LCALL 	BIN_BCD
	RET

;-------将16进制的温度数据转换成压缩BCD码
BIN_BCD:
	MOV 	DPTR,#TEMP_TAB
	MOV 	A,TEMPER_NUM
	MOVC 	A,@A+DPTR
	MOV 	TEMPER_NUM,A
	RET
TEMP_TAB:
	DB 00H,01H,02H,03H,04H,05H,06H,07H
	DB 08H,09H,10H,11H,12H,13H,14H,15H
	DB 16H,17H,18H,19H,20H,21H,22H,23H
	DB 24H,25H,26H,27H,28H,29H,30H,31H
	DB 32H,33H,34H,35H,36H,37H,38H,39H
	DB 40H,41H,42H,43H,44H,45H,46H,47H
	DB 48H,49H,50H,51H,52H,53H,54H,55H
	DB 56H,57H,58H,59H,60H,61H,62H,63H
	DB 64H,65H,66H,67H,68H,69H,70H,71H
	DB 72H,73H,74H,75H,76H,77H,78H,79H
	DB 80H,81H,82H,83H,84H,85H,86H,87H
	DB 88H,89H,90H,91H,92H,93H,94H,95H
	DB 96H,97H,98H,99H

;-------DS18B20初始化程序
INIT_1820:
	SETB 	DQ
	NOP
	CLR 	DQ
	MOV 	R0,#80H
TSR1:	DJNZ 	R0,TSR1 ; 延时
	SETB 	DQ
	MOV 	R0,#25H ;96US-25H
TSR2:	DJNZ 	R0,TSR2
	JNB 	DQ,TSR3
	LJMP 	TSR4 	; 延时
TSR3:	SETB 	FLAG1 	; 置标志位,表示DS1820存在
	LJMP 	TSR5
TSR4:	CLR 	FLAG1 	; 清标志位,表示DS1820不存在
	LJMP 	TSR7
TSR5:	MOV 	R0,#06BH ;200US
TSR6:	DJNZ 	R0,TSR6 ; 延时
TSR7:	SETB 	DQ
	RET

;-------重新写DS18B20暂存存储器设定值
RE_CONFIG:
	JB 	FLAG1,RE_CONFIG1 ; 若DS18B20存在,转RE_CONFIG1
	RET
RE_CONFIG1:
	MOV 	A,#0CCH ; 发SKIP ROM命令
	LCALL 	WRITE_1820
	MOV 	A,#4EH ; 发写暂存存储器命令
	LCALL 	WRITE_1820
	
	MOV 	A,#00H ; TH(报警上限)中写入00H
	LCALL 	WRITE_1820
	MOV 	A,#00H ; TL(报警下限)中写入00H
	LCALL 	WRITE_1820
	MOV 	A,#7FH ; 选择12位温度分辨率
	LCALL 	WRITE_1820
	RET

;------------------延时子程序
DELAY:
	MOV 	R7,#00H
MIN:	DJNZ 	R7,YS500
	RET
YS500:	LCALL 	YS500US
	LJMP 	MIN
YS500US:MOV 	R6,#00H
	DJNZ 	R6,$
	RET
DELAY1:	MOV 	R7,#20H
	DJNZ 	R7,$
	RET

	END

