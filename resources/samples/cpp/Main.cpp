#include <iostream>
#include <cmath> // Pre std::pow (Exponentiation - aj ked C++ nema priamy op)

namespace test_resource
{

int global_int = 100;
float global_float = 0.123f;
char  global_char = 'j';
char* global_string = nullptr;
bool  global_bool = false;

// class ExprTestClass {
// public:
//     int m_attribute = 5;
//     static int s_static_var;

//     ExprTestClass(int val) : m_attribute(val) {
//         ExprTestClass::s_static_var = 3;
//     }

//     void check_if(int p_param_val) {

//         this->m_attribute = p_param_val;
        
//         bool b_val = true;
//         char c_val = 'X';
//         float f_val = 3.14f;
//         const char* s_val = "Hello AST"; // const zatial nie je podporovane
//         int* null_ptr = nullptr;

//         if (b_val)
//             std::cout << "True." << std::endl;
//         else if (global_int > 50) {
//             global_float++;
//         } else
//             global_int = 0;

//         bool not_b = !b_val;
//         int neg_i = -(global_int);
//         int pos_i = +(global_int);

//         int *ptr_to_attr = &m_attribute;
//         int deref = *ptr_to_attr;

//         ++global_int;
//         global_int++;
//         --global_int;
//         global_int--;

//         int bit_flip = ~m_attribute; // BitFlip (Bitwise Not)
//     }

//     static void static_method(int x) {
//         s_static_var = x;
//     }
// };

// int ExprTestClass::s_static_var = 1;

void calculate_all_binaries(int A, int B) { 
    int result = 0; 
    
    // int assign_val = A = B; // toto bude treba dorobit
    
    result = A + B;
    result = A - B;
    result = A * B;
    result = A / B;
    result = A % B;
    result = A ^ B;
    
    double exp_val = std::pow((double)A, (double)B);
    
    bool cmp_res = (A == B); 
    cmp_res = (A != B); 
    cmp_res = (A < B);
    cmp_res = (A <= B);
    cmp_res = (A > B);
    cmp_res = (A >= B);
    
    bool log_res = (cmp_res && true);
    log_res = (cmp_res || false);
    
    result = A >> 1;
    result = A << 1;
    result = A & B;
    result = A | B;
    result = A ^ B;

    // int comma_res = (A = 10, B = 20, A + B); // toto som este nikdy nevidel, takze to isto nepodporuje c++ visitor
    
    A += B;
    A -= B;
    A *= B;
    A /= B;
    A %= B;
    
    A >>= B;
    A <<= B;
    A &= B;
    A |= B; 
    A ^= B; 
}

int main() {
    int i_val = 42; 

    calculate_all_binaries(i_val, global_int); 

    // ExprTestClass obj(50); 
    
    // obj.check_if(i_val); 

    int attr_val = 2;//obj.m_attribute;

    // ExprTestClass::static_method(200);

    auto lambda_func = [attr_val](int x) -> int {
        return x + attr_val;
    };
    
    int lambda_result = lambda_func(10);
    
    // ExprTestClass* new_obj = new ExprTestClass(60); 

    // delete new_obj;
    
    int paren_res = (i_val + 5) * 2; 
    return 0;
}
}
