#include <iostream>
#include <cmath>

using namespace std;

#define LINE_CAPACITY 10
#define AVERAGE_PACKET_LENGTH 1
#define MIN_LOAD 0.1
#define STEP 0.1
#define MAX_LOAD 1.2
#define TOTAL_PACKETS 2000
#define BUFFER_SIZE 100

class Router {
private:
    double* buffer;
    int head, tail;
public:
    Router() {
        buffer = new double[BUFFER_SIZE];
        head = tail = 0;
    }

    void enQueue(double packet_length) {
        buffer[tail] = packet_length;
        tail++;
    }

    void transmit() {
        for(int i = 0; i < tail - 1; i++) {
            buffer[i] = buffer[i + 1];
        }
        tail--;
    }

    double calculate_waiting_time() {
        double waiting_time = 0;
        for(int i = head; i < tail; i++)
            waiting_time += buffer[i]/LINE_CAPACITY;
        return waiting_time;
    }
};

double generate_exp_rv(double average);

void run_system_markovian(double average_l, double average_t, double *avg_waiting_time);

void run_system_deterministic(double average_l, double average_t, double *avg_waiting_time);

void calculate_theoretical(double average_l, double p, double *avg_waiting_time);

int main() {
    srand(time(NULL));
    double avg_waiting_times_mark[12];
    double avg_waiting_times_det[12];
    double avg_waiting_times_the[12];
    unsigned int loop = 0;
    double p = MIN_LOAD;

    cout << "\np\t\tM/M/1/K(SIM)\t\tM/D/1/K(SIM)\t\tM/M/1/K(THE)\n\n";

    while (p <= MAX_LOAD) {
        run_system_markovian(AVERAGE_PACKET_LENGTH, p*AVERAGE_PACKET_LENGTH*LINE_CAPACITY, &avg_waiting_times_mark[loop]);
        run_system_deterministic(AVERAGE_PACKET_LENGTH, p*AVERAGE_PACKET_LENGTH*LINE_CAPACITY, &avg_waiting_times_det[loop]);
        calculate_theoretical(AVERAGE_PACKET_LENGTH, p, &avg_waiting_times_the[loop]);

        //output to .dat file
        cout << p << "\t\t" << avg_waiting_times_mark[loop] << "\t\t" << avg_waiting_times_det[loop] << "\t\t" << avg_waiting_times_the[loop] << endl;

        p += STEP;
        loop++;
    }

    return 0;
}

double generate_exp_rv(double average) {
    return -average*log((double) rand() / (RAND_MAX));
}

void run_system_markovian(double average_l, double average_t, double *avg_waiting_time) {
    Router my_router;
    double waiting_times[TOTAL_PACKETS/2];
    double sim_time; // simulation time
    double ta = 0; // time of next arrival
    double td = INFINITY; // time of next departure
    double packet_l; // variable for packet length
    double packet_at; // variable for packet arrival time
    int packets_in_buffer = 0;
    int completions = 0; // packets transmitted

    // initialize waiting time array
    for (double & waiting_time : waiting_times) {
        waiting_time = 0;
    }

    // queue in transient state
    while(completions < TOTAL_PACKETS/2) {
        packet_l = generate_exp_rv(average_l);
        if(ta < td) {
            sim_time = ta;
            my_router.enQueue(packet_l);
            packets_in_buffer++;
            packet_at =generate_exp_rv(average_t);
            ta = sim_time + packet_at;
            if (packets_in_buffer == 1) {
                td = sim_time + (packet_l/LINE_CAPACITY);
            }
        }
        else {
            sim_time = td;
            my_router.transmit();
            completions++;
            packets_in_buffer--;
            if (packets_in_buffer > 0) {
                td = sim_time + (packet_l/LINE_CAPACITY);
            }
            else {
                td = sim_time;
            }
        }
    }

    // queue in steady state (start statistic collection)
    while (completions < TOTAL_PACKETS) {
        packet_l = generate_exp_rv(average_l);
        if(ta < td) {
            sim_time = ta;
            my_router.enQueue(packet_l);
            packets_in_buffer++;
            waiting_times[completions - (TOTAL_PACKETS/2)] = my_router.calculate_waiting_time();
            packet_at =generate_exp_rv(average_t);
            ta = sim_time + packet_at;
            if (packets_in_buffer == 1) {
                td = sim_time + (packet_l/LINE_CAPACITY);
            }
        }
        else {
            sim_time = td;
            my_router.transmit();
            completions++;
            packets_in_buffer--;
            if (packets_in_buffer > 0) {
                td = sim_time + (packet_l/LINE_CAPACITY);
            }
            else {
                td = INFINITY;
            }
        }
    }

    for (double waiting_time : waiting_times) {
        *avg_waiting_time += waiting_time/((double)TOTAL_PACKETS/2);
    }
}

void run_system_deterministic(double average_l, double average_t, double *avg_waiting_time) {
    Router my_router;
    double waiting_times[TOTAL_PACKETS/2];
    double sim_time; // simulation time
    double ta = 0; // time of next arrival
    double td = INFINITY; // time of next departure
    double packet_at; // variable for packet arrival time
    double packet_l = average_l;
    int packets_in_buffer = 0;
    int completions = 0; // packets transmitted

    // initialize waiting time array
    for (double & waiting_time : waiting_times) {
        waiting_time = 0;
    }

    // queue in transient state
    while(completions < TOTAL_PACKETS/2) {
        if(ta < td) {
            sim_time = ta;
            my_router.enQueue(packet_l);
            packets_in_buffer++;
            packet_at = generate_exp_rv(average_t);
            ta = sim_time + packet_at;
            if (packets_in_buffer == 1) {
                td = sim_time + (packet_l/LINE_CAPACITY);
            }
        }
        else {
            sim_time = td;
            my_router.transmit();
            completions++;
            packets_in_buffer--;
            if (packets_in_buffer > 0) {
                td = sim_time + (packet_l/LINE_CAPACITY);
            }
            else {
                td = sim_time;
            }
        }
    }

    // queue in steady state (start statistic collection)
    while (completions < TOTAL_PACKETS) {
        if(ta < td) {
            sim_time = ta;
            my_router.enQueue(packet_l);
            packets_in_buffer++;
            waiting_times[completions - (TOTAL_PACKETS/2)] = my_router.calculate_waiting_time();
            packet_at = generate_exp_rv(packet_l);
            ta = sim_time + packet_at;
            if (packets_in_buffer == 1) {
                td = sim_time + (packet_l/LINE_CAPACITY);
            }
        }
        else {
            sim_time = td;
            my_router.transmit();
            completions++;
            packets_in_buffer--;
            if (packets_in_buffer > 0) {
                td = sim_time + (packet_l/LINE_CAPACITY);
            }
            else {
                td = INFINITY;
            }
        }
    }

    for (double waiting_time : waiting_times) {
        *avg_waiting_time += waiting_time/((double)TOTAL_PACKETS/2);
    }
}

void calculate_theoretical(double average_l, double p, double *avg_waiting_time) {
    *avg_waiting_time = 1/(average_l*(1 - p));
}
