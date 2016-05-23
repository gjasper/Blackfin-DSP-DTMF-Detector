#include "defbf533.h"


//.text
.section/program;
.align 4;

.global _atualiza_vet_entr;

_atualiza_vet_entr:

//execução do filtro


	// Prologo da função 
	LINK 0;
	I0 = R0;		// I0 --> Ponteiro do vetor de amostras
	I1=R1;			// I1 --> Ponteiro dos coeficientes
	P0 = R2;		// Carregando o quarto parametro (tamanho do loop)
	//A0=0;			//zera saída do filtro
	
	nop; nop;nop;
	
	// lENDO as amostras e os coefs
	A0=0 || R0.H = w[I0++] || R0.L = w[I1++];
	loop loop2 LC1=P0;				//convolução e acumulação
	loop_begin loop2;
		A0 += R0.H * R0.L || R0.H = w[I0++] || R0.L = w[I1++];
	loop_end loop2;
	
	A0 = A0 >>> 16;
	R0 = A0;			//o retorno é no R0
	

	
	//Desloca o vetor de amostras
		
	I0 -= 4; // I1 -> ÚLTIMO
	I1 = I0;
	I1 -= 2;	// ANTE-PENULTIMO
	R3.L = W[I1--];
	//P0 += -1;
	loop proc_filt_fir LC0=P0;	//
	loop_begin proc_filt_fir;
		 
		R3.L = W[I1--] || W[I0--] = R3.L;
	loop_end proc_filt_fir;

	

	UNLINK;

	rts;
	
_atualiza_vet_entr.end:
