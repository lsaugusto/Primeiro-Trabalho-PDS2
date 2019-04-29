#include <iostream> //biblioteca padrao, cout, cin
#include <fstream> //lidar com arquivos
#include <string> //biblioteca de string
#include "Agenda.hpp" //Inclui arquivo que foi criado com interface
using namespace std; //Para evitar STD em cin, cout, Deixar o codigo mais claro

bool is_file_exist(const string fileName){ //Checa se existe o arquivo com o nome desejado
    ifstream infile(fileName);
    return infile.good();
}

void WriteMonths(string fileName){ //Recebe nome do arquivo e abre de acordo com o mes. Dentro dela foi criado vetor com cada mes do ano.
	string months[12] = {"January", "February", "March", "April", "May", "June", "Jully", "August", "September", "October", "November", "December"};
	ofstream file;
	file.open(fileName);
	for (int i = 0; i < 12; ++i) file << "- " << months[i] << endl;
	file.close();
}

void ReadAgenda(string fileName, Agenda* agenda){ //Funcao para receber a agenda, e o nome do arquivo.
	Event * event;

	ifstream file; //abertura do arquivo
	file.open(fileName);

	string title, day, time, month;
	char symbol;

	while(!file.eof()){ //funcao para ler a agenda, percorre cada mes. Caso houver evento, le o *, se possuir apenas -, passa pelo mes.
		file >> symbol;
		if(symbol == '-') file >> month;
		else if(symbol == '*'){
			file >> day >> time;
			getline(file, title);
			event = new Event(stoi(month), stoi(day), stoi(time), title);
			agenda->InsertEvent(event);
		}
	}
}

int main(){ //Imprime para o usuario as opçoes de abrir agenda ou criar uma nova (se o usuario entrar com outra opcao, avisa opcao invalida)
	cout << "------- AGENDA -------" << endl;
	cout << "1) Create new Agenda" << endl; //cria nova agenda
	cout << "2) Open existent Agenda" << endl; //abre agenda ja existente

	bool flag = false;
	int choice, c_year;
	cin >> choice;
	if(choice == 1){
		cout << "For which year? ";
		cin >> c_year;

		string fileName = "Agenda" + to_string(c_year) + ".txt";

		if(is_file_exist(fileName)){
			cout << endl << "This agenda already exists. Do you wanna open it?" << endl;
			cout << "1) Yes" << endl;
			cout << "2) No" << endl;
			cin >> choice;
			if(choice == 2){
				cout << "Goodbye..." << endl;
				return 0;
			}
		}
		else {
			cout << "Creating a new agenda for " << c_year << "..." << endl;
  			WriteMonths(fileName);
			cout << "Agenda successfully created!" << endl;
  		}

	} else if(choice == 2) {
		cout << "Which year? ";
		cin >> c_year;

		string fileName = "Agenda" + to_string(c_year) + ".txt";
		// caso em que nao existe uma agenda com o nome em questao
		if(!is_file_exist(fileName)){
			cout << endl << "This agenda doesn't exist. Do you wanna create it?" << endl;
			cout << "1) Yes" << endl;
			cout << "2) No" << endl;
			cin >> choice;
			if(choice == 2){
				cout << "Goodbye..." << endl;
				return 0;
			}
			else {
				cout << "Creating a new agenda for " << c_year << "..." << endl;
				WriteMonths(fileName);
				cout << "Agenda successfully created!" << endl;
			}
		// caso em que ja existe uma agenda com o nome em questao
		} else flag = true;

	} else {
		cout << "Invalid Option.\nGoodbye..." << endl;
		return 0;
	}

	Agenda* agenda = new Agenda(c_year);
	// a flag representa o caso em que ja existia uma agenda e que sera necessario la-la, apenas
	if(flag){
		string fileName = "Agenda" + to_string(c_year) + ".txt";
		ReadAgenda(fileName, agenda);
	}

	do{
		// a estrutura do-while permitira mostrar essas mensagens e executar as acoes enquanto
		// o usuario desejar (ate ele querer fechar a agenda)
		cout << endl << "############ " << c_year << " ############" << endl; //funcao do, while - Abre as opcoes para o usuario de acordo com o que ele deseja.
		cout << "1) View all events" << endl;
		cout << "2) View events by month" << endl;
		cout << "3) Create a new event" << endl;
		cout << "4) Remove an event" << endl;
		cout << "5) Check if an event exists" << endl;
		cout << "6) Save and exit" << endl;
		cout << "##############################" << endl;

		cin >> choice;
		switch(choice){
			case 1:{ //Caso queira ver todos os eventos do ano, o codigo printara na tela os eventos. (abre toda a agenda)
				agenda->PrintAgenda();
				break;
			}
			case 2:{ //Abre os eventos de acordo com o mes selecionado
				int month;
				do{
					cout << "Which month? (tap the number) ";
					cin >> month;
					if(month < 1 || month > 12) cout << "Invalid month number" << endl;
				} while(month < 1 || month > 12);
				agenda->ListMonthEvent(month);
				break;
			}
			case 3:{ //Cria um novo evento, composto por nome, mes, dia, hora. Se os dados forem validos,
				int month, day, time;
				string title;
				cout << "Event title: ";
				cin.ignore();
				getline(cin, title);
				cout << "Which month? (tap the number) ";
				cin >> month;
				cout << "Which day? ";
				cin >> day;
				cout << "Which time? ";
				cin >> time;
				if(agenda->IsValid(month, day, time)){ //Funcao verifica se o mes e valido, dia e hora, se for valido, insere, caso contrario, notifica que evento ja existe.
					if(agenda->getEvent(month, day, time)){
						cout << "There's another event already." << endl;
						break;
					}
					Event * event = new Event(month, day, time, title);
					agenda->InsertEvent(event);
					cout << "Event successfully inserted!" << endl;
				}
				else cout << "Invalid data!" << endl;
				break;
			}
			case 4:{ //remove um evento selecionado pelo usuario, criad similar ao inserir, mas verifica apenas mes, dia e horario (se os parametros tireverem ok, deleta o evento)
				int month, day, time;
				cout << "Which month? (tap the number) ";
				cin >> month;
				cout << "Which day? ";
				cin >> day;
				cout << "Which time? ";
				cin >> time;
				if(!agenda->IsValid(month, day, time)){
					cout << "Invalid data!" << endl;
				} else {
					Event* ev = agenda->getEvent(month, day, time);
					if(ev){
						agenda->DeleteEvent(ev);
						cout << "Event successfully deleted!" << endl;
					}
					else cout << "The event you're trying to delete doesn't exist!" << endl; //Pausa caso o evento que a pessoa esteja querendo remover nao existir.
				}
				break;
			}
			case 5:{ //Verifica se o evento ja existe na data, Verifica mes, dia e hora (Se for valido, retorna o evento, caso nao exista avisa ao usuario)
				int month, day, time;
				cout << "Which month? (tap the number) ";
				cin >> month;
				cout << "Which day? ";
				cin >> day;
				cout << "Which time? ";
				cin >> time;
				if(!agenda->IsValid(month, day, time)){
					cout << "Invalid data!" << endl;
				} else {
					Event* ev = agenda->getEvent(month, day, time);
					if(ev){
						cout << "You have an event scheduled!" << endl;
						cout << "\" " << ev->title << " \"" << endl;
					}
					else cout << "There isn't any event scheduled." << endl;
				}
				break;
			}
			case 6:{ //Salva modificacoes da agenda e fecha.
				string fileName = "Agenda" + to_string(c_year) + ".txt";
				agenda->SaveAgenda(fileName);
				cout << "Saving agenda..." << endl;
				cout << "Successfully closed!" << endl;
				cout << "Goodbye..." << endl;

				break;
			}
			default:{
				cout << "Invalid Option.\nGoodbye..." << endl;
				return 0;
			}
		}
	} while (choice != 6);
	// liberando a memoria
	delete(agenda);
	return 0;
}
