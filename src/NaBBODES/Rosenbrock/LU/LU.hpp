#ifndef LU_Include
#define LU_Include
#include<array>
#include<cmath>

/*---------------------Functions I need for LU decomposition-------------------------------------------------*/
template<const unsigned int N, class LD>
unsigned int ind_max(std::array<LD,N> &row, unsigned int len_col){
    /*   
    Find the index of the maximum of a list (row) of lentgth N up to len_col.
    */
    int _in=0;
    LD _max = std::abs(row[0]);

    for (unsigned int  i = 1; i < len_col; i++)
    {
        if(std::abs(row[i])>_max){_max=std::abs(row[i]); _in=i; }
    }
    

    return _in;
}




template<const unsigned int N, class LD>
void index_swap(std::array<LD,N> &A, int index_1, int index_2){

    /* 
        index_swap takes an array and interchanges 
         A[index_1] with A[index_2].
    */
    LD tmp=A[index_1];
    A[index_1]=A[index_2];
    A[index_2]=tmp;


}

template<const unsigned int N, class LD>
void index_swap(std::array<int,N> &A, int index_1, int index_2){

    /* 
        index_swap takes an array and interchanges 
         A[index_1] with A[index_2].
    */
    int tmp=A[index_1];
    A[index_1]=A[index_2];
    A[index_2]=tmp;


}

template<const unsigned int N, class LD>
void apply_permutations_vector(std::array<LD,N> &A, std::array<int,N> &P, std::array<LD,N> &Ap){
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

    for (unsigned int i = 0; i < N; i++){Ap[i]=A[ P[i] ];}

}

template<const unsigned int N, class LD>
void Map( LD (*F)(LD) , std::array<LD,N> &L, std::array<LD,N> &FL){
    for (unsigned int i = 0; i < N; i++){ FL[i] = F(L[i]); }
    
}

/*--------------------------------------------------------------------------------------------------------------*/

/*-----------------------------LUP decompositioning--------------------------------------------------------*/

template<const unsigned int N,class LD>
void LUP(std::array<std::array<LD,N>,N> &M, std::array<std::array<LD,N>,N> &L ,std::array<std::array<LD,N>,N> &U, std::array<int,N> &P, LD _tiny=1e-25){
    
    // Initialize LU
    for (unsigned int  i = 0; i < N; i++){
        P[i]=i;
        for (unsigned int  j = 0; j < N; j++)
        {
        if(i==j){L[i][j]=1;}
        if(i!=j){L[i][j]=0;}
        U[i][j]=M[i][j];
        }
    }
    std::array<LD,N> _col,tmpU,tmpL;
    unsigned int len_col,pivot;
    

    for (unsigned int  k = 1; k < N; k++){ for ( unsigned int  i = k; i < N; i++){   
    for (unsigned int _r=k-1 ; _r<N ; _r++ ) { _col[_r-(k-1)]=std::abs(U[_r][k-1]); }//we need to convert the index of _col because we start the loop from k-1
    
    
    len_col=N-(k-1);
    pivot=ind_max<N,LD>( _col,len_col) + k - 1;
    // std::cout<<pivot<<std::endl;

    if (std::abs(U[pivot][k-1]) < _tiny)  {break;}

    if (pivot != k-1){ 
            
        index_swap<N,LD>(P,k-1,pivot);
        
        for (unsigned int _r=k-1 ; _r<N ; _r++ ) { tmpU[_r-(k-1)]= U[k-1][_r] ; }//we need to convert the index of tmpU because we start the loop from k-1

        for (unsigned int _r=k-1 ; _r<N ; _r++ ) { U[k-1][_r]=U[pivot][_r] ; }
        
        for (unsigned int _r=k-1 ; _r<N ; _r++ ) { U[pivot][_r]=tmpU[_r-(k-1)] ; }//we need to convert the index of tmpU because we start the loop from k-1

        for (unsigned int _r=0 ; _r<k-1 ; _r++ ) {tmpL[_r]= L[k-1][_r] ; }
        
        for (unsigned int _r=0 ; _r<k-1 ; _r++ ) {L[k-1][_r]=L[pivot][_r] ; }
        
        for (unsigned int _r=0 ; _r<k-1 ; _r++ ) {L[pivot][_r]=tmpL[_r] ; }
    }

    L[i][k-1]=U[i][k-1]/U[k-1][k-1];

    for (unsigned int j=k-1 ; j<N ; j++ ) {  U[i][j]=U[i][j]-L[i][k-1]*U[k-1][j] ; }



    }}
    
}
/*-------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------Solve-LU----------------------------------------------------------*/
template<const unsigned int N, class LD>
void Solve_LU(std::array<std::array<LD,N>,N> &L,std::array<std::array<LD,N>,N> &U, std::array<int,N> &P, std::array<LD,N> &b , std::array<LD,N> &x){
    /*
    This solves M*x=b
    Input:
    L,U,P= LUP decomposition of M, which is the output of the function LUP.

    b=the right hand side of the equation
    N=the number of equations

    x=an array to store the solution of M*x=b
    */    

    std::array<LD,N> d{0}, bp{0};
    LD tmps=0;

    apply_permutations_vector<N,LD>(b,P,bp);


    d[0]=bp[0];

    for(unsigned int i=1; i<N  ; i++){
        tmps=0;
        for (unsigned int j = 0; j < i; j++){ tmps +=L[i][j]*d[j]; }
        
        d[i]=bp[i]-tmps;
    }
    


    x[N-1]  = d[N-1]/U[N-1][N-1] ;
    for (int i = N-2; i > -1; i--)
    {
        tmps=0;
        for (unsigned int j = i+1; j < N; j++){ tmps += U[i][j]*x[j];  }
        x[i]=(d[i]-tmps )/U[i][i];
    }

}
/*-------------------------------------------------------------------------------------------------------------------*/











/*------------------------------------------------Solve-LU----------------------------------------------------------*/
template<const unsigned int N, class LD>
void Inverse_LU(std::array<std::array<LD,N>,N> &L, std::array<std::array<LD,N>,N> &U, std::array<int,N> &P, std::array<std::array<LD,N>,N> &invM){
    /*
    Finds the Inverse matrix given its LU decomposition.
    Basically this solves M*M^{-1}=1

    Input:
    L,U,P= LUP decomposition of M, which is the output of the function LUP.

    N=dimension of the matrix (N*N)

    invM=an array to store the solution inverse matrix.
    */    

    //     
    std::array<LD,N> e;
    for(unsigned int i=0 ; i< N ; ++i){ e[i]=0; } 
    std::array<LD,N> x;

    for(unsigned int i=0 ; i< N ; ++i){
        e[i]=1;
        Solve_LU<N,LD>(L,U,P,e,x);

        for(unsigned int j=0 ; j<N ; ++j){
            invM[j][i]=x[j];
        }

        e[i]=0;
    }
}
/*-------------------------------------------------------------------------------------------------------------------*/
 
/*------------------------------------------------Product of two matrices----------------------------------------------------------*/
template<const unsigned int N, class LD>
void dot(std::array<std::array<LD,N>,N> &A, std::array<std::array<LD,N>,N> &B, std::array<std::array<LD,N>,N> &R){
    /*
    Calculates the product of two matrices.
    R=A*B
    */

    for(unsigned int i=0; i<N; ++i){
        for(unsigned int j=0; j<N; ++j){
            R[i][j]=0;
            for(unsigned int l=0; l<N; ++l){
                R[i][j] += A[i][l]*B[l][j];
            }
        }
    }
}
/*-------------------------------------------------------------------------------------------------------------------*/


/*------------------------------------------------Product of matrix with vector----------------------------------------------------------*/
template<const unsigned int N, class LD>
void dot(std::array<std::array<LD,N>,N> &A, std::array<LD,N> &x, std::array<LD,N> &b){
    /*
    Calculates the product of  matrix with vector.
    b=A*x
    */

    for(unsigned int i=0; i<N; ++i){
        b[i]=0;
        for(unsigned int j=0; j<N; ++j){
            b[i] += A[i][j]*x[j];
        }
    }
}
/*-------------------------------------------------------------------------------------------------------------------*/


#endif