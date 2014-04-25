#include "util.h"

Position::Position(size_t _x, size_t _y) :
    x(_x),
    y(_y)
{}

Position::Position(const Position &_other) :
    x(_other.x),
    y(_other.y)
{}

Position Position::operator =(const Position &_other)
{
    x = _other.x;
    y = _other.y;
    return *this;
}

void Position::print()
{
    cout << "x: " << x << ", y: " << y;
}

Table::Table(bool _turn, size_t n) :
    m_turn(_turn),
    m_size(n)
{
    m_table.resize(n*n, unplayed);
}

Table::Table(Table &other) :
    m_turn(other.m_turn),
    m_size(other.m_size)
{
    m_table = other.m_table;
}

bool Table::marcar(Position _pt)
{
    size_t pos = m_size*_pt.y + _pt.x;
    if(m_table[pos] != unplayed)
        return false;
    m_table[pos] = (m_turn) ? played_op : played_me;
    m_turn = !m_turn;
    return true;
}

void Table::check(float &_w1, float &_w2)
{
    _w1 = _w2 = 0.0;
    float   _step = 1.0 / m_size,
            _w1_aux, _w1_aux2,
            _w2_aux, _w2_aux2;

    //Verificar columnas completas
    _w1_aux = _w2_aux = 0.0;
    for(size_t i = 0; i < m_size; ++i) {
        if(m_table[i] != unplayed) {
            _w1_aux2 = _w2_aux2 = 0.0;
            for(size_t j = 0; j < m_size; ++j) {
                if(m_table[j*m_size + i] != m_table[i])
                    break;
                if(m_table[i] == played_me) {

                    _w1_aux2 = (j == m_size-1) ? 1.0 : _w1_aux2 + _step;
                }
                else {
                    _w2_aux2 = (j == m_size-1) ? 1.0 : _w2_aux2 + _step;
                }
            }
            if(_w1_aux2 > _w1_aux)
                _w1_aux = _w1_aux2;
            if(_w2_aux2 > _w2_aux)
                _w2_aux = _w2_aux2;
        }
    }
    if(_w1_aux > _w1)
        _w1 = _w1_aux;
    if(_w2_aux > _w2)
        _w2 = _w2_aux;

    //Verificar filas completas
    _w1_aux = _w2_aux = 0.0;
    for(size_t j = 0; j < m_size; ++j) {
        if(m_table[j*m_size] != unplayed) {
            _w1_aux2 = _w2_aux2 = 0.0;
            for(size_t i = 0; i < m_size; ++i) {
                if(m_table[j*m_size + i] != m_table[j*m_size])
                    break;
                if(m_table[j*m_size] == played_me)
                    _w1_aux2 = (i == m_size-1) ? 1.0 : _w1_aux2 + _step;
                else
                    _w2_aux2 = (i == m_size-1) ? 1.0 : _w2_aux2 + _step;
            }
            if(_w1_aux2 > _w1_aux)
                _w1_aux = _w1_aux2;
            if(_w2_aux2 > _w2_aux)
                _w2_aux = _w2_aux2;
        }
    }
    if(_w1_aux > _w1)
        _w1 = _w1_aux;
    if(_w2_aux > _w2)
        _w2 = _w2_aux;

    //Verificar primera diagonal
    _w1_aux = _w2_aux = 0.0;
    if(m_table[0] != unplayed) {
        _w1_aux2 = _w2_aux2 = 0.0;
        for(size_t i = 1; i < m_size; ++i){
            if(m_table[i*m_size+i] != m_table[0])
                break;
            if(m_table[0] == played_me)
                _w1_aux2 = (i == m_size-1) ? 1.0 : _w1_aux2 + _step;
            else
                _w2_aux2 = (i == m_size-1) ? 1.0 : _w2_aux2 + _step;
        }
        if(_w1_aux2 > _w1_aux)
            _w1_aux = _w1_aux2;
        if(_w2_aux2 > _w2_aux)
            _w2_aux = _w2_aux2;
    }
    if(_w1_aux > _w1)
        _w1 = _w1_aux;
    if(_w2_aux > _w2)
        _w2 = _w2_aux;

    //Verificar segunda diagonal
    _w1_aux = _w2_aux = 0.0;
    size_t _edge = m_size-1;
    if(m_table[_edge] != unplayed) {
        _w1_aux2 = _w2_aux2 = 0.0;
        for(size_t i = 1; i < m_size; ++i) {
            if(m_table[i*m_size + _edge-i] != m_table[_edge])
                break;
            if(m_table[_edge] == played_me)
                _w1_aux2 = (i == _edge) ? 1.0 : _w1_aux2 + _step;
            else
                _w2_aux2 = (i == _edge) ? 1.0 : _w2_aux2 + _step;
        }
        if(_w1_aux2 > _w1_aux)
            _w1_aux = _w1_aux2;
        if(_w2_aux2 > _w2_aux)
            _w2_aux = _w2_aux2;
    }
    if(_w1_aux > _w1)
        _w1 = _w1_aux;
    if(_w2_aux > _w2)
        _w2 = _w2_aux;
}

void Table::availablePositions(vector<Position> &_pos)
{
    _pos.clear();
    Position tmp;
    for(size_t i = 0; i < m_size; ++i) {
        for(size_t j = 0; j < m_size; ++j) {
            if(m_table[i * m_size + j] == unplayed) {
                tmp.y = i;
                tmp.x = j;
                _pos.push_back(tmp);
            }
        }
    }
}

float Table::weight()
{
    float w1, w2;
    check(w1, w2);
    return 2*w1-3*w2;
}

GameTreeNode::GameTreeNode(Table &_table, bool _minMax) :
    m_table(0),
    m_positions(new vector<Position>),
    m_weight(0),
    m_minmax_bool(_minMax)
{
    m_table = new Table(_table);
}

GameTreeNode::~GameTreeNode()
{
    if(m_positions) {
        delete m_positions;
        m_positions = NULL;
    }
    for(vector<GameTreeNode*>::iterator it = m_children.begin();
        it != m_children.end();
        ++it) {
        delete (*it);
    }
    if(m_table) {
        delete m_table;
        m_table = NULL;
    }
}

void GameTreeNode::buildTree(size_t _level, bool _notRoot)
{
    m_table->availablePositions(*m_positions);
    if(_level == 0 || m_positions->empty()) { //Alcanzó la profundidad deseada o ya no hay movimientos posibles
//        if(m_positions.empty())
//            cout << "No más movimientos disponibles" << endl; ////*********DEBUG*********////
        m_weight = m_table->weight();
        cout << "Peso: " << m_weight << endl; ////*********DEBUG*********////
        return;
    }
    if(_level > 0) {
        ////*********DEBUG*********////
//        cout << "Entro por nivel: " << _level << endl;
//        cout << "Posiciones (" << m_positions.size() << ") {";
//        for(size_t i = 0; i < m_positions.size(); ++i) {
//            m_positions[i].print(); cout << " | ";
//        }
//        cout << "}" << endl;
//        ////*********DEBUG*********////
        GameTreeNode *tmp;
        for(size_t i = 0; i < m_positions->size(); ++i) {
            tmp = new GameTreeNode(*m_table, !m_minmax_bool);
            tmp->playPosition((*m_positions)[i]);
            tmp->buildTree(_level-1);
//            cout << "Árbol construido" << endl; ////*********DEBUG*********////
            m_children.push_back(tmp);
//            cout << "Árbol añadido: " << _level << endl; ////*********DEBUG*********////
        }
//        cout << "Sale del nivel: " << _level << endl; ////*********DEBUG*********////
    }
//    cout << "Antes del delete en nivel: " << _level << endl; ////*********DEBUG*********////
    delete m_table;
    if(_notRoot && m_positions) {
        delete m_positions;
        m_positions = NULL;
    }
//    cout << "Después del delete" << endl; ////*********DEBUG*********////
}

void GameTreeNode::playPosition(Position &_pos)
{
    m_table->marcar(_pos);
}

Position &GameTreeNode::getPositionAt(size_t _pos)
{
    if(_pos < m_positions->size())
        return (*m_positions)[_pos];
    cout << "No finalizó" << endl; ////*********DEBUG*********////
    return (*m_positions)[0];  //MEJOR LANZAR EXCEPCIÓN
}

size_t GameTreeNode::getMaxMin()
{
    if(m_children.empty())  //Retorna su peso si es que el nodo ya no tiene hijos
        return 0;   //******PUEDE CAUSAR PROBLEMAS SI EL NIVEL DEL ÁRBOL ES 0******//
    m_children[0]->getMaxMin();
    int     tmp0 = m_children[0]->m_weight,   //Almacena el mayor/menor valor
            tmp1;
    size_t pos = 0;
    for(size_t i = 1; i < m_children.size(); ++i) {
        m_children[i]->getMaxMin();
        tmp1 = m_children[i]->m_weight;
        if(m_minmax_bool) {
            if(tmp1 > tmp0) {
                tmp0 = tmp1;
                pos = i;
            }
        }
        else {
            if(tmp1 < tmp0) {
                tmp0 = tmp1;
                pos = i;
            }
        }
    }
    m_weight = tmp0;
    return pos;
}

GameTree::GameTree(Table &_root) :
    m_root(new GameTreeNode(_root))
{}

GameTree::~GameTree()
{
    if(m_root)
        delete m_root;
}

void GameTree::build(size_t _level)
{
//    cout << "Build Tree" << _level << endl; ////*********DEBUG*********////
    m_root->buildTree(_level, false);
//    cout << "End build tree" << _level << endl; ////*********DEBUG*********////
}

Position &GameTree::minMax()
{
    return m_root->getPositionAt(m_root->getMaxMin());
}

Game::Game(bool _start_me, size_t _n) :
    m_gameTable(_start_me,_n)
{}

void Game::play(Position &_pos)
{
    if(_pos.x < m_gameTable.m_size && _pos.y < m_gameTable.m_size)
        m_gameTable.marcar(_pos);
    else
        return; //RETORNAR UNA EXCEPCIÓN PODRÍA SER MEJOR OPCIÓN

//****FUNCIONES ALTERNATIVAS, BORRARLAS CUANDO SE PUEDA****//
    draw();
    status winner = checkWinner();
    switch (winner) {
    case played_me:
        cout << "I won!" << endl;
        break;
    case played_op:
        cout << "You won!" << endl;
        break;
    default:
        break;
    }
    cout << endl;
//*********************************************************//
}

void Game::bestPlay(size_t _sizeOfTree)
{
    GameTree __gameTree(m_gameTable);
    __gameTree.build(_sizeOfTree);
    play(__gameTree.minMax());
////Posición al azar
//        vector<Position> pos;
//        m_gameTable.availablePositions(pos);
//        return pos[rand() % pos.size()];
}

status Game::checkWinner()
{
    float   w1,
            w2;
    m_gameTable.check(w1, w2);
    if(w1 >= 1.0)
        return played_me;
    if(w2 >= 1.0)
        return played_op;
    return unplayed;
}

void Game::draw()
{
    for(size_t i = 0; i < m_gameTable.m_size; ++i) {
        for(size_t j = 0; j < m_gameTable.m_size; ++j) {
            status val = m_gameTable.m_table[i*m_gameTable.m_size + j];
            switch (val) {
            case unplayed:
                cout << "- ";
                break;
            case played_me:
                cout << "x ";
                break;
            case played_op:
                cout << "/ ";
                break;
            default:
                break;
            }
        }
        cout << endl;
    }
}
