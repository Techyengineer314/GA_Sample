#include<iostream>
#include<string>
#include<stdio.h>
#include<random>
#include<time.h>

////////////////////////////////////////////////////////////////////////////////////////
//This Sample Sets  out to attempt a basic Genetic Algorithm to determine the smallest//
//Sequence needed to form a provided target number                                    //
////////////////////////////////////////////////////////////////////////////////////////

int power(int x, int y) {
	int ans = 1;
	for (int i = 0; i < y; i++) 
		ans *= x;
	
	return ans;
}

int swap_bits(int integer1, int integer2, int index) {
	//swaps bits at the specified index and returns integer 1 swapped
	int ans = 0, mask1, mask2, xor_mask = 0, and_mask = 0xFFFFFFFF;
	for (int i = 0; i <= index; i++) {
		mask1 = integer1 & power(2, i);
		mask2 = integer2 & power(2, i);
		if (mask2 != mask1) {
			xor_mask += power(2, i);
		}
	}
	ans = integer1 ^ xor_mask;
	return ans;
}

int high_search(double *a, int n) {
	//finds the highest number in the specified 100 element array
	double ans = 0;
	int index = 0;
	for (int i = 0; i < n; i++) {
		if (a[i] > ans) {
			index = i;
			ans = a[i];
		}
	}
	return index;
}

int next_high_search(double *a, int index, int n) {
	//finds the second highest element in the specified 100 element array with the index of the highest element provided
	double ans = 0;
	int index1 = 0;
	for (int i = 0; i < n; i++) {
		if (a[i] > ans && i != index) {
			index1 = i;
			ans = a[i];
		}
	}
	return index1;
}

bool digit(int a) {
	//sees if the integer is within the 0-9 range or not
	if (a <= 9) {
		return true;
	}
	return false;
}

bool op(int a) {
	//sees if the operator is one of the desired operators or not
	if (a > 20) {
		return true;
	}
	return false;
}


int main() {
	srand(time(NULL));
	std::default_random_engine genny; //seeds the RNG to the time of execution
	genny.seed(time(NULL));
	std::uniform_int_distribution<int> distribution(0, 0x0FFFFFFF);
	double fitness[50];
	unsigned int population[50]; //sets up the array to store population
	//expecting somewhere between 100 - 1000 children per generation
	bool found_number = false, found_op = false, answer_found = false;

	//////////////////////////////////////////////////////////////////////////
	//taarget number and other various declarations
	/////////////////////////////////////////////////////////////////////////

	int target = 50;



	int answer = 0, currentop = 0, count = 0, generation = 0;
	double total = 0, current = 0, next = 0, nextop = 0;
	int operator1[8];
		for (int i = 0; i < 50; i++)
			population[i] = distribution(genny); //chooses a completely random number between 0 and 0xFFFFFFFF
		for (int j = 0; j < 50;j++) {
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
					switch (operator1[i]) {
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
						operator1[i] = 0x20;
					}
				}
			}
			char a;
			for (int i = 0; i < 8;i++) {
				if (operator1[i] > 9) {
					a = operator1[i];
					std::cout << a << ' ';
				}
				else {
					std::cout << operator1[i] << ' ';
				}
				
			}
			std::cout << '\n';
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
				if (operator1[count] > 0x29 && count > 0) {
					current = 0;
					found_number = true;
					currentop = operator1[count];
					found_op = true;
				}
			}
			while (count < 7) {
				next = 20;
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
					if (found_number) {
						next = operator1[count];
					}
					count++;
				}
				if (total == 0) {
					total = current;
				}
				if (count > 7 && next == 20)
					break;
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
			std::cout << "The total is: " << total << '\n';
			std::cout << "At population number: " << j << "\nGeneration number: " << generation << '\n';
			if (target == total) {
				answer = population[j];
				std::cout << "An answer is: " << answer << '\n';
				answer_found = true;
				break;
			}
			fitness[j] = 1.0 / (target - total);
			std::cout << "The fitness of: " << population[j] << " is: " << fitness[j] << '\n';
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//beginning mutation algorithm will take two best fit numbers and crossover at a random point and  //
		//then will begin flipping bits at random														   //
		/////////////////////////////////////////////////////////////////////////////////////////////////////
	
		std::uniform_int_distribution<int> distribution_crossover(0, 30);
		std::uniform_int_distribution<int> distribution_mutation(0, 0xFFFFFF);
		int index,index2,parent1,parent2,index3,parent_choice = 0,count1 = 2,xor_mask = 0;
		while (!answer_found) {
			genny.seed(time(NULL));//randomizes seed every new generation to randomize more
			generation++;
			count1 = 2;
			xor_mask = 0;
			index = high_search(fitness,50);
			index2 = next_high_search(fitness, index, 50);
			parent1 = population[index];
			parent2 = population[index2];
			index3 = distribution_crossover(genny);//choosing random index for crossover
			population[0] = swap_bits(parent1, parent2, index3);//performing crossover
			population[1] = swap_bits(parent2, parent1, index3);


			while (count != 100) { //rebuilding population
				parent_choice = rand() % 2;
				xor_mask = distribution_mutation(genny);
				population[count] = xor_mask ^ population[parent_choice];
				count++;
			}
			for (int j = 0; j < 50;j++) {
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
						switch (operator1[i]) {
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
							operator1[i] = 0x20;
						}
					}
				}
				char a;
				for (int i = 0; i < 8;i++) {
					if (operator1[i] > 9) {
						a = operator1[i];
						std::cout << a << ' ';
					}
					else {
						std::cout << operator1[i] << ' ';
					}

				}
				std::cout << '\n';
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
					if (operator1[count] > 0x29 && count > 0) {
						current = 0;
						found_number = true;
						currentop = operator1[count];
						found_op = true;
					}
				}
				while (count < 7) {
					next = 20;
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
						if (found_number) {
							next = operator1[count];
						}
						count++;
					}
					if (total == 0) {
						total = current;
					}
					if (count > 7 && next == 20)
						break;
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
				std::cout << "The total is: " << total << '\n';
				std::cout << "At population number: " << j << "\nGeneration number: " << generation << '\n';
				if (target == total) {
					answer = population[j];
					std::cout << "An answer is: " << answer << '\n';
					answer_found = true;
					break;
				}
				fitness[j] = 1.0 / (target - total);
				std::cout << "The fitness of: " << population[j] << " is: " << fitness[j] << '\n';
			}
		}
		
	system("PAUSE");
	return 0;
}
