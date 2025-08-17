#include "list_interface.h"
#include <stdio.h>

typedef struct {
	char name[50];
	int horse_powers;
	float zero_60_mph;
	int price_usd;
} car;

inline void print_car(void* data) {
	car* c = data;

	printf("%s | %d HP | 0-60: %.1f s | %d $\n", c->name, c->horse_powers, c->zero_60_mph, c->price_usd);
}

int price(const void* a, const void* b) {
	const car* ca = a;
	const car* cb = b;

	return (ca->price_usd - cb->price_usd);
}

int power(const void* a, const void* b) {
	const car* ca = a;
	const car* cb = b;

	return (ca->horse_powers - cb->horse_powers);
}

int speed(const void* a, const void* b) {
	const car* ca = a;
	const car* cb = b;

	if (ca->zero_60_mph < cb->zero_60_mph){
		return -1;
	}

	if (ca->zero_60_mph > cb->zero_60_mph){
		return 1;
	}

	return 0;
}

int main(void) {
	list* cars = list_create(sizeof(car));

	const car c1 = {"Dodge Challenger Hellcat", 797, 3.6f, 113000};
	const car c2 = {"Lada Vesta Sport", 105, 11.8f, 12000};
	const car c3 = {"Jeep Grand Cherokee Trackhawk", 1200, 2.3f, 97000};

	list_append(cars, &c1);
	list_append(cars, &c2);
	list_append(cars, &c3);

	list_foreach(cars, print_car);

	printf("\nSort by price\n");
	sort(cars, price);
	list_foreach(cars, print_car);

	printf("\nSort by power\n");
	sort(cars, power);
	list_foreach(cars, print_car);

	printf("\nSort by speed\n");
	sort(cars, speed);
	list_foreach(cars, print_car);

	list_destroy(cars);

	return 0;
}
