#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <thread>
#include <chrono>

class Tableau { 
 private:
  std::vector<std::vector<char>> cells;
  int lignes;
  int colonnes;

 public:
  Tableau(int lignes, int colonnes) : lignes(lignes), colonnes(colonnes) {
    cells.resize(lignes, std::vector<char>(colonnes, ' '));
  }

  void setCell(int row, int col, char value) {
    if (row >= 0 && row < lignes && col >= 0 && col < colonnes) {
      cells[row][col] = value;
    }
  }

  char getCell(int row, int col) {
    if (row >= 0 && row < lignes && col >= 0 && col < colonnes) {
      return cells[row][col];
    }
  }

   
  int getlignes() const { return lignes; }

  int getCols() const { return colonnes; }

 /*  void afficher() const {
    std::vector<char> alphabet(26);
    for (int i = 0; i < 26; i++) {
      alphabet[i] = 'A' + i;
    }

    std::cout << "  ";
    for (int j = 1; j <= colonnes; ++j) {
      std::cout << " " << j << "  ";
    }
    std::cout << std::endl;

    for (int i = 0; i < lignes; ++i) {
      std::cout << "  ";
      for (int j = 0; j < colonnes; ++j) {
        std::cout << "+---";
      }
      std::cout << "+" << std::endl;

      for (int j = 0; j < colonnes; ++j) {
        if (j == 0) {
          std::cout << alphabet[i] << " ";
        }
        std::cout << "| " << cells[i][j] << " ";
      }
      std::cout << "|" << std::endl;
    }

    std::cout << "  ";
    for (int j = 0; j < colonnes; ++j) {
      std::cout << "+---";
    }
    std::cout << "+" << std::endl;
  }
 */
  
  void afficher() const {
        std::vector<char> alphabet(26);
        for (int i = 0; i < 26; i++) {
            alphabet[i] = 'A' + i;
        }

        std::cout << "  ";
        for (int j = 1; j <= colonnes; ++j) {
            std::cout << " " << j << "  ";
        }
        std::cout << std::endl;

        for (int i = 0; i < lignes; ++i) {
            std::cout << "  ";
            for (int j = 0; j < colonnes; ++j) {
                std::cout << "+---";
            }
            std::cout << "+" << std::endl;

            for (int j = 0; j < colonnes; ++j) {
                if (j == 0) {
                    std::cout << alphabet[i] << " ";
                }
                std::cout << "| ";
                
                if (cells[i][j] == 'H') {
                    std::cout << "\033[32m"; 
                }
                std::cout << cells[i][j] << " ";
                if (cells[i][j] == 'H') {
                    std::cout << "\033[0m"; 
                }
            }
            std::cout << "|" << std::endl;
        }

        std::cout << "  ";
        for (int j = 0; j < colonnes; ++j) {
            std::cout << "+---";
        }
        std::cout << "+" << std::endl;
    }
  
  void clear() {
    for (int i = 0; i < lignes; ++i) {
      for (int j = 0; j < colonnes; ++j) {
        cells[i][j] = ' ';
      }
    }
  }
};

class Animal {
 protected:
  int vie;
  int faim;
  int positionX;
  int positionY;

 public:
  Animal(int vie, int faim, int positionX, int positionY, char type)
      : vie(vie), faim(faim), positionX(positionX), positionY(positionY) {
    tableau.setCell(positionX, positionY, type);
  }
  static Tableau& tableau;

  virtual ~Animal() = default;

  virtual char type() const = 0;

  int getX() const { return positionX; }

  int getY() const { return positionY; }

  int getvie() const { return vie; }

  void setvie(int l) {
    vie = l;
    if (vie == 0) {
      tableau.setCell(positionX, positionY, ' ');
    }
  }

  int getfaim() const { return faim; }

  void setfaim(int h) { faim = h; }

  bool est_proche(const Animal& other) const {
    return abs(positionX - other.positionX) <= 1 &&
           abs(positionY - other.positionY) <= 1;
  }

  

  virtual void deplacer() = 0;
  virtual void manger(Animal& other) {}
};

class Herbe : public Animal {
 public:
  Herbe(int positionX, int positionY)
      : Animal(1, 0, positionX, positionY, type()) {}
  char type() const override {
    if (vie >= 1) {
      return 'H';
    } else {
      return 'S';
    }
  }

  void deplacer() override {}
};

class Mouton : public Animal {
public:
    Mouton(int positionX, int positionY)
        : Animal(50, 5, positionX, positionY, type()) {}

    char type() const override { return vie ? 'M' : ' '; }

    void deplacer() override {
        tableau.setCell(positionX, positionY, ' ');

        int direction = rand() % 8; 
        int newX = positionX;
        int newY = positionY;
        switch (direction) {
            case 0: if (newY > 0) newY--; break;
            case 1: if (newY > 0 && newX < tableau.getlignes() - 1) { newY--; newX++; } break;
            case 2: if (newX < tableau.getlignes() - 1) newX++; break;
            case 3: if (newY < tableau.getCols() - 1 && newX < tableau.getlignes() - 1) { newY++; newX++; } break;
            case 4: if (newY < tableau.getCols() - 1) newY++; break;
            case 5: if (newY < tableau.getCols() - 1 && newX > 0) { newY++; newX--; } break;
            case 6: if (newX > 0) newX--; break;
            case 7: if (newY > 0 && newX > 0) { newY--; newX--; } break;
        }

        if (tableau.containsHerbe(newX, newY)) {
            tableau.setCell(newX, newY, ' '); // Mange l'herbe
            faim = 5; // Réinitialiser la faim
        }

        positionX = newX;
        positionY = newY;
        tableau.setCell(positionX, positionY, type());

        faim--;
        vie--;
    }
};


class Loup : public Animal {
 public:
  Loup(int positionX, int positionY)
      : Animal(60, 10, positionX, positionY, type()) {}
  char type() const override { return vie ? 'L' : ' '; }

 void deplacer() override {
    
    tableau.setCell(positionX, positionY, ' ');

    
    int direction = rand() % 8; 
    switch (direction) {
      case 0: 
        if (positionY > 0) positionY--;
        break;
      case 1:  
        if (positionY > 0 && positionX < tableau.getlignes() - 1) {
          positionY--;
          positionX++;
        }
        break;
      case 2:  
        if (positionX < tableau.getlignes() - 1) positionX++;
        break;
      case 3:  
        if (positionY < tableau.getCols() - 1 &&
            positionX < tableau.getlignes() - 1) {
          positionY++;
          positionX++;
        }
        break;
      case 4:  
        if (positionY < tableau.getCols() - 1) positionY++;
        break;
      case 5:  
        if (positionY < tableau.getCols() - 1 && positionX > 0) {
          positionY++;
          positionX--;
        }
        break;
      case 6:  
        if (positionX > 0) positionX--;
        break;
      case 7:  
        if (positionY > 0 && positionX > 0) {
          positionY--;
          positionX--;
        }
        break;
    }
    
    
    if (tableau.getCell(positionX, positionY) != 'H') {
        
        tableau.setCell(positionX, positionY, type());
    } else {
       
        tableau.setCell(positionX, positionY, 'H');
    }
faim--;
vie--;
}

void manger(Animal& other) override {
if (other.type() == 'M' && est_proche(other)) {
other.setvie(0);
faim = 10; 
}
}
};

Tableau tab(12, 12);
Tableau& Animal::tableau = tab;

int main() {
srand(static_cast<unsigned>(time(0)));

tab.clear();

std::vector<Herbe> herbes;
for (int i = 0; i < tab.getlignes(); i++) {
    for (int j = 0; j < tab.getCols(); j++) {
        Herbe herbe(i, j);  
        herbes.push_back(herbe);  
        tab.setCell(i, j, 'H');
    }
}

std::vector<Mouton> moutons = {Mouton(0, 0), Mouton(1, 1), Mouton(7, 1), Mouton(9, 9), Mouton(11, 11), Mouton(10, 10), Mouton(8, 8), Mouton(6, 6), Mouton(4, 4), Mouton(3, 3), Mouton(2, 2)};

std::vector<Loup> loups = {Loup(4, 5), Loup(2, 6), Loup(11,11)};

int compteur_tour = 0;
// tab.afficher();
while (true) {
for (auto& mouton : moutons) {
      mouton.deplacer();
      for(auto& herbe : herbes){
        mouton.manger(herbe);
      }
      std::cout << "Faim du mouton = " << mouton.getfaim() << std::endl;
    }
    
    for (auto& loup : loups) {
      loup.deplacer();
      for (auto& mouton : moutons) {
        loup.manger(mouton);

      }
      std::cout << "Faim du loup = " << loup.getfaim() << std::endl;

    }
 


/*  for (int i = 0; i < moutons.size(); ++i) {
      if (moutons[i].getvie() <= 0 || moutons[i].getfaim() <= 0){
        for (int j = i; j < moutons.size() - 1; ++j) {
          moutons[j] = moutons[j + 1];
        }
        moutons.pop_back();
        --i;
      }
    } */



  /*    for (int i = 0; i < moutons.size(); ++i) {
      if (moutons[i].getvie() <= 0 || moutons[i].getfaim() <= 0){
        for (int j = i; j < moutons.size() - 1; ++j) {
          moutons[j] = moutons[j + 1];
        }
        moutons.pop_back();
        --i;
      }
    } */




    for (int i = 0; i < moutons.size(); ++i) {
        if (moutons[i].getvie() <= 0 || moutons[i].getfaim() <= 0){
            // Supprimer le mouton de la grille
            tab.setCell(moutons[i].getX(), moutons[i].getY(), ' ');

            for (int j = i; j < moutons.size() - 1; ++j) {
                moutons[j] = moutons[j + 1];
            }
            moutons.pop_back();
            --i;
        }
    }




      for(int i = 0; i < loups.size(); ++i) {
        if (loups[i].getvie() <= 0 || loups[i].getfaim() <= 0){
            // Supprimer le loup de la grille
            tab.setCell(loups[i].getX(), loups[i].getY(), ' ');

            for (int j = i; j < loups.size() - 1; ++j) {
                loups[j] = loups[j + 1];
            }
            loups.pop_back();
            --i;
        }
    }


/*  for(int i = 0; i < loups.size(); ++i) {
      if (loups[i].getvie() <= 0 || loups[i].getfaim() <= 0){
        for (int j = i; j < loups.size() - 1; ++j) {
          loups[j] = loups[j + 1];
        }
        loups.pop_back();
        --i;
      }
    }  */




for (int i = 0; i < herbes.size(); ++i) {
      if (herbes[i].getvie() <= 0){
        for (int j = i; j < herbes.size() - 1; ++j) {
          herbes[j] = herbes[j + 1];
        }
        herbes.pop_back();
        --i;
      }
    }
tab.afficher();


std::cout << "Tour: " << ++compteur_tour << std::endl;
std::cout << "Nombre de loups: " << loups.size() << std::endl;
std::cout << "Nombre de moutons: " << moutons.size() << std::endl;

std::this_thread::sleep_for(std::chrono::seconds(1));

if ( moutons.empty()) {
  break;
}

std::cout << "Ecosysteme terminé après " << compteur_tour << " tours."
<< std::endl;
}
return 0;
}
