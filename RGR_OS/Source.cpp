#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <shared_mutex>

std::string value = "";
std::shared_mutex mutex;
std::shared_mutex Mutex;

// Считывает из хранилища
void readValue(std::string& v) {
	mutex.lock_shared();

	// Имитация задержки
	std::this_thread::sleep_for(std::chrono::seconds(3));

	Mutex.lock();

	std::ifstream fin; fin.open("1.txt"); //читателям доступно все,
	getline(fin, value); fin.close(); //что есть в хранилище

	if (value != "")
		std::cout << "Reader " << std::this_thread::get_id() << " is reading " << value << "\n";
	else std::cout << "Reader " << std::this_thread::get_id() << " is reading nothing" << "\n";
	Mutex.unlock();
	v = value;
	mutex.unlock_shared();

}

// Передает то, что внес писатель
void setValue(std::string v) {
	mutex.lock();
	std::cout << "\nWriter " << std::this_thread::get_id() << " is here (NO READERS!)\n\n";
	std::this_thread::sleep_for(std::chrono::seconds(2));

	std::cout << "Writer " << std::this_thread::get_id() << " is writing " << v << "\n";

	std::ifstream fin; fin.open("1.txt");
	std::string s; getline(fin, s); fin.close();
	std::ofstream fout; fout.open("1.txt");
	fout << s << " " << v; fout.close();//теперь в хранилище больше "книг"

	value = v;
	mutex.unlock();

}

int main() {

	std::string read = ""; //что читает поток читателей
	std::ofstream f; f.open("1.txt");
	f.clear(); f.close();//очистка файла хранилища

	//читателям получают, что они будут читать
	//писатели передают значение
	std::thread tr1(readValue, std::ref(read));
	std::thread tw1(setValue, "a1 a2 a3");
	std::thread tr2(readValue, std::ref(read));
	std::thread tr3(readValue, std::ref(read));
	std::thread tr4(readValue, std::ref(read));
	std::thread tw2(setValue, "b1 b2");
	std::thread tr5(readValue, std::ref(read));
	std::thread tr6(readValue, std::ref(read));
	std::thread tr7(readValue, std::ref(read));
	std::thread tw3(setValue, "c1 c2 c3 c4");
	std::thread tr8(readValue, std::ref(read));
	std::thread tr9(readValue, std::ref(read));
	std::thread tr10(readValue, std::ref(read));

	tr1.join(); tr2.join(); tr3.join(); tr4.join(); tr5.join();//потоки
	tr6.join(); tr7.join(); tr8.join(); tr9.join(); tr10.join();//читателей

	tw1.join(); tw2.join(); tw3.join(); //потоки писателей


	system("pause");
	return 0;
}