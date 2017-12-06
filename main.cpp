#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <fstream>
#include <random>
#include <functional>
#include <cstdlib>
#include <climits>
#include <cstdio>
#include "getch.h"
#include "colors.h"


using namespace std;

int record = INT_MAX;

int cont = 1;


void convert(auto a) {
	cout << GREEN << a/1000 << RESET << " s " << GREEN << a % 1000 << RESET << " ms ";
}

void timer() {
	int counter = 0;
	using namespace std::chrono_literals;
	while (cont) {
		cout << '\r';
		convert(counter);
		fflush(stdout);
		std::this_thread::sleep_for(10ms);
		counter += 10;
	}
	cout << '\n';
}

void insp() {
	int counter = 15;
	using namespace std::chrono_literals;
	while (counter >= 0 && cont) {
		cout << '\r';
		convert (counter * 1000);
		std::this_thread::sleep_for(1s);
		fflush(stdout);
		counter--;
	}
	cont = 0;
	cout << '\n';
}

void wait() {
	getch();
	cont = 0;
}

void scramble() {
	char moves[6] = {'F', 'B', 'L', 'R', 'U', 'D'};
	char dirs[2] = {'\0', '`'};
	char nos[2] = {'\0', '2'};
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(seed);
	uniform_int_distribution<int> distribution(0,5);
	auto move = bind(distribution, generator);
	default_random_engine gen2(seed);
	uniform_int_distribution<int> dist2(0,1);
	auto dir = bind(dist2, gen2);
	int m = move();
	int d = dir();
	int n = dir();
	cout << YELLOW << moves[m] << RESET;
	if (n == 0)
		cout << dirs[d];
	cout << ORANGE << nos[n] << RESET << ' ';
	for (int i = 0; i < 19; i++) {
		int t = move();
		while (t == m)
			t = move();
		m = t;
		d = dir();
		n = dir();
		cout << YELLOW << moves[m] << RESET;
		if (n == 0)
			cout << dirs[d];
		cout << ORANGE << nos[n] << RESET << ' ';

	}
	cout << '\n';
}

void solve() {
	system(CLEAR);
	cout << BGGREEN << YELLOW;
	cout << "      _   \n";
	cout << " _ __| |_ \n";
	cout << "| '__| __|\n";
	cout << "| |  | |_ \n"; 
	cout << "|_|   \\__|\n";
	cout << RESET;
	cout << LBLUE << "Welcome to rt!\n";
	cout << "Press:\n";
	cout << BOLD <<  "spacebar:" << RESET << LBLUE << " start the timer\n";
	cout << BOLD << "h:" << RESET  << LBLUE << " view solve history and statistics\n";
	cout << BOLD << "l:" << RESET  << LBLUE << " view solve log\n";
	cout << BOLD << "q:" << RESET << LBLUE << " to exit\n" << RESET;
	cout << LBLUE << "Random Scramble:\n" << RESET;
	scramble();
	char c;
	c = getch();
	if (c == 'q') {
		cout << "Goodbye!\n";
		return;
	} else if (c == 'h') {
		system(CLEAR);
		cout << BGBLUE << YELLOW;
		cout << "===================================================================\n";
		cout << "                          SOLVE STATISTICS                         \n";
		cout << "===================================================================\n";
		cout << RESET;

		ifstream ifile("save.txt", ios::in);
		if (!ifile) {
			cout << "No History or Unable to open history file.\n";
		}
		else {
			int c;
			vector<int> hist;
		
			while (ifile >> c) {
				hist.push_back(c);
			}
			int s = hist.size();
			int ao[100];
			int mx = max(hist[s-1], hist[s-2]);
			int mn = min(hist[s-1], hist[s-2]);
			int sum = 0;
			for (int i = s - 3; i >= 0 && s - i - 1 < 100; i--) {
				if (hist[i] > mx) {
					sum += mx;
					mx = hist[i];
				} 
				if (hist[i] < mn) {
					sum += mn;
					mn = hist[i];
				} 
				if (hist[i] >= mn && hist[i] <= mx) {
					sum += hist[i];
				}
				ao[s - i - 1] = sum/(s - i - 2);
			}
			if (s < 100) {
				for (int i = s; i < 100; i++) {
					ao[i] = ao[i-1];
				}
			}
			
			cout << BOLD << BLUE << "ao3: " << RESET;
			convert(ao[2]);
			cout << "| " << BOLD << BLUE << "ao5: " << RESET;
			convert(ao[4]);	
			cout << '\n';
			cout << BOLD << BLUE << "ao12: " << RESET;
			convert(ao[11]);
			cout << "| " << BOLD << BLUE << "ao50: " << RESET;
			convert(ao[49]);
			cout << "| " << BOLD << BLUE << "ao100: " << RESET;
			convert(ao[99]);
			cout << '\n';
			cout << BOLD << BLUE << "Best time: " << RESET;
			convert(mn);
			cout << "| " << BOLD << BLUE << "Worst time: " << RESET;
			convert(mx);
			cout << '\n';
			
			cout << BLUE << BOLD << "Progress Chart:\n" << RESET;
			char bar[20][25];
			int conversion = mx/20;
			int i,x;
			i = x = (s > 25)? s - 25 : 0;
			for (; i < s; i++) {
				int j;
				for (j = 0; j < hist[i]/conversion; j++) {
					bar[20 - j - 1][i] = '.';
				}
				for (; j < 20; j++) {
					bar[20 - j - 1][i] = ' ';
				}

			}
			cout << YELLOW;
			for (int i = 0; i < 20; i++) {
				for (int j = 0; j < s - x; j++) {
					if (bar[i][j] == '.') {
						cout << "\u25A0";
					} else {
						cout << bar[i][j];
					}
					cout << "  ";
				}
				cout << '\n';
			}
			cout << RESET << LBLUE;
			for (int i = 0; i < s - x; i++) {
				if (i+1 < 10)
					cout << i+1 << ' ';
				else
					cout << i+1;
				cout << ' ';
			}
			cout << RESET << '\n';
	
			ifile.close();
		}
		cout << "Press any key to return to the main menu.\n";
	
		getch(); 
	} else if (c == 'l') {
			system(CLEAR);
			cout << BGBLUE << YELLOW;
			cout << "===================================================================\n";
			cout << "                          SOLVE LOG                                \n";
			cout << "===================================================================\n";
			cout << RESET;
			ifstream ifile("save.txt", ios::in);
			if (!ifile) {
				cout << "No History or Unable to open history file.\n";
			}
			else {
				int c;
				vector<int> hist;
		
				while (ifile >> c) {
					hist.push_back(c);
				}
				int s = hist.size();
				for (int i = s - 1; i >= 0; i--) {
					cout << s - i << ". ";
					convert(hist[i]);
					cout << '\n';
				}
			}
		cout << "Press any key to return to the main menu.\n";
	
		getch();
	} else if (c == ' ') {
		cout << "Inspection\n";
		cont = 1;
		thread second(insp);
		thread third(wait);
		second.join();
		cout << "Time started.\n";
		cont = 1;

		auto a = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count();
		thread first(timer);

		c = getch();
		cont = 0;
		auto b = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count();

		first.join();


		cout << ORANGE << "Your time is: " << RESET;
		convert(b-a);
		cout << '\n'; 
		cout << LBLUE;
		cout << "Press:\n";
		cout << BOLD << "s: " << RESET << LBLUE << " to save the solve\n";
		cout << BOLD << "2: " << RESET << LBLUE << " to save the solve with +2 s\n";
		cout << BOLD << "u: " << RESET << LBLUE << " to not save.\n";
		
		c = getch();

		if (c == 's') {
			ofstream ofile("save.txt", ios::app);
			ofile << b - a << '\n';
			ofile.close();
			cout << "Saved!\n";
		}
		if (c == '2') {
			b += 2000;
			ofstream ofile("save.txt", ios::app);
			ofile << b - a << '\n';
			ofile.close();
			cout << "Saved!\n";
		}

		if (b - a < record) {
			record = b - a;
			cout << GREEN << BOLD;
			cout << "New Best Record!\n";
			cout << RESET;
		}
		cout << "Press any key to return to the main menu.\n";
		
		getch();
	}

	solve();
}

int main() {
	ifstream ifile("save.txt", ios::in);
	
	if (ifile) {
		int c;
		vector<int> hist;
		while (ifile >> c) {
			if (c < record)
				record = c;
		}
	}
	solve();
	return 0;
}