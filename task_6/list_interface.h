#include <stddef.h>

typedef struct list_node{
	void* data;							// указатель на данные элемента
	struct list_node* next; // указатель на следующий элемент списка
} list_node;

typedef struct list {
	list_node* head;		 // указатель на первый элемент списка
	size_t element_size; // размер одного элемента списка
} list;

list* list_create(size_t const element_size);								 // создание списка
void list_append(list* list, const void* element);					 // добавление эдемента
void list_foreach(const list* list, void (*func)(void*));		 // применение функции к каждому элементу списка
void sort(list* list, int (*cmp)(const void*, const void*)); // сортировка списка по заданному критерию
void list_destroy(list* list);															 // освобождение памяти, занятой списком
