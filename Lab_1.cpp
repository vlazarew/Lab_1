// 10B: Добавить к каждому числу половину последнего отрицательного числа.

#define _CRT_Secure_no_warnings

#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include <iterator>
#include <deque>
#include <string>
#include <time.h>

using namespace std;

//Создание файла случайных целых чисел с помощью цикла
ofstream CreateRandomFile(string FileName, int N, int M)
{
	ofstream fs(FileName);
	srand((unsigned int)time(0));

	for (int i = 0; i < N; ++i)
	{
		int r = rand() % (2 * M + 1) - M;
		fs << r << " ";
	}
	fs.close();
	return fs;
}

//Создание файла случайных целых чисел с помощью generate
ofstream CreateRandomFileGenerate(string FileName, int N, int M)
{
	ofstream fs;
	fs.open(FileName);
	srand((unsigned int)time(0));
	generate_n(std::ostream_iterator<int>(fs, " "), N, [M]() {return rand() % (2 * M + 1) - M; });
	fs.close();
	return fs;
}

//Загрузка данных из файла
deque<double> LoadFromFile(ifstream &fin)
{
	deque<double> dec;
	if (!fin)
	{
		cout << "Файл не найден." << endl;
		return dec;
	}
	int a;
	if (!(fin >> a))
	{
		cout << "Неверный формат данных" << endl;
		return dec;
	}
	if (fin.eof())
	{
		cout << "Файл пуст." << endl;
	}
	else
	{
		while (!fin.eof())
		{
			dec.push_back(a);
			if (!(fin >> a) && !fin.eof())
			{
				cout << "Неверный формат данных" << endl;
				dec.clear();
				return dec;
			}
		}
	}
	return dec;
}

double FindLastNegative(deque<double>::iterator begin, deque<double>::iterator end)
{
	double last_negative = 0;
	for (deque<double>::iterator i = end - 1; i != begin - 1; i--)
	{
		if (*i < 0)
		{
			last_negative = *i;
			break;
		}
	}
	return last_negative;
}

//Преобразование дека (добаляю к каждому числу половину последнего отрицательного) с циклом for
deque<double> Modify(deque<double> &dec1)
{
	double last_negative = 0;
	deque<double> dec = dec1;
	int len = dec.size();
	for (int i = len - 1; i >= 0; i--)
	{
		if (dec[i] < 0)
		{
			last_negative = dec[i];
			break;
		}
	}

	double negative = last_negative / 2;
	for (double &i : dec)
	{
		i += negative;
	}
	return dec;
}

//Преобразование дека (добаляю к каждому числу половину последнего отрицательного) с итераторами
void Modify(deque<double>::iterator begin, deque<double>::iterator end)
{
	/*double last_negative = 0;
	for (deque<double>::iterator i = end - 1; i != begin - 1; i--)
	{
		if (*i < 0)
		{
			last_negative = *i;
			break;
		}
	}
	double negative = last_negative / 2;
	*/
	double negative = FindLastNegative(begin,end) / 2;
	for (deque<double>::iterator i = begin; i != end; i++)
	{
		*i += negative;
	}
}

//Преобразование дека (добаляю к каждому числу половину последнего отрицательного) с transform
void Modify_Transform(deque<double>::iterator begin, deque<double>::iterator end)
{
	/*double last_negative = 0;
	transform(begin, end, begin, [&last_negative](double i) {if (i < 0) last_negative = i; return i; });
	double negative = last_negative / 2;*/
	double negative = FindLastNegative(begin, end) / 2;
	transform(begin, end, begin, [negative](double i) {return i += negative; });
}

//Преобразование дека (добаляю к каждому числу половину последнего отрицательного) с for_each
void Modify_For_Each(deque<double>::iterator begin, deque<double>::iterator end)
{
	double last_negative = 0;
	for_each(begin, end, [&last_negative](double i) {if (i < 0) last_negative = i; });

	double negative = last_negative / 2;
	/*for (deque<double>::iterator i = begin; i != end; i++)
	{
		*i += negative;
	}*/
	for_each(begin, end, [negative](double &i) {i += negative; });
}

//Расчет суммы элементов
double Summ(deque<double> dec)
{
	double summ = 0;
	for_each(dec.begin(), dec.end(), [&summ](double i) {summ += i; });
	return summ;
}

//Расчет среднего арифметического
double Average(deque<double> &dec)
{
	return Summ(dec) / dec.size();
}

//Печать в файл
void Print(fstream &st, deque<double> &dec)
{
	for_each(dec.begin(), dec.end(), [&st](double i) {st << i << " "; });
}

//Вывод на экран (перегруженный метод)
void Print(ostream &st, deque<double> &dec)
{
	for_each(dec.begin(), dec.end(), [&st](double i) {st << i << " "; });
}

//Ввод параметра
int InputParametr(int min, int max)
{
	int n;
	do
	{
		cout << "Ваш выбор: " << endl;
		while ((!(cin >> n)) || cin.get() != '\n')
		{
			cout << "Неправильный ввод! Повторите снова." << endl;
			cin.clear();//сбрасывает все биты потока, то есть поток становится "хорошим"
			cin.sync();//удаляем данные из буфера
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Ваш выбор: " << endl;
		}
	} while ((n < min) || (n > max));
	return n;
}

//Ввод числа
int InputNumber(string str)
{
	int n;
	do {
		cout << str << endl;
		while ((!(cin >> n)) || cin.get() != '\n')
		{
			cout << "Неправильный ввод! Повторите снова." << endl;
			cin.clear();
			cin.sync();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << str << endl;
		}
	} while (n <= 0);
	return n;
}

//Меню
int Menu()
{
	cout << "\n";
	cout << "Меню" << endl;
	cout << "1 - Modify - цикл for" << endl;
	cout << "2 - Modify - iterator" << endl;
	cout << "3 - Modify - transform" << endl;
	cout << "4 - Modify - for_each" << endl;
	cout << "5 - Посчитать сумму" << endl;
	cout << "6 - Посчитать среднее арифметическое" << endl;
	cout << "7 - Вывод на экран" << endl;
	cout << "8 - Сохранить в файл" << endl;
	cout << "0 - Выход" << endl;
	int res = InputParametr(0, 8);
	cout << "\n";
	return res;
}

//Ввод имени файла
string InputFileName()
{
	string name;
	cout << "Введите имя файла: " << endl;
	cin >> name;
	return name;
}


int main()
{
	setlocale(LC_ALL, "Russian");
	bool enterfile = true;
	deque<double> dec;
	string FileName = "";
	int N, M, changemenu;
	ofstream ofs;
	ifstream ifs;
	cout << "--Меню--" << endl;
	cout << "1 - Заполнение текстового файла случайными числами (через цикл)" << endl;
	cout << "2 - Заполнение текстового файла случайными числами (через generate)" << endl;
	cout << "0 - Выход" << endl;
	changemenu = InputParametr(0, 2);
	switch (changemenu)
	{
	case 1:
		N = InputNumber("Введите количество элементов");
		M = InputNumber("Введите максимальный модуль чисел");
		FileName = InputFileName();
		ofs = CreateRandomFile(FileName, N, M);
		break;
	case 2:
		N = InputNumber("Введите количество элементов");
		M = InputNumber("Введите максимальный модуль чисел");
		FileName = InputFileName();
		ofs = CreateRandomFileGenerate(FileName, N, M);
		break;
	default:
		enterfile = false;
		break;
	}

	if (enterfile)
	{
		ifs.open(FileName);
		dec = LoadFromFile(ifs);
		ifs.close();
		for (;;)
		{
			int n = Menu();
			switch (n)
			{
			case 1:
				dec = Modify(dec);
				break;
			case 2:
				Modify(dec.begin(), dec.end());
				break;
			case 3:
				Modify_Transform(dec.begin(), dec.end());
				break;
			case 4:
				Modify_For_Each(dec.begin(), dec.end());
				break;
			case 5:
				cout << "Сумма: " << Summ(dec) << endl;
				break;
			case 6:
				cout << "Среднее арифметическое: " << Average(dec) << endl;
				break;
			case 7:
				Print(cout, dec);
				break;
			case 8:
				FileName = InputFileName();
				ofs.open(FileName);
				Print(ofs, dec);
				ofs.close();
				break;
			default:
				cout << "Выход" << endl;
				exit(0);
				break;
			}
		}
	}
	else
		cout << "Настало время прощаться" << endl;
	system("pause");
	return 0;
}
