/* programa para testes com arquivos
-- Lendo arquivo de entrada
-- Processa: Executa o filtro Fir
-- Gera arquivo de saida
-- walter 1.0 
*/

#include <stdio.h>
#include <string.h>
#include <fract.h>
#include <cycles.h>


#define NSAMPLES       800

#define minAmplitude  8100

/* Carregando o vetor de Coeficientes*/
short Coefs[NSAMPLES] =
{
	#include "kernel_singlefreqBP_1477.dat" 
	//#include "coefs_txt.txt"
};

short kernel_697 [NSAMPLES] = {
	#include "kernel_singlefreqBP_697.dat"
	};
short kernel_770 [NSAMPLES] = {
	#include "kernel_singlefreqBP_770.dat"
	};
short kernel_852 [NSAMPLES] = {
	#include "kernel_singlefreqBP_852.dat"
	};
short kernel_941 [NSAMPLES] = {
	#include "kernel_singlefreqBP_941.dat"
	};
short kernel_1209[NSAMPLES] = {
	#include "kernel_singlefreqBP_1209.dat"
	};
short kernel_1336[NSAMPLES] = {
	#include "kernel_singlefreqBP_1336.dat"
	};
short kernel_1477[NSAMPLES] = {
	#include "kernel_singlefreqBP_1477.dat"
	};

extern fract16 proc_alg( fract16 *, fract16 *, int);
extern short filtro_fir(short *vet, short *coef_fir,short Num_Coef );
extern short atualiza_vet_entr(short *vet, short *coef_fir,short Num_Coef );


int main(int argc,char *argv[])
{
    cycle_stats_t stats;
    
	FILE *fin,*fout;
	fract16 entrada;
	fract16 saida;
	
	long long int k = 0;
	fract32 lastDigitIndex = 0;
	fract32 lastSavedDigitIndex = 1000;
	fract32 lastDigit = 0;
	
	fract16 y_697,y_770,y_852,y_941,y_1209,y_1336,y_1477;


  	fract16 Vet_entr[NSAMPLES] = {0};
  
	
	int i;
	
	CYCLES_INIT(stats);			// Inicializa a estrutura para a contagem de ciclos
	
	
	printf("***************************************************************\n");
	printf("* TESTE COM ARQUIVOS					           		      *\n");
	printf("*                                                             *\n");
	printf("***************************************************************\n");
	printf("\n");
	
	
	fin = fopen("..\\cel_gustavo.pcm","rb");
//	fin = fopen("..\\imp.pcm","rb");
    	if ((fin)==NULL)
  	{
    		printf("\nErro: nao abriu o arquivo de Entrada\n");
    		return 0;
  	}
    fout = fopen("..\\dtmf_out2.pcm","wb");
    	if ((fout)==NULL)
  	{
    		printf("\nErro: nao abriu o arquivo de Saida\n");
    		return 0;
  	}
  	

  	
  printf("Processando ...\n ");

  while (fread(&entrada,sizeof(short),1,fin) == 1) 
  {
		Vet_entr[0] = entrada;
		
		CYCLES_START(stats);	
		
		y_697  = filtro_fir(Vet_entr,kernel_697 ,NSAMPLES);
		y_770  = filtro_fir(Vet_entr,kernel_770 ,NSAMPLES);
		y_852  = filtro_fir(Vet_entr,kernel_852 ,NSAMPLES);
		y_941  = filtro_fir(Vet_entr,kernel_941 ,NSAMPLES);
		y_1209 = filtro_fir(Vet_entr,kernel_1209,NSAMPLES);
		y_1336 = filtro_fir(Vet_entr,kernel_1336,NSAMPLES);
		y_1477 = filtro_fir(Vet_entr,kernel_1477,NSAMPLES);
		
		if (y_697 > minAmplitude && y_1209 > minAmplitude){
			lastDigit = 1;
			lastDigitIndex = k;
		}else if (y_770 > minAmplitude && y_1209 > minAmplitude){
			lastDigit = 4;
			lastDigitIndex = k;
		}else if (y_852 > minAmplitude && y_1209 > minAmplitude){
			lastDigit = 7;
			lastDigitIndex = k;
		}else if (y_941 > minAmplitude && y_1209 > minAmplitude){
			lastDigit = '*';
			lastDigitIndex = k;
		}else if (y_697 > minAmplitude && y_1336 > minAmplitude){
			lastDigit = 2;
			lastDigitIndex = k;
		}else if (y_770 > minAmplitude && y_1336 > minAmplitude){
			lastDigit = 5;
			lastDigitIndex = k;
		}else if (y_852 > minAmplitude && y_1336 > minAmplitude){
			lastDigit = 8;
			lastDigitIndex = k;
		}else if (y_941 > minAmplitude && y_1336 > minAmplitude){
			lastDigit = 0;
			lastDigitIndex = k;
		}else if (y_697 > minAmplitude && y_1477 > minAmplitude){
			lastDigit = 3;
			lastDigitIndex = k;
		}else if (y_770 > minAmplitude && y_1477 > minAmplitude){
			lastDigit = 6;
			lastDigitIndex = k;
		}else if (y_852 > minAmplitude && y_1477 > minAmplitude){
			lastDigit = 9;
			lastDigitIndex = k;
		}else if (y_941 > minAmplitude && y_1477 > minAmplitude){
			lastDigit = '#';
			lastDigitIndex = k;
		}else if (((k-lastDigitIndex) > 15)&&((k-lastSavedDigitIndex) > 1050)){
			lastSavedDigitIndex = k;
			printf("%d",lastDigit);
		}

		k++;
		
		saida = atualiza_vet_entr(Vet_entr,Coefs,NSAMPLES);// Rotina em asm
		
		CYCLES_STOP(stats);	
		
		fwrite(&y_770,sizeof(short),1,fout);	
	
		
	}

    printf("\n\nterminado!\n");
		
    CYCLES_PRINT(stats);	
	
		fclose(fin);
		fclose(fout);
		
    return 0;
}


