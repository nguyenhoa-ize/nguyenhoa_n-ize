#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <cstdlib> 
#include <ctime> 

using namespace std;
const int dx[] = {0, 0, -1, 1};
const int dy[] = {-1, 1, 0, 0};

struct Maze
{
	int value;
	bool visited;
};
struct Robot 
{
	int x;
	int y;
	int score;
};

void gotoxy(int x, int y) {
    printf("\033[%d;%dH", y, x);
}

int welcome_game() {
	system("cls");
	cout << "Chao mung den voi Me cung so!" << endl;
	cout << "1. Bat dau tro choi" << endl;
	cout << "0. Thoat khoi tro choi" << endl;
	cout << "Lua chon cua ban: ";
	int option;
	cin >> option;
	return option;
}

int mode_game()
{
	system("cls");
	cout << "Chon che do choi :" << endl;
    cout << "1. Mot nguoi choi" << endl;
    cout << "2. Hai nguoi choi" << endl;
    cout << "0. Quay lai" << endl;
    cout << "Lua chon cua ban la: ";
    int option;
    cin >> option;
    return option;
}

void read_maze(Maze** &maze, int& rows, int& cols) {
	string file_name;
	cout << "Nhap ten file chua me cung (*.txt): ";
	cin.ignore();
	getline(cin, file_name);
    ifstream inputfile(file_name);
    if (inputfile.is_open()) {
        inputfile >> rows >> cols;
        maze = new Maze*[rows];
        for (int i = 0; i < rows; ++i) {
            maze[i] = new Maze[cols];
            for (int j = 0; j < cols; ++j){
                inputfile >> maze[i][j].value;
                maze[i][j].visited = false;
            }
        };
        inputfile.close();
    }
    else {
        cout << "Khong mo duoc file" << endl;
    }
}

void create_random_maze(Maze** &maze, int &rows, int &cols) {
    srand(time(NULL));
    cout << "Nhap so hang: ";
    cin >> rows;
    cout << "Nhap so cot: ";
    cin >> cols;

    maze = new Maze*[rows];
    for (int i = 0; i < rows; ++i)
        maze[i] = new Maze[cols];

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
        	bool exists;
        	int num;
        	do {
                num = rand() % 100 + 1;
                exists = false;

                for (int k = 0; k < i; k++) {
                    for (int l = 0; l < j; l++) {
                        if (maze[k][l].value == num) {
                            exists = true;
                            break;
                        }
                    }
                }
            } while (exists);
            maze[i][j].value = num;
            maze[i][j].visited = false;
        }
    }
}

int length(int n) {
  int len = 0;
  while (n > 0) {
    len++;
    n /= 10;
  }
  return len;
}

void reset_maze(Maze** &maze, int rows, int cols ) {
	for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            maze[i][j].visited = false;
}

void visualize(Maze **maze, int rows, int cols, char mark = 'X') {

	int max_width = 0;
	for (int i = 0; i < rows; i++) {
    	for (int j = 0; j < cols; j++) {
			int width = length(maze[i][j].value);
			if (width > max_width) {
				max_width = width;
			}
		}
	}

	// Ve hang ngang tren cung cua bang
	cout << "+";
	for (int j = 0; j < cols; j++) {
    	for (int k = 0; k < max_width; k++) {
			cout << "-";
    	}
    	cout << "+";
	}
	cout << endl;

	// Ve cac hang con lai cua bang
	for (int i = 0; i < rows; i++) {
		cout << "|";
		for (int j = 0; j < cols; j++) {
    		if (maze[i][j].visited)
        		cout << setw(max_width) << mark;
			else 
				cout << setw(max_width) << maze[i][j].value;
        	cout << "|";
    	}
    	cout << endl;

    	// Ve hang ngang phan cach giua cac hang
    	cout << "+";
    	for (int j = 0; j < cols; j++) {
			for (int k = 0; k < max_width; k++) {
				cout << "-";
			}
			cout << "+";	
		}
		cout << endl;
	}
}

bool is_valid_move(Maze** maze, int rows, int cols, int x, int y) {
    if (x < 0 || x > rows - 1 || y < 0 || y > cols - 1)
        return false;
    if (maze[x][y].visited)
        return false;
    return true;
}


void push_element(Robot *&path_robot, Robot new_element, int& size) {
    Robot *tmp_arr = new Robot[size + 1];
    
    for (int i = 0; i < size; i++)
        tmp_arr[i] = path_robot[i];
    
    tmp_arr[size] = new_element;

    delete[] path_robot;
    path_robot = tmp_arr;
    
    size++;
}

void find_path(Maze **&maze, int rows, int cols, int x, int y, Robot *&path_robot, int& size)
{
	int max_value = -1;
    int next_x = -1, next_y = -1;
    
    maze[x][y].visited = true;
    Robot new_element;
    new_element.x = x;
    new_element.y = y;
    new_element.score = maze[x][y].value;
    push_element(path_robot, new_element, size);
    
    for (int i = 0; i < 4; ++i) {
        int new_x = x + dx[i];
        int new_y = y + dy[i];
        if (is_valid_move(maze, rows, cols, new_x, new_y) && maze[new_x][new_y].value > max_value) {
            max_value = maze[new_x][new_y].value;
            next_x = new_x;
            next_y = new_y;
        }
    }
    
    if (next_x != -1 && next_y != -1) {
        find_path(maze, rows, cols, next_x, next_y, path_robot, size);
    }
}

void display_path_robot(Robot *path_robot, int step)
{
	for (int i = 0; i < step; i++)
		cout << path_robot[i].score << " ";
	cout << endl;
}

void overlap_of_robots(Robot *path_robot1, int step1, Robot *path_robot2, int step2)
{
	cout << "Nhung vi tri hai robot di trung o nhau: ";
	for (int i = 0; i < step1; i++)
		for (int j = 0; j < step2; j++)
			if (path_robot1[i].score == path_robot2[j].score)
				cout << "(" << path_robot1[i].x << ";" << path_robot1[i].y << ") " ;
	cout << endl;
}
	
int score_robot(Robot *path_robot, int step)
{
	int score = 0;
	for (int i = 0; i < step; i++)
		score += path_robot[i].score;
	return score;
}

void write_result(Robot *path_robot, int step) {
	string file_name;
	cout << "Nhap ten file chua ket qua (*.txt): ";
	cin.ignore();
	getline(cin, file_name);
    ofstream outputfile(file_name);
    outputfile << step << endl;
    for (int i = 0; i < step; i++ ) {
        outputfile << path_robot[i].score << " ";
    }
    outputfile << endl;
    outputfile.close();
}

void end_game() {
    cout << "Ban co muon tiep tuc khong? (nhap [n] de thoat va nhap ki tu bat ky de tiep tuc): ";
    char is_continue;
    cin >> is_continue;
    
    if (is_continue == 'n') {	
		cout << "Tam biet!";
		exit(1);
	}
}

void mode_single_player(Maze**& maze, int rows, int cols) {
    Robot robot;

	cout << "Me cung co " << rows << " hang " << " va " << cols << " cot" << endl;
    do {
        cout << "Nhap vi tri cua robot (x, y): ";
        cin >> robot.x >> robot.y;
        if (!is_valid_move(maze, rows, cols, robot.x, robot.y)) {
            cout << "Vi tri khong hop le. Vui long thu lai!" << endl;
        }
    } while (!is_valid_move(maze, rows, cols, robot.x, robot.y));

    int step = 0;
    Robot *path_robot = new Robot[step];
    find_path(maze, rows, cols, robot.x, robot.y, path_robot, step);
    reset_maze(maze, rows, cols);
    system("cls");
    cout << "Mo phong duong di cua robot" << endl;
    for (int i = 0; i < step; i++) {
    	maze[path_robot[i].x][path_robot[i].y].visited = true;
    	gotoxy(0, 2);
    	visualize(maze, rows, cols);
    	sleep(1);
    }

    robot.score = score_robot(path_robot, step);
    cout << "Diem cua Robot: " << robot.score << endl;

    display_path_robot(path_robot, step);
    write_result(path_robot, step);
    reset_maze(maze, rows, cols);
    delete[] path_robot;
}

void start_two_players(Maze**& maze, int rows, int cols, bool mode) {
    Robot robot1, robot2;
    
	cout << "Me cung co " << rows << " hang " << " va " << cols << " cot" << endl;
    do {
        cout << "Nhap vi tri cua robot 1 (x, y): ";
        cin >> robot1.x >> robot1.y;
        if (!is_valid_move(maze, rows, cols, robot1.x, robot1.y)) {
            cout << "Vi tri khong hop le. Vui long thu lai!" << endl;
        }
    } while (!is_valid_move(maze, rows, cols, robot1.x, robot1.y));

    do {
        cout << "Nhap vi tri cua robot 2 (x, y): ";
        cin >> robot2.x >> robot2.y;
        if (!is_valid_move(maze, rows, cols, robot2.x, robot2.y)) {
            cout << "Vi tri khong hop le. Vui long thu lai!" << endl;
        } else if (robot1.x == robot2.x && robot1.y == robot2.y) {
            cout << "Nguoi choi 1 da chon vi tri nay. Vui long thu lai!" << endl;
            continue;
        }
        else break;
    } while (true);

	int step1 = 0, step2 = 0;
    Robot *path_robot1 = new Robot[step1];
    Robot *path_robot2 = new Robot[step2];
    
    find_path(maze, rows, cols, robot1.x, robot1.y, path_robot1, step1);
    if (mode)
		reset_maze(maze, rows, cols);
    find_path(maze, rows, cols, robot2.x, robot2.y, path_robot2, step2);

	robot1.score = score_robot(path_robot1, step1);
	robot2.score = score_robot(path_robot2, step2);
	
	system("cls");
	cout << "Mo phong duong di cua robot" << endl;
	cout << "Robot 1: " << endl;
	reset_maze(maze, rows, cols);
    for (int i = 0; i < step1; i++) {
    	maze[path_robot1[i].x][path_robot1[i].y].visited = true;
    	gotoxy(0, 3);
    	visualize(maze, rows, cols);
    	sleep(1);
    }
    
    cout << "Robot 2: " << endl;
    reset_maze(maze, rows, cols);
    for (int i = 0; i < step2; i++) {
    	maze[path_robot2[i].x][path_robot2[i].y].visited = true;
    	gotoxy(0, rows * 2 + 5);
    	visualize(maze, rows, cols, 'O');
    	sleep(1);
    }
    
    if (robot1.score > robot2.score) {
        cout << "Robot 1 chien thang!" << endl;
    } else if (robot1.score < robot2.score) {
        cout << "Robot 2 chien thang!" << endl;
    } else {
        cout << "Hoa nhau!" << endl;
    }
    
   
    cout << "Diem cua robot 1: " << robot1.score << std::endl;
    display_path_robot(path_robot1, step1);
    write_result(path_robot1, step1);

    cout << "Diem cua robot 2: " << robot2.score << endl;
    display_path_robot(path_robot2, step2);
    write_result(path_robot2, step2);
    
    if (mode)
    	overlap_of_robots(path_robot1, step1, path_robot2, step2);
    reset_maze(maze, rows, cols);
    delete[] path_robot1;
    delete[] path_robot2;
}

void mode_two_player(Maze **maze, int rows, int cols, bool &go_back)
{
	cout << "Chon che do choi: " << endl;
	cout << "1. Hai robot khong di trung o nhau" << endl;
	cout << "2. Hai robot co the di trung o nhau" << endl;
	cout << "0. Quay lai" << endl;
	cout << "Lua chon cua ban la: ";
	int option;
    cin >> option;
    switch (option)
	{
		case 1:
			start_two_players(maze, rows, cols, false);
			break;
		case 2:
		    start_two_players(maze, rows, cols, true);
        	break;
		case 0:
			go_back = true;
			break;
		default:
		    cout << "Lua chon khong hop le." << endl;
		    break;
	}
}

void maze_menu(Maze **&maze, int  &rows, int &cols, bool &go_back) {
    system("cls");
    cout << "Mo phong me cung :" << endl;
    cout << "1. Tao me cung tu file" << endl;
    cout << "2. Tao me cung ngau nhien" << endl;
    cout << "0. Quay lai" << endl;
    cout << "Lua chon cua ban la: ";
    int option;
    cin >> option;
	switch (option)
	{
		case 1:
		    read_maze(maze, rows, cols);
		    break;
		case 2:
		    create_random_maze(maze, rows, cols);
		    break;
		case 0:
		    go_back = true;
		    break;
		default:
		    cout << "Lua chon khong hop le!" << endl;
			end_game();
            maze_menu(maze, rows, cols, go_back);
		    break;
	}
}

int main() {
	Maze **maze;
	int rows, cols;
	
	int option;
	while (true)
	{
		bool go_back1 = false, go_back2 = false, go_back3 = false;
	    int option = welcome_game();
	    switch (option)
	    {
		    case 1:
		    	while (true)
		    	{
		    	    maze_menu(maze, rows, cols, go_back1);
		            if (go_back1)
		                break;
		            while (true)
		            {
		                int option_mode = mode_game();
		                switch (option_mode)
		            	{
		        	    	case 1:
		        	    		mode_single_player(maze, rows, cols);
		        		    	break;
		        	    	case 2:
		        	    		mode_two_player(maze, rows, cols, go_back3);
		        		    	break;
		        	    	case 0:
		                	    go_back2 = true;
		                	    break;
		                	default:
		                	    cout << "Lua chon khong hop le." << endl;
		                	    break;
		                }
		                if (go_back2)
		                	break;
		                if (go_back3)
		        	    {
		        		    go_back3 = false;
		                    continue;
		                }
		                end_game();
		            }
		            if (go_back2)
		        	{
		        		go_back2 = false;
		                continue;
		            }
		        }
		        break;
		    case 0:
		    	cout << "Tam biet!";
		    	return 0;
		    default:
		    	cout << "Lua chon khong hop le!" << endl;
		    	break;
		}
		
		if (go_back1)
		{	
			go_back1 = false;
		    continue;
		}
		end_game();	   
	}             	
}
