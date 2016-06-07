//2^31
#define CONST1 2147483648.0
//2^31 -1
#define CONST2 2147483647

#define MASK 2147483648

#define BIT_SET(a,b) ((a) |= (1<<(b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1<<(b)))
#define BIT_FLIP(a,b) ((a) ^= (1<<(b)))
#define BIT_CHECK(a,b) ((a) & (1<<(b)))

int rand(int *seed){

    int s  = *seed;
    s = abs((s * 16807) % CONST2);    
    *seed = s;        
    return s; 
}

float u_rand(int * seed){
    return (float)rand(seed) / CONST1;
}
