#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include<list>
#include<stdexcept>
#include<functional>

#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>

#include <libInterpolate/Interpolate.hpp>

#include <libIntegrate/Integrate.hpp>
#include <libIntegrate/libIntegrate_version.h>
#include <libIntegrate/_1D/RandomAccessLambda.hpp>

using namespace std;
namespace po = boost::program_options;


void print_version()
{
  cout<<"integrate-cli - linked against libIntegrate version "<< libIntegrate_VERSION_FULL << endl;
}

void print_usage(char prog_name[])
{

  cout<<"usage: "<<prog_name<<" [OPTIONS]"<<"\n";
  
}

void print_documentation( )
{
  cout<<"Reads function from a file an integrates it."
      <<"\n"
      <<"The data is contained in a gnuplot-style text file, with each x-y pair on a new line, separated by white space."
      <<"\n";
}

std::function<double(std::vector<double>&,std::vector<double>&,long,long)> create(std::string type)
{
  if( boost::starts_with("riemann",type) )
    return _1D::RiemannRule<double>();

  if( boost::starts_with("trapezoid",type) )
    return _1D::TrapezoidRule<double>();

  if( boost::starts_with("simpson",type) )
    return _1D::SimpsonRule<double>();

  if( boost::starts_with("gauss-legendre",type) )
    return [](std::vector<double>& x, std::vector<double>& y,long ai, long bi){
      // create an interpolator to pass to the integrator
      _1D::CubicSplineInterpolator<double> interp(x,y);

      while(bi < 0)
        bi += x.size();

      double min = x[ai];
      double max = x[bi];

      // we have quadratures of order
      // 8, 16, 32, and 64.
      if( bi - ai <= 8 )
      {
        _1D::GQ::GaussLegendreQuadrature<double,8> integ;
        return integ( interp, min, max );
      }
      if( bi - ai  <= 16 )
      {
        _1D::GQ::GaussLegendreQuadrature<double,16> integ;
        return integ( interp, min, max );
      }
      if( bi - ai  <= 32 )
      {
        _1D::GQ::GaussLegendreQuadrature<double,32> integ;
        return integ( interp, min, max );
      }

      _1D::GQ::GaussLegendreQuadrature<double,64> integ;
      return integ( interp, min, max );
    };

  return nullptr;
}

void ReadFunction(std::istream &_in, double *&_x, double *&_y, int &_n, int _dimensions = 1, int _multiplicity = 1);


int main( int argc, char* argv[])
{
    po::options_description options("Allowed options");
    options.add_options()
      ("help,h"      , "print help message")
      ("batch,b"     , "output in 'batch' mode")
      ("method,m"    , po::value<string>()->default_value("riemann"),     "integration method.")
      ("list,l"      ,                                                   "list available integration methods.")
      ("indefinate,i" ,                                                "compute the indefinate integral g(x) = \\int_a^x f(x') dx'.")
      ("integrate-data" , po::value<string>()->default_value("-"),        "file containing data to be integrated.")
      ;

    po::positional_options_description args;
    args.add("integrate-data", 1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(options).positional(args).run(), vm);
    po::notify(vm);


    if (argc == 1 || vm.count("help"))
    {
      print_version();
      print_usage( argv[0] );
      cout<<"\n";
      cout << options<< "\n";
      cout<<"\n";
      print_documentation();
      cout<<"\n";
      return 1;
    }

    if (vm.count("list"))
    {
      print_version();
      cout<<"\t'riemann' : simple riemann sum\n";
      cout<<"\t'trapezoid' : trapezoid rule\n";
      cout<<"\t'simpson' : simposon's rule (uses interpolation)\n";
      cout<<"\t'gauss-legendre' : Gauss-Legendre interpolation (uses interpolation)\n";
      return 1;
    }


    ifstream in;
    double *x, *y, sum;
    int n;

    // load data
    if( vm["integrate-data"].as<string>() == "-" )
    {
      in.open("/dev/stdin");
    }
    else
    {
      in.open( vm["integrate-data"].as<string>().c_str() );
    }

    if( !in.is_open() )
    {
      cerr << "ERROR: could not open file: "<<vm["integrate-data"].as<string>()<< endl;
      return 0;
    }

    ReadFunction( in, x, y, n );
    in.close();

    std::function<double(std::vector<double>&,std::vector<double>&,long,long)> integrate;
    integrate = create(vm["method"].as<string>());
    if( !integrate )
    {
      delete[] x;
      delete[] y;
      cerr << "ERROR: Unrecognized integration method (" << vm["method"].as<string>()<< ")." << endl;
      return 0;
    }
    
    std::vector<double> X(x,x+n),Y(y,y+n);

    delete[] x;
    delete[] y;

    if(vm.count("indefinate"))
    {
      for( size_t n = 1; n < X.size(); n++)
      {
        sum = integrate(X,Y,0,n);
        std::cout << X[n] << " " << sum <<  "\n";
      }
      
    }
    else
    {
    sum = integrate(X,Y,0,-1);
    std::cout << sum << "\n";
    }
  
    return 0;
}





void ReadFunction(std::istream &_in, double  *&_x, double *&_y, int *&_n, int _dimensions, int _multiplicity)
{

  double *xbuffer1, *xbuffer2;
  double *ybuffer1, *ybuffer2;

  //////////////////////////////
  //  READ IN DATA FROM FILE  //
  //////////////////////////////

  const int chunck = 1000;
  int buffsize,buffsize_old;
  int i,j;
  int n;

  std::string line;
  

  buffsize = chunck;

  xbuffer1 = new double[buffsize*_dimensions  ];
  ybuffer1 = new double[buffsize*_multiplicity];

  i = 0;
  while( getline( _in, line ) )
  {
    // if line is blank, skip it
    if( line == "" )
      continue;
    std::vector<std::string> columns;
    boost::char_separator<char> sep(" \t");
    boost::tokenizer<boost::char_separator<char>> toks(line,sep);
    for( auto it = toks.begin(); it != toks.end(); ++it )
      columns.push_back( *it );

    // if first column starts with a #, this line is a comment
    // and we should skip it.
    if( columns.size() > 0 && columns[0].find('#') == 0 )
      continue;

    if(i == buffsize - 1)  // this "grows" the buffer
    {
      buffsize_old = buffsize;
      buffsize = buffsize + chunck;

      xbuffer2 = new double[buffsize*_dimensions  ];
      std::copy( xbuffer1, xbuffer1 + buffsize_old*_dimensions, xbuffer2);
      delete[] xbuffer1;

      ybuffer2 = new double[buffsize*_multiplicity];
      std::copy( ybuffer1, ybuffer1 + buffsize_old*_multiplicity, ybuffer2);
      delete[] ybuffer1;

      xbuffer1 = xbuffer2;
      ybuffer1 = ybuffer2;
    }

    for(j = 0; j < _dimensions; j++)
      xbuffer1[i*_dimensions + j]   = boost::lexical_cast<double>(columns[j]);
    for(j = 0; j < _multiplicity; j++)
      ybuffer1[i*_multiplicity + j] = boost::lexical_cast<double>(columns[j+_dimensions]);

    i++;
  }

  n = i;


  /** \todo add actual multi-coordinate support */

  _n = new int[_dimensions];
  _x = new double[n * _dimensions  ];
  _y = new double[n * _multiplicity];

  for(j = 0; j < n * _dimensions  ; j++)
    _x[j] = xbuffer1[j];
  for(j = 0; j < n * _multiplicity; j++)
    _y[j] = ybuffer1[j];
  for(j = 0; j < _dimensions; j++)
    _n[j] = n;

  delete[] xbuffer1;
  delete[] ybuffer1;

}

void ReadFunction(std::istream &_in, double *&_x, double *&_y, int &_n, int _dimensions, int _multiplicity)
{
  int *n;
  ReadFunction(_in, _x, _y, n, _dimensions, _multiplicity);
  _n = n[0];
  delete[] n;
}


