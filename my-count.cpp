#include <iostream>
#include <fstream>
#include <cmath>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string>
#include <cstdlib>
#include <cstring>

using namespace std;

void computeArrayElements(int* input, int* shared_memory, int start, int end, int i) {
    for (int j = start; j < end; ++j) {
        if (j < (int)pow(2, i)) {
            shared_memory[j] = input[j];
        } else {
            shared_memory[j] = input[j] + input[j - (int)pow(2, i)];
        }
    }

}

void synchronize_barrier(int* barrier, int num_processes, int current_iteration) {
    for(int j = 0; j < num_processes; ++j) {
        while(barrier[j] <  current_iteration + 1) {

        }
    }

}


void computePrefixSum(int* input, int* output, int n, int num_processes, int log_n) {

    int segment_id3 = shmget(IPC_PRIVATE, n * sizeof(int), IPC_CREAT | 0666);
    if (segment_id3 < 0) {
        cerr << "Error creating shared memory segment." << endl;
        exit(1);
    }

    int segment_id4 = shmget(IPC_PRIVATE, num_processes * sizeof(int), IPC_CREAT | 0666);
    if (segment_id4 < 0) {
        cerr << "Error creating shared memory segment." << endl;
        exit(1);
    }

    int* shared_memory = (int*)shmat(segment_id3, NULL, 0);
    if ((long)shared_memory == -1) {
        cerr << "Error attaching shared memory." << endl;
        exit(1);
    }

    int* barrier = (int*)shmat(segment_id4, NULL, 0);
    if ((long)barrier == -1) {
        cerr << "Error attaching shared memory." << endl;
        exit(1);
    }

    // Initialize shared memory with input data
    memcpy(shared_memory, input, n * sizeof(int));

    for (int i = 0; i <= log_n; ++i) {

        int workload = (n + num_processes - 1) / num_processes;

        for (int j = 0; j < num_processes; ++j) {

            pid_t pid = fork();

            if (pid == 0) { // Child process
                int start = j * workload;
                int end = start + workload;
                if (j == num_processes - 1) { // Last process handles remainder
                    end = n;
                }

                computeArrayElements(input, shared_memory, start, end, i);

                barrier[j] = i + 1;
                exit(0);
            } else if (pid < 0) {
                cerr << "Error forking process." << endl;
                exit(1);
            }

        }

        synchronize_barrier(barrier, num_processes, i);
        memcpy(input, shared_memory, n * sizeof(int));
    }

    memcpy(output, shared_memory, n * sizeof(int));

    // Detach and remove shared memory
    shmdt(shared_memory);
    shmctl(segment_id3, IPC_RMID, NULL);
    shmdt(barrier);
    shmctl(segment_id4, IPC_RMID, NULL);

}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        cerr << "Usage: " << argv[0] << " <num_elements> <num_processes> <input_file> <output_file>" << endl;
        return 1;
    }

    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    string inputFileName = argv[3];
    string outputFileName = argv[4];

    //Check if arguments are valid
    if (n <= 0) {
        cerr << "Error: incorrect argument passed for num of elements" << endl;
        return 1;
    }
    if (m <= 0) {
        cerr << "Error: incorrect argument passed for num of cores" << endl;
        return 1;
    }

    if (inputFileName.empty() || outputFileName.empty()) {
        cerr << "Error: input file or output file not provided" << endl;
        return 1;
    }

    int log_n = floor(log2(n));

    //Create shared memory arrays for input and output
    int segment_id = shmget(IPC_PRIVATE, n * sizeof(int), IPC_CREAT | 0666);
    if (segment_id < 0) {
        cerr << "Error creating shared memory segment." << endl;
        exit(1);
    }

    int* input = (int*)shmat(segment_id, NULL, 0);
    if ((long)input == -1) {
        cerr << "Error attaching shared memory." << endl;
        exit(1);
    }

    int segment_id2 = shmget(IPC_PRIVATE, n * sizeof(int), IPC_CREAT | 0666);
    if (segment_id2 < 0) {
        cerr << "Error creating shared memory segment." << endl;
        exit(1);
    }

    int* output = (int*)shmat(segment_id2, NULL, 0);
    if ((long)output == -1) {
        cerr << "Error attaching shared memory." << endl;
        exit(1);
    }

    ifstream inputFile(inputFileName.c_str());
    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open input file." << endl;
        return 1;
    }

    // Counting the number of elements in the file
    int fileElementCount = 0;
    int temp; // Temporary variable to hold elements while counting
    while(inputFile >> temp) {
        ++fileElementCount;
    }

    // Reset the file stream to the beginning for the actual reading
    inputFile.clear(); // Clear EOF and other flags
    inputFile.seekg(0, ios::beg); // Seek to the beginning of the file

    // Check if the number of elements matches n
    if (fileElementCount != n) {
    cerr << "Error: The number of elements in the input file (" << fileElementCount << ") does not match the expected number (" << n << ")." << endl;
         return 1;
    }

    for (int i = 0; i < n; ++i) {
        inputFile >> input[i];
    }
    inputFile.close();

    computePrefixSum(input, output, n, m, log_n);

    ofstream outputFile(outputFileName.c_str());
    if (!outputFile.is_open()) {
        cerr << "Error: Unable to open output file." << endl;
        return 1;
    }
    for (int i = 0; i < n; ++i) {
        outputFile << output[i] << " ";
    }
    outputFile << endl;

    // Detach and remove shared memory
    shmdt(input);
    shmctl(segment_id, IPC_RMID, NULL);
    shmdt(output);
    shmctl(segment_id2, IPC_RMID, NULL);

    return 0;
}
