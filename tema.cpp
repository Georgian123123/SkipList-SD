// Copyright Comana Marian-Georgian 2019 311CA
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
// made a player class to constitutie details for a player
class Player{
	private:
		// id and points for players;
        int id, points;

	public:
		explicit Player(int id = 0, int points = 0) {
			this->id = id;
			this->points = points;
		}
		Player(const Player &cpy) {
			this->id = cpy.id;
			this->points = cpy.points;
		}
		~Player(){}
		int getId() {
			return id;
		}
		int getPoints() {
			return points;
		}
		void setId(int id) {
			this->id = id;
		}
		void setPoints(int points) {
			this->points = points;
		}
		// overloaded <=
		bool operator <=(const Player &others) {
			return points <= others.points;
		}
		// overloadede !=
		bool operator !=(const Player &others) {
			return points != others.points;
		}
};
// made a struct for ecery node which contains data type class
// used a double pointer next to move in skiplist
// moving in skiplist in one way and by levels
template <class T>
struct Node {
	T data;
	struct Node<T> **next;
	Node(T dataToAdd, int level) : data(dataToAdd) {
		// constructor
		next = new Node*[level + 1];
		for (int i = 0; i < level + 1; ++i) {
			next[i] = 0;
		}
	}
	~Node() {
		delete[] next;
	}
};
// class skiplist which contains all nodes
template <class T>
class SkipList{
	int MAXLVL;
	int level;
	struct Node<T> *header;
	public :
		explicit SkipList(int N) {
			// constructor
			MAXLVL = log(N);
			level = 0;
			Player *P = new Player(-1000000, -1000000);
			header = new Node<T>(*P, MAXLVL);
			delete P;
		}
		SkipList(const SkipList &oth) {
			Node<T> *current = header->next[0];
			Node<T> *current2 = oth->header->next[0];
			while(current != NULL && current2 != NULL) {
				Node<T> *it = current;
				Node<T> *it2 = current2;
				it = it2;
				current = current->next[0];
				current2 = current->next[0];
			}
		}
		~SkipList() {
			// destructor
			Node<T> *current = header->next[0];
			while(current != NULL) {
				Node<T> *it = current;
				current = current->next[0];
				delete it;
			}
			delete header;
		}
		int searchInList(int x) {
			// search in skiplist the player with id which i need
			Node<T> *current = header->next[0];
			while(current->data.getId() != x) {
				current = current->next[0];
			}
			return current->data.getPoints();
		}
		void addElem(Player data) {
			// adding elements
			// I'm setting a path vector that will contain every node
			// that i'm moving for
			// Searching the player with bigger points that i want to put
			// if = I'm putting it by id
			Node<T> *it = header;
			Node<T>* vec[MAXLVL + 1];
			for (int i = 0; i < MAXLVL + 1; ++i) {
				vec[i] = 0;
			}
			for(int i = level; i >= 0; --i) {
				while(it->next[i] != NULL &&
					it->next[i]->data <= data) {
					if((it->next[i]->data.getPoints() == data.getPoints()
						&& it->next[i]->data.getId() < data.getId())){
						break;
					} else {
						it = it->next[i];
					}
				}
				vec[i] = it;
			}
			it = it->next[0];
			if(it == NULL || it->data != data || it->data.getId() < data.getId()) {
				int newlvl = randomLevel();
				if(newlvl > level) {
					for(int i = level + 1; i < newlvl + 1; ++i) {
						vec[i] = header;
					}
				level = newlvl;
				}
				Node<T> *n = new Node<T>(data, level);
				for(int i = 0; i <= newlvl; ++i) {
					n->next[i] = vec[i]->next[i];
					vec[i]->next[i] = n;
				}
			}
		}
		void displayListprint(int &ok, int N, int print[], std::ofstream &outfile) {
				// display List
				// I'm moving in lowest leve and get every node's information
			    Node<T> *node = header->next[0];
			    int id[N] = {0}
			   	for (int i = 1; i <= N; ++i) {
			    	id[i] = node->data.getId();
			       	points[i] = node->data.getPoints();
			       	node = node->next[0];
			    }
			    for (int i = N; i > 0; --i) {
			       	if(ok == 0) {
			       		outfile << id[i] << " " << points[i] <<
			       		" " << "0" << "\n";
			       	} else {
			       		outfile << id[i] << " " << points[i] <<
			       		" " << i - print[id[i]] << "\n";
			       	}
			      		print[id[i]] = i;
			    }
			    ok = 1;
			    delete node;
		}

		void deleteFromList(int id, int points) {
			// deleting a node from list
			// searching what i need to delete by id and points
			Node<T> *current = header;
			Node<T> *Path[MAXLVL + 1];
			for (int i = 0; i < MAXLVL + 1; ++i) {
				Path[i] = 0;
			}
			for (int i = level; i >= 0; --i) {
				while(current->next[i] != NULL
					&& current->next[i]->data.getPoints() <= points) {
					if((current->next[i]->data.getPoints() == points
						&& current->next[i]->data.getId() == id) ||
						(current->next[i]->data.getPoints() == points &&
							current->next[i]->data.getId() < id)) {
						break;
					} else {
						current = current -> next[i];
					}
				}
				Path[i] = current;
			}
			current = current -> next[0];
			if(current != NULL  && current -> data.getId() == id &&
				current -> data.getPoints() == points) {
				for(int i = 0; i <= level; ++i) {
					if(Path[i]->next[i] != current) {
						break;
					}
					Path[i]->next[i] = current->next[i];
				}
				delete current;
				while(level > 0 && header->next[level] == NULL) {
					--level;
				}
			}
		}
		int randomLevel() {
			// getting random level for a node
			float ration = (float)rand() / RAND_MAX;
			int lvl = 0;
			float P = 1./2;
				while (ration < P && lvl < MAXLVL) {
					++lvl;
					ration = (float)rand() / RAND_MAX;
				}
				return lvl;
		}
		void getPoz(int last_poz[], int N) {
			// getting last position for every player from the scoreboard
			Node<T> *current = header->next[0];
			int k = N;
			while(current) {
				last_poz[current->data.getId()] = k;
				--k;
				current = current->next[0];
			}
			delete current;
		}
	};

	void readDates(std::ifstream &infile, int &k, int &m, int &n) {
		// getting dates
		infile >> n >> m >> k;
	}

	void sortVector(int vector1[], int vector2[], int k) {
		// sorting vectors
		for(int i = 0; i < k - 1; ++i) {
			for(int j = i + 1; j < k; ++j) {
			 	int temp;
			 	if(vector1[i] > vector1[j]) {
					temp = vector1[i];
	 				vector1[i] = vector1[j];
					vector1[j] = temp;
					temp = vector2[i];
			 		vector2[i] = vector2[j];
					vector2[j] = temp;
				}
			}
		}
	}
	void getPointsByLastPoz(int vector3[], int vector2[], int vector1[], int k) {
		// sorting vectors by last position
		for(int i = 0; i < k - 1; ++i) {
		 	for(int j = i + 1; j < k; ++j){
		 		if(vector3[i] == vector3[j]) {
				if(vector1[vector2[i]] > vector1[vector2[j]]) {
		 			int temp = vector3[j];
		 			vector3[j] = vector3[i];
		 			vector3[i] = temp;
		 			temp = vector2[i];
		 			vector2[i] = vector2[j];
		 			vector2[j] = temp;
		 		}
		 	}
		 	}
		}
	}
	int putId(int vector1[], int vector2[], int k) {
		// puting the id for every player
		int p = 0;
		 	for(int i = 0; i < k; i++) {
		 		vector2[i] = i + 1;
		 		if(vector1[i] != 0) {
		 			++p;
		 		}
		 	}
		return p;
	}
	void initializeList(SkipList<Player> *list, int N, int vector1[], int id[]) {
		// initializing list with basic score
		for(int i = 0; i < N; ++i) {
			id[i] = i + 1;
			Player *p = new Player(i + 1, 0);
			list->addElem(*p);
			delete p;
		}
		 list->getPoz(vector1, N);
	}
	void getWinner(std::ifstream &infile, int N,
		SkipList<Player> *list, std::ofstream &outfile) {
		// made the game logic
		// get every score for every player
		// everytime made a new scoreboard
		std::string line;
		int v[N] = {0}, k = 1, id[N] = {0}, print[N] = {0},
		last_poz[N] = {0}, p = 0, ok = 0;
		initializeList(list, N, last_poz, id);
		getline(infile, line);
		while(!infile.eof()) {
		 	getline(infile, line);
		 	if(line == "print") {
		 		list->displayListprint(ok, N, print, outfile);
		 		outfile << '\n';
			} else {
		 	  v[0] = atoi(&line[0]);
		 		for(unsigned int j = 1; j < line.length() ; ++j) {
		 			if(line[j] != ' ' && line[j - 1] == ' ') {
		 				v[k] = atoi(&line[j]);
		 				k++;
		 				}
		 			}
		 		p = putId(v, id, k);
		 	  	sortVector(v, id, k);
		 	  	getPointsByLastPoz(v, id, last_poz, k);
		 	    int l = 0;
		 	    for (int i = 0; i < k; ++i) {
		 	    	int punctaj = p / 2 - l;
			 			if(p % 2 == 0 && punctaj == 0) {
			 				--punctaj;
			 				l++;
			 			}
		 	    	if(v[i] != 0) {
			 			Player *p = new Player(id[i], punctaj + list->searchInList(id[i]));
			 			list->deleteFromList(id[i], list->searchInList(id[i]));
			 			list->addElem(*p);
			 			l++;
			 			delete p;
			 		}
			 	}
			 	 list->getPoz(last_poz, N);
		 	}
			k = 1;
		 }
	}
	// reading from the input files
	// initializing the data structure
int main() {
	int  K = 0, N = 0, M = 0;

	std::ifstream infile("races.in");
	std::ofstream outfile("races.out");

	readDates(infile, K, M, N);
	SkipList<Player> *list = new SkipList<Player>(N);
	getWinner(infile, N, list, outfile);

	infile.close();
	outfile.close();

	delete list;
	return 0;
}
