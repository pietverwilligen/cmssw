#include "DataFormats/GEMDigi/interface/ME0Digi.h"
#include <iostream>

ME0Digi::ME0Digi (int strip, float tof, int pdgid, bool prompt) :
  strip_(strip),
  tof_(tof),
  pdgid_(pdgid),
  prompt_(prompt)
{}

ME0Digi::ME0Digi ():
  strip_(0),
  tof_(-1.),
  pdgid_(0),
  prompt_(0)
{}

// Comparison
bool ME0Digi::operator == (const ME0Digi& digi) const
{
  return strip_ == digi.strip() and tof_ == digi.tof();
}

// Comparison
bool ME0Digi::operator != (const ME0Digi& digi) const
{
  return strip_ != digi.strip() or tof_ != digi.tof();
}

///Precedence operator
bool ME0Digi::operator<(const ME0Digi& digi) const
{
  if (digi.tof() == tof_){
      return digi.strip() < strip_;
  } else {
    return digi.tof() < tof_;
  }
}

std::ostream & operator<<(std::ostream & o, const ME0Digi& digi)
{
  return o << "local strip=" << digi.strip() << " tof="<<digi.tof()<<" ns"<<" pdgID "<<digi.pdgid()<<" prompt? "<<digi.prompt();
}

void ME0Digi::print() const
{
  std::cout << "local strip=" << this->strip() << " tof="<<this->tof()<<" ns"<<" pdgID "<<this->pdgid()<<" prompt? "<<this->prompt()<<std::endl;
}

