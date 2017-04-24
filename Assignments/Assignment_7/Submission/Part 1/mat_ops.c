#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cblas.h>

double * myread(FILE * file, int row, int col);
void mywrite(double * C, int m, int o, FILE * outfile);
void mycopy(double * Mat1, double * Mat2, int m, int n);
void myerror(FILE * outfile,int m,int n,int o,int p,int q,int r);

int main(int argc, char * argv[]){
	// Check number of arguments
	if (argc != 4) {
		printf("Error: please input 3 arguments.\n");
		return 1;
	}
	// Create output file names
	FILE *outfile1, *outfile2, *outfile3, *outfile4, *outfile5;
	char *filename1,*filename2,*filename3,*filename4,*filename5;
	filename1 = malloc( (15+ strlen(argv[1]) + strlen(argv[2]) + strlen(argv[3])) * sizeof(char) );
	filename2 = malloc( (15 + strlen(argv[1]) + strlen(argv[2]) + strlen(argv[3])) * sizeof(char) );
	filename3 = malloc( (15 + strlen(argv[1]) + strlen(argv[2]) + strlen(argv[3])) * sizeof(char) );
	filename4 = malloc( (15 + strlen(argv[1]) + strlen(argv[2]) + strlen(argv[3])) * sizeof(char) );
	filename5 = malloc( (15 + strlen(argv[1]) + strlen(argv[2]) + strlen(argv[3])) * sizeof(char) );
	sprintf(filename1, "%s_mult_%s_mult_%s", argv[1], argv[2], argv[3]);
	sprintf(filename2, "%s_mult_%s_plus_%s", argv[1], argv[2], argv[3]);
	sprintf(filename3, "%s_plus_%s_mult_%s", argv[1], argv[2], argv[3]);
	sprintf(filename4, "%s_mult_%s_minus_%s", argv[1], argv[2], argv[3]);
	sprintf(filename5, "%s_minus_%s_mult_%s", argv[1], argv[2], argv[3]);
	outfile1 = fopen(filename1, "w");
	outfile2 = fopen(filename2, "w");
	outfile3 = fopen(filename3, "w");
	outfile4 = fopen(filename4, "w");
	outfile5 = fopen(filename5, "w");
	free(filename1);
	free(filename2);
	free(filename3);
	free(filename4);
	free(filename5);
	// Open input filess
	FILE *file1, *file2, *file3;
	file1 = fopen(argv[1],"r");
	file2 = fopen(argv[2],"r");
	file3 = fopen(argv[2],"r");
	// Scan input files and implement each case
	int m, n, o, p, q, r;
	fscanf(file1, "%d %d", &m, &n);
	fscanf(file2, "%d %d", &o, &p);
	fscanf(file3, "%d %d", &q, &r);
	double *A, *B, *C, *D, *E;
	A = myread(file1,m,n);
	B = myread(file2,o,p);
	C = myread(file3,q,r);
	fclose(file1);
	fclose(file2);
	fclose(file3);
	int alpha, beta;
	// case 1
	if(n==o && p==q){
		D = calloc(m*p,sizeof(double));
		alpha = 1;
		beta = 0;
		cblas_dgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,m,p,n,alpha,A,n,B,p,beta,D,p);
		E = calloc(m*r,sizeof(double));
		cblas_dgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,m,r,p,alpha,D,n,C,p,beta,E,r);
		mywrite(E, m, r, outfile1);
		free(D);
		free(E);
    	}
	else{myerror(outfile1,m,n,o,p,q,r);}
	// case 2 & 4
	if(n==o && m==q && p==r){
		// case 2
		D = calloc(q*r,sizeof(double));
		mycopy(C,D,q,r);
		alpha = 1;
		beta = 1;
		cblas_dgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,m,p,n,alpha,A,n,B,p,beta,D,r);
		mywrite(D, q, r, outfile2);
		// case 4
		mycopy(C,D,q,r);
		alpha = 1;
		beta = -1;
		cblas_dgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,m,p,n,alpha,A,n,B,p,beta,D,r);
		mywrite(D, q, r, outfile4);
		free(D);
    	}
	else{myerror(outfile2,m,n,o,p,q,r);myerror(outfile4,m,n,o,p,q,r);}
	// case 3 & 5
	if(p==q && o==m && n==r){
		D = calloc(m*n,sizeof(double));
		// case 3
		mycopy(A,D,m,n);
		alpha = 1;
		beta = 1;
		cblas_dgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,o,n,p,alpha,B,p,C,r,beta,D,n);
		mywrite(D, m, n, outfile3);
		// case 5
		mycopy(A,D,m,n);
		alpha = -1;
		beta = 1;
		cblas_dgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,o,n,p,alpha,B,p,C,r,beta,D,n);
		mywrite(D, m, n, outfile5);
		free(D);
	    }
	else{myerror(outfile3,m,n,o,p,q,r);myerror(outfile5,m,n,o,p,q,r);}
	// Free dynamically allocated memory and close files
	free(A);
	free(B);
	free(C);
	fclose(outfile1);
	fclose(outfile2);
	fclose(outfile3);
	fclose(outfile4);
	fclose(outfile5);
	return 0;
}

double * myread(FILE * file, int row, int col){
	double * Mat = malloc(row*col*sizeof(double));

	int i,j;

	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			fscanf(file, "%lf", Mat+(i*col+j));
			//printf( "Row: %d Col: %d Value: %d\n", i+1, j+1, *(A+(i*col+j)) );
		}
	}

	return Mat;
}

void mywrite(double * Mat, int m, int n, FILE * outfile){
	fprintf(outfile, "%d %d\n", m, n);
	int i,j;
	for(i=0;i<m;i++){
		for(j=0;j<n;j++){
			fprintf(outfile, "%f ", *(Mat+(i*n+j)) );
		}
		fprintf(outfile, "\n");
	}	
}

void mycopy(double * Mat1, double * Mat2, int m, int n){
    int i,j;
    for(i=0;i<m;i++){
        for(j=0;j<n;j++){
            *(Mat2+(i*n+j)) = *(Mat1+(i*n+j));
        }
    }
}

void myerror(FILE * outfile,int m,int n,int o,int p,int q,int r){
	fprintf(outfile,"Operation was not carried out because, the dimensions of the matrices do not meet the requirements.\n");
        fprintf(outfile,"A dimensions: %d X %d\n",m,n);
        fprintf(outfile,"B dimensions: %d X %d\n",o,p);
        fprintf(outfile,"C dimensions: %d X %d\n",q,r);
}
