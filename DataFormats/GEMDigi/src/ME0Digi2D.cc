#include "DataFormats/GEMDigi/interface/ME0Digi2D.h"
#include <iostream>

ME0Digi2D::ME0Digi2D (int stripx, int stripy, float tof, int pdgid, bool prompt) :
  stripx_(stripx),
  stripy_(stripy),
  tof_(tof),
  pdgid_(pdgid),
  prompt_(prompt)
{}

ME0Digi2D::ME0Digi2D ():
  stripx_(0),
  stripy_(0),
  tof_(-1.),
  pdgid_(0),
  prompt_(0)
{}


// Comparison
bool ME0Digi2D::operator == (const ME0Digi2D& digi) const
{
  return stripx_ == digi.stripx() and stripy_ == digi.stripy() and tof_ == digi.tof();
}


// Comparison
bool ME0Digi2D::operator != (const ME0Digi2D& digi) const
{
  return stripx_ != digi.stripx() or stripy_ != digi.stripy() or tof_ != digi.tof();
}


///Precedence operator
bool ME0Digi2D::operator<(const ME0Digi2D& digi) const
{
  if (digi.tof() == tof_){
    if(digi.stripx() == stripx_)
      return digi.stripy() < stripy_;
    else 
      return digi.stripx() < stripx_;
  } else {
    return digi.tof() < tof_;
  }
}


std::ostream & operator<<(std::ostream & o, const ME0Digi2D& digi)
{
  return o << "local stripx=" << digi.stripx() << " cm stripy=" << digi.stripy()<<" cm tof="<<digi.tof()<<" ns"<<" pdgID "<<digi.pdgid()<<" prompt? "<<digi.prompt();
}

void ME0Digi2D::print() const
{
  std::cout << "local stripx=" << this->stripx() << " cm stripy=" << this->stripy() <<" cm tof="<<this->tof()<<" ns"<<" pdgID "<<this->pdgid()<<" prompt? "<<this->prompt()<<std::endl;
}

