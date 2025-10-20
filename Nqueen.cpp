#include <iostream>
#include <ctime>
#include <vector>
#include <algorithm>
#include <random>
#include <numeric>
#include <iostream>
#include <fstream>
#include <queue> 
#include <map>
#include <utility>

using namespace std;

int IDS_time_limit = 10000;
vector<int> getRandomRoot(int N);
int getAttackPairs(vector<int> board); 
bool isSafe(vector<int> board, int N, int row, int col); 
void printBoard(vector<int> board, int N); 
vector<int> HC(int N); 
void HCRepeat(int N, int times); 
void IDSRepeat(int N, int times); 
vector<int> IDS(int N); 
vector<int> DLS(vector<int> board, int row, int N, int maxDepth, chrono::steady_clock::time_point startTime);  
vector<int> mutate(vector<int> board, int N);  
void GARepeat(int N, int times); 
vector<int> GA(int N); 
bool cmp (const vector<int>& a, const vector<int>& b); 
vector<vector<int>> cycleCrossover(const vector<int>& board1, const vector<int>& board2, int N); 
vector<int> IDS_best; 

using namespace std; 

int main() {
    
    int N, T, M; 
    cout << "Enter algorithm type (1 for IDS, 2 for HC, 3 for GA): "; 
    cin >> M; 
    cout << "Enter number of queens: "; 
    cin >> N; 
    cout << "Enter number of trials: "; 
    cin >> T; 
    
    if (M == 1) {
        IDSRepeat(N, T); 
    } else if (M == 2) {
        HCRepeat(N, T); 
    } else if (M == 3) {
        GARepeat(N, T); 
    }
    return 0; 
}

void IDSRepeat(int N, int times) {
    ofstream IDSResult("IDSResult.txt"); 
    for (int i = 0; i < times; i++) {
        auto start = chrono::steady_clock::now();
        vector<int> ans = IDS(N); 
        auto end = chrono::steady_clock::now();
        auto diff = end - start;
        cout << "\n===========================\n"; 
        cout << "Trial: " << i << "\n"; 
        if (!ans.empty()) {
            cout << "Answer found: \n";
            printBoard(ans, N); 
            cout << getAttackPairs(ans) << " attacks \n"; 
        } else cout << "No answer\n"; 
        cout << "Number of queens: " << N << "\n"; 
        cout << chrono::duration <double> (diff).count() << " seconds";
        cout << "\n===========================\n"; 
        IDSResult << getAttackPairs(ans) << "," << chrono::duration <double> (diff).count() << "\n"; 
    }
    IDSResult.close(); 
}

void HCRepeat(int N, int times) {
    ofstream HCResult("HCResult.txt"); 
    for (int i = 0; i < times; i++) {
        auto start = chrono::steady_clock::now();
        vector<int> ans = HC(N); 
        auto end = chrono::steady_clock::now();
        auto diff = end - start;
        cout << "\n===========================\n"; 
        cout << "Trial: " << i << "\n"; 
        if (!ans.empty()) {
            cout << "Answer found: \n";
            printBoard(ans, N); 
            cout << getAttackPairs(ans) << " attacks \n"; 
        } else cout << "No answer\n"; 
        cout << "Number of queens: " << N << "\n"; 
        cout << chrono::duration <double> (diff).count() << " seconds";
        cout << "\n===========================\n"; 
        HCResult << getAttackPairs(ans) << "," << chrono::duration <double> (diff).count() << "\n"; 
    }
    HCResult.close(); 
}

int isAttack(vector<int> board, int i, int N) {
    for (int j = 0; j < N; j++) {
        if (i!=j && (abs(board[i] - board[j]) == abs(i-j))) return 1; 
    }
    return 0; 
}

vector<int> IDS(int N) {
    if (N < 4) return {}; 
    vector<int> root;
    int i = 0; 
    while (2*i < N) root.push_back(2*i++); 
    i = 0; 
    while (2*i+1 < N) root.push_back(2*(i++)+1); 
    
    vector<int> ans; 
    IDS_best = root; 
    auto startTime = chrono::steady_clock::now(); 
    for (int i = 0; i < N+1; i++) {
        ans = DLS(root, 0, N, i, startTime); 
        if (!ans.empty()) return ans; 
    }
    return ans; 
}

vector<int> DLS(vector<int> board, int row, int N, int maxDepth, chrono::steady_clock::time_point startTime) {
    vector<int> found; 
    auto curTime = chrono::steady_clock::now();
    auto diff = curTime - startTime; 
    if (getAttackPairs(board) < getAttackPairs(IDS_best)) IDS_best = board; 
    if (chrono::duration <double> (diff).count() > IDS_time_limit) {         
        return IDS_best; 
    }
    if (row == maxDepth && getAttackPairs(board) == 0) return board; 
    else if (row < maxDepth) {
        int j = board[row];
        for (int i = 0; i < N; i++) {
            if (isSafe(board, N, row, i)) {
                board[row] = i; 
                cout << "row limit: " << maxDepth << ", row: " << row << ", attack: " << getAttackPairs(board) << "\n"; 
                // printBoard(board, N); 
                found = DLS(board, row+1, N, maxDepth, startTime); 
                if (!found.empty()) return found; 
                board[row] = j; 
            }
        }
    }
    return found; 
}

vector<int> getRandomRoot(int N) {
    vector<int> root(N); 
    for (int i = 0; i < N; i++) root[i] = i; 
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(root.begin(), root.end(), std::default_random_engine(seed));
    return root; // root[i] is the col of queen in row i
} 

int getAttackPairs(vector<int> board) {
    int total = 0; 
    int N = board.size(); 
    for (int i = 0; i < N; i++) {
        for (int j = i+1; j < N; j++) {
            if (board[i] == board[j]) total += 1; // Check vertical attacks
            if (abs(board[i] - board[j]) == abs(i-j)) total += 1; // Check diagonal attacks 
        }
    }
    return total; 
}

bool isSafe(vector<int> board, int N, int row, int col) {
    // Check if the new queen in 'row' at 'col' conflicts with existing queens
    for (int i = 0; i < row; i++) {
        if (board[i] == col || abs(board[i] - col) == abs(i - row)) {
            return false; // Conflicts with an existing queen
        }
    }
    return true;
}

void printBoard(vector<int> board, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i] == j) cout << "Q ";
            else cout << ". ";
        }
        cout << "\n";
    }
    cout << "\n";
}

vector<int> HC(int N) {
    if (N < 4) return {}; 
    vector<int> cur = getRandomRoot(N); 
    vector<int> ans = cur; 
    for (int i = 0; i < 100000; i++) { 
        int s = 0; 
        while (getAttackPairs(cur) != 0) {
            bool swapped = false; 
            int swapj, swapk;  
            int curAttack = getAttackPairs(cur); 
            for (int j = 0; j < N-1; j++) {
                for (int k = j+1; k < N; k++) {
                    swap(cur[j], cur[k]); 
                    if (getAttackPairs(cur) < curAttack) {
                        swapj = j; 
                        swapk = k; 
                        swapped = true; 
                        curAttack = getAttackPairs(cur); 
                    }
                    swap(cur[j], cur[k]); 
                }
            }
            if (!swapped) break;
            swap(cur[swapj], cur[swapk]); 
            cout << "climb #" << i << ", step #" << s++ << ", attack: " << getAttackPairs(cur) << "\n"; 
            //printBoard(cur, N);
        }
        if (getAttackPairs(cur) < getAttackPairs(ans)) ans = cur; 
        if (getAttackPairs(ans) == 0) break;
        cur = getRandomRoot(N); 
    }
    return ans; 
}

vector<int> mutate(vector<int> board, int N) {
    int i = rand()%N; 
    int j = rand()%N; 
    while (i == j) j = rand()%N; 
    swap(board[i], board[j]); 
    return board; 
}

vector<vector<int>> cycleCrossover(const vector<int>& board1, const vector<int>& board2, int N) {
    vector<int> cycle(N, 0);
    vector<int> offspring1(N);
    vector<int> offspring2(N);
    vector<int> offsprings; 
    bool side = 1; 
    int index = 0; 
    while (1) {
        while (cycle[index] && index < N) index++; 
        if (index == N) break;
        while (cycle[index] == 0) {
            cycle[index] = 1; 
            if (side) {
                offspring1[index] = board1[index]; 
                offspring2[index] = board2[index]; 
            } else {
                offspring2[index] = board1[index]; 
                offspring1[index] = board2[index]; 
            }
            int nextIndex = 0; 
            while (board1[nextIndex] != board2[index]) nextIndex++; 
            index = nextIndex; 
        }
        side = !side; 
    }
    return {offspring1, offspring2}; 
}

void GARepeat(int N, int times) {
    ofstream GAResult("GAResult.txt"); 
    for (int i = 0; i < times; i++) {
        auto start = chrono::steady_clock::now();
        vector<int> ans = GA(N); 
        auto end = chrono::steady_clock::now();
        auto diff = end - start;
        cout << "\n===========================\n"; 
        cout << "Trial: " << i << "\n"; 
        if (!ans.empty()) {
            cout << "Answer found: \n";
            printBoard(ans, N); 
            cout << getAttackPairs(ans) << " attacks \n"; 
        } else cout << "No answer\n"; 
        cout << "Number of queens: " << N << "\n"; 
        cout << chrono::duration <double> (diff).count() << " seconds";
        cout << "\n===========================\n"; 
        GAResult << getAttackPairs(ans) << "," << chrono::duration <double> (diff).count() << "\n"; 
    }
    GAResult.close(); 

}

vector<int> GA(int N) {
    int populationSize = 1000; 
    int parentSize = populationSize/20; 
    double mutationPr = 0.5; 
    vector<vector<int>> population; 
    srand(time(NULL)); 
    
    // Make initial population
    for (int i = 0; i < populationSize; i++) {
        population.push_back(getRandomRoot(N)); 
    }
    std::sort(population.begin(), population.end(), cmp);
    int gen = 0;
    if (N < 4) return {};  
    while (getAttackPairs(population[0]) != 0) {
        vector<vector<int>> newPopulation; 
        for (int i = 0; i < populationSize; i++) {
            int k = rand()%parentSize; 
            int m = rand()%parentSize; 
            vector<vector<int>> child = cycleCrossover(population[k], population[m], N); 
            double x = (double) rand() / (RAND_MAX + 1.0);
            if (x < mutationPr) child[0] = mutate(child[0], N);
            x = (double) rand() / (RAND_MAX + 1.0);
            if (x < mutationPr) child[1] = mutate(child[1], N);
            newPopulation.push_back(child[0]); 
            newPopulation.push_back(child[1]); 
        }
        population = newPopulation; 
        std::sort(population.begin(), population.end(), cmp); 
        cout << "gen: " << gen++ << ", attack: " << getAttackPairs(population[0]) << "\n"; 
        //printBoard(population[0], N); 
    } 
    return population[0]; 
}

bool cmp (const vector<int>& a, const vector<int>& b) {
    return getAttackPairs(a) < getAttackPairs(b);
}



