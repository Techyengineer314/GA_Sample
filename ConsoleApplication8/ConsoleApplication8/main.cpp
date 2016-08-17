#include<iostream>
#include<string>
#include<stdio.h>
#include<random>

////////////////////////////////////////////////////////////////////////////////////////
//This Sample Sets  out to attempt a basic Genetic Algorithm to determine the smallest//
//Sequence needed to form a provided target number                                    //
////////////////////////////////////////////////////////////////////////////////////////

bool digit(int a) {
	if (a <= 9) {
		return true;
	}
	return false;
}

bool op(int a) {
	if (a > 15) {
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

	int target = 42, answer = 0, currentop = 0, count = 0;
	double total = 0, current = 0, next = 0, nextop = 0;
	int operator1[8];
		for (int i = 0; i < 1000; i++)
			population[i] = distribution(genny); //chooses a completely random number between 0 and 0xFFFFFFFF
		for (int j = 0; j < 1000;j++) {
			current = 0;
			found_number = false;
			found_op = false;
			nextop = 0;
			next = 0;
			total = 0;
			count = 0;
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
						operator1[i] = 0xF;
					
					}
				}
			}
			//////////////////////////////////////////////////////////////////////
			//Beginning algorithm to check for total provided by random solution//
			//////////////////////////////////////////////////////////////////////
			/********************************************************************
			Algorithm uses following method to determine solution:
				Read through operator array until number is found and store that number in current
				Read through operator array until operation is found and store said operator in current op
				Read through until next number is found and perform that calculation with that number in next and store it in current num as well as total
				repeat till end of array
				handle special cases of no numbers or no operators or one number  and 7 operators with search function 		
				Also begin a check to determine if there are leading zeroes that should be ignored
			*********************************************************************/
			while (operator1[count] == 0) {
				count++;
				if (count == 8) {
					total = 0;
					break;
				}
				if (operator1[count] > 9 && count > 0) {
					current = 0;
					currentop = operator1[count];
					found_op = true;
				}
			}
			while (count < 7) {
				if (!(currentop == operator1[count])) {
					found_op = false;
				}
				while (!found_number) {
					if (count > 7) {
						break;
					}
					found_number = digit(operator1[count]);
					current = operator1[count];
					count++;
				}


				while (!found_op) {
					if (count > 7) {
						break;
					}
					found_op = op(operator1[count]);
					currentop = operator1[count];
					count++;
				}

				found_number = false;

				while (!found_number) {
					if (count > 7) {
						break;
					}
					found_number = digit(operator1[count]);
					next = operator1[count];
					count++;
				}
				if (total == 0) {
					total = current;
				}
				switch (currentop) {
				case 0x2A:
					total *= next;
					break;
				case 0x2B:
					total += next;
					break;
				case 0x2D:
					total -= next;
					break;
				case 0x2F:
					total /= next;
					break;
				default:
					total = total;
				}
			}
			if (target == total) {
				answer = population[j];
				break;
			}

			fitness[j] = 1.0 / (target - total);
			std::cout << "The fitness of: " << population[j] << " is: " << fitness[j] << '\n';
		}
	
		
	system("PAUSE");
	return 0;
}
