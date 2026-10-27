#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;


struct CS
{
	static int maxId;
	int id;
	string name;
	int workshop;
	int activworkshop;
	int class_station;

	CS()
	{
		id = ++maxId;
		workshop = 0;
		activworkshop = 0;
		class_station = 0;
	}
};

int CS::maxId = 0;

void menu()
{
	cout << "1.Добавление трубы\n" << "2.Добавление КС\n" << "3.Просмотр объектов \n" << "4.Поиск и редактирование труб\n" << "5.Поиск и редактирование КС\n" << "6.Сохранение\n" << "7.Загрузка\n" << "0.Выход\n";
}

void Log(const string& text)
{
	ofstream flog("log.txt", ios::app);   
	flog << text << "\n";
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
	Log("Ввод: " + to_string(number));
	return number;
}

void NewPipe(Pipe& p)
{
	cin >> p;
}

void NewCS(CS& s)
{
	cout << "Введите название КС :";
	getline(cin >> ws, s.name);
	Log("Ввод названия: " + s.name); 
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
	if (p.get_name().empty())
		cout << "Труба не создана\n";
	else
		cout << "Труба id=" << p.get_id() << " | " << p.get_name() << " | диаметр=" << p.get_d() << " | длина=" << p.get_length()
		<< " | в ремонте=" << (p.get_remont() ? "да" : "нет") << "\n";
}



void ShowCS(const CS& s)
{
	if (s.name.empty())
		cout << "КС не создана\n";
	else
		cout << "КС id=" << s.id << " | " << s.name << " | цехов=" << s.workshop << " | в работе=" << s.activworkshop
		<< " | класс=" << s.class_station << "\n";
}

void ShowAll(const map<int, Pipe>& pipes, const map<int, CS>& stations)
{
	if (pipes.empty())
		cout << "Труб нет\n";
	for (const auto& item : pipes)
		ShowPipe(item.second);

	if (stations.empty())
		cout << "КС нет\n";
	for (const auto& item : stations)
		ShowCS(item.second);
}



// каждая функция возвращает id найденных объектов 

vector<int> FindPipesByName(const map<int, Pipe>& pipes, const string& query)
{
	vector<int> found;
	for (const auto& item : pipes)
		if (item.second.get_name().find(query) != string::npos)
			found.push_back(item.first);
	return found;
}

vector<int> FindPipesByRemont(const map<int, Pipe>& pipes, bool remont)
{
	vector<int> found;
	for (const auto& item : pipes)
		if (item.second.get_remont() == remont)
			found.push_back(item.first);
	return found;
}

vector<int> FindCSByName(const map<int, CS>& stations, const string& query)
{
	vector<int> found;
	for (const auto& item : stations)
		if (item.second.name.find(query) != string::npos)
			found.push_back(item.first);
	return found;
}

vector<int> FindCSByIdle(const map<int, CS>& stations, int minPercent)
{
	vector<int> found;
	for (const auto& item : stations)
	{
		const CS& s = item.second;
		double idle = (s.workshop - s.activworkshop) * 100.0 / s.workshop;   // процент незадействованных
		if (idle >= minPercent)
			found.push_back(item.first);
	}
	return found;
}

// все найденные или конкретные id 

vector<int> ChooseIds(const vector<int>& found)
{
	cout << "1 - все найденные, 2 - выбрать по id: ";
	int mode = VvodChisla();
	while (mode != 1 && mode != 2)
	{
		cout << "Ошибка, введите 1 или 2: ";
		mode = VvodChisla();
	}
	if (mode == 1)
		return found;

	vector<int> chosen;
	cout << "Вводите id по одному (0 - закончить):\n";
	while (true)
	{
		int id = VvodChisla();
		if (id == 0)
			break;
		if (find(found.begin(), found.end(), id) == found.end())
			cout << "Этого id нет среди найденных\n";
		else if (find(chosen.begin(), chosen.end(), id) != chosen.end())
			cout << "Этот id уже выбран\n";
		else
			chosen.push_back(id);
	}
	return chosen;
}


void SearchEditPipes(map<int, Pipe>& pipes)
{
	if (pipes.empty())
	{
		cout << "Труб нет\n";
		return;
	}
	cout << "Искать: 1 - по названию, 2 - в ремонте, 3 - не в ремонте: ";
	int mode = VvodChisla();
	while (mode < 1 || mode > 3)
	{
		cout << "Ошибка, введите от 1 до 3: ";
		mode = VvodChisla();
	}

	vector<int> found;
	if (mode == 1)
	{
		cout << "Введите название: ";
		string query;
		getline(cin >> ws, query);
		Log("Поиск трубы по названию: " + query);
		found = FindPipesByName(pipes, query);
	}
	else
		found = FindPipesByRemont(pipes, mode == 2);

	Log("Найдено труб: " + to_string(found.size()));
	if (found.empty())
	{
		cout << "Ничего не найдено\n";
		return;
	}
	for (int id : found)
		ShowPipe(pipes.at(id));

	vector<int> chosen = ChooseIds(found);
	if (chosen.empty())
		return;

	cout << "1 - поставить 'в ремонте', 2 - снять 'в ремонте', 3 - удалить: ";
	int action = VvodChisla();
	while (action < 1 || action > 3)
	{
		cout << "Ошибка, введите от 1 до 3: ";
		action = VvodChisla();
	}
	for (int id : chosen)
	{
		if (action == 3)
			pipes.erase(id);
		else
			pipes.at(id).set_remont(action == 1);
	}
	Log("Действие " + to_string(action) + " применено к трубам: " + to_string(chosen.size()) + " шт.");
}


void SearchEditCS(map<int, CS>& stations)
{
	if (stations.empty())
	{
		cout << "КС нет\n";
		return;
	}
	cout << "Искать: 1 - по названию, 2 - по проценту незадействованных цехов: ";
	int mode = VvodChisla();
	while (mode != 1 && mode != 2)
	{
		cout << "Ошибка, введите 1 или 2: ";
		mode = VvodChisla();
	}

	vector<int> found;
	if (mode == 1)
	{
		cout << "Введите название: ";
		string query;
		getline(cin >> ws, query);
		Log("Поиск КС по названию: " + query);
		found = FindCSByName(stations, query);
	}
	else
	{
		cout << "Минимальный процент незадействованных цехов (0-100): ";
		int percent = VvodChisla();
		while (percent < 0 || percent > 100)
		{
			cout << "Ошибка, введите от 0 до 100: ";
			percent = VvodChisla();
		}
		found = FindCSByIdle(stations, percent);
	}

	Log("Найдено КС: " + to_string(found.size()));
	if (found.empty())
	{
		cout << "Ничего не найдено\n";
		return;
	}
	for (int id : found)
		ShowCS(stations.at(id));

	vector<int> chosen = ChooseIds(found);
	if (chosen.empty())
		return;

	cout << "1 - запустить цех, 2 - остановить цех, 3 - удалить КС: ";
	int action = VvodChisla();
	while (action < 1 || action > 3)
	{
		cout << "Ошибка, введите от 1 до 3: ";
		action = VvodChisla();
	}
	for (int id : chosen)
	{
		if (action == 3)
			stations.erase(id);
		else
		{
			CS& s = stations.at(id);
			if (action == 1 && s.activworkshop < s.workshop)
				s.activworkshop++;
			else if (action == 2 && s.activworkshop > 0)
				s.activworkshop--;
		}
	}
	Log("Действие " + to_string(action) + " применено к КС: " + to_string(chosen.size()) + " шт.");
}


void SaveAll(const map<int, Pipe>& pipes, const map<int, CS>& stations)
{
	cout << "Введите имя файла: ";
	string filename;
	getline(cin >> ws, filename);
	Log("Сохранение в файл: " + filename);

	ofstream fout(filename);
	if (!fout)
	{
		cout << "Ошибка открытия файла для записи\n";
		return;
	}

	fout << pipes.size() << "\n";
	for (const auto& item : pipes)
	{
		const Pipe& p = item.second;
		fout << p.get_id() << "\n"
			<< p.get_name() << "\n"
			<< p.get_d() << "\n"
			<< p.get_length() << "\n"
			<< p.get_remont() << "\n";
	}

	fout << stations.size() << "\n";
	for (const auto& item : stations)
	{
		const CS& s = item.second;
		fout << s.get_id() << "\n"
			<< s.get_name() << "\n"
			<< s.get_workshop() << "\n"
			<< s.get_activworkshop() << "\n"
			<< s.get_class_station() << "\n";
	}

	cout << "Данные сохранены\n";
}

void LoadAll(map<int, Pipe>& pipes, map<int, CS>& stations)
{
	cout << "Введите имя файла: ";
	string filename;
	getline(cin >> ws, filename);
	Log("Загрузка из файла: " + filename);

	ifstream fin(filename);
	if (!fin)
	{
		cout << "Файл не найден\n";
		return;
	}

	pipes.clear();
	stations.clear();

	Pipe::reset_id();
	
	int count;
	fin >> count;
	for (int i = 0; i < count; i++)
	{
		Pipe p;
		p.load(fin); 
		pipes.emplace(p.get_id(), p); 
	}

	fin >> count;
	for (int i = 0; i < count; i++)
	{
		CS s;
		s.load(fin); 
		stations.emplace(s.get_id(), s); 
	}

	if (!fin)
	{
		pipes.clear();
		stations.clear();
		cout << "Ошибка чтения данных из файла\n";
		return;
	}
	cout << "Данные загружены\n";
}


int main()
{
	
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Russian");
	
	
	map<int, Pipe> pipes;       
	map<int, CS> stations;
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

		Log("Пункт меню: " + to_string(choice));

		switch (choice)
		{
		case 1: 
		{
			Pipe p;                      
			NewPipe(p);                  
			pipes.emplace(p.get_id(), p);
			break;
		}
		case 2: 
		{
			CS s;
			NewCS(s);
			stations.emplace(s.id, s);
			break;
		}
		case 3: ShowAll(pipes, stations); break;
		case 4: SearchEditPipes(pipes); break;
		case 5: SearchEditCS(stations); break;
		case 6: SaveAll(pipes, stations); break;
		case 7: LoadAll(pipes, stations); break;
		case 0:
			cout << "Выход из программы\n";
			return 0;
		}
	}
}
