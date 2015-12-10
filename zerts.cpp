#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <Windows.h>
#include <string>
#include <fstream>

using namespace std;

ofstream out("output.txt");

const int FIELD_SIZE = 8;
const int DIRETIONS = 8;
int currUser = 1;
int enemy, me;
const vector<int> directionX({-1, -1, 0, 1, 1, 1, 0, -1});
const vector<int> directionY({0, 1, 1, 1, 0, -1, -1, -1});
int p1 = 25000, p2 = 5000, p3 = 1000, p4 = 500, p5 = 200, p6 = 50, p7 = 10;
const vector< vector <int> > priority({
	{ p1, p5, p2, p3, p3, p2, p5, p1 },
	{ p5, p6, p6, p6, p6, p6, p6, p5 },
	{ p2, p6, p4, p4, p4, p4, p6, p2 },
	{ p3, p6, p4, p7, p7, p4, p6, p3 },
	{ p3, p6, p4, p7, p7, p4, p6, p3 },
	{ p2, p6, p4, p4, p4, p4, p6, p2 },
	{ p5, p6, p6, p6, p6, p6, p6, p5 },
	{ p1, p5, p2, p3, p3, p2, p5, p1 },
});

void changeUser(int newUser)
{
	currUser = newUser;
	//currUser = ((currUser % 2) + 1);
}

void printField()
{
	for (int i = 0; i < FIELD_SIZE; i++)
	{
		for (int j = 0; j < FIELD_SIZE; j++)
		{
			if (field[i][j] == 0)
				cout << ". ";
			else
				cout << field[i][j] << " ";
		}
		cout << endl;
	}
}

bool inField(int x, int y)
{
	return (x >= 0 && x < FIELD_SIZE && y >= 0 && y < FIELD_SIZE);
}

int distance(int startX, int startY, int currX, int currY)
{
	return max(abs(startX - currX), abs(startY - currY));
}

bool possibleTurnWithDirection(int x, int y, int currDirection)
{
	if (field[x][y] != 0)
		return false;

	int currX = x + directionX[currDirection], currY = y + directionY[currDirection];
	while (inField(currX, currY) && field[currX][currY] != currUser)
	{
		currX += directionX[currDirection];
		currY += directionY[currDirection];
	}
	if (!inField(currX, currY))
		return false;
	if (distance(x, y, currX, currY) <= 1)
		return false;
	for (int i = x + directionX[currDirection], j = y + directionY[currDirection]; 
		i != currX || j != currY; i += directionX[currDirection], j += directionY[currDirection])
	{
		if (field[i][j] == 0)
			return false;
	}
	return true;
}

bool posiibleTurn(int x, int y)
{
	if (field[x][y] != 0)
	{
		return false;
	}
	for (int i = 0; i < DIRETIONS; i++)
	{
		if (possibleTurnWithDirection(x, y, i))
		{
			return true;
		}
	}
	return false;
}

void printFieldBeforeTurn()
{
	out << endl;
	for (int i = 0; i < FIELD_SIZE; i++)
	{
		for (int j = 0; j < FIELD_SIZE; j++)
		{
			if (posiibleTurn(i, j))
			{
				out << "* ";
			}
			else if (field[i][j] != 0)
			{
				out << field[i][j] << " ";
			}
			else
			{
				out << ". ";
			}
		}
		out << endl;
	}
}

bool posibleTurnForUser()
{
	for (int i = 0; i < FIELD_SIZE; i++)
	{
		for (int j = 0; j < FIELD_SIZE; j++)
		{
			if (posiibleTurn(i, j))
			{
				return true;
			}
		}
	}
	return false;
}

void turn()
{
	for (int dir = 0; dir < DIRETIONS; dir++)
	{
		if (possibleTurnWithDirection(turnX, turnY, dir))
		{
			int currX = turnX, currY = turnY;
			while (field[currX][currY] != currUser)
			{
				currX += directionX[dir];
				currY += directionY[dir];
			}
			for (int i = turnX + directionX[dir], j = turnY + directionY[dir];
			i != currX || j != currY; i += directionX[dir], j += directionY[dir])
			{
				field[i][j] = currUser;
			}
		}
	}
	field[turnX][turnY] = currUser;
}

void generateRandomTurn()
{
	for (int i = 0; i < FIELD_SIZE; i++)
	{
		for (int j = 0; j < FIELD_SIZE; j++)
		{
			if (posiibleTurn(i, j))
			{
				turnX = i;
				turnY = j;
				return;
			}
		}
	}
}

void printWinner()
{
	int firstUserResult = 0, secondUserResult = 0;
	for (int i = 0; i < FIELD_SIZE; i++)
	{
		for (int j = 0; j < FIELD_SIZE; j++)
		{
			if (field[i][j] == 1)
			{
				firstUserResult++;
			}
			if (field[i][j] == 2)
			{
				secondUserResult++;
			}
		}
	}
	if (firstUserResult == secondUserResult)
	{
		cout << "It's a draw!\n";
		return;
	}
	cout << "User #";
	if (firstUserResult > secondUserResult)
	{
		cout << 1;
	}
	else
	{
		cout << 2;
	}
	cout << " is winner!\n";
	cout << firstUserResult << " against " << secondUserResult << endl;
}

int getBestPriority()
{
	int best = 0;
	for (int i = 0; i < FIELD_SIZE; i++)
	{
		for (int j = 0; j < FIELD_SIZE; j++)
		{
			if (posiibleTurn(i, j) && priority[i][j] > best)
			{
				best = priority[i][j];
			}
		}
	}
	return best;
}

vector< pair<int, int> > getPlacesWithBestPriority()
{
	vector< pair<int, int> > result;
	int bestPriority = getBestPriority();
	for (int i = 0; i < FIELD_SIZE; i++)
	{
		for (int j = 0; j < FIELD_SIZE; j++)
		{
			if (posiibleTurn(i, j) && bestPriority/ priority[i][j] < 5)
			{
				result.push_back(make_pair(i, j));
			}
		}
	}
	return result;
}

bool cornerIsPossible()
{
	return posiibleTurn(0, 0) || posiibleTurn(0, 7) || posiibleTurn(7, 0) || posiibleTurn(7, 7);
}

void generateCornerTurn()
{
	if (posiibleTurn(0, 0))
	{
		turnX = 0;
		turnY = 0;
		return;
	}
	if (posiibleTurn(0, 7))
	{
		turnX = 0;
		turnY = 7;
		return;
	}
	if (posiibleTurn(7, 0))
	{
		turnX = 7;
		turnY = 0;
		return;
	}
	if (posiibleTurn(7, 7))
	{
		turnX = 7;
		turnY = 7;
		return;
	}
}

bool generateGoodSideTurn()
{
	for (int i = 1; i < 7; i++)
	{
		if ((possibleTurnWithDirection(0, i, 2) && field[0][i - 1] != enemy)
			|| (possibleTurnWithDirection(0, i, 6) && field[0][i + 1] != enemy))
		{
			turnX = 0;
			turnY = i;
			return true;
		}
		if ((possibleTurnWithDirection(i, 0, 0) && field[i + 1][0] != enemy)
			|| (possibleTurnWithDirection(i, 0, 4) && field[i - 1][0] != enemy))
		{
			turnX = i;
			turnY = 0;
			return true;
		}


		if ((possibleTurnWithDirection(7, i, 2) && field[7][i - 1] != enemy)
			|| (possibleTurnWithDirection(7, i, 6) && field[7][i + 1] != enemy))
		{
			turnX = 7;
			turnY = i;
			return true;
		}
		if ((possibleTurnWithDirection(i, 7, 0) && field[i + 1][7] != enemy)
			|| (possibleTurnWithDirection(i, 7, 4) && field[i - 1][7] != enemy))
		{
			turnX = i;
			turnY = 7;
			return true;
		}
	}
	return false;
}

bool generateUnstoppableTurn()
{
	vector < vector<int> > fieldCopy(FIELD_SIZE, vector<int>(FIELD_SIZE));
	for (int i = 0; i < FIELD_SIZE; i++)
	{
		for (int j = 0; j < FIELD_SIZE; j++)
		{
			fieldCopy[i][j] = field[i][j];
		}
	}

	for (int i = 0; i < FIELD_SIZE; i++)
	{
		for (int j = 0; j < FIELD_SIZE; j++)
		{
			currUser = me;
			if (posiibleTurn(i, j))
			{
				turnX = i;
				turnY = j;
				turn();
				currUser = enemy;
				if (!posibleTurnForUser())
				{
					currUser = me;
					out << "Unstop!\n";
					for (int ii = 0; ii < FIELD_SIZE; ii++)
					{
						for (int jj = 0; jj < FIELD_SIZE; jj++)
						{
							field[ii][jj] = fieldCopy[ii][jj];
						}
					}
					return true;
				}
				currUser = me;
				for (int ii = 0; ii < FIELD_SIZE; ii++)
				{
					for (int jj = 0; jj < FIELD_SIZE; jj++)
					{
						field[ii][jj] = fieldCopy[ii][jj];
					}
				}
			}
		}
	}

	for (int i = 0; i < FIELD_SIZE; i++)
	{
		for (int j = 0; j < FIELD_SIZE; j++)
		{
			field[i][j] = fieldCopy[i][j];
		}
	}
	return false;
}

long long getSimplePotential(int x, int y)
{
	long long result = 0;
	vector < vector<int> > fieldCopy(FIELD_SIZE, vector<int>(FIELD_SIZE));
	for (int i = 0; i < FIELD_SIZE; i++)
	{
		for (int j = 0; j < FIELD_SIZE; j++)
		{
			fieldCopy[i][j] = field[i][j];
		}
	}

	turnX = x;
	turnY = y;
	turn();
	currUser = enemy;

	if (cornerIsPossible())
	{
		result = 1000000000;
	}
	else if (generateGoodSideTurn())
	{
		result = 600000000;
	}
	else
	{
		for (int i = 0; i < FIELD_SIZE; i++)
		{
			for (int j = 0; j < FIELD_SIZE; j++)
			{
				if (posiibleTurn(i, j))
				{
					result += priority[i][j];
				}
			}
		}
		result *= 3;
		result /= 2;
	}

	currUser = me;

	for (int i = 0; i < FIELD_SIZE; i++)
	{
		for (int j = 0; j < FIELD_SIZE; j++)
		{
			field[i][j] = fieldCopy[i][j];
		}
	}
	return result;
}

long long recursiveFindingPriority(int n, int user)
{
	long long result = 0;
	currUser = user;

	if (n == 0)
	{
		for (int i = 0; i < FIELD_SIZE; i++)
		{
			for (int j = 0; j < FIELD_SIZE; j++)
			{
				if (posiibleTurn(i, j))
				{
					result += priority[i][j];
				}
			}
		}
		return result;
	}

	vector < vector<int> > fieldCopy(FIELD_SIZE, vector<int>(FIELD_SIZE));
	for (int i = 0; i < FIELD_SIZE; i++)
	{
		for (int j = 0; j < FIELD_SIZE; j++)
		{
			fieldCopy[i][j] = field[i][j];
		}
	}

	if (user == enemy)
	{
		for (int i = 0; i < FIELD_SIZE; i++)
		{
			for (int j = 0; j < FIELD_SIZE; j++)
			{
				if (posiibleTurn(i, j))
				{
					turnX = i;
					turnY = i;
					turn();
					result = max(result, recursiveFindingPriority(n - 1, 3 - user));

					for (int ii = 0; ii < FIELD_SIZE; ii++)
					{
						for (int jj = 0; jj < FIELD_SIZE; jj++)
						{
							field[ii][jj] = fieldCopy[ii][jj];
						}
					}
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < FIELD_SIZE; i++)
		{
			for (int j = 0; j < FIELD_SIZE; j++)
			{
				if (posiibleTurn(i, j))
				{
					turnX = i;
					turnY = i;
					turn();
					if (result != 0)
						result = min(result, recursiveFindingPriority(n - 1, 3 - user));
					else
						result = recursiveFindingPriority(n - 1, 3 - user);

					for (int ii = 0; ii < FIELD_SIZE; ii++)
					{
						for (int jj = 0; jj < FIELD_SIZE; jj++)
						{
							field[ii][jj] = fieldCopy[ii][jj];
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < FIELD_SIZE; i++)
	{
		for (int j = 0; j < FIELD_SIZE; j++)
		{
			field[i][j] = fieldCopy[i][j];
		}
	}

	return result;
}

long long getPotential(int x, int y)
{
	long long result = 0;
	vector < vector<int> > fieldCopy(FIELD_SIZE, vector<int>(FIELD_SIZE));
	for (int i = 0; i < FIELD_SIZE; i++)
	{
		for (int j = 0; j < FIELD_SIZE; j++)
		{
			fieldCopy[i][j] = field[i][j];
		}
	}

	turnX = x;
	turnY = y;
	turn();

	result = recursiveFindingPriority(2, enemy);

	currUser = me;

	for (int i = 0; i < FIELD_SIZE; i++)
	{
		for (int j = 0; j < FIELD_SIZE; j++)
		{
			field[i][j] = fieldCopy[i][j];
		}
	}
	return result;
}

void generateTurn()
{
	if (generateUnstoppableTurn())
	{
		return;
	}
	if (cornerIsPossible())
	{
		generateCornerTurn();
		return;
	}
	if (generateGoodSideTurn())
	{
		out << "side\n";
		return;
	}
	vector< pair<int, int> > places = getPlacesWithBestPriority();
	vector<long long> potential;

	for (int i = 0; i < places.size(); i++)
	{
		potential.push_back(getSimplePotential(places[i].first, places[i].second));
		out << potential.back() << " ";
	}
	out << endl;
	for (int i = 0; i < places.size(); i++)
	{
		potential[i] = max(potential[i], getPotential(places[i].first, places[i].second));
		out << potential[i] << " ";
	}
	out << endl;
	long long bestPotential = potential[0];
	int curr = 0;
	for (int i = 1; i < potential.size(); i++)
	{
		bestPotential = min(bestPotential, potential[i]);
		curr = i;
	}
	turnX = places[curr].first;
	turnY = places[curr].second;
}

int main()
{
	vector< vector<int> > field(FIELD_SIZE);
	int turnX, turnY;
	for (int i = 0; i < FIELD_SIZE; i++)
	{
		field[i].assign(FIELD_SIZE, 0);
	}
	field[3][3] = 2;
	field[3][4] = 1;
	field[4][3] = 1;
	field[4][4] = 2;
	string server;
	cin >> server;
	cin >> server;
	if (server == "black")
	{
		enemy = 2;
		me = 1;
	}
	else
	{
		enemy = 1;
		me = 2;
	}
	int enemyX, enemyY;
	char enemyYchar, turnYchar;
	do
	{
		cin >> server;
		if (server == "turn")
		{
			changeUser(me);
			printFieldBeforeTurn();
			generateTurn();
			turn();
			turnYchar = 'a' + turnY;
			
			out << endl << turnYchar << turnX + 1 << endl;
			cout << "move " << turnYchar << " " << turnX + 1 << endl;		
			//printFieldBeforeTurn();
		}
		else if (server == "move")
		{
			changeUser(enemy);
			cin >> enemyYchar >> enemyX;
			turnX = enemyX - 1;
			turnY = enemyYchar - 'a';
			turn();
			//printFieldBeforeTurn();
		}
	} while (server == "turn" || server == "move");
	out.close();
	return 0;
}
