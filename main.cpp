#include <bits/stdc++.h>
#include <sys/time.h>
#include <atcoder/all>

using namespace std;

#define n_kinds 26

struct RandGenerator {
    random_device seed_gen;
    mt19937 engine;
    mt19937_64 engine64;
    RandGenerator() : engine(seed_gen()), engine64(seed_gen()) {}
    int rand(int mod) {
        return engine() % mod;
    }
    long long randll(long long mod) {
        return engine64() % mod;
    } 
} ryuka;

struct Timer {
    double global_start;
    double gettime() {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return tv.tv_sec + tv.tv_usec * 1e-6;
    }
    void init() {
        global_start = gettime();
    }
    double elapsed() {
        return gettime() - global_start;
    }
} toki;

struct Input {
    int D;
    vector<long long> c;
    vector<vector<long long>> s;
    void read() {
        cin >> D;
        c.resize(n_kinds);
        s.resize(D, vector<long long>(n_kinds));
        for(int i = 0; i < n_kinds; i++) cin >> c[i];
        for(int i = 0; i < D; i++) {
            for(int j = 0; j < n_kinds; j++) cin >> s[i][j];
        }
    }
} input;

struct Output {
    vector<int> t;
    Output() : t(vector<int>(input.D, 0)) {}
    void print() {
        for(int i = 0; i < input.D; i++) {
            cout << t[i]+1 << "\n";
        }
    }
};

namespace Utils {
    long long calcScore(const Output& output) {
        long long res = 0;
        vector<int> last(n_kinds, 0);
        for(int i = 0; i < input.D; i++) {
            res += input.s[i][output.t[i]];
            last[output.t[i]] = i + 1;
            for(int j = 0; j < n_kinds; j++) {
                res -= input.c[j] * (i+1 - last[j]);
            }
        }
        return res;
    }
};

template<class STATE>
struct IterationControl {
    int iteration_counter;
    int swap_counter;
    double average_time;
    double start_time;
    IterationControl() : iteration_counter(0), swap_counter(0) {}
    STATE climb(double time_limit, STATE initial_state) {
        start_time = toki.gettime();
        average_time = 0;
        STATE best_state = initial_state;
        cerr << "[INFO] - IterationControl::climb - Starts climbing..." << endl;
        while(toki.gettime() - start_time + average_time < time_limit) {
            STATE current_state = STATE::generateState(best_state);
            if(current_state.score > best_state.score) {
                swap(best_state, current_state);
                swap_counter++;
            }
            iteration_counter++;
            average_time = (toki.gettime() - start_time) / iteration_counter;
        }
        cerr << "[INFO] - IterationControl::climb - Iterated " << iteration_counter << " times and swapped " << swap_counter << " times.\n";
        return best_state;
    }
};

struct State {
    Output output;
    long long score;   
    State() : score(-(1LL<<60)) {}
    static State initState() {
        State res;
        for(int i = 0; i < input.D; i++) {
            res.output.t[i] = ryuka.rand(n_kinds);
        }
        res.score = Utils::calcScore(res.output);
        return res;
    }
    static State generateState(const State& input_state) {
        State res = input_state;
        int i = ryuka.rand(input.D);
        res.output.t[i] = ryuka.rand(n_kinds);
        res.score = Utils::calcScore(res.output);
        return res;
    }
};

int main() {
    toki.init();
    input.read();   
    IterationControl<State> sera;
    State best = sera.climb(1.8, State::initState());
    best.output.print();
    cerr << "[INFO] - main - MyScore = " << best.score << "\n";
}
