
#include <iostream>

#include <string>

#include <fstream>

#include <windows.h>

using namespace std;

//Структура трубы

struct Pipe
{
	string name;
	int d;
	double length;
	bool remont;
};


//Структура КС

struct CS
{
	string name;
	int workshop;
	int activworkshop;
	int class_station;
};

//Создаем меню
void menu()
{
	cout << "1.Добавление трубы\n" << "2.Добавление КС\n" << "3.Просмотр объектов для трубы и КС\n" << "4.Редактирование трубы\n" << "5.Редактирование КС\n" << "6.Сохранение\n" << "7.Загрузка\n" << "0.Выход\n";
}

//Защита от некорректного ввода 

int VvodChisla()
{
	int chislo;
	cin >> chislo;
	while (cin.fail())
	{
		cin.clear();
		cin.ignore(10000000, '\n');
		cout << "Ошибка, введите число: ";
		cin >> chislo;
	}
	return chislo;
}

//Ошибки (труба)
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

	cout << "Введите длину трубы :";
	cin >> p.length;
	while (cin.fail() || p.length <= 0)
	{
		cin.clear();
		cin.ignore(10000000, '\n');
		cout << "Ошибка, длина должна быть больше 0: ";
		cin >> p.length;
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

//Ошибки (КС)

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

//Просмотр объектов для трубы и КС

void ShowAll(const Pipe& p, const CS& s)
{
	if (p.name.empty()) //Пусто
		cout << "Труба не создана\n";
	else
		cout << "Труба: " << p.name << " | диаметр=" << p.d << " | длина=" << p.length
		<< " | в ремонте=" << (p.remont ? "да" : "нет") << "\n";

	if (s.name.empty()) //Пусто
		cout << "КС не создана\n";
	else
		cout << "КС: " << s.name << " | цехов=" << s.workshop << " | в работе=" << s.activworkshop
		<< " | класс=" << s.class_station << "\n";
}

// Редактирование признака "в ремонте" для трубы

void EditPipe(Pipe& p)
{
	if (p.name.empty()) //Пусто
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

// Запуск и останов цеха в КС

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
		if (s.activworkshop < s.workshop)  // Проверка, есть ли свободные цеха
		{
			// Запускаем ещё один
			s.activworkshop++;
			cout << "Цех запущен. В работе: " << s.activworkshop << " из " << s.workshop << "\n";
		}
		else
			cout << "Все цеха уже запущены\n";
	}
	else if (c == 7)
	{
		if (s.activworkshop > 0)  // Проверка, есть хоть один работающий
		{
			// Останавливаем
			s.activworkshop--;
			cout << "Цех остановлен. В работе: " << s.activworkshop << " из " << s.workshop << "\n";
		}
		else
			cout << "Все цеха уже остановлены\n";
	}
	else
		cout << "Некорректная команда\n";
}

//Сохранение в файл 

void SaveFile(const Pipe& p, const CS& s)
{
	ofstream fout("f.txt"); //Вывод в файл

	if (!fout) //Если файл не открылся
	{
		cout << "Ошибка открытия файла для записи\n";
		return;
	}

	fout << p.name << "\n" << p.d << "\n" << p.length << "\n" << p.remont << "\n";
	fout << s.name << "\n" << s.workshop << "\n" << s.activworkshop << "\n" << s.class_station << "\n";

	fout.close();
	cout << "Данные сохранены в f.txt\n";
}

//Загрузка данных из файла

void LoadFile(Pipe& p, CS& s)
{
	ifstream fin ("f.txt");  //Чтение файла
	if (!fin)  //Если файл не открылся

	{
		cout << "Файл не найден\n";
		return;
	}
	
	getline(fin >> ws, p.name);
	fin >> p.name >> p.d >> p.length >> p.remont;
	
	getline(fin >> ws, s.name);
	fin >> s.name >> s.workshop >> s.activworkshop >> s.class_station;

	fin.close();
	cout << "Данные загружены из f.txt\n";
}

//Добавление объектов 

int main()
{
	//Поддержка русского при вводе, выводе
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Russian");
	
	
	Pipe p;
	CS s;
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
