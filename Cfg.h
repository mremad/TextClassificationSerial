#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <ctime>

#define DATA_PATH_EMAD "/Users/Mohamed/Desktop/TextClassificationSerial/TrainingData.txt"
#define DATA_PATH_MOUMEN "TrainingData.txt"
#define DATA_PATH_SHAABAN "TrainingData.txt"

#define TEST_PATH_EMAD "/Users/Mohamed/Desktop/TextClassificationSerial/TestData.txt"
#define TEST_PATH_MOUMEN ""
#define TEST_PATH_SHAABAN "TestData.txt"

#define HASH_TABLE_SIZE 2039
#define HASH_ROW_SIZE 60
#define HASH_WORD_SIZE 100

#define MAX_NUM_LABELS 20

//#define CUDA_LIKELIHOOD
//#define CUDA_CLASSIFY
#define CUDA_FEATURE_VECTOR

#define CLASSIFY_THREADS_X		1024
#define CLASSIFY_THREADS_Y		1

#define LIKELIHOOD_THREADS_X	32
