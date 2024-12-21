#ifndef move_H
#define move_H
#include <defines.hpp>

/*
Função única para movimentação do robô.
Entrada: char referente ao sentido de cada motor e a velocidade dos mesmos
Possíveis char: "F" (frente) / "T" (trás)
Faixa de uint32_t: [0-255]
*/
void move(char SE, uint32_t VE, char SD, uint32_t VD) {
	/*
		Variáveis com final D são do motor direito e com final E são do esquerdo.
		S = sentido de rotação
		V = velocidade de rotação
	*/

	if (SD == 'T')
	{
		digitalWrite(a1, 0);
        digitalWrite(a2, 1);
		analogWrite(pwmA, VD); /*PARA A TB66 DEVEMOS DESCOMENTAR AQ E COLOCAR A VEL NO PWM 
        e colocar digitalWrite no b1 e b2 (analogo para todas as seções de movimentação)
        */
	}
	else 
	{
		digitalWrite(a1, 1);
        digitalWrite(a2, 0);
	    analogWrite(pwmA, VD);
	}

	if (SE == 'F')
	{
		digitalWrite(b1,0);
		digitalWrite(b2,1);
		analogWrite(pwmB, VE);
		
	}
	else 
	{
		digitalWrite(b1, 1);
		digitalWrite(b2, 0);
		analogWrite(pwmB, VE);
		
	}
}
#endif