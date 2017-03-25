#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>

float * myread(FILE * file, int row, int col);
float * multiply(float * A, float * B, int m, int o, int n);
float mysum(float * A, float * B, int i, int j, int n, int o);
void mywrite(float * C, int m, int o, FILE * outfile);

int main(int argc, char * argv[]){
	// Check number of arguments
	if(argc != 3){
	printf("Please input 2 filenames as arguments.\n");
	return 1;
	}
	// Open input files
	FILE *p1, *p2;
	p1 = fopen(argv[1],"r");
	p2 = fopen(argv[2],"r");
	// Scan input files and check dimensions
	int Arow, Acol, Brow, Bcol;
	fscanf(p1, "%d %d", &Arow, &Acol);
	fscanf(p2, "%d %d", &Brow, &Bcol);
	if(Acol != Brow){
	printf("# of columns of A must be equal to # of rows of B.\n");
	printf("A dimensions: %d X %d\n",Arow,Acol);
	printf("B dimensions: %d X %d\n",Brow,Bcol);
	fclose(p1);
	fclose(p2);
	return 1;
	}
	// Read in matrices and do matrix multiplication
	float *A, *B, *C;
	A = myread(p1,Arow,Acol);
	B = myread(p2,Brow,Bcol);
	C = multiply(A,B,Arow,Bcol,Acol);
	free(A);
	free(B);
	// Print resulting matrix to a file
	int m, n, o;
	m = Arow;
	n = Acol;
	o = Bcol;	
	FILE *outfile;
	char *file3;
	file3 = malloc( (strlen(argv[1]) + 6 + strlen(argv[2])) * sizeof(char) );
	sprintf(file3, "%s_mult_%s", argv[1], argv[2]);
	outfile = fopen(file3, "w");
	mywrite(C, m, o, outfile);
	// Free dynamically allocated memory and close files
	free(C);
	free(file3);
	fclose(p1);
	fclose(p2);
	fclose(outfile);
	return 0;
}

float * myread(FILE * file, int row, int col){
	float * A = malloc(row*col*sizeof(float));

	int i,j;

	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			fscanf(file, "%f", A+(i*col+j));
			//printf( "Row: %d Col: %d Value: %d\n", i+1, j+1, *(A+(i*col+j)) );
		}
	}

	return A;
}

float * multiply(float * A, float * B, int m, int o, int n){
	float * C = malloc(m*o*sizeof(float));
	
	int i, j;
	
	for(i=0;i<m;i++){
		for(j=0;j<o;j++){ 
			*(C+(i*o+j)) = mysum(A,B,i,j,n,o);
			//printf( "Row: %d Col: %d Value: %d\n", i+1, j+1, *(C+(i*o+j)) );
		}
	} 	
	return C;
}

float mysum(float * A, float * B, int i, int j, int n, int o){
	float total = 0;
	int k;
	for(k=0;k<n;k++){
		total = total + ( *(A+(i*n+k)) * *(B+(k*o+j)) );
	}
	return total;
}
void mywrite(float * C, int m, int o, FILE * outfile){
	fprintf(outfile, "%d %d\n", m, o);
	int i,j;
	for(i=0;i<m;i++){
		for(j=0;j<o;j++){
			fprintf(outfile, "%f ", *(C+(i*o+j)) );
		}
		fprintf(outfile, "\n");
	}	
}
