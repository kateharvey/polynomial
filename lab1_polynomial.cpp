#include "lab1_polynomial.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cassert>

using namespace std;

Polynomial::Polynomial(int A[], int size) {
    if (size != 0) { 
        data.resize(size);
        for(int i = 0 ; i < size ; i ++) data[i] = A[i];
    } else { // all empty constructions are converted to 0 polynomials
        data.resize(1);
        data[0] = 0;
    }
}

Polynomial::Polynomial() {
    data.resize(rand() % 1000 + 1); // random degree in range [1, 1000]
    for(int i = 0 ; i < data.size() ; i ++) {
        data[i] = rand() % 2001 + (-1000); // make coefficients in range [-1000, 1000]
    }
}

Polynomial::Polynomial(string file_name) {
    ifstream my_file; // create file reader
    my_file.open(file_name); // open the file
    int coefficients;
    int degree;
    if(my_file.is_open()) {
        my_file >> degree;

        while(!my_file.eof()) {
            my_file >> coefficients; // store each line of the file in coefficient variable
            data.push_back(coefficients); // add coefficients to data
        }
        // number of terms in data cannot exceed the degree specified in the first line of the file
        if(data.size() > degree) {
            data.resize(degree);
        }
        while(data.size() < degree) {
            data.push_back(0);
        }
        my_file.close();
    }
}

Polynomial::~Polynomial() {}

bool Polynomial::operator==(const Polynomial& target) {
    bool are_equal = true;
    if(data.size() != target.data.size()) return false;
    for(int i = 0 ; i < data.size() ; i ++) {
        if(data[i] != target.data[i]) return false;
    }
    return are_equal;
}

Polynomial Polynomial::operator+(const Polynomial& target) {
    Polynomial highest_degree;
    if(data.size() >= target.data.size()) highest_degree = *this;
    else highest_degree = target;
    int coefficients[highest_degree.data.size()];
    
    for(int i = 0 ; i < highest_degree.data.size() ; i ++) {
        if(i >= data.size() || i >= target.data.size()) coefficients[i] = highest_degree.data[i];
        else coefficients[i] = data[i] + target.data[i];
    }

    Polynomial sum(coefficients, highest_degree.data.size());
    return sum;
}

Polynomial Polynomial::operator-(const Polynomial& target) {
    Polynomial highest_degree;
    if(data.size() >= target.data.size()) highest_degree = *this;
    else highest_degree = target;
    int coefficients[highest_degree.data.size()];

    for(int i = 0 ; i < highest_degree.data.size() ; i ++) {
        if(i >= data.size()) coefficients[i] = -(highest_degree.data[i]);
        else if(i >= target.data.size()) coefficients[i] = highest_degree.data[i];
        else coefficients[i] = data[i] - target.data[i];
    }
    
    Polynomial difference(coefficients, highest_degree.data.size());
    return difference;
}

Polynomial Polynomial::operator*(const Polynomial& target) {
    // accounts for 0/empty multiplication
    if (target.data.size() == 1 && target.data[0] == 0) return Polynomial({},0);
    if (data.size() == 1 && data[0] == 0) return Polynomial({},0);
    
    int size = data.size() + target.data.size() - 1;
    int co[size];
    for (int i = 0 ; i < size ; i ++) co[i] = 0;
    
    for (int i = 0 ; i < target.data.size() ; i ++) {
        for (int j = 0 ; j < data.size() ; j ++) co[i + j] += (data[j]*target.data[i]);
    }

    Polynomial product(co, size);
    return product;
}

Polynomial Polynomial::derivative() {
    int deriv_coefficients[data.size() - 1];
    for(int i = 0 ; i < data.size() - 1 ; i ++) {
        deriv_coefficients[i] = data[i + 1]*(i + 1);
    }
    
    Polynomial derivative(deriv_coefficients, data.size() - 1);
    return derivative;
}

void Polynomial::print() {
    for(int i = data.size() - 1 ; i >= 0 ; i --) {
        // first term
        if(i == data.size() - 1) {
            if(data[0] == 0) {};
            cout << data[i] << "x^" << i;
        }

        // last term (constant)
        else if(i == 0) {
            if(data[i] > 0) cout << " + " << data[i];
            else if(data[i] < 0) cout << " - " << abs(data[i]);
        }

        // middle terms
        else {
            if(data[i] > 0) cout << " + " << data[i] << "x^" << i;
            else if(data[i] < 0) cout << " - " << abs(data[i]) << "x^" << i;
        }
    }
}

void PolynomialTest::setup() {

    cout << "Setting up test Polynomials: \n";
    // 5x^2 - 2x + 3
    test_polynomial_1.data.resize(0);
    test_polynomial_1.data.push_back(poly_1[0]);
    test_polynomial_1.data.push_back(poly_1[1]);
    test_polynomial_1.data.push_back(poly_1[2]);
    
    // 4x^3 + 4x^2 - 2x - 1
    test_polynomial_2.data.resize(0);
    test_polynomial_2.data.push_back(poly_2[0]);
    test_polynomial_2.data.push_back(poly_2[1]);
    test_polynomial_2.data.push_back(poly_2[2]);
    test_polynomial_2.data.push_back(poly_2[3]);

    // 0
    test_empty.data.resize(0);
    test_empty.data.push_back(0);

    // 1
    test_one.data.resize(0);
    test_one.data.push_back(1);
    
    assert(test_polynomial_1.data.size() == 3);
    cout << "Size test passed for TP1 \n";
    assert(test_polynomial_2.data.size() == 4);
    cout << "Size test passed for TP2 \n";
    assert(test_empty.data.size() == 1); 
    cout << "Size test passed for empty TP3 \n";
    assert(test_one.data.size() == 1); 
    cout << "Size test passed for constant polynomial TP4 \n";
    cout << endl;
}

void  PolynomialTest::test_string_constructor() {
    Polynomial file_1("test_polynomial.txt");
    cout << "Reading from file test_polynomial.txt \n";
    file_1.print();
    cout << endl;
    cout << "Polynomial degree: " << file_1.data.size() - 1 << " \n";
    cout << endl;
    
    assert(file_1.data.size() == 6);
    cout << "Test 1 for string constructor passed: polynomial degree is correct \n";

    assert(file_1.data[0] == 6);
    assert(file_1.data[1] == 5);
    assert(file_1.data[2] == 4);
    assert(file_1.data[3] == 3);
    assert(file_1.data[4] == 2);
    assert(file_1.data[5] == 1);
    cout << "Test 2 for string constructor passed: coefficients read from file and stored correctly \n";

    Polynomial file_2("test_polynomial_2.txt");
    assert(file_2.data.size() == 3);
    cout << "Test 3 for string constructor passed: data is correctly resized when there are too many terms in the file \n";

    assert(file_2.data[0] == 4);
    assert(file_2.data[1] == 5);
    assert(file_2.data[2] == 6);
    cout << "Test 4 for string constructor passed: coefficients read from file and stored correctly (extra terms are disregarded) \n";

    Polynomial file_3("test_polynomial_3.txt");
    assert(file_3.data.size() == 5);
    assert(file_3.data[0] == 2);
    assert(file_3.data[2] == 0);
    assert(file_3.data[3] == 0);
    assert(file_3.data[4] == 0);
    cout << "Test 5 for string constructor passed: missing coefficients are replaced with 0s \n";
    cout << endl;
}

void  PolynomialTest::test_default_constructor() {
    Polynomial random;
    Polynomial random_2;

    assert(random.data.size() > 0 && random.data.size() < 1001);
    cout << "Test 1 for default constructor passed: constructor correctly creates a polynomial of degree 1000 \n";
    
    for(int i = 0 ; i < random.data.size() ; i ++) {
        assert(random.data[i] <= 1000);
        assert(random.data[i] >= -1000);
    }
    cout << "Test 2 for default constructor passed: coefficients are in range [-1000, 1000] \n";

    assert(!(random == random_2));
    cout << "Test 3 for default constructor passed: constructor creates a different polynomial each time \n";
    cout << endl;
}

void PolynomialTest::test_array_constructor() {
    int testVals[3] = {3,2,1};
    Polynomial constructTest(testVals, 3);

    for (int i = 0; i<3; i++) {
        assert(constructTest.data[i] == testVals[i]);
    }
    cout << "Test 1 for array constructor passed: coefficients equal to array \n";

    Polynomial constructEmpty({}, 0);
    assert(constructEmpty.data.size() == 1 && constructEmpty.data[0] ==0);
    cout << "Test 2 for array constructor passed: empty constructor results in a 0 polynomial \n";

    int testOne[1] = {3};
    Polynomial constructOne(testOne, 1);
    assert(constructOne.data.size() == 1 && constructOne.data[0] == 3);
    cout << "Test 3 for array constructor passed: constructor paramater of size one results in a constant polynomial \n";
    cout<< endl;

}


void PolynomialTest::test_equal() {
    Polynomial p1(poly_1, 3);
    Polynomial p2({}, 0);

    assert(p1.operator==(test_polynomial_1) == true);
    cout << "Test 1 for operator== passed: polynomials are equal \n";

    p1.data[0] = -3;
    assert(p1.operator==(test_polynomial_1) == false);
    cout << "Test 2 for operator== passed: polynomials with different signs not equal \n";

    p1.data.pop_back();
    assert(p1.operator==(test_polynomial_1) == false);
    cout << "Test 3 for operator== passed: polynomials not equal \n";
    
    assert(p2.operator==(test_empty) == true);
    cout<< "Test 4 for operator== passed: empty cases equal to 0 \n";
    
    cout <<endl;
}

void  PolynomialTest::test_addition() {
    assert(test_polynomial_1.operator+(test_polynomial_2).data.size() == 4);
    cout << "Test 1 for operator+ passed: degree of sum polynomial is correct \n";

    assert(test_polynomial_1.operator+(test_polynomial_2).data[0] == 2);
    cout << "Test 2 for operator+ passed: constant term of sum polynomial is correct \n";

    assert(test_polynomial_1.operator+(test_polynomial_2).data[3] == 4);
    cout << "Test 3 for operator+ passed: x^2 term of sum polynomial has correct coefficient \n";

    assert(test_polynomial_1.operator+(test_empty) == test_polynomial_1);
    cout << "Test 4 for operator+ passed: adding by empty/0 polynomial does not change result \n";

    assert(test_polynomial_1.operator+(test_polynomial_2) == test_polynomial_2.operator+(test_polynomial_1));
    cout << "Test 5 for operator+ passed: TP1 + TP2 returns same result as TP2 + TP1 \n";

    cout << endl;
}

void  PolynomialTest::test_subtraction() {
    assert(test_polynomial_1.operator-(test_polynomial_2).data.size() == 4);
    cout << "Test 1 for operator- passed: degree of poylnomial is correct \n";

    assert(test_polynomial_1.operator-(test_polynomial_2).data[0] == 4);
    assert(test_polynomial_1.operator-(test_polynomial_2).data[1] == 0);
    assert(test_polynomial_1.operator-(test_polynomial_2).data[2] == 1);
    assert(test_polynomial_1.operator-(test_polynomial_2).data[3] == -4);
    cout << "Test 2 for operator- passed: all coefficients are correct for TP1 - TP2 \n";

    assert(test_polynomial_2.operator-(test_polynomial_1).data[0] == -4);
    assert(test_polynomial_2.operator-(test_polynomial_1).data[1] == 0);
    assert(test_polynomial_2.operator-(test_polynomial_1).data[2] == -1);
    assert(test_polynomial_2.operator-(test_polynomial_1).data[3] == 4);
    cout << "Test 3 for operator- passed: all coefficients are correct for TP2 - TP1 \n";

    //Polynomial zero(0, 0);
    assert(test_polynomial_1.operator-(test_empty) == test_polynomial_1);
    cout << "Test 4 for operator- passed: subtracting by empty/0 polynomial does not change the result \n";

    cout << endl;
}

void  PolynomialTest::test_multiplication() { // jess

    int productP1P2[6] = {-3, -4, 11, -6, 12, 20};
    Polynomial prodTest(productP1P2, 6);
    assert(test_polynomial_1.operator*(test_polynomial_2) == prodTest);
    cout << "Test 1 for operator* passed: all coefficients are correct for TP1*TP2 \n";

    Polynomial zeroCase({},0);
    assert(test_polynomial_1.operator*(test_empty) == zeroCase);
    cout << "Test 2 for operator* passed: multiplying by 0 results in 0 \n";

    assert(test_polynomial_1.operator*(test_one) == test_polynomial_1);
    cout << "Test 3 for operator* passed: multiplying by 1 results in itself \n";

    cout<< endl;
}

void  PolynomialTest::test_derivative() { // jess
    int derivativeP1[2] = {-2,10};
    Polynomial derivativetest(derivativeP1, 2);

    assert(test_polynomial_1.derivative() == derivativetest);
    cout << "Test 1 for derivative passed: derivative coefficients match with expected for test polynomial 1 \n";

    assert(test_one.derivative() == test_empty);
    cout << "Test 2 for derivative passed: derivative of a constant is 0 \n";

    assert(test_empty.derivative() == test_empty);
    cout << "Test 3 for derivative passed: derivative of an empty polynomial is 0 \n";
    cout<< endl;
}

void PolynomialTest::run() {
    setup();
    test_string_constructor();
    test_default_constructor();
    test_array_constructor();

    test_equal();
    test_addition();
    test_subtraction();
    test_multiplication();
    test_derivative();
}

int main() {
    PolynomialTest test_1;
    test_1.run();
}
