#ifndef LAB1
#define LAB1

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class Polynomial {
    vector<int> data;

public:
    // Constructors
    Polynomial(int A[], int size);
    Polynomial();
    Polynomial(string file_name);

    bool operator==(const Polynomial& target);
    Polynomial operator+(const Polynomial& target);
    Polynomial operator-(const Polynomial& target);
    Polynomial operator*(const Polynomial& target);
    Polynomial derivative();

    void print();
    ~Polynomial();
    friend class PolynomialTest;
};

class PolynomialTest {
    int poly_1[3] = {3, -2, 5};
    int poly_2[4] = {-1, -2, 4, 4};
    Polynomial test_polynomial_1;
    Polynomial test_polynomial_2;
    Polynomial test_empty;
    Polynomial test_one;
    
public:
    void setup();
    void test_string_constructor();
    void test_default_constructor();
    void test_array_constructor();
    void test_equal();
    void test_addition();
    void test_subtraction(); 
    void test_multiplication();
    void test_derivative();
    void run();
};

#endif