#ifndef GEMDigi_ME0Digi2D_h
#define GEMDigi_ME0Digi2D_h

/** \class ME0Digi2D
 *
 * Digi for ME0
 *  
 * \author Piet Verwilligen
 *
 */

#include <boost/cstdint.hpp>
#include <iosfwd>

class ME0Digi2D{

public:
  explicit ME0Digi2D (int stripx, int stripy, float tof, int pdgid, bool prompt);
  ME0Digi2D ();

  bool operator==(const ME0Digi2D& digi) const;
  bool operator!=(const ME0Digi2D& digi) const;
  bool operator<(const ME0Digi2D& digi) const;

  int stripx() const { return stripx_; }
  int stripy() const { return stripy_; }
  float tof()  const { return tof_;}
  // coding mc-truth
  int pdgid() const { return pdgid_;}
  bool prompt() const { return prompt_;}
  void print() const;

private:
  int stripx_;
  int stripy_;
  float tof_;
  // coding mc-truth
  int pdgid_;
  bool prompt_;
};

std::ostream & operator<<(std::ostream & o, const ME0Digi2D& digi);

#endif

