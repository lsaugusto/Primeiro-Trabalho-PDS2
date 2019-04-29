#ifndef AGENDA_H
#define AGENDA_H
#include <iostream>
#include <string>
using namespace std; //bibliotecas padrao. ja explicadas em hpp

typedef struct Event{ //Struct para evento, definindo valores inteiros para hora, dia, mes. Evento aponta para o proximo evento
	int time;
	int day;
	int month;
	string title;
	struct Event * next;

	Event(int month, int day, int time, string title); //Ordem do evento
} Event;

typedef struct Day{ //Struct para o dia como inteiro
	int id;
	struct Event * event_list;

	Day(int id);
} Day;

typedef struct Month{ //Struct para mes
	int id;
	string name;
	int days;
	struct Day ** day_vector;
	struct Month * next;

	Month(int id, string name, int days); //Ordem do mes
	~Month();
} Month;

typedef struct Agenda{ //Struct para agenda, caracterizado por mes, e lista dos meses.
	int year;
	struct Month * month_list; //vetor do tipo mes, Numero do mes, string do mes, e inteiro para representar os dias, vetor com os dias e apontador para proximo mes.

	Agenda(int year);
	void InsertMonth(Month* new_month);
	void PrintAgenda();
	void InsertEvent(Event* event);
	Month* FindCurrentMonth(int m);
	void DeleteEvent(Event* event);
	Event* getEvent(int month, int day, int time);
	void ListMonthEvent(int month);
	bool IsValid(int month, int day, int hour);
	void SaveAgenda(string fileName);
	~Agenda();
} Agenda;

#endif
