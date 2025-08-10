#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <stdexcept>
#include <string>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>

#include <libIntegrate/IO.hpp>
#include <libIntegrate/Integrate.hpp>
#include <libIntegrate/_1D/RandomAccessLambda.hpp>
#include <libIntegrate/version.h>
#include <libInterpolate/Interpolate.hpp>

using namespace std;
namespace po = boost::program_options;

void print_usage(char prog_name[])
{
  cout << "version: " << libIntegrate_FULL_VERSION << "\n\n";
  cout << "usage: " << prog_name << " [OPTIONS]" << "\n";
}

void print_documentation()
{
  cout << "Reads function from a file an integrates it."
       << "\n"
       << "The data is contained in a gnuplot-style text file, with each x-y pair on a new line, separated by white space."
       << "\n";
}

std::function<double(std::vector<double> &, std::vector<double> &, long, long)> create_1d(std::string type)
{
  if(boost::starts_with("riemann", type))
    return _1D::RiemannRule<double>();

  if(boost::starts_with("trapezoid", type))
    return _1D::TrapezoidRule<double>();

  if(boost::starts_with("simpson", type))
    return _1D::SimpsonRule<double>();

  if(boost::starts_with("gauss-legendre", type))
    return [](std::vector<double> &x, std::vector<double> &y, long ai, long bi) {
      // create an interpolator to pass to the integrator
      _1D::CubicSplineInterpolator<double> interp(x, y);

      while(bi < 0)
        bi += x.size();

      double min = x[ai];
      double max = x[bi];

      // we have quadratures of order
      // 8, 16, 32, and 64.
      if(bi - ai <= 8) {
        _1D::GQ::GaussLegendreQuadrature<double, 8> integ;
        return integ(interp, min, max);
      }
      if(bi - ai <= 16) {
        _1D::GQ::GaussLegendreQuadrature<double, 16> integ;
        return integ(interp, min, max);
      }
      if(bi - ai <= 32) {
        _1D::GQ::GaussLegendreQuadrature<double, 32> integ;
        return integ(interp, min, max);
      }

      _1D::GQ::GaussLegendreQuadrature<double, 64> integ;
      return integ(interp, min, max);
    };

  return nullptr;
}

std::function<double(std::vector<double> &, std::vector<double> &, std::vector<std::vector<double>> &)> create_2d(std::string type)
{
  if(boost::starts_with("riemann", type))
    return _2D::RiemannRule<double>();
  if(boost::starts_with("trapezoid", type))
    return _2D::TrapezoidRule<double>();
  if(boost::starts_with("simpson", type))
    return _2D::SimpsonRule<double>();

  return nullptr;
}

int main(int argc, char *argv[])
{
  po::options_description options("Allowed options");
  options.add_options()("help,h", "print help message")("batch,b", "output in 'batch' mode")("dimensions,d", po::value<int>()->default_value(1), "number of dimensions (1 or 2).")("method,m", po::value<string>()->default_value("riemann"), "integration method.")("list,l", "list available integration methods.")("indefinate,i", "compute the indefinate integral g(x) = \\int_a^x f(x') dx'.")("integrate-data", po::value<string>()->default_value("-"), "file containing data to be integrated.");

  po::positional_options_description args;
  args.add("integrate-data", 1);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(options).positional(args).run(), vm);
  po::notify(vm);

  if(argc == 1 || vm.count("help")) {
    print_usage(argv[0]);
    cout << "\n";
    cout << options << "\n";
    cout << "\n";
    print_documentation();
    cout << "\n";
    return 1;
  }

  if(vm.count("list")) {
    cout << "\t'riemann' : simple riemann sum\n";
    cout << "\t'trapezoid' : trapezoid rule\n";
    cout << "\t'simpson' : simposon's rule (uses interpolation)\n";
    cout << "\t'gauss-legendre' : Gauss-Legendre interpolation (uses interpolation)\n";
    return 1;
  }

  ifstream in;

  // open data stream
  if(vm["integrate-data"].as<string>() == "-") {
    in.open("/dev/stdin");
  } else {
    in.open(vm["integrate-data"].as<string>().c_str());
  }

  if(!in.is_open()) {
    cerr << "ERROR: Could not open file: " << vm["integrate-data"].as<string>() << endl;
    return 1;
  }

  // load data
  auto data = libIntegrate::gnuplot::readGnuplotData(in);

  if(vm["dimensions"].as<int>() == 1) {
    // create integrator
    std::function<double(std::vector<double> &, std::vector<double> &, long, long)> integrate;
    integrate = create_1d(vm["method"].as<string>());
    if(!integrate) {
      cerr << "ERROR: Unrecognized integration method (" << vm["method"].as<string>() << ")." << endl;
      return 1;
    }

    // extract data
    std::vector<double> X, Y;
    libIntegrate::gnuplot::extract(data, X, Y);

    // integrate
    if(vm.count("indefinate")) {
      for(size_t n = 1; n < X.size(); n++) {
        auto sum = integrate(X, Y, 0, n);
        std::cout << X[n] << " " << sum << "\n";
      }

    } else {
      auto sum = integrate(X, Y, 0, -1);
      std::cout << sum << "\n";
    }
  }

  if(vm["dimensions"].as<int>() == 2) {
    // create integrator
    std::function<double(std::vector<double> &, std::vector<double> &, std::vector<std::vector<double>> &)> integrate;
    integrate = create_2d(vm["method"].as<string>());
    if(!integrate) {
      cerr << "ERROR: Unrecognized integration method (" << vm["method"].as<string>() << ")." << endl;
      return 1;
    }

    // extract data
    std::vector<double>              X, Y;
    std::vector<std::vector<double>> Z;
    libIntegrate::gnuplot::extract(data, X, Y, Z);

    // integrate
    if(vm.count("indefinate")) {
      cerr << "ERROR: Indefinate integrals are not supported with 2D integrals (yet)." << std::endl;
      return 1
    } else {
      auto sum = integrate(X, Y, Z);
      std::cout << sum << "\n";
    }
  }

  return 0;
}
