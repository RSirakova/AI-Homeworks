#include <iostream>
#include <chrono> 
#include <ctime> 
#include <unordered_set>
#include <vector>
#include <string>
#include <algorithm>
#include <math.h>
using namespace std;


class Point
{
private:
	int x, y,id;
public:
	Point(int id_);
	Point(int x_, int y_,int id_);
	int get_x()const;
	int get_y()const;
	int get_id()const;

	double get_distance(Point other)const;

	bool operator==(const Point& other)const;

	void print_point()const;

};

class TravelOrganiser
{
	private:
	vector<Point> points; // списък от градовете
	public:
		//добавяне на град
		void addPoint(Point point);

		// връща град по даден индекс
		 Point getCity(int i);

		// връща списъка от градовете
		 vector<Point> getPointList();
	};


class Travel
{
private:

	vector<Point> current_tavel;
	double distance = 0;
	double fitness = 0;
	int size;
	// Gets distance between two cities
	double distanceTwoCities(Point p1, Point p2);
	
public:
	//
	Travel(vector<Point> p)
	{
		createIndividual(p);
	}


	Travel()
	{
		size = 0;
	}


	void createIndividual(vector<Point> p)
	{
		current_tavel = p; // Copies vector of cities
		random_shuffle(current_tavel.begin(), current_tavel.end()); 
	}

	Point getPoint(int i)
	{
		return current_tavel.at(i);
	}

	void setPoint(Point p)
	{
		current_tavel.push_back(p);
		size++;
	}

	void setPoint(int i, Point p)
	{
		current_tavel.at(i) = p;
	}

	double getDistance()
	{
		if (distance == 0)
		{
			int size = current_tavel.size();
			for (int i = 0; i < size - 1; i++)
			{
				distance += distanceTwoCities(current_tavel.at(i), current_tavel.at(i + 1));
			}
			distance += distanceTwoCities(current_tavel.at(0), current_tavel.at(size - 1));
		}
		return distance;
	}


	double getFitness()
	{
		return 1 / getDistance();
	}


	int getTravelSize()
	{
		size = current_tavel.size();
		return size;
	}
};


// Population of tours
class Population
{
	vector<Travel> travels; // популацията от пътувания
	TravelOrganiser o;
	int size = 0;
public:
	// отново става на случаен принцип
	Population(int pSize, int pCoint)
	{
		for (int i = 0; i < pCoint; i++) //заради това, че имаме вектор
		{
			o.addPoint(Point(i));
		}
		for (int i = 0; i < pSize; i++) // популация от пътувания
		{
			travels.push_back(o.getPointList());
		}
		size = travels.size();
	}

	Population():size(0)
	{
	}

	// добавяне на пъруването по дадено пътуване
	void addTravel(Travel t)
	{
		travels.push_back(t);
		size++;
	}

	//добавяне на пъруването по даден индекс
	void addTravel(Travel t, int i)
	{
		travels.at(i) = t;
	}

	// извличане на пътуване по индекс
	Travel getTravel(int i)
	{
		return travels.at(i);
	}

	//най-късия път
	Travel getFittest()
	{
		Travel fittest = travels.at(0);
		for (int i = 1; i < size; i++)
		{
			if (fittest.getFitness() < travels.at(i).getFitness())
			{
				fittest = travels.at(i);
			}
		}
		return fittest;
	}

	int getPopulationSize()
	{
		return size;
	}
};



class GA
{
	const double mutationRate = .015; 
	const int tournamentSize = 5;
	bool elitism = true; //ако е true ще се създаде ново поколение
	vector<Travel> travels;
	// избира кои индивиди да се кръстоват
	Travel slection(Population p)
	{
		Population current;
		int range = p.getPopulationSize();
		for (int i = 0; i < tournamentSize; i++) // на случаен принцип избира 
		{
			int randomTravel = rand() % range;
			current.addTravel(p.getTravel(randomTravel));
		}
		Travel fitness = current.getFittest();
		return fitness;
	}

	// изключване от популацията на по-слабите
	Travel exclusion(Population p, Travel t)
	{
		Population current;
		int range = p.getPopulationSize() - 1;
		int size = t.getTravelSize();
		string travel1 = "";
		for (int i = 0; i < size; i++)
		{
			travel1 += t.getPoint(i).get_id();
		}

		int i = 0;
		while (i < tournamentSize)
		{
			int random = rand() % range;
			for (int j = 0; j < size; j++)
			{
				if (p.getTravel(random).getPoint(j).get_id() != travel1[j])
				{
					current.addTravel(p.getTravel(random));
					i++; 
					break;
				}
			}

		}
		Travel fitness = current.getFittest();
		return fitness;
	}

	// На случаен принцип индивидите мутират - това става на базата на коефициент на мутация
	void mutate(Travel t)
	{
		int tSize = t.getTravelSize(); 
		for (int i = 0; i < tSize; i++)
		{
			if (double(rand() / RAND_MAX) <= mutationRate) 
			{
				//на базта на размяна
				int swap = rand() % tSize; 
				Point c1 = t.getPoint(i);
				Point c2 = t.getPoint(swap);
				t.setPoint(swap, c1);
				t.setPoint(i, c2);
			}
		}
	}

	// Кръстосване на множество от родители и връщане на полученото дете
	Travel crossover(Travel parent1, Travel parent2)
	{
		int tSize = parent1.getTravelSize();
		Travel child = Travel();
		unordered_set<int> points; // ненаредено множество,съдържащо ид на точките в текущото пътуване
		int split1 = rand() % (tSize / 2); // колко да вземем от първата част на родителя
		int split2 = rand() % (tSize / 2); // колко да вземам от втората част на родителя
		split2 = tSize - split2; // мястото от където ще започнем да вземаме от втората хромозома
		for (int i = 0; i < split1; i++) //добавяне от първата част на първия родител
		{
			child.setPoint(parent1.getPoint(i));
			points.insert(parent1.getPoint(i).get_id());
		}
		for (int i = split2; i < tSize; i++)
		{
			child.setPoint(parent1.getPoint(i));
			points.insert(parent1.getPoint(i).get_id());
		}
		// незапълнените места се попълват от втория родител
		int index = 0;
		while (child.getTravelSize() != tSize)
		{
			if (points.count(parent2.getPoint(index).get_id()) == 0)
			{
				child.setPoint(parent2.getPoint(index));
			}
			index++;
		}
		return child;
	}

public:
	// еволюция за поколение
	Population evolution(Population p)
	{
		int size = p.getPopulationSize();
		Population nextGeneration = Population();
		int eliteOffset = 0;
		if (elitism) // запазва най-добрите индивидди
		{
			eliteOffset = 1;
			nextGeneration.addTravel(p.getFittest());
		}
		for (int i = eliteOffset; i < size; i++) // създаване на ново поколение
		{
			Travel parent1 = slection(p);
			Travel parent2 = exclusion(p, parent1);
			Travel child = crossover(parent1, parent2);
			nextGeneration.addTravel(child);
		}

		for (int i = eliteOffset; i < size; i++) // мутация
		{
			mutate(nextGeneration.getTravel(i));
		}
		return nextGeneration;
	}
};

int main()
{
	const int N = 50; // точките
	int populationSize = 30; // размера на популацията
	const int numberGenerations = 50; // броят на поколенията които ще еволюират
	srand(time(nullptr)); 
	vector<int> testing;
	Population p = Population(populationSize, N); 
	Travel route = p.getFittest();
	GA g;

	
	for (int i = 0; i < N; i++) 
	{
		cout << "(" << route.getPoint(i).get_x() << "," << route.getPoint(i).get_y() << ") -> ";
	}

	cout << "(" << route.getPoint(0).get_x() << "," << route.getPoint(0).get_y() << ")" << endl;

	double initialDistance = route.getDistance();
	cout << "Intial distance= " << initialDistance << endl;

	int start = clock();
	for (int i = 0; i < numberGenerations; i++) 
	{
		if (i == 10)
			cout << "10th: " << p.getFittest().getDistance() << endl;
		p = g.evolution(p);
	}
	int end = clock();
	route = p.getFittest();
	double finalDistance = route.getDistance();
	double improvement = initialDistance - finalDistance;
	cout << "Final distance: " << finalDistance<<endl;
	cout << "Improvement: " << improvement << endl;

	for (int i = 0; i < N; i++)
	{
		cout << "(" << route.getPoint(i).get_x() << "," << route.getPoint(i).get_y() << ") -> ";
	}

	cout << "(" << route.getPoint(0).get_x() << "," << route.getPoint(0).get_y() << ")" << endl;


	//cout << "Execution time was: " << end - start << endl;


}

Point::Point(int id_)
{
	x = (int)(rand() % (1000));
	y = (int)(rand() % (1000));
	id = id_;
}

Point::Point(int x_, int y_,int id_)
{
	x = x_;
	y = y_;
	id = id_;
}

int Point::get_x() const
{
	return x;
}

int Point::get_y() const
{
	return y;
}

int Point::get_id() const
{
	return id;
}

double Point::get_distance(Point other) const
{
	int xDistance = abs(x - other.get_x());
	int yDistance = abs(y - other.get_y());
	double distance = sqrt((xDistance * xDistance) + (yDistance * yDistance));

	return distance;
}

bool Point::operator==(const Point& other) const
{
	return(x == other.get_x() && y == other.get_y());
}

void Point::print_point() const
{
	cout << '(' << get_x() << ',' << get_y() << ')' << endl;
}



void TravelOrganiser::addPoint(Point point)
{
	points.push_back(point);
}

Point TravelOrganiser::getCity(int i)
{
	return points.at(i);
}

vector<Point> TravelOrganiser::getPointList()
{
	return points;
}

double Travel::distanceTwoCities(Point p1, Point p2)
{
	return p1.get_distance(p2);
}
