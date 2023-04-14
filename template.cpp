#include <bits/stdc++.h>
#include <sys/time.h>
#include <atcoder/all>

using namespace std;

#define n_kinds 26

/*乱数生成器*/
struct RandGenerator {
    random_device seed_gen;
    mt19937 engine;
    mt19937_64 engine64;
    static const int pshift = 1000000000;
    RandGenerator() : engine(seed_gen()), engine64(seed_gen()) {}
    /*mod以下の乱数を返す（32bit）*/
    int rand(int mod) {
        return engine() % mod;
    }
    /*mod以下の乱数を返す（64bit）*/
    long long randll(long long mod) {
        return engine64() % mod;
    } 
    /*確率pでTrueを返す*/
    bool pjudge(double p) {
        int p_int;
        if(p > 1) p_int = pshift;
        else p_int = p * pshift;
        return rand(pshift) < p_int;
    }
} ryuka;

/*タイマー*/
struct Timer {
    double global_start;
    /*現在の時刻を返す*/
    double gettime() {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return tv.tv_sec + tv.tv_usec * 1e-6;
    }
    void init() {
        global_start = gettime();
    }
    /*プログラム開始からの経過時間を返す*/
    double elapsed() {
        return gettime() - global_start;
    }
} toki;

struct Input {
    /*TODO: ここに入力変数を定義する*/
    void read() {
        /*TODO: ここで入力を受け取る*/
    }
} input;

struct Output {
    /*TODO: ここに出力変数を定義する*/
    Output() {
        /*TODO：ここで出力変数を初期化する。vectorのメモリ確保など*/
    }
    void print() {
        /*TODO：ここで答えを出力する*/
    }
};

namespace Utils {
    long long calcScore(const Output& output) {
        long long res = 0;
        /*TODO: ここでスコアを計算する*/
        return res;
    }
};

/*イテレーション管理クラス*/
template<class STATE>
struct IterationControl {
    int iteration_counter;
    int swap_counter;
    double average_time;
    double start_time;
    IterationControl() : iteration_counter(0), swap_counter(0) {}
    /*山登り法*/
    STATE climb(double time_limit, STATE initial_state) {
        start_time = toki.gettime();
        average_time = 0;
        STATE best_state = initial_state;
        double time_stamp = start_time;
        cerr << "[INFO] - IterationControl::climb - Starts climbing...\n";
        while(time_stamp - start_time + average_time < time_limit) {
            STATE current_state = STATE::generateState(best_state);
            if(current_state.score > best_state.score) {
                swap(best_state, current_state);
                swap_counter++;
            }
            iteration_counter++;
            time_stamp = toki.gettime();
            average_time = (time_stamp - start_time) / iteration_counter;
        }
        cerr << "[INFO] - IterationControl::climb - Iterated " << iteration_counter << " times and swapped " << swap_counter << " times.\n";
        return best_state;
    }
    /*焼きなまし法*/
    STATE anneal(double time_limit, double temp_start, double temp_end, STATE initial_state) {
        start_time = toki.gettime();
        average_time = 0;
        STATE best_state = initial_state;
        double elapsed_time = 0;
        cerr << "[INFO] - IterationControl::anneal - Starts annealing...\n";
        while(elapsed_time + average_time < time_limit) {
            double normalized_time = elapsed_time / time_limit;
            double temp_current = pow(temp_start, 1.0 - normalized_time) * pow(temp_end, normalized_time);
            STATE current_state = STATE::generateState(best_state);
            long long delta = current_state.score - best_state.score;
            if(delta > 0 || ryuka.pjudge(exp(1.0 * delta / temp_current)) ) {
                swap(best_state, current_state);
                swap_counter++;
            }
            iteration_counter++;
            elapsed_time = toki.gettime() - start_time;
            average_time = elapsed_time / iteration_counter;
        }
        cerr << "[INFO] - IterationControl::anneal - Iterated " << iteration_counter << " times and swapped " << swap_counter << " times.\n";
        return best_state;
    }
};

/*解を管理するクラス*/
struct State {
    Output output;
    long long score;
    State() : score(-(1LL<<60)) {}
    static State initState() {
        State res;
        /*TODO: ここで初期解を作成する*/
        res.score = Utils::calcScore(res.output);
        return res;
    }
    static State generateState(const State& input_state) {
        State res = input_state;
        /*TODO: ここでinput_stateを変化させた解を作る（局所探索）*/
        res.score = Utils::calcScore(res.output);
        return res;
    }
};

int main() {
    toki.init();
    input.read();   
    IterationControl<State> sera;
    /*山登りの場合は、山登りする時間を第一引数で渡す*/
    State ans = sera.climb(1.8, State::initState());
    /*焼きなましの場合は、焼きなます時間を第一引数で、初期温度を第二引数で、終了温度を第三引数で渡す*/
    State ans = sera.anneal(1.8, 2e3, 6e2, State::initState());
    ans.output.print();
    cerr << "[INFO] - main - MyScore = " << ans.score << "\n";
}
