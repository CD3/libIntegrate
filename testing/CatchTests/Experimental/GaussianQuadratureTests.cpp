#include "catch.hpp"
#include "fakeit.hpp"

#include "Experimental/_1D/GaussianQuadratures/GaussLegendre.hpp"

namespace Experimental
{
namespace GausssLegendreTests
{

TEST_CASE( "Testing experimental 1D GQ:8 rule on polynomials." ) {

  _1D::GQ::GaussLegendreQuadrature<double,8> integrate;
  _1D::GQ::GaussLegendreQuadrature<double,8>::State<double> state;

  for(int N = 0; N < 8; N++)
  {

    auto fi = [&N](double x) { double sum = 0; for(int i = 0; i < N+1; i++){ sum = sum + pow(x,i+1)/(i+1); }; return sum; };

    state.initialized = false;
    while( integrate( 0., 1., state ) )
    {
      double sum = 0;
      for(int i = 0; i < N+1; i++)
        sum = sum + pow(state.x,i);
      state.f = sum;
    }
    CHECK( state.sum  == Approx( fi(1)  - fi(0) ) );


  }

}


//TEST_CASE( "Testing 1D GQ:16 rule on polynomials." ) {

  //_1D::GQ::GaussLegendreQuadrature<double,16> integrate;
  //for(int N = 0; N < 16; N++)
  //{
    //auto f  = [&N](double x) { double sum = 0; for(int i = 0; i < N+1; i++){ sum = sum + pow(x,i); }; return sum; };
    //auto fi = [&N](double x) { double sum = 0; for(int i = 0; i < N+1; i++){ sum = sum + pow(x,i+1)/(i+1); }; return sum; };
    //CHECK( integrate( f, 0., 1. )  == Approx( fi(1)  - fi(0) ) );
    //CHECK( integrate( f, 1., 10. ) == Approx( fi(10) - fi(1) ) );
    //CHECK( integrate( f, 0., 100. ) == Approx( fi(100) - fi(0) ) );
  //}

//}


//TEST_CASE( "Testing 1D GQ:32 rule on polynomials." ) {

  //_1D::GQ::GaussLegendreQuadrature<double,32> integrate;
  //for(int N = 0; N < 32; N++)
  //{
    //auto f  = [&N](double x) { double sum = 0; for(int i = 0; i < N+1; i++){ sum = sum + pow(x,i); }; return sum; };
    //auto fi = [&N](double x) { double sum = 0; for(int i = 0; i < N+1; i++){ sum = sum + pow(x,i+1)/(i+1); }; return sum; };
    //CHECK( integrate( f, 0., 1. )  == Approx( fi(1)  - fi(0) ) );
    //CHECK( integrate( f, 1., 10. ) == Approx( fi(10) - fi(1) ) );
    //CHECK( integrate( f, 0., 100. ) == Approx( fi(100) - fi(0) ) );
  //}

//}


//TEST_CASE( "Testing 1D GQ:64 rule on polynomials." ) {

  //_1D::GQ::GaussLegendreQuadrature<double,64> integrate;
  //for(int N = 0; N < 64; N++)
  //{
    //auto f  = [&N](double x) { double sum = 0; for(int i = 0; i < N+1; i++){ sum = sum + pow(x,i); }; return sum; };
    //auto fi = [&N](double x) { double sum = 0; for(int i = 0; i < N+1; i++){ sum = sum + pow(x,i+1)/(i+1); }; return sum; };
    //CHECK( integrate( f, 0., 1. )  == Approx( fi(1)  - fi(0) ) );
    //CHECK( integrate( f, 1., 10. ) == Approx( fi(10) - fi(1) ) );
    //CHECK( integrate( f, 0., 100. ) == Approx( fi(100) - fi(0) ) );
  //}

//}


//TEST_CASE( "Comparing 1D GQ:8 and GQ:64 rules on polynomials." ) {

  //_1D::GQ::GaussLegendreQuadrature<double,8> integrate8;
  //_1D::GQ::GaussLegendreQuadrature<double,64> integrate64;
  //// GQ order 8 should be *exact* up to 8th order polynomials.
  //// but it's actually pretty darn close for even higher orders...
  //for(int N = 0; N < 20; N++)
  //{
    //auto f  = [&N](double x) { double sum = 0; for(int i = 0; i < N+1; i++){ sum = sum + pow(x,i); }; return sum; };
    //CHECK( integrate8( f, 0., 1.   ) == Approx( integrate64( f, 0., 1.   ) ) );
    //CHECK( integrate8( f, 1., 10.  ) == Approx( integrate64( f, 1., 10.  ) ) );
    //CHECK( integrate8( f, 0., 100. ) == Approx( integrate64( f, 0., 100. ) ) );
  //}

//}



//TEST_CASE( "Testing 1D GQ:64 rule on trig functions." ) {
  //_1D::GQ::GaussLegendreQuadrature<double,64> integrate;

  //CHECK( integrate( sin, 0., M_PI )  == Approx( 2 ) );
  //CHECK( integrate( sin, M_PI, 2*M_PI )  == Approx( -2 ) );
  //CHECK( integrate( sin, M_PI/2, M_PI )  == Approx( 1 ) );

//}



//TEST_CASE( "Testing 2D GQ:64 rule on simple linear function." ) {

  //_2D::GQ::GaussLegendreQuadrature<double,64> integrate;
  //double I;

  //auto f  = [](double x, double y){ return 10*x + 20*y + 30;};
  //auto fi = [](double x, double y){ return 10*x*x*y/2 + 20*y*y*x/2 + 30*x*y;};

  //I = integrate( f, 2., 5., 3., 6. );
  //CHECK( I == Approx( ( fi(5,6) - fi(5,3) ) - ( fi(2,6) - fi(2,3) ) ).epsilon(0.001) );

//}

//TEST_CASE( "Testing 2D GQ:64 rule on 2-var polynomial function." ) {

  //_2D::GQ::GaussLegendreQuadrature<double,64> integrate;

  //for(int N = 0; N < 64; N = N + 4)
  //{
    //auto f  = [&N](double x,double y)
    //{ double sum = 0;
      //for(int i = 0; i < N+1; i++){
      //for(int j = 0; j < N+1; j++){
        //sum = sum + pow(x,i)*pow(y,j);
      //}}; return sum; };
    //auto fi = [&N](double x,double y)
    //{ double sum = 0;
      //for(int i = 0; i < N+1; i++){
      //for(int j = 0; j < N+1; j++){
        //sum = sum + pow(x,i+1)/(i+1)*pow(y,j+1)/(j+1);
      //}}; return sum; };

    //CHECK( integrate( f, 0., 1., 0., 1.   )  == Approx( (fi(1,1) - fi(1,0)) - (fi(0,1) - fi(0,0) ) ) );
  //}


//}

//TEST_CASE( "Testing 2D GQ:64 rule on trig functions." ) {

  //_2D::GQ::GaussLegendreQuadrature<double,64> integrate;

  //CHECK( integrate( [](double x, double y){ return sin(x)*cos(y); },
                    //0., M_PI,
                    //0., M_PI ) == Approx( (-cos(M_PI)*(sin(M_PI) - sin(0)) + cos(0)*(sin(M_PI) - sin(0)) ) ) );


//}



}
}
