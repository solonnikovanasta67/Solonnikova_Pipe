#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>

using namespace std;


struct Pipe
{
	string name;
	int d;
	double length;
	bool remont;
};



struct CS
{
	string name;
	int workshop;
	int activworkshop;
	int class_station;
};

void menu()
{
	cout << "1.Добавление трубы\n" << "2.Добавление КС\n" << "3.Просмотр объектов для трубы и КС\n" << "4.Редактирование трубы\n" << "5.Редактирование КС\n" << "6.Сохранение\n" << "7.Загрузка\n" << "0.Выход\n";
}


int VvodChisla()
{
	int number; cin >> number;
	while (cin.fail() || cin.peek() != '\n')
	{
			cin.clear();
			cin.ignore(100, '\n');

			cout << "Ошибка. Введите целое число: ";
			cin >> number;
	}
	return number;
}

void NewPipe(Pipe& p)
{
	cout << "Введите название трубы :";
	getline(cin >> ws, p.name);
	cout << "Введите диаметр трубы:";
	p.d = VvodChisla();
	while (p.d <= 0)
	{
		cout << "Ошибка, диаметр должен быть больше 0: ";
		p.d = VvodChisla();
	}
	
	cout << "Введите длину трубы: ";
	p.length = VvodChisla();

	while (p.length <= 0)
	{
		cout << "Ошибка, длина должна быть больше 0: ";
		p.length = VvodChisla();
	}

	int repair;
	cout << "Труба в ремонте? (1 - да, 0 - нет): ";
	repair = VvodChisla();
	while (repair != 0 && repair != 1)
	{
		cout << "Ошибка, введите 0 или 1: ";
		repair = VvodChisla();
	}
	p.remont = repair;
}


void NewCS(CS& s)
{
	cout << "Введите название КС :";
	getline(cin >> ws, s.name);
	cout << "Введите количество цехов:";
	s.workshop = VvodChisla();
	while (s.workshop <= 0)
	{
		cout << "Ошибка, количество цехов должно быть больше 0: ";
		s.workshop = VvodChisla();
	}
	cout << "Введите количество цехов в работе:";
	s.activworkshop = VvodChisla();
	while (s.activworkshop < 0 || s.activworkshop > s.workshop)
	{
		cout << "Ошибка, должно быть от 0 до " << s.workshop << ": ";
		s.activworkshop = VvodChisla();
	}
	cout << "Введите класс станции(6 или 7):";
	s.class_station = VvodChisla();
	while (s.class_station != 6 && s.class_station != 7)
	{
		cout << "Ошибка, класс станции должен быть 6 или 7: ";
		s.class_station = VvodChisla();
	}

}



void ShowPipe(const Pipe& p)
{
	if (p.name.empty()) 
		cout << "Труба не создана\n";
	else
		cout << "Труба: " << p.name << " | диаметр=" << p.d << " | длина=" << p.length
		<< " | в ремонте=" << (p.remont ? "да" : "нет") << "\n";
}



void ShowCS(const CS& s)
{
	if (s.name.empty()) 
		cout << "КС не создана\n";
	else
		cout << "КС: " << s.name << " | цехов=" << s.workshop << " | в работе=" << s.activworkshop
		<< " | класс=" << s.class_station << "\n";
}



void ShowAll(const Pipe& p, const CS& s)
{
	ShowPipe(p);
	ShowCS(s);
}



void EditPipe(Pipe& p)
{
	if (p.name.empty()) 
	{
		cout << "Труба не создана\n";
		return;
	}
	cout << "Труба в ремонте? (1 - да, 0 - нет): ";
	int repair = VvodChisla();
	while (repair != 0 && repair != 1)
	{
		cout << "Ошибка, введите 0 или 1: ";
		repair = VvodChisla();
	}
	p.remont = repair;
}


void EditCS(CS& s)
{
	if (s.workshop == 0)
	{
		cout << "КС не создана\n";
		return;
	}
	cout << "6 - запустить цех, 7 - остановить цех: ";
	int c = VvodChisla();

	if (c == 6)
	{
		if (s.activworkshop < s.workshop)  
		{
			s.activworkshop++;
			cout << "Цех запущен. В работе: " << s.activworkshop << " из " << s.workshop << "\n";
		}
		else
			cout << "Все цеха уже запущены\n";
	}
	else if (c == 7)
	{
		if (s.activworkshop > 0)  
		{
			
			s.activworkshop--;
			cout << "Цех остановлен. В работе: " << s.activworkshop << " из " << s.workshop << "\n";
		}
		else
			cout << "Все цеха уже остановлены\n";
	}
	else
		cout << "Некорректная команда\n";
}


void SavePipe(ofstream& fout, const Pipe& p)
{
	if (p.name.empty())
		fout << "Net" << "\n" << 0 << "\n" << 0 << "\n" << 0 << "\n";
	else
		fout << p.name << "\n" << p.d << "\n" << p.length << "\n" << p.remont << "\n";
}


void SaveCS(ofstream& fout, const CS& s)
{
	if (s.name.empty())
		fout << "Net" << "\n" << 0 << "\n" << 0 << "\n" << 0 << "\n";
	else
		fout << s.name << "\n" << s.workshop << "\n" << s.activworkshop << "\n" << s.class_station << "\n";
}

void SaveFile(const Pipe& p, const CS& s) {
	ofstream fout("f.txt"); 

	if (!fout) 
	{
		cout << "Ошибка открытия файла для записи\n";
		return;
	}

	SavePipe(fout, p);
	SaveCS(fout, s);

	cout << "Данные сохранены в f.txt\n";
}



void LoadPipe(ifstream& fin, Pipe& p)
{
	getline(fin >> ws, p.name);
	fin >> p.d >> p.length >> p.remont;
	if (p.name == "Net")
		p.name = "";
}

void LoadCS(ifstream& fin, CS& s)
{
	getline(fin >> ws, s.name);
	fin >> s.workshop >> s.activworkshop >> s.class_station;
	if (s.name == "Net")
		s.name = "";
}

void LoadFile(Pipe& p, CS& s)
{
	ifstream fin("f.txt");  
	if (!fin)  
	{
		cout << "Файл не найден\n";
		return;
	}

	LoadPipe(fin, p);
	LoadCS(fin, s);

	cout << "Данные загружены из f.txt\n";
}


int main()
{
	
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Russian");
	
	
	Pipe p{};
	CS s{};
	int choice;

	while (true)
	{
		menu();
		cout << "Выберите действие: ";
		choice = VvodChisla();

		while (choice < 0 || choice > 7)
		{
			cout << "Ошибка, введите число от 0 до 7: ";
			choice = VvodChisla();
		}
		switch (choice)
		{
		case 1: NewPipe(p); break;
		case 2: NewCS(s); break;
		case 3: ShowAll(p, s); break;
		case 4: EditPipe(p); break;
		case 5: EditCS(s); break;
		case 6: SaveFile(p, s); break;
		case 7: LoadFile(p, s); break;
		case 0:
			cout << "Выход из программы\n";
			return 0;
		}
	}
}
