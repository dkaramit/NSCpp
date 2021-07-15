#ifndef LU_Include
#define LU_Include

#include<cmath>


/*---------------------Functions I need for LU decomposition-------------------------------------------------*/
template<class LD>
int ind_max(LD *row , int N){
    /*   
    Find the index of the maximum of a list (row) of lentgth N.
    */
    int _in=0;
    LD _max = row[0];

    for (int  i = 1; i < N; i++)
    {
        if(row[i]>_max){_max=row[i]; _in=i; }
    }
    

    return _in;
}




template<class LD>
void index_swap(LD *A, int index_1, int index_2){

    /* 
        index_swap takes an array and interchanges 
         A[index_1] with A[index_2].
    */
    LD tmp=A[index_1];
    A[index_1]=A[index_2];
    A[index_2]=tmp;


}

template<class LD>
void index_swap(int *A, int index_1, int index_2){

    /* 
        index_swap takes an array and interchanges 
         A[index_1] with A[index_2].
    */
    int tmp=A[index_1];
    A[index_1]=A[index_2];
    A[index_2]=tmp;


}

template<class LD>
void apply_permutations_vector(LD *A, int *P, int N, LD *Ap){
    /*
    Applies the permutations given by P from LUP
    to a list A of length N, and stores the result to Ap.
    
    Example:
    If we do this:

    LD A[]={1,2,5,8,3};
    int P[]={2,4,0,3,1};

    LD Ap[5];
    apply_permutations_vector(A,P,5,Ap)

    we get Ap={5,3,1,8,2}
    */

    for (int i = 0; i < N; i++){Ap[i]=A[ P[i] ];}

}

template<class LD>
void Map( LD (*F)(LD) , LD *L, int N, LD *FL){
    for (int i = 0; i < N; i++){ FL[i] = F(L[i]); }
    
}

/*--------------------------------------------------------------------------------------------------------------*/

/*-----------------------------LUP decompositioning--------------------------------------------------------*/

template<const int N,class LD>
void LUP(LD (&M)[N][N], LD (&L)[N][N] ,LD (&U)[N][N], int (&P)[N], LD _tiny=1e-25){
    
    // Initialize LU
    for (int  i = 0; i < N; i++){
        P[i]=i;
        for (int  j = 0; j < N; j++)
        {
        if(i==j){L[i][j]=1;}
        if(i!=j){L[i][j]=0;}
        U[i][j]=M[i][j];
        }
    }
    LD _col[N],tmpU[N],tmpL[N];
    int len_col,pivot;
    

    for (int  k = 1; k < N; k++){ for (int  i = k; i < N; i++)
    {   
    
    for (int _r=k-1 ; _r<N ; _r++ ) { _col[_r-(k-1)]=fabs(U[_r][k-1]);  }//we need to convert the index of _col because we start the loop from k-1
    
    
    len_col=N-(k-1);
    pivot=ind_max<LD>( _col ,len_col) + k - 1;
    // std::cout<<pivot<<std::endl;

    if (fabs(U[pivot][k-1]) < _tiny)  {break;}

    if (pivot != k-1){ 
            
        index_swap<LD>(P,k-1,pivot);
        
        for (int _r=k-1 ; _r<N ; _r++ ) { tmpU[_r-(k-1)]= U[k-1][_r] ; }//we need to convert the index of tmpU because we start the loop from k-1

        for (int _r=k-1 ; _r<N ; _r++ ) { U[k-1][_r]=U[pivot][_r] ; }
        
        for (int _r=k-1 ; _r<N ; _r++ ) { U[pivot][_r]=tmpU[_r-(k-1)] ; }//we need to convert the index of tmpU because we start the loop from k-1

        for (int _r=0 ; _r<k-1 ; _r++ ) {tmpL[_r]= L[k-1][_r] ; }
        
        for (int _r=0 ; _r<k-1 ; _r++ ) {L[k-1][_r]=L[pivot][_r] ; }
        
        for (int _r=0 ; _r<k-1 ; _r++ ) {L[pivot][_r]=tmpL[_r] ; }
    }

    L[i][k-1]=U[i][k-1]/U[k-1][k-1];

    for (int j=k-1 ; j<N ; j++ ) {  U[i][j]=U[i][j]-L[i][k-1]*U[k-1][j] ; }



    }}
    
}
/*-------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------Solve-LU----------------------------------------------------------*/
template<const int N, class LD>
void Solve_LU(LD (&L)[N][N] ,LD (&U)[N][N], int (&P)[N], LD (&b)[N] , LD (&x)[N] ){
    /*
    This solves M*x=b
    Input:
    L,U,P= LUP decomposition of M, which is the output of the function LUP.

    b=the right hand side of the equation
    N=the number of equations

    x=an array to store the solution of M*x=b
    */    

    LD d[N], bp[N];
    LD tmps=0;

    apply_permutations_vector<LD>(b,P,N,bp);


    d[0]=bp[0];

    for(int i=1; i<N  ; i++){
        tmps=0;
        for (int j = 0; j < i; j++){ tmps +=L[i][j]*d[j]; }
        
        d[i]=bp[i]-tmps;
    }
    


    x[N-1]  = d[N-1]/U[N-1][N-1] ;
    for (int i = N-2; i > -1; i--)
    {
        tmps=0;
        for (int j = i+1; j < N; j++){ tmps += U[i][j]*x[j];  }
        x[i]=(d[i]-tmps )/U[i][i];
    }

}
/*-------------------------------------------------------------------------------------------------------------------*/











/*------------------------------------------------Solve-LU----------------------------------------------------------*/
template<const int N, class LD>
void Inverse_LU(LD (&L)[N][N] ,LD (&U)[N][N], int (&P)[N] , LD (&invM)[N][N] ){
    /*
    Finds the Inverse matrix given its LU decomposition.
    Basically this solves M*M^{-1}=1

    Input:
    L,U,P= LUP decomposition of M, which is the output of the function LUP.

    N=dimension of the matrix (N*N)

    invM=an array to store the solution inverse matrix.
    */    

    //     
    LD e[N];
    for(int i=0 ; i< N ; ++i){ e[i]=0; } 
    LD x[N];

    for(int i=0 ; i< N ; ++i){
        e[i]=1;
        Solve_LU(L,U,P,e,x);

        for(int j=0 ; j<N ; ++j){
            invM[j][i]=x[j];
        }

        e[i]=0;
    }
}
/*-------------------------------------------------------------------------------------------------------------------*/
 
/*------------------------------------------------Product of two matrices----------------------------------------------------------*/
template<const int N, class LD>
void dot(LD (&A)[N][N] ,LD (&B)[N][N], LD (&R)[N][N] ){
    /*
    Calculates the product of two matrices.
    R=A*B
    */

    for(int i=0; i<N; ++i){
        for(int j=0; j<N; ++j){
            R[i][j]=0;
            for(int l=0; l<N; ++l){
                R[i][j] += A[i][l]*B[l][j];
            }
        }
    }
}
/*-------------------------------------------------------------------------------------------------------------------*/


/*------------------------------------------------Product of matrix with vector----------------------------------------------------------*/
template<const int N, class LD>
void dot(LD (&A)[N][N] ,LD (&x)[N], LD (&b)[N] ){
    /*
    Calculates the product of  matrix with vector.
    b=A*x
    */

    for(int i=0; i<N; ++i){
        b[i]=0;
        for(int j=0; j<N; ++j){
            b[i] += A[i][j]*x[j];
        }
    }
}
/*-------------------------------------------------------------------------------------------------------------------*/


#endif