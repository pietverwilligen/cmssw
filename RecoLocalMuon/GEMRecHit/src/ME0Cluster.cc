#include "ME0Cluster.h"
#include <iostream>
#include <fstream>
#include <math.h> 

using namespace std;

ME0Cluster::ME0Cluster() : fstrip(0), lstrip(0), ystrip(0), cltime(0)
{
}

ME0Cluster::ME0Cluster(int fs, int ls, int ys, double t) : 
  fstrip(fs), lstrip(ls), ystrip(ys), cltime(t)
{
}

ME0Cluster::~ME0Cluster()
{
}

int
ME0Cluster::firstStrip() const
{
  return fstrip;
}

int
ME0Cluster::lastStrip() const
{
  return lstrip;
}

int 
ME0Cluster::etaPartition() const
{
  return ystrip;
}

int
ME0Cluster::clusterSize() const
{
  return -(fstrip-lstrip)+1;
}

double
ME0Cluster::tof() const
{
  return cltime;
}

bool ME0Cluster::isAdjacent(const ME0Cluster& cl, double timeres) const{
  
  return ((cl.etaPartition() == this->etaPartition()) && 
	  (cl.firstStrip() == this->firstStrip()-1) &&
	    fabs(cl.tof() - this->tof())<timeres ); 
}

void ME0Cluster::merge(const ME0Cluster& cl, double timeres){
  
  if(this->isAdjacent(cl,timeres))
     { 
       fstrip = cl.firstStrip();  
     }
}

bool ME0Cluster::operator<(const ME0Cluster& cl) const{
  
  if(cl.etaPartition() == this->etaPartition()) {
    if (cl.tof() == this->tof())
      return cl.firstStrip()<this->firstStrip();
    else 
      return cl.tof()<this->tof();
  }
  else
    return cl.etaPartition()<this->etaPartition();
}

bool 
ME0Cluster::operator==(const ME0Cluster& cl) const {
  return ( (this->etaPartition() == cl.etaPartition()) &&
	   (this->clusterSize()  == cl.clusterSize())  &&
	   (this->tof()          == cl.tof())          && 
	   (this->firstStrip()   == cl.firstStrip()) );
}
