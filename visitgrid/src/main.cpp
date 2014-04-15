// find out the number of ways to visit an N by M matrix.

#include <iostream>
#include <list>
#include <sstream>

using namespace std;

struct Pos {
    int r,c;
    Pos(int r=0, int c=0): r(r), c(c) {}
    Pos operator+(const Pos& a) const {
        return Pos(r+a.r, c+a.c);
    }
};
const int NDIRECTIONS = 8;
Pos Directions[NDIRECTIONS] = {Pos(1,1), Pos(1,0), Pos(0,1), Pos(-1, -1), Pos(-1,0), Pos(0,-1), Pos(1, -1), Pos(-1,1)};

struct Node {
    bool visited;
    Node():visited(false){}
};
template <int NROW, int NCOL>
struct Grid {
    enum {ROWS = NROW, COLS = NCOL, LEN=NROW*NCOL};
    Node nodes[NROW][NCOL];
    Node& operator() (int i, int j) {
        return nodes[i][j];
    }
    Node& operator() (const Pos& p) {
        return nodes[p.r][p.c];
    }
    Node* at(int i, int j) {
        if(i>=0 && i<ROWS && j>=0 && j<COLS)
            return &nodes[i][j];
        return NULL;
    }
};
template <int NROW, int NCOL>
struct Visitor {
    typedef Grid<NROW, NCOL>        ThisGrid;
    ThisGrid          g;

    typedef list<Pos> PosStack;
    PosStack    stackPos;
    int nSolutions;

    Visitor(): nSolutions(0){}

    int start(int i=0, int j=0) {
        return visit(Pos(0,0), 0);
    }

    int visit(const Pos& cur, int level) {
        if(g(cur).visited) return 0;

        g(cur).visited = true;
        stackPos.push_back(cur);
        level++;
        int n = 0;

        for(int i=0; i<NDIRECTIONS; ++i) {
            if( available(cur+Directions[i]) )
                n += visit(cur+Directions[i], level);
        }
        if( n == 0 && level == ThisGrid::LEN ) {
            n = 1;
            ++nSolutions;
//            cout << "-- " << nSolutions << " ---" << endl << printStack() << endl;
        }
        g(cur).visited = false;
        stackPos.pop_back();
        return n;
    }
    bool available(const Pos& p) {
        if( Node *node = g.at(p.r, p.c) ) {
            if( !node->visited ) return true;
        }
        return false;
    }

    string printStack()const {
        stringstream ss;
        int i = 0;
        for(PosStack::const_iterator it=stackPos.begin(); it!=stackPos.end(); ++it) {
            ss << ++i << ": (" << it->r << "," << it->c << ")" << endl;
        }

        return ss.str();
    }
};

int main(int argc, const char *argv)
{
    Visitor<2, 10> v;
    v.start();
    cout << v.nSolutions << endl;
    cout << "program end" << endl;
    return 0;
}
