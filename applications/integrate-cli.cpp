#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include<list>
#include<stdexcept>
#include<functional>

#include<boost/program_options.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

#include "Integrate.hpp"

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


std::function<double(std::vector<double>&,std::vector<double>&)> create(std::string type)
{
  if( type == "riemann" )
    return _1D::RiemannRule<double>();

  if( type == "trapezoid" )
    return _1D::TrapezoidRule<double>();

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
      ("integrate-data" ,                                                "file containing data to be integrated.")
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
      //cout<<"\tlinear\n";
      //cout<<"\tspline\n";
      //cout<<"\tmonotonic\n";
      return 1;
    }


    ifstream in;
    double *x, *y, sum;
    int n;

    // load data
    in.open( vm["integrate-data"].as<string>().c_str() );
    ReadFunction( in, x, y, n );
    in.close();

    std::function<double(std::vector<double>&,std::vector<double>&)> interp;
    interp = create(vm["method"].as<string>());
    if( !interp )
    {
      delete[] x;
      delete[] y;
      cerr << "ERROR: Unrecognized interpolation method (" << vm["method"].as<string>()<< ")." << endl;
      return 0;
    }
    
    std::vector<double> X(x,x+n),Y(y,y+n);

    delete[] x;
    delete[] y;

    sum = interp(X,Y);

    std::cout << sum << std::endl;
  
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


