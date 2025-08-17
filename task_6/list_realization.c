#include "list_interface.h"
#include <stdlib.h>
#include <string.h>

list* list_create(size_t const element_size) {
	list* list = malloc(sizeof(*list)); // выделяем память под структуру
	list->head = NULL;									// при создании изначально голова пустая
	list->element_size = element_size;	// устанавливаем размер
	return list;												// возвращаем указатель на созданный список
}

void list_append(list* list, const void* element) {
	list_node* node = malloc(sizeof(list_node)); // указатель на новый узел списка

	node->data = malloc(list->element_size); // выделяем память под данные узла

	memcpy(node->data, element, list->element_size); // копируем данные из переданного элемента в узел списка

	node->next = NULL; // устанавливаем указатель на следующий узел в NULL, т.к. это будет последний узел в списке

	if (!list->head) { // если список пуст, то новый узел становится головой списка
		list->head = node;
	}
	else { // иначе находим последний узел в списке и добавляем новый узел в конец
		list_node* current = list->head;

		while (current->next) { // пока есть следующий узел, двигаемся по списку
			current = current->next;
		}

		current->next = node; // устанавливаем указатель на новый узел как следующий для последнего узла
	}
}

void list_foreach(const list* list, void (*func)(void*)) { // применяем функцию к каждому элементу списка
	const list_node* current = list->head;									 // начинаем с головы списка

	while (current) {					 // пока текущий узел не NULL
		func(current->data);		 // применяем функцию к данным текущего узла
		current = current->next; // переходим к следующему узлу
	}
}

void sort(list* list, int (*cmp)(const void*, const void*)) { // сортировка списка по заданному критерию
	if (!list || !list->head || !list->head->next) {						// если список пуст или содержит только один элемент, то сортировка не требуется
		return;
	}

	int swapped; // флаг, указывающий, были ли произведены обмены элементов в текущей итерации

	do {
		swapped = 0; 

		list_node* prev = NULL;
		list_node* curr = list->head;
		list_node* next = curr->next;

		while (next) {
			if (cmp(curr->data, next->data) > 0) {
				void* tmp = curr->data;
				curr->data = next->data;
				next->data = tmp;
				swapped = 1;
			}

			curr = next;
			next = next->next;
		}
	}
	while (swapped);
}

void list_destroy(list* list) {		 // освобождение памяти, занятой списком
	list_node* current = list->head; // начинаем с головы списка

	while (current) {									 // пока текущий узел не NULL
		list_node* next = current->next; // сохраняем указатель на следующий узел

		free(current->data); // освобождаем память, занятую данными узла
		free(current);			 // освобождаем память, занятую самим узлом
		current = next;			 // переходим к следующему узлу
	}

	free(list); // освобождаем память, занятую списком
}