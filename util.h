#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <cstdlib>
#include <iostream>
using namespace std;

enum status {
    unplayed,
    played_me,
    played_op
};

struct Position {
    size_t  x,
            y;
    Position(size_t _x = 0, size_t _y = 0);
    Position (const Position &_other);
    Position operator =(const Position &_other);

    void print();   //SÓLO POR FINES DEMOSTRATIVOS
};

class Game;

class Table {
    friend class Game;
public:
    Table(bool _turn, size_t n);
    Table(Table &other);

    bool marcar(Position _pt);   //True si la casilla estaba libre
    void check(size_t &_w1, size_t &_w2);   //nRaya completos: _w1 -> míos, _w2 -> oponente
    void availablePositions(vector<Position> &_pos); //_pos se vacía y se llena con posiciones disponibles
    int weight();

private:
    bool m_turn;    //Indicador del jugador al que le corresponde el turno (0: yo, 1: oponente)
    size_t m_size;
    vector<status> m_table;
};

class GameTreeNode {
public:
    GameTreeNode(Table &_table, bool _minMax = true);
    ~GameTreeNode();
    void buildTree(size_t _level);
    void playPosition(Position &_pos);
private:
    Table *m_table;
    vector<Position> m_positions;
    vector<GameTreeNode> m_children;
    int m_weight;
    bool m_minmax_bool; //True -> Max, False -> Min
};

class GameTree {
public:
    GameTree(Table &_root);
    ~GameTree();
    void build(size_t _level);
    Position minMax();
private:
    GameTreeNode *m_root;
};

class Game {
public:
    Game(bool _start_me = 0, size_t _n = 3);
    void play(Position &_pos);
    Position &bestPlay();
    void playBestPlay();
    status checkWinner(); //Debe retornar status, por lo pronto mostrar en consola
    void draw();

private:
    Table m_gameTable;  //Tablero de Juego
};

#endif // UTIL_H
