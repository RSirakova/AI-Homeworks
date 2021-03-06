#include <iostream>
#include <algorithm>
#include <array>
#include <vector>

using namespace std;

char board[3][3] = {
	{ '-','-','-' },
	{ '-','-','-' },
	{ '-','-','-' }
};


vector<vector<pair<int, int>>> good_states
{
	//РЕДOВЕ
	{ make_pair(0, 0), make_pair(0, 1), make_pair(0, 2) },
	{ make_pair(1, 0), make_pair(1, 1), make_pair(1, 2) },
	{ make_pair(2, 0), make_pair(2, 1), make_pair(2, 2) },

	//КОЛОНИ
	{ make_pair(0, 0), make_pair(1, 0), make_pair(2, 0) },
	{ make_pair(0, 1), make_pair(1, 1), make_pair(2, 1) },
	{ make_pair(0, 2), make_pair(1, 2), make_pair(2, 2) },

	//ДИАГОНАЛИ
	{ make_pair(0, 0), make_pair(1, 1), make_pair(2, 2) },
	{ make_pair(2, 0), make_pair(1, 1), make_pair(0, 2) }

};


void print_board(char board[3][3])
{
	cout << "=========================" << endl;
	cout << board[0][0] << " " << board[0][1] << " " << board[0][2] << endl;
	cout << board[1][0] << " " << board[1][1] << " " << board[1][2] << endl;
	cout << board[2][0] << " " << board[2][1] << " " << board[2][2] << endl;
	cout << "=========================" << endl;

}

vector<pair<int, int>> get_allowed_movements(char board[3][3])
{
	vector<pair<int, int>> moves;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (board[i][j] != 'O' && board[i][j] != 'X')
			{
				moves.push_back(make_pair(i, j));
			}
		}
	}

	return moves;
}

bool position_occupied(char board[3][3], pair<int, int> pos)
{
	vector<pair<int, int>> allowed_movements = get_allowed_movements(board);

	for (int i = 0; i < allowed_movements.size(); i++)
	{
		if (pos.first == allowed_movements[i].first && pos.second == allowed_movements[i].second)
		{
			return false;
		}
	}

	return true;
}

//Функция провяряваща има ли свободни места на дъската
bool full_board(char board[3][3])
{
	if (get_allowed_movements(board).size() == 0) return true;
	return false;
}

//Функция връщаща позициите на дъската заети от подадения като аргумент играч - O или X
vector<pair<int, int>> get_positions(char player, char board[3][3])
{
	vector<pair<int, int>> positions;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (player == board[i][j])
			{
				positions.push_back(make_pair(i, j));
			}
		}
	}

	return positions;
}

bool won_game(vector<pair<int, int>> positions)
{
	bool won = true;

	for (int i = 0; i < good_states.size(); i++)
	{
		won = true;
		vector<pair<int, int>> curr_win_state = good_states[i];
		for (int j = 0; j < 3; j++)
		{
			if (!(find(begin(positions), end(positions), curr_win_state[j]) != end(positions)))
			{
				won = false;
				break;
			}
		}

		if (won)
		{
			break;
		}
	}
	return won;
}
char get_another_player(char first_player)
{
	if (first_player == 'O') return 'X';
	else return 'O';
}
int get_current_state(char current_palyer, char board[3][3])
{
	char another_player = get_another_player(current_palyer);

	vector<pair<int, int>> pos = get_positions(current_palyer, board);
	//Печели
	if (won_game(pos)) return 1000;

	pos = get_positions(another_player, board);
	//Губи
	if (won_game(pos)) return -1000;

	//НЯМА СВOБOДНИ МЕСТА - РАВЕНСТВO!
	if (full_board(board)) return 0;

	return 0;
}


/*  ПСЕВДОКОД::
function minimax(node, depth, isMaximizingPlayer, alpha, beta):

	if node is a leaf node :
		return value of the node

	if isMaximizingPlayer :
		bestVal = -INFINITY
		for each child node :
			value = minimax(node, depth+1, false, alpha, beta)
			bestVal = max( bestVal, value)
			alpha = max( alpha, bestVal)
			if beta <= alpha:
				break
		return bestVal

	else :
		bestVal = +INFINITY
		for each child node :
			value = minimax(node, depth+1, true, alpha, beta)
			bestVal = min( bestVal, value)
			beta = min( beta, bestVal)
			if beta <= alpha:
				break
		return bestVal

*/

// MINIMAX
pair<int, pair<int, int>> minimax(char board[3][3], char player, int depth, int alpha, int beta)
{
	// НАМИРАНЕ НА НАЙ-ДOБРИЯ ВЪЗМOЖЕН ХOД
	pair<int, int> best_move = make_pair(-1, -1);
	int best;
	if (player == 'O') //CPU
		best = -1000;
	else
		best = 1000;//PLAYER


	// АКO СМЕ В ЛИСТO ВРЪЩАМЕ 
	if (get_current_state('O', board) != 0 || full_board(board))
	{
		best = get_current_state('O', board);
		return make_pair(best, best_move);
	}

	vector<pair<int, int>> allowed_movements = get_allowed_movements(board);

	for (int i = 0; i < allowed_movements.size(); i++)
	{
		pair<int, int> curr_move = allowed_movements[i];
		board[curr_move.first][curr_move.second] = player;

		if (player == 'O')
		{
			int score = minimax(board, 'X', depth + 1, alpha, beta).first;

			if (best < score)
			{
				best = score - depth * 10;
				best_move = curr_move;

				alpha = max(alpha, best);
				board[curr_move.first][curr_move.second] = '-';
				if (beta <= alpha)
				{
					break;//!!
				}
			}

		} 
		else
		{
			int score = minimax(board, 'O', depth + 1, alpha, beta).first;

			if (best > score)
			{
				best = score + depth * 10;
				best_move = curr_move;
				beta = min(beta, best);
				board[curr_move.first][curr_move.second] = '-';
				if (beta <= alpha)
				{
					break;//!!
				}
			}
		}
		//Oтмяна на хода
		board[curr_move.first][curr_move.second] = '-';

	}

	return make_pair(best, best_move);
}


bool game_over(char board[3][3])
{
	if (full_board(board)) return true;
	if (get_current_state('O', board) != 0 ) return true;

	return false;
}

void TIC_TAC_TOE()
{


	print_board(board);

	cout << "Enter 1 to be first player or 2 to be second player:" << endl;
	int turn;
	cin >> turn;
	bool humanFirst;
	if (turn == 1)
		humanFirst = true;
	else
		humanFirst = false;

	if (humanFirst)
	{
		while (!game_over(board))
		{
			int row, col;
			cout << "Enter coordinates of your move:" << endl;
			cout << "Insert row [1,3]: ";
			cin >> row;
			cout << "Insert column [1,3]: ";
			cin >> col;
			cout << endl << endl;

			row--;
			col--;
			if (position_occupied(board, make_pair(row, col)))
			{
				cout << "[" << row << ", " << col << "] is not allowed. Insert another one." << endl;
				continue;
			}
			else
			{
				board[row][col] = 'X';
			}

			pair<int, pair<int, int>> computer_move = minimax(board, 'O', 0, -1000, 1000);

			board[computer_move.second.first][computer_move.second.second] = 'O';

			print_board(board);
		}
	}
	else
	{
		pair<int, pair<int, int>> computer_move = minimax(board, 'O', 0, -1000, 1000);

		board[computer_move.second.first][computer_move.second.second] = 'O';

		print_board(board);

		while (!game_over(board))
		{
			int row, col;
			cout << "Enter coordinates of your move:" << endl;
			cout << "Insert row [1,3]: ";
			cin >> row;
			cout << "Insert column [1,3]: ";
			cin >> col;
			cout << endl << endl;

			row--;
			col--;
			if (position_occupied(board, make_pair(row, col)))
			{
				cout << "The position (" << row << ", " << col << ") is not allowed. Try another one..." << endl;
				continue;
			}
			else
			{
				board[row][col] = 'X';
			}

			pair<int, pair<int, int>> computer_move = minimax(board, 'O', 0, -1000, 1000);

			board[computer_move.second.first][computer_move.second.second] = 'O';

			print_board(board);
		}
	}
	int result = get_current_state('O', board);

	if (result == 1000) cout << "COMPUTER WINS" << endl;
	else if (result == -1000) cout << "YOU WINS" << endl;
	else  cout << "DRAW GAME" << endl;

}

int main() {
	
	TIC_TAC_TOE();
	return 0;

}
