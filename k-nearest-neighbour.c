#include<stdio.h>
#include<stdbool.h>
#include<math.h>

FILE *dataset = NULL;
FILE *testData = NULL;

struct flower {
	char  className[100];
	float a1;
	float a2;
	float a3;
	float a4;
}flowers[500];

struct quality {
	float distance;
	char *className;
};

struct test {
	float a1;
	float a2;
	float a3;
	float a4;
	struct quality qualityMetric[500];	
}data[500];

int numberOfTrainingData = 0;
int numberOfTestData = 0;

int insertTrainingSet();
int insertTestData();
bool buildTrainingArray();
bool buildTestArray();
struct test findQualityMetric(struct test);
float euclideanDistance(struct flower,struct test);
void printStoredData();
void printTestData();
float square(float);

int insertTrainingSet() {
	char fileName[100];
	printf("\nPlease insert the name of the file having the training set : ");
	scanf(" %s",fileName);
        dataset = fopen(fileName,"r");

	if( dataset == NULL ) {
		return 1;	
	} else {
		return 0;
	}
}

int insertTestData() {
	char fileName[100];
	printf("\nPlease insert the name of the file having the test set : ");
	scanf("%s",fileName);
	testData = fopen(fileName, "r");
	
	if(testData == NULL) {
		return 1;
	} else {
		return 0;
	}
}

bool buildTrainingArray() {
	//here we will read the file and insert into the dynamic structure array that 
	//we have already made.
	int j = 0;
	int result = 0;
	
	while(true) {  
		result = fscanf(dataset,"%f %f %f %f %s",&flowers[j].a1,&flowers[j].a2,&flowers[j].a3,&flowers[j].a4,flowers[j].className);	
		if(result == 5) {
			j++;
			numberOfTrainingData = j;	
		} else if(result== EOF) {
			printf("\nEnd of file training.");
			break;	
		} else {
			printf("\nCould not read. (%d) read (%d)",j,result);
			break;
		} 	
	}
}

bool buildTestArray() {
	int j = 0;
        int result = 0;

        while(true) {
                result = fscanf(testData,"%f %f %f %f",&data[j].a1,&data[j].a2,&data[j].a3,&data[j].a4);
	        if(result == 4) {
                        j++;
                        numberOfTestData = j;
                } else if(result== EOF) {
                        printf("\nEnd of file test");
                        break;
                } else {
                        printf("\nCould not read. (%d) read (%d)",j,result);
                        break;
                }
        }
}


struct test findQualityMetric(struct test data) {
	//this function will find distance of each test data node with each training data node
	//thus per test data node, all training data must must be iterated

	int i = 0;
	for(i = 0; i < numberOfTrainingData; i++ ) {
		data.qualityMetric[i].className = flowers[i].className;
		data.qualityMetric[i].distance = euclideanDistance( flowers[i] , data );					
	}

	return data;	
}

float euclideanDistance(struct flower f, struct test d ) {
	float distance;
	distance = (float)sqrt( square(f.a1 - d.a1) + square(f.a2 - d.a2) + square(f.a3 - d.a3) + square(f.a4 - d.a4));			
	return distance;	
}

float square(float a ) {
	return a*a;
}
	
void printStoredData() {
	int i = 0;
	printf("\nThis is the present dataset:\n");
	for( i = 0; i < numberOfTrainingData; i++ ) {
		printf("\n %s \n",flowers[i].className);	
	}
}

void printTestResult() {
        int i = 0;
	int j = 0;
	int t = 0;
	struct quality temp;
	printf("\nAnd the result ( quality analysis, distance and class ) --> ");
	for(i = 0; i < numberOfTestData; i++) {
		printf("\nThis is the test data: ",numberOfTestData);
		printf("\n %f %f %f %f \n",data[i].a1,data[i].a2,data[i].a3,data[i].a4);
		//insertion sort the qualityMetric array distance to get the min distance
		//The data belongs to the class with min distance.
		for( j = 1; j < numberOfTrainingData; j++ ) { 
		      temp = data[i].qualityMetric[j];
		      t = j - 1;
		      while( ( temp.distance < data[i].qualityMetric[t].distance ) && (t >= 0 ) ) {
		      		data[i].qualityMetric[t+1] =  data[i].qualityMetric[t];
			  	t = t - 1;
		      }
		      data[i].qualityMetric[t+1] = temp;
		}

		//as this is sorted low to high, minimum is in the top position or 0 index
		printf("\n The test data is of class %s with a distance of %d \n", data[i].qualityMetric[0].className , data[i].qualityMetric[0].distance);
		
	} 
}

int main() {
	int i = 0;
	if(insertTrainingSet() == 1) {
		perror("\nError reading training file.");
		return (1);
	}

	if(insertTestData() == 1) {
		perror("\nError reading test data file.");
		return 1;
	}
		
	printf("File reading successfull.");
	buildTrainingArray();
	buildTestArray();
	for(i = 0; i < numberOfTestData; i++) {
		data[i] = findQualityMetric(data[i]);
	}
	//printStoredData();
	printTestResult();
			
	fclose(dataset);
	return (0);

}

