#include<iostream>
#include<string>
#include<stdio.h>
#include<random>

////////////////////////////////////////////////////////////////////////////////////////
//This Sample Sets  out to attempt a basic Genetic Algorithm to determine the smallest//
//Sequence needed to form a provided target number                                    //
////////////////////////////////////////////////////////////////////////////////////////

bool digit(int a) {
	if (a < 9) {
		return true;
	}
	return false;
}


int main() {
	std::default_random_engine genny; //seeds the RNG to the time of execution
	std::uniform_int_distribution<int> distribution(0, 0x0FFFFFFF);
	double fitness[1000];
	unsigned int population[1000]; //sets up the array to store population
	//expecting somewhere between 100 - 1000 children per generation
	bool found_number = false, found_op = false;

	int target = 42, current = 0, nextop = 0, total = 0, answer = 0, next = 0, currentop = 0, count = 0;
	int operator1[8];
	while (!answer) {
		for (int i = 0; i < 1000; i++)
			population[i] = distribution(genny); //chooses a completely random number between 0 and 0xFFFFFFFF
		for (int j = 0; j < 1000;j++) {
			current = 0;
			found_number = false;
			found_op = false;
			nextop = 0;
			next = 0;
			total = 0;
			for (int i = 7; i >= 0;i--) {
				operator1[i] = 0xF & (population[j] >> ((7 - i) * 4));
				if (operator1[i] > 9) {
					switch(operator1[i]) {
					case 10:
						operator1[i] = 0x2B;
						break;
					case 11:
						operator1[i] = 0x2D;
						break;
					case 12:
						operator1[i] = 0x2A;
						break;
					case 13:
						operator1[i] = 0x2F;
						break;
					default:
						operator1[i] = 0;
					
					}
				}
			}
			//////////////////////////////////////////////////////////////////////
			//Beginning algorithm to check for total provided by random solution//
			//////////////////////////////////////////////////////////////////////
			/********************************************************************
			Algorithm uses following method to determine solution:
				Read through operator array until number is found and store that number in current num
				Read through operator array until operation is found and store said operator in current op
				Read through until next number is found and perform that calculation with that number in next num and store it in current num as well as total
				repeat till end of array
				handle special cases of no numbers or no operators or one number  and 7 operators with search function 							
			*********************************************************************/

			if (target == total) {
				answer = population[j];
				break;
			}
			fitness[j] = 1 / (target - total);
		}
	}
		
	system("PAUSE");
	return 0;
}
