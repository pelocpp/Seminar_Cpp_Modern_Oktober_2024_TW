// =====================================================================================
// RangeBasedForLoop.cpp
// =====================================================================================

module;

#include "../ScopedTimer/ScopedTimer.h"

module modern_cpp:range_based_for_loop;

/*
 *  12 ways to iterate through a vector 
 */

namespace RangeBasedForLoop {

    // global function
    static void processElement(int n)
    {
        std::cout << n << " ";
    }

    // functor: "callable object" - class which implements operator()
    class ElementProcessor
    {
    public:
        void operator() (int n) const { 
            std::cout << n << " ";
        }
    };


    void ausgabe(int  wert)
    {
        std::cout << wert << " ";
    }


    static void test()
    {
        // container of integral data type
        std::vector<int> vec{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

        // =========================================================================
        // "Classic style" examples

        //std::list<int> list;
        //int first = list[0];

        // a) Very, very classic style ... C-stylistic
        for (size_t i = 0; i != vec.size(); ++i) {
            
            // [] Operator geht nur bei SEQUENTIELLEN Containern
            // Random-Access Operator
            std::cout << vec[i] << " ";
        }

        std::cout << std::endl;

        // b) Looks like C++, very classic style
        std::for_each(
            vec.cbegin(),                   // Iterator-object for begin of range
            vec.cend(),                     // Iterator-object for end of range
            processElement                 // function pointer
        );
        std::cout << std::endl;


        // c) Same as b), modified ranges
        std::for_each(
            std::next(vec.cbegin()),        // Iterator-object for begin of range
            std::prev(vec.cend()),          // Iterator-object for end of range
            processElement                 // function pointer
        );
        std::cout << std::endl;

        // d) Looks like C++, classic style, 'Functor'-stylistic
        std::for_each(
            vec.cbegin(),                   // Iterator-object for begin of range
            vec.cend(),                     // Iterator-object for end of range
            ElementProcessor{}             // "callable" object
        );
        std::cout << std::endl;

        // e) Using STL algorithm std::for_each with a lambda: Modern style
        std::for_each(
            vec.cbegin(),                   // Iterator-object for begin of range
            vec.cend(),                     // Iterator-object for end of range
            [](int n) {                    // anonymous method (lambda)
                std::cout << n << " ";
            }
        );
        std::cout << std::endl;

        // =========================================================================
        // Range-based 'for' Loop examples

        // f) Using Range-based 'for' Loop: Very modern style
        for (int n : vec) {
            std::cout << n << " ";
            if (n == 2)  // break is possible
                break;
        }
        std::cout << std::endl;

        // g) Same as f), using 'const int&'
        for (const int& n : vec) {
            std::cout << n << " ";
        }
        std::cout << std::endl;

        // h) Same as f): Using 'auto' keyword to cause type inference to be used
        for (auto n : vec) {
            std::cout << n << " ";
        }
        std::cout << std::endl;

        // i) Same as g), using 'const auto&': type inference by reference
        for (const auto& n : vec) {
            std::cout << n << " ";
        }
        std::cout << std::endl;

        // j) Real C++: How the compiler 'sees' a Range-based 'for' Loop
        std::vector<int>::const_iterator begin = vec.cbegin();
        std::vector<int>::const_iterator end = vec.cend();

        while (begin != end) {

            int n{ *begin };
            std::cout << n << " ";
            ++begin;
        }
        std::cout << std::endl;

        // k) Same as h), using 'auto'
        auto begin2 = vec.cbegin();
        auto end2 = vec.cend();

        while (begin2 != end2) {

            int n{ *begin2 };
            std::cout << n << " ";
            ++begin2;
        }
        std::cout << std::endl;

        // l) Range-based 'for' loop with initializer,
        // we can now use the index variable inside the for statement
        for (int index{}; int n : vec) {
            std::cout << index << ": " << n << " " << std::endl;
            ++index;
        }
    }
}

namespace RangeBasedForLoop {

    static auto calculate_sum_classic(const std::vector<double>& values) noexcept
    {
        std::println("Classic Loop: ");

        double sum{};

        ScopedTimer watch{};

        for (size_t i = 0; i != values.size(); ++i) {
            sum += values[i];
        }

        return sum;
    }

    static auto calculate_sum_iterator(const std::vector<double>& values) noexcept
    {
        std::println("Iterator Loop: ");

        double sum{};

        ScopedTimer watch{};

        for (auto it = values.cbegin(); it != values.cend(); ++it) {
            sum += *it;
        }

        return sum;
    }

    static auto calculate_sum_range_based_loop (const std::vector<double>& values) noexcept
    {
        std::println("Range-based Loop: ");

        double sum{};

        ScopedTimer watch{};

        for (const auto& value : values) {
            sum += value;
        }

        return sum;
    }

    static auto calculate_sum_standard_algorithm (const std::vector<double>& values) noexcept
    {
        std::println("Standard Algorithm - std::for_each:");

        double sum{};

        ScopedTimer watch{};

        std::for_each(
            values.cbegin(),
            values.cend(),
            [&sum] (const auto& value) {sum += value; }
        );

        return sum;
    }

    static auto calculate_sum_accumulate(const std::vector<double>& values) noexcept
    {
        std::println("Standard Algorithm - std::accumulate:");

        ScopedTimer watch{};

        double sum{ 
            std::accumulate(
                values.cbegin(),
                values.cend(),
                0.0
            ) 
        };

        return sum;
    }

    static void benchmark() {

        constexpr int Length = 100'000'000;   // use with release mode
        //constexpr int Length = 10'000'000;        // use with debug mode
        //constexpr int Length = 10;             // just for testing software

        std::vector<double> values(Length);

        std::generate(
            values.begin(),
            values.end(), 
            [value = 0.0]() mutable { ++value; return value; }
        );

        double sum{};

        sum = calculate_sum_classic(values);
        std::println("sum: {:15.20g}", sum);

        sum = calculate_sum_iterator(values);
        std::println("sum: {:15.20g}", sum);

        sum = calculate_sum_range_based_loop(values);
        std::println("sum: {:15.20g}", sum);
        
        sum = calculate_sum_standard_algorithm(values);
        std::println("sum: {:15.20g}", sum);
        
        sum = calculate_sum_accumulate(values);
        std::println("sum: {:15.20g}", sum);
    }

    static void benchmarks()
    {
        benchmark();
    }

    // =================================================

    // wird von einem Algorithmus
    // zum Vorbelegen aufgerufen
    int vorbelege()
    {
        static int start = 0;
        start++;
        return start;
    }

    static void test_algorithms()
    {
        const int Max = 100'000'000;

        {
            ScopedTimer watch;  // RAII

            std::vector<int> v;  // leer

            for (size_t i = 0; i != Max; ++i) {
                v.push_back((int) i);

                //std::println("{}: size: {} - capacity: {}", 
                //    i, v.size(), v.capacity());
            }
        }

        {
            ScopedTimer watch;  // RAII

            std::vector<int> v;  
            v.reserve(Max);    // added

            for (size_t i = 0; i != Max; ++i) {
                v.push_back((int)i);
            }
        }

        std::vector<int> vec;  // leer
        vec.resize(100);

        {
            ScopedTimer watch;  // RAII

            // Container vorbelegen - mit konstantem Wert
            for (size_t i = 0; i != vec.size(); ++i) {
                vec[i] = 1;
            }
        }

        {
            ScopedTimer watch;

            // ist performanter
            std::fill(
                vec.begin(),
                vec.end(),
                2
            );
        }

        {
            ScopedTimer watch;  // RAII

            // Container vorbelegen - mit konstantem Wert
            for (size_t i = 0; i != vec.size(); ++i) {
                vec[i] = (int) i;
            }
        }

        {
            ScopedTimer watch;  // RAII

            // 1, 2, 3, 4, 5
            std::generate(
                vec.begin(),
                vec.end(),
                vorbelege
            );
        }


        {
            // erste Option: einen Container durchlaufen ohne Algorithmus
            auto anfang = vec.begin();
            auto ende = vec.end();

            while (anfang != ende) {

                int elem = *anfang;
                anfang++;
            }
        }

        {
            // erste Option: einen Container durchlaufen mit Algorithmus
            std::for_each(
                vec.begin(),
                vec.end(),
                ausgabe
            );
        }
    }

    static void test_lambdas(const std::string& header)
    {
        std::vector<int> vec{ 1, 2, 3, 4, 5 };

        std::vector vecNeu{ 1, 2, 3, 4, 5 };

       // std::string header = "Wert: ";

        std::for_each(
            vec.begin(),
            vec.end(),
            [ n = 0, &header] (int wert) mutable {
                n++;

                std::cout << n << " - " << header << wert << std::endl;
            }
        );

        auto myLambda = [](int wert) { std::cout << wert << " "; };

        std::function< void (int) > myLambda2 = [](int wert) { std::cout << wert << " "; };

        // C20: CTAD Class Template Argument Deduction
        std::function myLambda3 = [](int wert) { std::cout << wert << " "; };

        std::vector<std::function< void(int) > > functions;

        // Frage:
        
    }
}

//void ausgabe(int  wert)
//{
//    std::cout << wert << " ";
//}

void main_range_based_for_loop()
{
    using namespace RangeBasedForLoop;
    //test_algorithms();
    test_lambdas("Wert: ");
    //test(); 
    //benchmarks();
}

// =====================================================================================
// End-of-File
// =====================================================================================
